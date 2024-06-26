/* $Id$ */
/* --- START LICENSE --- */
/* -------------------------------------------------------------------------- */
/*                                                  The WWW Library - LibW3   */
/*                                                  developed by Nishi        */
/*                                                  http://nishi.boats/libw3  */
/* -------------------------------------------------------------------------- */
/* Copyright (c) 2024 Nishi.                                                  */
/* Redistribution and use in source and binary forms, with or without modific */
/* ation, are permitted provided that the following conditions are met:       */
/*     1. Redistributions of source code must retain the above copyright noti */
/* ce, this list of conditions and the following disclaimer.                  */
/*     2. Redistributions in binary form must reproduce the above copyright n */
/* otice, this list of conditions and the following disclaimer in the documen */
/* tation and/or other materials provided with the distribution.              */
/*     3. Neither the name of the copyright holder nor the names of its contr */
/* ibutors may be used to endorse or promote products derived from this softw */
/* are without specific prior written permission.                             */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS */
/* " AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, TH */
/* E IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPO */
/* SE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS  */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CON */
/* SEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITU */
/* TE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPT */
/* ION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, S */
/* TRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN AN */
/* Y WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY  */
/* OF SUCH DAMAGE.                                                            */
/* -------------------------------------------------------------------------- */
/* --- END LICENSE --- */

#include "W3URL.h"

#include "W3Util.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct W3URL* W3_Parse_URL(const char* _url) {
	char* url = __W3_Strdup(_url);
	__W3_Debug("URL", "Parsing URL");
	struct W3URL* r = malloc(sizeof(*r));
	r->protocol = NULL;
	r->host = NULL;
	r->path = NULL;
	r->username = NULL;
	r->password = NULL;
	r->port = -1;
	if(strlen(url) > 3) {
		int i;
		bool found = false;
		for(i = 0; i < strlen(url) - 3; i++) {
			if(url[i] == ':' && url[i + 1] == '/' && url[i + 2] == '/') {
				found = true;
				break;
			}
		}
		if(!found) {
			__W3_Debug("URL", "Failed to parse");
			W3_Free_URL(r);
			r = NULL;
		} else {
			url[i] = 0;
			{
				char* str = malloc(strlen(url) + 64);
				sprintf(str, "Protocol is %s", url);
				__W3_Debug("URL", str);
				free(str);
			}
			r->protocol = __W3_Strdup(url);
			i += 3;
			int start = i;
			int port_start = -1;
			bool auth = false;
			int atmark = 0;
			for(; url[i] != 0; i++) {
				if(url[i] == '@') {
					auth = true;
					atmark = i;
					break;
				}
			}
			if(auth) {
				__W3_Debug("URL", "This URL has an authentication field");
				i = start;
				bool password = false;
				char* cbuf = malloc(2);
				cbuf[1] = 0;
				for(; i < atmark; i++) {
					if(url[i] == ':' && !password) {
						password = true;
						continue;
					}
					cbuf[0] = url[i];
					if(!password) {
						if(r->username == NULL) {
							r->username = malloc(1);
							r->username[0] = 0;
						}
						char* tmp = r->username;
						r->username = __W3_Concat(tmp, cbuf);
						free(tmp);
					} else {
						if(r->password == NULL) {
							r->password = malloc(1);
							r->password[0] = 0;
						}
						char* tmp = r->password;
						r->password = __W3_Concat(tmp, cbuf);
						free(tmp);
					}
				}
				free(cbuf);
				if(r->username != NULL) {
					char* str = malloc(64 + strlen(r->username));
					sprintf(str, "Username is %s", r->username);
					__W3_Debug("URL", str);
					free(str);
				}
				if(r->password != NULL) {
					char* str = malloc(64 + strlen(r->password));
					sprintf(str, "Password is %s", r->password);
					__W3_Debug("URL", str);
					free(str);
				}
				i = atmark + 1;
			} else {
				i = start;
			}
			for(; url[i] != 0; i++) {
				if(url[i] == '/') {
					r->path = __W3_Strdup(url + i);
					url[i] = 0;
					break;
				} else if(url[i] == ':') {
					port_start = i + 1;
					url[i] = 0;
				}
			}
			if(port_start != -1) {
				r->port = atoi(url + port_start);
			}
			if(r->port == -1) {
				if(strcmp(r->protocol, "http") == 0) {
					r->port = 80;
				} else if(strcmp(r->protocol, "https") == 0) {
					r->port = 443;
				} else if(strcmp(r->protocol, "gopher") == 0 || strcmp(r->protocol, "gophers") == 0) {
					r->port = 70;
				} else if(strcmp(r->protocol, "gemini") == 0) {
					r->port = 1965;
				} else if(strcmp(r->protocol, "finger") == 0) {
					r->port = 79;
				} else if(strcmp(r->protocol, "nex") == 0) {
					r->port = 1900;
				} else if(strcmp(r->protocol, "ftp") == 0) {
					r->port = 21;
				} else if(strcmp(r->protocol, "nntp") == 0 || strcmp(r->protocol, "news") == 0) {
					r->port = 119;
				} else if(strcmp(r->protocol, "irc") == 0) {
					r->port = 6667;
				} else if(strcmp(r->protocol, "ircs") == 0) {
					r->port = 6697;
				} else if(strcmp(r->protocol, "spartan") == 0) {
					r->port = 300;
				}
			}
			r->host = __W3_Strdup(url + start + (atmark == 0 ? 0 : (atmark - 1)));
			char* str = malloc(strlen(r->host) + 64);
			sprintf(str, "Host is %s", r->host);
			__W3_Debug("URL", str);
			free(str);
			if(r->path == NULL) r->path = __W3_Strdup("/");
			str = malloc(strlen(r->path) + 64);
			sprintf(str, "Path is %s", r->path);
			__W3_Debug("URL", str);
			free(str);
			str = malloc(64);
			sprintf(str, "Port is %d", r->port);
			__W3_Debug("URL", str);
			free(str);
		}
	} else {
		W3_Free_URL(r);
		r = NULL;
	}
	return r;
}

void W3_Free_URL(struct W3URL* url) {
	if(url == NULL) return;
	if(url->protocol != NULL) free(url->protocol);
	if(url->host != NULL) free(url->host);
	if(url->path != NULL) free(url->path);
	if(url->username != NULL) free(url->username);
	if(url->password != NULL) free(url->password);
	free(url);
}
