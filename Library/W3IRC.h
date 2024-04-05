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

#ifndef __W3IRC_H__
#define __W3IRC_H__

/**
 * @file W3IRC.h
 * @brief IRC part of LibW3
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "W3Core.h"

void __W3_IRC_Request(struct W3* w3);

/**
 * @brief Sets the username of the IRC connection.
 * @param w3 Pointer to the struct
 * @param username IRC username
 */
void W3_IRC_Set_Username(struct W3* w3, const char* username);

/**
 * @brief Sets the hostname of the IRC connection.
 * @param w3 Pointer to the struct
 * @param hostname IRC hostname
 */
void W3_IRC_Set_Hostname(struct W3* w3, const char* hostname);

/**
 * @brief Sets the realname of the IRC connection.
 * @param w3 Pointer to the struct
 * @param realname IRC realname
 */
void W3_IRC_Set_Realname(struct W3* w3, const char* realname);

/**
 * @brief Sets the servername of the IRC connection.
 * @param w3 Pointer to the struct
 * @param servername IRC servername
 */
void W3_IRC_Set_Servername(struct W3* w3, const char* servername);

/**
 * @brief Sets the nickname of the IRC connection.
 * @param w3 Pointer to the struct
 * @param nickname IRC nickname
 */
void W3_IRC_Set_Nickname(struct W3* w3, const char* nickname);

/**
 * @brief Sets the password of the IRC connection.
 * @param w3 Pointer to the struct
 * @param password IRC password
 */
void W3_IRC_Set_Password(struct W3* w3, const char* password);

/**
 * @brief Sends the IRC command.
 * @param w3 Pointer to the struct
 * @note This function is different with W3_Send_Request!
 * @note When W3_Send_Request just handshakes, this function sends the IRC command!
 */
void W3_IRC_Send_Request(struct W3* w3);

/**
 * @brief Disconnects from the IRC.
 * @param w3 Pointer to the struct
 */
void W3_IRC_Disconnect(struct W3* w3);

#ifdef __cplusplus
}
#endif

#endif
