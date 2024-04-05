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

#ifndef __W3VERSION_H__
#define __W3VERSION_H__

/**
 * @file W3Version.h
 * @brief LibW3 info
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Version of LibW3, but consider doing extern const char* W3_Version;
 */
#define LIBW3_VERSION "2.21B" \
SUFFIX

ifdef(`HTTP_SUPPORT', `/**
 * Defined if HTTP support is enabled
 */
#define LIBW3_HTTP_SUPPORT', `')
ifdef(`FTP_SUPPORT', `/**
 * Defined if FTP support is enabled
 */
#define LIBW3_FTP_SUPPORT', `')
ifdef(`GOPHER_SUPPORT', `/**
 * Defined if Gopher suppport is enabled
 */
#define LIBW3_GOPHER_SUPPORT', `')
ifdef(`GEMINI_SUPPORT', `/**
 * Defined if Gemini support is enabled
 */
#define LIBW3_GEMINI_SUPPORT', `')
ifdef(`FINGER_SUPPORT', `/**
 * Defined if Finger support is enabled
 */
#define LIBW3_FINGER_SUPPORT', `')
ifdef(`NEX_SUPPORT', `/**
 * Defined if Nex support is enabled
 */
#define LIBW3_NEX_SUPPORT', `')
ifdef(`FILE_SUPPORT', `/**
 * Defined if File support is enabled
 */
#define LIBW3_FILE_SUPPORT', `')
ifdef(`IRC_SUPPORT', `/**
 * Defined if IRC support is enabled
 */
#define LIBW3_IRC_SUPPORT', `')
ifdef(`NNTP_SUPPORT', `/**
 * Defined if NNTP support is enabled
 */
#define LIBW3_NNTP_SUPPORT', `')
ifdef(`SPARTAN_SUPPORT', `/**
 * Defined if Spartan support is enabled
 */
#define LIBW3_SPARTAN_SUPPORT', `')

#ifdef __cplusplus
}
#endif

#endif
