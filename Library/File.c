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

#include "W3File.h"

#include "W3Core.h"
#include "W3Util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void __W3_File_Request(struct W3* w3) {
	__W3_Debug("LibW3-File", "Sending the request");
	FILE* f = fopen(w3->path, "r");
	if(f == NULL) {
		void* funcptr = __W3_Get_Event(w3, "status");
		if(funcptr != NULL) {
			void (*func)(struct W3*, int) = (void (*)(struct W3*, int))funcptr;
			func(w3, LIBW3_FILE_NOT_FOUND);
		}
	} else {
		void* funcptr = __W3_Get_Event(w3, "status");
		if(funcptr != NULL) {
			void (*func)(struct W3*, int) = (void (*)(struct W3*, int))funcptr;
			func(w3, LIBW3_FILE_FOUND);
		}
		char* buf = malloc(w3->readsize);
		while(true) {
			int len = fread(buf, 1, w3->readsize, f);
			void* funcptr = __W3_Get_Event(w3, "data");
			if(funcptr != NULL) {
				void (*func)(struct W3*, char*, size_t) = (void (*)(struct W3*, char*, size_t))funcptr;
				char* buffer = malloc(len);
				memcpy(buffer, buf, len);
				func(w3, buffer, len);
				free(buffer);
			}
			if(feof(f)) break;
		}
		free(buf);
	}
}
