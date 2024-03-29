/* $Id$ */
/* START LICENSE */
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
/* END LICENSE */

#include "W3NNTP.h"

#include "W3Core.h"
#include "W3DNS.h"
#include "W3Util.h"

#include <stdlib.h>
#include <string.h>

#ifndef __MINGW32__
extern int strcasecmp(const char* s1, const char* s2);
#endif

void __W3_NNTP_Request(struct W3* w3) {
	__W3_Debug("LibW3-NNTP", "Sending the request");
	char* buf = malloc(w3->readsize);
	char* cbuf = malloc(2);
	char* line = malloc(1);
	line[0] = 0;
	cbuf[1] = 0;
	int flag = 0;
	w3->generic = &flag;
	char* data = malloc(1);
	data[0] = 0;
	while(true) {
		int len = __W3_Auto_Read(w3, buf, w3->readsize);
		if(len <= 0) break;
		int i;
		for(i = 0; i < len; i++) {
			if(buf[i] == '\n') {
				int sendstatus = -1;
				char* senddata = NULL;
				bool freedata = false;
				if(flag & 1) {
					if(strcmp(line, ".") == 0) {
						flag &= ~1;
						int j;
						for(j = 0; data[j] != 0; j++) {
							if(data[j] == ' ') {
								data[j] = 0;
								sendstatus = atoi(data);
								senddata = data + j + 1;
								break;
							}
						}
						freedata = true;
					} else {
						char* tmp = data;
						data = __W3_Concat3(tmp, strlen(data) != 0 ? "\n" : "", line);
						free(tmp);
					}
				} else {
					int j;
					for(j = 0; line[j] != 0; j++) {
						if(line[j] == ' ') {
							line[j] = 0;
							sendstatus = atoi(line);
							senddata = line + j + 1;
							break;
						}
					}
				}
				if(sendstatus != -1) {
					void* funcptr = __W3_Get_Event(w3, "nntpresp");
					if(funcptr != NULL) {
						void (*func)(struct W3*, int, char*) = (void (*)(struct W3*, int, char*))funcptr;
						func(w3, sendstatus, senddata);
					}
				}
				if(freedata) {
					free(data);
					data = malloc(1);
					data[0] = 0;
				}
				free(line);
				line = malloc(1);
				line[0] = 0;
			} else if(buf[i] != '\r') {
				cbuf[0] = buf[i];
				char* tmp = line;
				line = __W3_Concat(tmp, cbuf);
				free(tmp);
			}
		}
	}
	free(data);
	free(line);
	free(cbuf);
	free(buf);
}

void W3_NNTP_Send_Request(struct W3* w3) {
	if(strcasecmp(w3->method, "LIST") == 0) {
		(*(int*)w3->generic) |= 1;
		__W3_Auto_Write(w3, "LIST\r\n", 6);
	} else if(strcasecmp(w3->method, "HEAD") == 0) {
		(*(int*)w3->generic) |= 1;
		__W3_Auto_Write(w3, "HEAD\r\n", 6);
	} else if(strcasecmp(w3->method, "BODY") == 0) {
		(*(int*)w3->generic) |= 1;
		__W3_Auto_Write(w3, "BODY\r\n", 6);
	} else if(strcasecmp(w3->method, "NEXT") == 0) {
		__W3_Auto_Write(w3, "NEXT\r\n", 6);
	} else if(strcasecmp(w3->method, "GROUP") == 0) {
		__W3_Auto_Write(w3, "GROUP ", 6);
		__W3_Auto_Write(w3, w3->path, strlen(w3->path));
		__W3_Auto_Write(w3, "\r\n", 2);
	}
}

void W3_NNTP_Disconnect(struct W3* w3) { __W3_Auto_Write(w3, "QUIT\r\n", 6); }
