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

#ifndef __W3POP3_H__
#define __W3POP3_H__

/**
 * @file W3POP3.h
 * @~english
 * @brief POP3 part of LibW3
 * @deprecated Nishi might abandon this
 *
 * @~japanese
 * @brief LibW3のPOP3処理部分
 * @deprecated Nishiが廃止するかもしれません
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "W3Core.h"

void __W3_POP3_Request(struct W3* w3);

/**
 * @~english
 * @brief Sets the username of the POP3 connection.
 * @param w3 Pointer to the struct
 * @param username POP3 username
 *
 * @~japanese
 * @brief POP3接続のユーザー名を設定します。
 * @param w3 構造体へのポインター
 * @param username POP3のユーザー名
 *
 */
void W3_POP3_Set_Username(struct W3* w3, const char* username);

/**
 * @~english
 * @brief Sets the password of the POP3 connection.
 * @param w3 Pointer to the struct
 * @param password POP3 password
 *
 * @~japanese
 * @brief POP3接続のパスワードを設定します。
 * @param w3 構造体へのポインター
 * @param password POP3のパスワード
 *
 */
void W3_POP3_Set_Password(struct W3* w3, const char* password);

/**
 * @~english
 * @brief Sends the POP3 command.
 * @param w3 Pointer to the struct
 * @note This function is different with W3_Send_Request!
 * @note When W3_Send_Request just handshakes, this function sends the POP3 command!
 *
 * @~japanese
 * @brief POP3のコマンドを送ります。
 * @param w3 構造体へのポインター
 * @note この関数は W3_Send_Request とは違います！
 * @note W3_Send_Request は単純にハンドシェイクするだけですが、この関数はPOP3のコマンドを送ります！
 *
 */
void W3_POP3_Send_Request(struct W3* w3);

/**
 * @~english
 * @brief Disconnects from the POP3.
 * @param w3 Pointer to the struct
 *
 * @~japanese
 * @brief POP3から切断します。
 * @param w3 構造体へのポインター
 *
 */
void W3_POP3_Disconnect(struct W3* w3);

#ifdef __cplusplus
}
#endif

#endif
