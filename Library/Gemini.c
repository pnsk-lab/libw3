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

#include "W3Gemini.h"

#include "W3Util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void __W3_Gemini_Request(struct W3* w3) {
	__W3_Debug("LibW3-Gemini", "Sending the request");
	__W3_Auto_Write(w3, "gemini://", 9);
	__W3_Auto_Write(w3, w3->hostname, strlen(w3->hostname));
	char* portstr = malloc(10);
	memset(portstr, 0, 10);
	sprintf(portstr, ":%d", w3->port);
	__W3_Auto_Write(w3, portstr, strlen(portstr));
	__W3_Auto_Write(w3, w3->path, strlen(w3->path));
	__W3_Auto_Write(w3, "\r\n", 2);
	char* buf = malloc(w3->readsize);
	bool status = true;
	char* code = malloc(1);
	code[0] = 0;
	char* meta = malloc(1);
	meta[0] = 0;
	bool bcode = true;
	while(true) {
		int len = __W3_Auto_Read(w3, buf, w3->readsize);
		if(len <= 0) break;
		int i = 0;
		if(status) {
			for(i = 0; i < len; i++) {
				if(buf[i] == '\n') {
					status = false;
					break;
				} else if(buf[i] == '\r') {
					if(!bcode) {
						if(atoi(code) == 20) {
							void* funcptr = __W3_Get_Event(w3, "header");
							if(funcptr != NULL) {
								void (*func)(struct W3*, char*, char*) = (void (*)(struct W3*, char*, char*))funcptr;
								func(w3, "Content-Type", meta);
							}
						} else if(atoi(code) >= 30 && atoi(code) < 40) {
							void* funcptr = __W3_Get_Event(w3, "header");
							if(funcptr != NULL) {
								void (*func)(struct W3*, char*, char*) = (void (*)(struct W3*, char*, char*))funcptr;
								func(w3, "Location", meta);
							}
						}
					}
				} else if(!bcode) {
					char* tmp = meta;
					char* cbuf = malloc(2);
					cbuf[0] = buf[i];
					cbuf[1] = 0;
					meta = __W3_Concat(tmp, cbuf);
					free(tmp);
					free(cbuf);
				} else if(bcode) {
					if(buf[i] == ' ') {
						bcode = false;
						void* funcptr = __W3_Get_Event(w3, "status");
						if(funcptr != NULL) {
							void (*func)(struct W3*, int) = (void (*)(struct W3*, int))funcptr;
							func(w3, atoi(code));
						}
					} else {
						char* tmp = code;
						char* cbuf = malloc(2);
						cbuf[0] = buf[i];
						cbuf[1] = 0;
						code = __W3_Concat(tmp, cbuf);
						free(tmp);
						free(cbuf);
					}
				}
			}
			i++;
		}
		void* funcptr = __W3_Get_Event(w3, "data");
		if(funcptr != NULL) {
			void (*func)(struct W3*, char*, size_t) = (void (*)(struct W3*, char*, size_t))funcptr;
			func(w3, buf + i, len - i);
		}
	}
	free(meta);
	free(code);
	free(buf);
}
