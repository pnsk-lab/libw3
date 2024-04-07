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

#include "W3DNS.h"

#include "W3Util.h"

#include <stdlib.h>
#include <string.h>

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#ifdef SSL_SUPPORT
#include <openssl/ssl.h>
#endif

#ifndef __MINGW32__
#define ADDRINFO struct addrinfo
#endif

#ifdef __FreeBSD__
#include <netinet/in.h>
#endif

int __W3_DNS_Connect(const char* hostname, bool ssl, uint16_t port
#ifdef SSL_SUPPORT
		     ,
		     void** o_ssl, void** o_ctx
#endif
) {
	__W3_Debug("DNS-Connect", "Resolving");
	ADDRINFO hints;
	ADDRINFO* result;
	ADDRINFO* rp;
	int s;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_TCP;
	char* strport = malloc(6); /* Enough for 5 digits */
	memset(strport, 0, 6);
	sprintf(strport, "%d", port);
	s = getaddrinfo(hostname, strport, &hints, &result);
	if(s != 0) {
		free(strport);
		__W3_Debug("Resolve", "Failed");
		return -1; /* Failed to resolve */
	}
	int sock;
	for(rp = result; rp != NULL; rp = rp->ai_next) {
		sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(sock == -1) continue;
		int nbyt = 65535;
		setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&nbyt, sizeof(nbyt));
		nbyt = 65535;
		setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&nbyt, sizeof(nbyt));
		int yes = 1;
		setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&yes, sizeof(yes));
		if(connect(sock, rp->ai_addr, rp->ai_addrlen) != -1) break;
		close(sock);
	}
	freeaddrinfo(result);
	free(strport);
	if(rp == NULL) {
		__W3_Debug("Connect", "Failed to connect");
		return -1; /* Failed to connect */
	}
	__W3_Debug("Connect", "Conencted");
#ifdef SSL_SUPPORT
	if(ssl) {
		__W3_Debug("SSL", "Initializing");
#ifdef __sun__
		const SSL_METHOD* method = TLSv1_2_client_method();
#else
		const SSL_METHOD* method = TLS_client_method();
#endif
		*o_ctx = SSL_CTX_new(method);
		*o_ssl = SSL_new(*o_ctx);
		SSL_set_fd(*o_ssl, sock);
		SSL_set_tlsext_host_name(*o_ssl, hostname);
		if(SSL_connect(*o_ssl) != 1) {
			SSL_CTX_free(*o_ctx);
			SSL_free(*o_ssl);
			*o_ctx = NULL;
			*o_ssl = NULL;
			close(sock);
			sock = -1;
		} else {
			__W3_Debug("SSL", "Connected");
		}
	}
#endif
	return sock;
}
