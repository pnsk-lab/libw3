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

#ifndef __W3CORE_H__
#define __W3CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include "W3Version.h"

struct W3 {
	int sock;	  /* Socket */
	int port;	  /* Port */
	char* protocol;	  /* As you can read from its name */
	char* method;	  /* Used in HTTP	*/
	char* path;	  /* As you can read from its name */
	char* hostname;	  /* As you can read from its name */
	char** headers;	  /* As you can read from its name */
	void** events;	  /* As you can read from its name */
	int status;	  /* As you can read from its name */
	char* data;	  /* As you can read from its name */
	size_t size;	  /* Size of the data */
	size_t readsize;  /* Read buffer size, default is 512 */
	char** props;	  /* Properties */
	void* generic;	  /* Depends on the protocol */
	char* tcl_header; /* Used in Tcl */
	char* tcl_data;	  /* Used in Tcl */
	char* tcl_status; /* Used in Tcl */
	void* tcl_in;	  /* Used in Tcl */
	void* ssl;	  /* Actually SSL*, NULL if no SSL */
	void* ssl_ctx;	  /* Actually SSL_CTX* */

	bool reading;
	bool writing;
};

int W3_Library_Init(void);						    /* Initialize the Library */
struct W3* W3_Create(const char* protocol, const char* hostname, int port); /* Create the struct */

void W3_Set_Method(struct W3* w3, const char* method);		       /* Set the method */
void W3_Set_Path(struct W3* w3, const char* path);		       /* Set the path */
void W3_Send_Request(struct W3* w3);				       /* Send the request */
void W3_Set_Header(struct W3* w3, const char* key, const char* value); /* Set the header */
void W3_Free(struct W3* w3);					       /* Free the struct */
void W3_On(struct W3* w3, const char* eventname, void* func);	       /* Set Handlers */
void W3_Disconnect(struct W3* w3);				       /* Disconnect */
void W3_Set_Data(struct W3* w3, char* data, size_t size);	       /* Send the data - LibW3 won't free the data */
void W3_Set_Read_Size(struct W3* w3, size_t size);		       /* Change the read buffer size */

#ifdef __cplusplus
}
#endif

#endif
