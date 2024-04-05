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

#ifndef __W3URL_H__
#define __W3URL_H__

/**
 * @file W3URL.h
 * @brief URL parser
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief URL struct
 */
struct W3URL {
	/**
	 * @brief Protocol
	 */
	char* protocol;

	/**
	 * @brief Host
	 */
	char* host;

	/**
	 * @brief Port
	 */
	int port;

	/**
	 * @brief Path
	 */
	char* path;

	/**
	 * @brief Username
	 * @note `NULL` if the parser could not find the username.
	 */
	char* username;

	/**
	 * @brief Password
	 * @note `NULL` if the parser could not find the password.
	 */
	char* password;
};

/**
 * @brief Parses the URL.
 * @param url URL
 * @return
 * - non-`NULL` if successful.
 * - `NULL` if not.
 */
struct W3URL* W3_Parse_URL(const char* url);

/**
 * @brief Frees the struct.
 * @param url Pointer to the struct
 */
void W3_Free_URL(struct W3URL* url);

#ifdef __cplusplus
}
#endif

#endif
