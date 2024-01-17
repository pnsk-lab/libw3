/* $Id$ */
#include "W3HTTP.h"

#include "W3Core.h"
#include "W3Util.h"

#include <string.h>

void __W3_HTTP_Request(struct W3* w3){
	__W3_Debug("LibW3-HTTP", "Sending the request");
	__W3_Auto_Write(w3, w3->method, strlen(w3->method));
	__W3_Auto_Write(w3, " ", 1);
	__W3_Auto_Write(w3, w3->path, strlen(w3->path));
	__W3_Auto_Write(w3, " ", 1);
	__W3_Auto_Write(w3, "HTTP/1.1", 8);
	__W3_Auto_Write(w3, "\r\n", 2);
	__W3_Auto_Write(w3, "Host: ", 6);
	__W3_Auto_Write(w3, w3->hostname, strlen(w3->hostname));
	__W3_Auto_Write(w3, "\r\n", 2);
	if(!__W3_Have_Header(w3, "user-agent")){
		__W3_Auto_Write(w3, "User-Agent: ", 12);
		__W3_Auto_Write(w3, "LibW3/", 6);
		__W3_Auto_Write(w3, LIBW3_VERSION, strlen(LIBW3_VERSION));
		char* platform = __W3_Get_Platform();
		__W3_Auto_Write(w3, " (", 2);
		__W3_Auto_Write(w3, platform, strlen(platform));
		free(platform);
		__W3_Auto_Write(w3, ")", 1);
		__W3_Auto_Write(w3, "\r\n", 2);
	}
	__W3_Auto_Write(w3, "Connection: ", 12);
	__W3_Auto_Write(w3, "closed", 6);
	__W3_Auto_Write(w3, "\r\n", 2);
	if(w3->headers != NULL){
		int i;
		for(i = 0; w3->headers[i] != NULL; i += 2){
			if(strcmp(w3->headers[i], "connection") == 0) continue;
			__W3_Auto_Write(w3, w3->headers[i], strlen(w3->headers[i]));
			__W3_Auto_Write(w3, ": ", 2);
			__W3_Auto_Write(w3, w3->headers[i + 1], strlen(w3->headers[i + 1]));
			__W3_Auto_Write(w3, "\r\n", 2);
		}
	}
	__W3_Auto_Write(w3, "\r\n", 2);
	__W3_Auto_Write(w3, "\r\n", 2);
}
