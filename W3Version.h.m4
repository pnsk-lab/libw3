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
changequote(`{{', `}}')

#ifndef __W3VERSION_H__
#define __W3VERSION_H__

/**
 * @file W3Version.h
 * @~english
 * @brief LibW3 info
 *
 * @~japanese
 * @brief LibW3の情報
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

const char* W3_Get_Version(void);

/**
 * @~english
 * @brief Version of LibW3
 * @note Consider using W3_Get_Version.
 *
 * @~japanese
 * @brief LibW3のバージョン
 * @note W3_Get_Version を使用することを検討してください。
 *
 */
#define LIBW3_VERSION "2.22F" \
SUFFIX

ifdef({{HTTP_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if HTTP support is enabled
 *
 * @~japanese
 * @brief HTTPサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_HTTP_SUPPORT}}, {{}})

ifdef({{FTP_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if FTP support is enabled
 *
 * @~japanese
 * @brief FTPサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_FTP_SUPPORT}}, {{}})

ifdef({{GOPHER_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if Gopher suppport is enabled
 *
 * @~japanese
 * @brief Gopherサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_GOPHER_SUPPORT}}, {{}})

ifdef({{GEMINI_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if Gemini support is enabled
 *
 * @~japanese
 * @brief Geminiサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_GEMINI_SUPPORT}}, {{}})

ifdef({{FINGER_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if Finger support is enabled
 *
 * @~japanese
 * @brief Fingerサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_FINGER_SUPPORT}}, {{}})

ifdef({{NEX_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if Nex support is enabled
 *
 * @~japanese
 * @brief Nexサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_NEX_SUPPORT}}, {{}})

ifdef({{FILE_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if File support is enabled
 *
 * @~japanese
 * @brief Fileサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_FILE_SUPPORT}}, {{}})

ifdef({{IRC_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if IRC support is enabled
 *
 * @~japanese
 * @brief IRCサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_IRC_SUPPORT}}, {{}})

ifdef({{NNTP_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if NNTP support is enabled
 *
 * @~japanese
 * @brief NNTPサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_NNTP_SUPPORT}}, {{}})

ifdef({{SPARTAN_SUPPORT}}, {{/**
 * @~english
 * @brief Defined if Spartan support is enabled
 *
 * @~japanese
 * @brief Spartanサポートが有効化されてる場合に定義
 *
 */
#define LIBW3_SPARTAN_SUPPORT}}, {{}})

#ifdef __cplusplus
}
#endif

#endif
