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

#ifndef __W3TAG_H__
#define __W3TAG_H__

/**
 * @file W3Tag.h
 * @~english
 * @brief Tag parser
 *
 * @~japanese
 * @brief タグのパーサー
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * @~english
 * @brief Parses the data.
 * @param data Data to be parsed
 * @param size Size of the data
 * @param tagfunc Function to be called when the parser hits a tag
 * @param tagfunc.tagname Tag name
 * @param tagfunc.attr Tag attributes
 * @param textfunc Function to be called when the parser hits a text
 * @param textfunc.data Text
 *
 * @~japanese
 * @brief データをパースします。
 * @param data パースされるデータ
 * @param size データのサイズ
 * @param tagfunc パーサーがタグを見つけた時に呼ばれる関数ポインター
 * @param tagfunc.tagname タグ名
 * @param tagfunc.attr タグの属性
 * @param textfunc パーサーがテキストを見つけた時に呼ばれる関数ポインター
 * @param textfunc.data テキスト
 *
 */
void W3_Tag_Parse(char* data, size_t size, void (*tagfunc)(char* tagname, char* attr), void (*textfunc)(char* data));

/**
 * @~english
 * @brief Gets the attribute.
 * @param data Data to be parsed
 * @param name Attribute name
 * @return
 * - non-`NULL` if it could find the attribute
 * - `NULL` if it couldn't
 *
 * @~japanese
 * @brief 属性を取得します。
 * @param data パースされるデータ
 * @param name 取得したい属性名
 * @return
 * - 見つかった場合は`NULL`以外。
 * - それ以外の場合は`NULL`。
 *
 */
char* W3_Tag_Attr(char* data, const char* name);

#ifdef __cplusplus
}
#endif

#endif
