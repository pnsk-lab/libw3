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

/**
 * @file W3Core.h
 * @brief Core of LibW3
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include "W3Version.h"

/**
 * @warning
 * This struct is not intended to be used directly!
 */
struct W3 {
	int sock;
	int port;
	char* protocol;
	char* method;
	char* path;
	char* hostname;
	char** headers;
	void** events;
	int status;
	char* data;
	size_t size;
	size_t readsize;
	char** props;
	void* generic;
	char* tcl_header;
	char* tcl_data;
	char* tcl_status;
	void* tcl_in;
	void* ssl;
	void* ssl_ctx;

	bool reading;
	bool writing;
};


/**
 * @brief Initializes LibW3.
 * @return
 * - `0` if successful.
 * - `<0` if not.
 */
int W3_Library_Init(void);

/**
 * @brief Creates the W3 struct.
 * @param protocol Protocol
 * @param hostname Hostname
 * @param port Port
 * @return
 * - non-`NULL` if successful.
 * - `NULL` if not.
 */
struct W3* W3_Create(const char* protocol, const char* hostname, int port);

/**
 * @brief Sets the method, or the command name.
 * @param w3 Pointer to the struct
 * @param method Method, or command name
 */
void W3_Set_Method(struct W3* w3, const char* method);

/**
 * @brief Sets the path, or the argument.
 * @param w3 Pointer to the struct
 * @param path Path, or argument
 */
void W3_Set_Path(struct W3* w3, const char* path);

/**
 * @brief Sends the request, or just handshake.
 * @param w3 Pointer to the struct
 */
void W3_Send_Request(struct W3* w3);

/**
 * @brief Sets the header.
 * @param w3 Pointer to the struct
 * @param key Header key
 * @param value Header value
 */
void W3_Set_Header(struct W3* w3, const char* key, const char* value);

/**
 * @brief Frees the struct.
 * @param w3 Pointer to the struct
 */
void W3_Free(struct W3* w3);

/**
 * @brief Sets the event handler.
 * @param w3 Pointer to the struct
 * @param eventname Event name
 * @param func Function pointer
 */
void W3_On(struct W3* w3, const char* eventname, void* func);

/**
 * @brief Force-disconnects from the server.
 * @param w3 Pointer to the struct
 */
void W3_Disconnect(struct W3* w3);

/**
 * @brief Sets the data to be sent.
 * @param w3 Pointer to the struct
 * @param data Data
 * @param size Size of the data
 */
void W3_Set_Data(struct W3* w3, char* data, size_t size);

/**
 * @brief Sets the read buffer size.
 * @param w3 Pointer to the struct
 * @param size Size of the read buffer
 * @note This method must be called before the W3_Send_Request.
 */
void W3_Set_Read_Size(struct W3* w3, size_t size);

#ifdef __cplusplus
}
#endif

#endif
