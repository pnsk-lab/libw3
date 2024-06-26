/*
 * $Id$
 *
 * Simple browser
 */

#include <W3Core.h>
#include <W3HTTP.h>
#include <W3NNTP.h>
#include <W3Tag.h>

#include <W3URL.h>
#include <W3Util.h> /* It has some useful functions, you know */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __MINGW32__
#include <windows.h>
HANDLE winstdout;
#else
#include <sys/ioctl.h>
#endif

#ifdef __sun__
#include <sys/termios.h>
#endif

char* allurl = NULL;
int termw, termh;

#ifndef __MINGW32__
extern int strcasecmp(const char* s1, const char* s2);
#endif

void clear_console() {
#ifdef __MINGW32__
	DWORD written = 0;
	const char* seq = "\x1b[m\x1b[2J\x1b[1;1H";
	WriteConsole(winstdout, seq, strlen(seq), &written, NULL);
#else
	printf("\x1b[m\x1b[2J\x1b[1;1H");
	fflush(stdout);
#endif
}

void get_terminal_size(int* width, int* height) {
#ifdef __MINGW32__
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(winstdout, &csbi);
	*width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	*height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	*width = ws.ws_col;
	*height = ws.ws_row;
#endif
}

char* databuf;
int datalen;
bool isredir;
char* ctype;

int x = 0;

void header_handler(struct W3* w3, char* key, char* value) {
	if(strcasecmp(key, "content-type") == 0) {
		if(ctype != NULL) free(ctype);
		ctype = __W3_Strdup(value);
		int i;
		for(i = 0; ctype[i] != 0; i++) {
			if(ctype[i] == ';') {
				ctype[i] = 0;
				break;
			}
		}
	}
}

void status_handler(struct W3* w3, int st) {
	if(st >= 300 && st < 400) {
		isredir = true;
	} else {
		isredir = false;
	}
}

void data_handler(struct W3* w3, char* data, size_t size) {
	if(isredir) return;
	if(databuf == NULL) {
		databuf = malloc(size);
		datalen = size;
		memcpy(databuf, data, size);
	} else {
		char* oldbuf = databuf;
		databuf = malloc(datalen + size);
		memcpy(databuf, oldbuf, datalen);
		memcpy(databuf + datalen, data, size);
		datalen += size;
		free(oldbuf);
	}
}

struct W3URL* global_u;
void nntpresp_handler(struct W3* w3, int status, char* data) {
	if(status == 200) {
		if(databuf == NULL) {
			databuf = malloc(1);
			databuf[0] = 0;
		}
		char* tmp = databuf;
		char* hyp = malloc(2 + strlen(data) + 3 + 1);
		hyp[0] = '\n';
		int i;
		for(i = 0; data[i] != 0; i++) {
			hyp[i + 1] = '-';
		}
		for(i = 0; i < 3; i++) hyp[strlen(data) + i + 1] = '-';
		hyp[strlen(data) + 3 + 1] = '\n';
		hyp[strlen(data) + 3 + 2] = 0;
		databuf = __W3_Concat3(tmp, data, hyp);
		free(hyp);
		free(tmp);
		datalen = strlen(databuf);

		W3_Set_Method(w3, "GROUP");
		W3_Set_Path(w3, global_u->path + 1);
		W3_NNTP_Send_Request(w3);
	} else if(status == 211 || status == 223) {
		W3_Set_Method(w3, "HEAD");
		W3_NNTP_Send_Request(w3);
	} else if(status == 221) {
		W3_Set_Method(w3, "NEXT");
		W3_NNTP_Send_Request(w3);
	} else {
		W3_NNTP_Disconnect(w3);
	}
}

void access_site(const char* url) {
	struct W3URL* u = W3_Parse_URL(url);
	global_u = u;
	if(u != NULL) {
		struct W3* w3 = W3_Create(u->protocol, u->host, u->port);
		if(w3 != NULL) {
			if(databuf != NULL) {
				free(databuf);
			}
			databuf = NULL;
			datalen = 0;
			W3_Set_Method(w3, "GET");
			W3_Set_Path(w3, u->path);
			W3_Set_Header(w3, "User-Agent", "LibW3/" LIBW3_VERSION " W3B/");
			W3_HTTP_Enable_Redirect(w3);
			W3_On(w3, "status", (void*)status_handler);
			W3_On(w3, "header", (void*)header_handler);
			W3_On(w3, "nntpresp", (void*)nntpresp_handler);
			W3_On(w3, "data", (void*)data_handler);
			if(ctype != NULL) free(ctype);
			ctype = NULL;
			W3_Send_Request(w3);
			W3_Free(w3);
			printf("%d bytes\n", datalen);
		} else {
			fprintf(stderr, "Failed to connect\n");
		}
		W3_Free_URL(u);
	} else {
		fprintf(stderr, "Failed to parse\n");
	}
}

bool title;
bool italic;
bool bold;
bool pre;
char* titlebuf = NULL;

int nl;
int start = 0;

bool style = false;

void html_handler(char* tagname, char* attr) {
	if(nl - start > termh - 3) return;
	if(style) {
		if(strcasecmp(tagname, "/style") == 0) style = false;
		return;
	}
	int oldnl = nl;
	if(strcasecmp(tagname, "title") == 0) {
		title = true;
	} else if(strcasecmp(tagname, "/title") == 0) {
		title = false;
	} else if(strcasecmp(tagname, "i") == 0) {
		italic = true;
	} else if(strcasecmp(tagname, "/i") == 0) {
		italic = false;
	} else if(strcasecmp(tagname, "pre") == 0) {
		pre = true;
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/pre") == 0) {
		pre = false;
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "br") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "h1") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/h1") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "h2") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/h2") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "h3") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/h3") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "h4") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/h4") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "h5") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/h5") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "h6") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/h6") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "p") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/p") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "dl") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/dl") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "dt") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "/dt") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "dd") == 0) {
		if(nl >= start) printf("\n");
		nl++;
		printf("    ");
		x += 4;
	} else if(strcasecmp(tagname, "/dd") == 0) {
		if(nl >= start) printf("\n");
		nl++;
	} else if(strcasecmp(tagname, "style") == 0) {
		style = true;
	} else if(strcasecmp(tagname, "img") == 0) {
		if(nl >= start) {
			char* alt = W3_Tag_Attr(attr, "alt");
			if(alt != NULL) {
				printf("[%s]", alt);
				x += 2 + strlen(alt);
				free(alt);
			} else {
				printf("[IMG]");
				x += 5;
			}
		}
	} else if(strcasecmp(tagname, "hr") == 0) {
		int i;
		if(nl >= start) printf("\n ");
		nl++;
		for(i = 0; i < termw - 2; i++) {
			if(nl >= start) printf("-");
		}
		if(nl >= start) printf("\n");
		nl++;
	}
	fflush(stdout);
	if(x >= termw) {
		if(nl >= start) {
			nl += x / termw;
			x = x % termw;
		}
	}
	if(oldnl != nl) x = 0;
}

void text_handler(char* data) {
	if(style) return;
	if(title) {
		if(titlebuf == NULL) {
			titlebuf = malloc(1);
			titlebuf[0] = 0;
		}
		char* tmp = titlebuf;
		titlebuf = __W3_Concat(tmp, data);
		free(tmp);
	} else {
		if(nl - start > termh - 3) return;
		char* text = malloc(strlen(data) + 64);
		int i;
		char* fmt_data = malloc(strlen(data) * 2);
		if(pre) {
			free(fmt_data);
			fmt_data = __W3_Strdup(data);
			for(i = 0; i < strlen(data); i++) {
				if(data[i] == '\n') {
					nl++;
				}
			}
		} else {
			fmt_data[0] = 0;
			char* cbuf = malloc(2);
			cbuf[1] = 0;
			bool spc = false;
			bool firstspc = true;
			for(i = 0; i < strlen(data); i++) {
				cbuf[0] = data[i];
				if(data[i] == '\n' || data[i] == '\t' || data[i] == ' ') {
					if(!spc && !firstspc) {
						spc = true;
						char* tmp = fmt_data;
						fmt_data = __W3_Concat(tmp, " ");
						free(tmp);
					}
				} else {
					firstspc = false;
					spc = false;
					char* tmp = fmt_data;
					fmt_data = __W3_Concat(tmp, cbuf);
					free(tmp);
				}
			}
			free(cbuf);
		}
		sprintf(text, "%s%s%s\x1b[m", italic ? "\x1b[3m" : "", bold ? "\x1b[1m" : "", fmt_data);
		if(nl >= start) printf("%s", text);
		x += strlen(text);
		if(x >= termw) {
			if(nl >= start) {
				nl += x / termw;
				x = x % termw;
			}
		}
		fflush(stdout);
		free(text);
		free(fmt_data);
	}
}

void render_site() {
	if(databuf != NULL) {
		if(titlebuf != NULL) {
			free(titlebuf);
		}
		title = false;
		italic = false;
		bold = false;
		pre = false;
		titlebuf = NULL;
		nl = 0;
		style = false;
		if(ctype != NULL && strcasecmp(ctype, "text/html") == 0) {
			W3_Tag_Parse(databuf, datalen, html_handler, text_handler);
		} else {
			int i;
			for(i = 0; i < datalen; i++) {
				if(databuf[i] == '\n') {
					if(nl >= start && (nl - start) <= termh - 1) write(1, databuf + i, 1);
					nl++;
				} else if(nl >= start && (nl - start) <= termh - 1) {
					write(1, databuf + i, 1);
				}
			}
			titlebuf = __W3_Concat("Raw: ", allurl);
		}
		char* seq = malloc(1024);
		sprintf(seq, "\x1b[1;%dH", termw - strlen(titlebuf != NULL ? titlebuf : "No title") - 1);
#ifdef __MINGW32__
		DWORD written = 0;
		char* str = malloc(strlen(seq) + 512);
		sprintf(str, "%s\x1b[7m %s \x1b[m", seq, titlebuf != NULL ? titlebuf : "No title");
		WriteConsole(winstdout, str, strlen(str), &written, NULL);
		free(str);
#else
		printf("%s\x1b[7m %s \x1b[m", seq, titlebuf != NULL ? titlebuf : "No title");
		fflush(stdout);
#endif
		free(seq);
	}
}

int main(int argc, char** argv) {
	int i;
	databuf = __W3_Strdup("<html><head><title>Welcome to W3B</title></head><body><h1>Welcome to W3B</h1><hr>W3B is a test application for the LibW3, which works as a basic browser.</body></html>");
	ctype = __W3_Strdup("text/html");
	datalen = strlen(databuf);
	char* url = NULL;
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--version") == 0) {
			printf("LibW3 %s\n", LIBW3_VERSION);
			return 0;
		} else if(argv[i][0] == '-') {
			fprintf(stderr, "%s: unknown option: %s\n", argv[0], argv[i]);
			return 1;
		} else {
			if(url != NULL) {
				free(url);
				fprintf(stderr, "%s: garbage argument found\n", argv[0]);
				return 1;
			}
			url = __W3_Strdup(argv[i]);
		}
	}
	W3_Do_Debug(false);
	W3_Library_Init();
	int phase = 0;
	char c = 0;
	bool acc = false;
	if(url != NULL) {
		allurl = url;
		access_site(url);
		acc = true;
	}
#ifdef __MINGW32__
	winstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(winstdout, &mode);
	const DWORD origmode = mode;
	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(winstdout, mode);
#endif
	bool rendered = false;
	clear_console();
	start = 0;
	get_terminal_size(&termw, &termh);
	while(true) { /* Loop */
		if(!rendered) {
			clear_console();
			render_site();
			rendered = true;
		}
		if(c != '\n' && c != '\r') {
			printf("\x1b[%d;1H(O)pen, (Q)uit", termh);
			if(acc) {
				printf(", Go (u)p, Go (d)own");
			}
			printf("? ");
			fflush(stdout);
		}
		if((c = getchar()) == EOF) break;
		switch(tolower(c)) {
		case 'q':
			goto exitnow;
		case 'o':
			printf("URL: ");
			fflush(stdout);
			if(url != NULL) free(url);
			url = malloc(2049);
			scanf("%s", url);
			allurl = url;
			acc = false;
			break;
		case 'd':
			rendered = false;
			start += termh;
			break;
		case 'u':
			rendered = false;
			start -= termh;
			if(start < 0) start = 0;
			break;
		case '\n':
		case '\r':
			break;
		default:
			printf("What do you mean?\n");
			break;
		}
		if(!acc && url != NULL) {
			start = 0;
			access_site(url);
			acc = true;
			rendered = false;
		}
	}
	printf("\n");
exitnow:;
#ifdef __MINGW32__
	SetConsoleMode(winstdout, origmode);
#endif
}
