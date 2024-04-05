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

#ifndef __W3UTIL_H__
#define __W3UTIL_H__

/**
 * @file W3Util.h
 * @brief LibW3 utilities
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "W3Core.h"

#include <stdbool.h>

/**
 * @brief Turn on/off debugging.
 * @param do_debug Do debugging or not
 */
void W3_Do_Debug(bool do_debug);
void __W3_Debug(const char* title, const char* message);
char* __W3_Concat(const char* str1, const char* str2);
char* __W3_Concat3(const char* str1, const char* str2, const char* str3);
char* __W3_Strdup(const char* str);
unsigned long __W3_Auto_Write(struct W3* w3, char* data, unsigned long length);
unsigned long __W3_Auto_Read(struct W3* w3, char* data, unsigned long length);
bool __W3_Have_Header(struct W3* w3, const char* name);
bool __W3_Have_Prop(struct W3* w3, const char* name);
void __W3_Add_Prop(struct W3* w3, const char* name, const char* value);
char* __W3_Get_Prop(struct W3* w3, const char* name);
void* __W3_Get_Event(struct W3* w3, const char* eventname);
char* __W3_Get_Platform(void);

#ifdef __cplusplus
}
#endif

#endif
