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

#include "W3Util.h"

#include "W3Core.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __MINGW32__
#include <windows.h>
#include <winsock2.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#endif

#ifdef SSL_SUPPORT
#include <openssl/ssl.h>
#endif

#define __DEBUG_LEN 16

bool debug_flag = true;

void W3_Do_Debug(bool do_debug) { debug_flag = do_debug; }

void __W3_Debug(const char* title, const char* message) {
	if(debug_flag) {
		char* periods = malloc(__DEBUG_LEN - strlen(title) + 1);
		periods[__DEBUG_LEN - strlen(title)] = 0;
		memset(periods, '.', __DEBUG_LEN - strlen(title));
		fprintf(stderr, "%s%s %s\n", title, periods, message);
	}
}

char* __W3_Concat(const char* str1, const char* str2) {
	char* str = malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(str, str1);
	strcpy(str + strlen(str1), str2);
	str[strlen(str1) + strlen(str2)] = 0;
	return str;
}

char* __W3_Concat3(const char* str1, const char* str2, const char* str3) {
	char* tmp = __W3_Concat(str1, str2);
	char* str = __W3_Concat(tmp, str3);
	free(tmp);
	return str;
}

char* __W3_Strdup(const char* str) {
	char* result = malloc(strlen(str) + 1);
	memcpy(result, str, strlen(str));
	result[strlen(str)] = 0;
	return result;
}

unsigned long __W3_Auto_Write(struct W3* w3, char* data, unsigned long length) {
	while(w3->writing)
		;
	w3->writing = true;
	unsigned long code;
#ifdef SSL_SUPPORT
	if(w3->ssl != NULL) {
		code = SSL_write(w3->ssl, data, length);
	} else {
		code = send(w3->sock, data, length, 0);
	}
#else
	code = send(w3->sock, data, length, 0);
#endif
	w3->writing = false;
	return code;
}

unsigned long __W3_Auto_Read(struct W3* w3, char* data, unsigned long length) {
	while(w3->reading)
		;
	w3->reading = true;
	unsigned long code;
#ifdef SSL_SUPPORT
	if(w3->ssl != NULL) {
		code = SSL_read(w3->ssl, data, length);
	} else {
		code = recv(w3->sock, data, length, 0);
	}
#else
	code = recv(w3->sock, data, length, 0);
#endif
	w3->reading = false;
	return code;
}

void* __W3_Get_Event(struct W3* w3, const char* eventname) {
	if(w3->events == NULL) return NULL;
	int i;
	for(i = 0; w3->events[i] != NULL; i += 2) {
		if(strcmp(w3->events[i], eventname) == 0) {
			return w3->events[i + 1];
		}
	}
	return NULL;
}

bool __W3_Have_Header(struct W3* w3, const char* name) {
	if(w3->headers == NULL) return false;
	int i;
	for(i = 0; w3->headers[i] != NULL; i += 2) {
		if(strcmp(w3->headers[i], name) == 0) {
			return true;
		}
	}
	return false;
}

char* __W3_Get_Platform(void) {
#ifdef __MINGW32__
	OSVERSIONINFOA info;
	ZeroMemory(&info, sizeof(OSVERSIONINFOA));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	GetVersionEx(&info);
	char* result = malloc(33);
	sprintf(result, "Windows %u.%u.%u", info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber);
	return result;
#else
	struct utsname un;
	uname(&un);
	return __W3_Concat3(un.sysname, "/", un.release);
#endif
}

bool __W3_Have_Prop(struct W3* w3, const char* name) {
	if(w3->props == NULL) return false;
	int i;
	for(i = 0; w3->props[i] != NULL; i += 2) {
		if(strcmp(w3->props[i], name) == 0) return true;
	}
	return false;
}

char* __W3_Get_Prop(struct W3* w3, const char* name) {
	if(w3->props == NULL) return NULL;
	int i;
	for(i = 0; w3->props[i] != NULL; i += 2) {
		if(strcmp(w3->props[i], name) == 0) return w3->props[i + 1];
	}
	return NULL;
}

void __W3_Add_Prop(struct W3* w3, const char* name, const char* value) {
	char* str = __W3_Concat3("Adding a prop `", name, "`");
	__W3_Debug("Prop", str);
	free(str);
	int len = 0;
	if(w3->props == NULL) {
		w3->props = malloc(sizeof(*w3->props) * 3);
	} else {
		char** oldprops = w3->props;
		for(len = 0; oldprops[len] != NULL; len++)
			;
		w3->props = malloc(sizeof(*w3->props) * (len + 3));
		for(len = 0; oldprops[len] != NULL; len++) {
			w3->props[len] = oldprops[len];
		}
		free(oldprops);
	}
	w3->props[len] = __W3_Strdup(name);
	w3->props[len + 1] = __W3_Strdup(value);
	w3->props[len + 2] = NULL;
}
