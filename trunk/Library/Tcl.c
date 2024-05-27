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

#include "W3Tcl.h"

#include <tcl.h>

#include "W3Core.h"
#include "W3Util.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

extern int strcasecmp(const char* s1, const char* s2);

void** __dictionary;
void** __files;

void tcl_w3_data_handler(struct W3* w3, char* data, size_t size) {
	int i;
	for(i = 0; __dictionary[i] != NULL; i += 2) {
		if(__dictionary[i + 1] == w3) {
			if(w3->tcl_data != NULL) {
				Tcl_ObjSetVar2(w3->tcl_in, Tcl_NewStringObj("event_size", 10), NULL, Tcl_NewLongObj(size), 0);
				char* data1 = malloc(size + 1);
				memcpy(data1, data, size);
				data1[size] = 0;
				Tcl_ObjSetVar2(w3->tcl_in, Tcl_NewStringObj("event_data_nulend", 17), NULL, Tcl_NewStringObj(data1, size + 1), 0);
				if(Tcl_Eval(w3->tcl_in, w3->tcl_data) == TCL_ERROR) {
					const char* errmsg = Tcl_GetStringResult(w3->tcl_in);
					fprintf(stderr, "Tcl error: %s\n", errmsg);
				}
				free(data1);
				Tcl_UnsetVar(w3->tcl_in, "event_data_nulend", 0);
				Tcl_UnsetVar(w3->tcl_in, "event_size", 0);
			}
		}
	}
}

void tcl_w3_header_handler(struct W3* w3, char* key, char* value) {
	int i;
	for(i = 0; __dictionary[i] != NULL; i += 2) {
		if(__dictionary[i + 1] == w3) {
			if(w3->tcl_header != NULL) {
				Tcl_ObjSetVar2(w3->tcl_in, Tcl_NewStringObj("event_key", 9), NULL, Tcl_NewStringObj(key, strlen(key) + 1), 0);
				Tcl_ObjSetVar2(w3->tcl_in, Tcl_NewStringObj("event_value", 11), NULL, Tcl_NewStringObj(value, strlen(value) + 1), 0);
				if(Tcl_Eval(w3->tcl_in, w3->tcl_header) == TCL_ERROR) {
					const char* errmsg = Tcl_GetStringResult(w3->tcl_in);
					fprintf(stderr, "Tcl error: %s\n", errmsg);
				}
				Tcl_UnsetVar(w3->tcl_in, "event_value", 0);
				Tcl_UnsetVar(w3->tcl_in, "event_key", 0);
			}
		}
	}
}

void tcl_w3_status_handler(struct W3* w3, int status) {
	int i;
	for(i = 0; __dictionary[i] != NULL; i += 2) {
		if(__dictionary[i + 1] == w3) {
			if(w3->tcl_status != NULL) {
				char* status_text = Tcl_Alloc(17);
				sprintf(status_text, "%d", status);
				Tcl_ObjSetVar2(w3->tcl_in, Tcl_NewStringObj("event_status", 12), NULL, Tcl_NewStringObj(status_text, strlen(status_text) + 1), 0);
				if(Tcl_Eval(w3->tcl_in, w3->tcl_status) == TCL_ERROR) {
					const char* errmsg = Tcl_GetStringResult(w3->tcl_in);
					fprintf(stderr, "Tcl error: %s\n", errmsg);
				}
				Tcl_UnsetVar(w3->tcl_in, "event_status", 0);
				Tcl_Free(status_text);
			}
		}
	}
}

int Tcl_W3FIOCmd(ClientData dummy, Tcl_Interp* interp, int objc, Tcl_Obj* CONST objv[]) {
	if(objc < 2) {
		const char* errmsg = "argument error: arguments should be \"w3fio subcommand arg1 arg2 ...\"";
		Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		return TCL_ERROR;
	}
	char* subcommand = Tcl_GetString(objv[1]);
	if(strcasecmp(subcommand, "open") == 0) {
		if(objc != 5) {
			const char* errmsg = "argument error: arguments should be \"w3fio open name filename mode\"";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		}
		char* name = Tcl_GetString(objv[2]);
		FILE* fp = fopen(Tcl_GetString(objv[3]), Tcl_GetString(objv[4]));
		if(fp == NULL) {
			const char* errmsg = "connect error: failed to open";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		} else {
			int len = 0;
			if(__files == NULL) {
				__files = malloc(sizeof(*__files) * (len + 3));
				__files[len] = __W3_Strdup(name);
				__files[len + 1] = fp;
				int i;
				for(i = 0; ((char*)__files[len])[i] != 0; i++) {
					((char*)__files[len])[i] = tolower(((char*)__files[len])[i]);
				}
				__files[len + 2] = NULL;
			} else {
				for(len = 0; __files[len] != NULL; len++)
					;
				void** __files2 = __files;
				__files = malloc(sizeof(*__files) * (len + 3));
				for(len = 0; __files2[len] != NULL; len++) {
					__files[len] = __files2[len];
				}
				__files[len] = __W3_Strdup(name);
				__files[len + 1] = fp;
				__files[len + 2] = NULL;
				int i;
				for(i = 0; ((char*)__files[len])[i] != 0; i++) {
					((char*)__files[len])[i] = tolower(((char*)__files[len])[i]);
				}
				free(__files2);
			}
			Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
		}
	} else if(strcasecmp(subcommand, "write") == 0) {
		if(objc != 5) {
			const char* errmsg = "argument error: arguments should be \"w3fio write name data size\"";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		}
		char* name = Tcl_GetString(objv[2]);
		if(__files == NULL) {
			const char* errmsg = "argument error: a file handler named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		} else {
			int i;
			for(i = 0; __files[i] != NULL; i += 2) {
				if(strcasecmp((char*)__files[i], name) == 0) {
					fwrite(objv[3]->bytes, atoi(Tcl_GetString(objv[4])), 1, __files[i + 1]);
					Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
					return TCL_OK;
				}
			}
			const char* errmsg = "argument error: a file handler named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		}
	} else if(strcasecmp(subcommand, "close") == 0) {
		if(objc != 3) {
			const char* errmsg = "argument error: arguments should be \"w3fio close name\"";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		}
		char* name = Tcl_GetString(objv[2]);
		if(__files == NULL) {
			const char* errmsg = "argument error: a file handler named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		} else {
			int i;
			int except = -1;
			int len = 0;
			for(i = 0; __files[i] != NULL; i += 2) {
				if(strcasecmp((char*)__files[i], name) == 0) {
					fclose(__files[i + 1]);
					free(__files[i]);
					except = i;
					break;
				}
			}
			for(len = 0; __files[len] != NULL; len++)
				;
			if(except != -1) {
				void** oldfiles = __files;
				__files = malloc(sizeof(*__files) * (len - 1));
				int incr = 0;
				for(i = 0; oldfiles[i] != NULL; i += 2) {
					if(except == i) continue;
					__files[incr] = oldfiles[incr];
					__files[incr + 1] = oldfiles[incr + 1];
					incr += 2;
				}
				__files[incr] = NULL;
				free(oldfiles);
				Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
				return TCL_OK;
			}
			const char* errmsg = "argument error: a file handler named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		}
	} else {
		const char* errmsg = "argument error: invalid subcommand";
		Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		return TCL_ERROR;
	}
	return TCL_OK;
}

int Tcl_W3Cmd(ClientData dummy, Tcl_Interp* interp, int objc, Tcl_Obj* CONST objv[]) {
	if(objc < 2) {
		const char* errmsg = "argument error: arguments should be \"w3 subcommand arg1 arg2 ...\"";
		Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		return TCL_ERROR;
	}
	char* subcommand = Tcl_GetString(objv[1]);
	if(strcasecmp(subcommand, "create") == 0) {
		if(objc != 6) {
			const char* errmsg = "argument error: arguments should be \"w3 create name protocol hostname port\"";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		}
		char* name = Tcl_GetString(objv[2]);
		if(__dictionary != NULL) {
			int i;
			for(i = 0; __dictionary[i] != NULL; i += 2) {
				if(strcasecmp(__dictionary[i], name) == 0) {
					const char* errmsg = "argument error: a client named that already exists";
					Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
					return TCL_ERROR;
				}
			}
		}
		char* protocol = Tcl_GetString(objv[3]);
		char* hostname = Tcl_GetString(objv[4]);
		int port = atoi(Tcl_GetString(objv[5]));
		struct W3* w3 = W3_Create(protocol, hostname, port);
		w3->tcl_in = interp;
		w3->tcl_data = NULL;
		if(w3 == NULL) {
			const char* errmsg = "connect error: failed to connect";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		} else {
			int len = 0;
			if(__dictionary == NULL) {
				__dictionary = malloc(sizeof(*__dictionary) * (len + 3));
				__dictionary[len] = __W3_Strdup(name);
				__dictionary[len + 1] = w3;
				int i;
				for(i = 0; ((char*)__dictionary[len])[i] != 0; i++) {
					((char*)__dictionary[len])[i] = tolower(((char*)__dictionary[len])[i]);
				}
				__dictionary[len + 2] = NULL;
			} else {
				for(len = 0; __dictionary[len] != NULL; len++)
					;
				void** __dictionary2 = __dictionary;
				__dictionary = malloc(sizeof(*__dictionary) * (len + 3));
				for(len = 0; __dictionary2[len] != NULL; len++) {
					__dictionary[len] = __dictionary2[len];
				}
				__dictionary[len] = __W3_Strdup(name);
				__dictionary[len + 1] = w3;
				__dictionary[len + 2] = NULL;
				int i;
				for(i = 0; ((char*)__dictionary[len])[i] != 0; i++) {
					((char*)__dictionary[len])[i] = tolower(((char*)__dictionary[len])[i]);
				}
				free(__dictionary2);
			}
			Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
		}
	} else if(strcasecmp(subcommand, "set_method") == 0 || strcasecmp(subcommand, "set_path") == 0) {
		if(objc != 4) {
			if(strcasecmp(subcommand, "set_method") == 0) {
				const char* errmsg = "argument error: arguments should be \"w3 set_method name method\"";
				Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			} else {
				const char* errmsg = "argument error: arguments should be \"w3 set_path name path\"";
				Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			}
			return TCL_ERROR;
		}
		char* name = Tcl_GetString(objv[2]);
		if(__dictionary == NULL) {
			const char* errmsg = "argument error: a client named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		} else {
			int i;
			for(i = 0; __dictionary[i] != NULL; i += 2) {
				if(strcasecmp((char*)__dictionary[i], name) == 0) {
					if(strcasecmp(subcommand, "set_method") == 0) {
						W3_Set_Method(__dictionary[i + 1], Tcl_GetString(objv[3]));
					} else {
						W3_Set_Path(__dictionary[i + 1], Tcl_GetString(objv[3]));
					}
					Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
					return TCL_OK;
				}
			}
			const char* errmsg = "argument error: a client named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		}
	} else if(strcasecmp(subcommand, "set_data_handler") == 0 || strcasecmp(subcommand, "set_header_handler") == 0 || strcasecmp(subcommand, "set_status_handler") == 0) {
		if(objc != 4) {
			const char* errmsg = "argument error: arguments should be \"w3 set_*_handler name handler\"";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		}
		char* name = Tcl_GetString(objv[2]);
		char* handler = Tcl_GetString(objv[3]);
		if(__dictionary == NULL) {
			const char* errmsg = "argument error: a client named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		} else {
			int i;
			for(i = 0; __dictionary[i] != NULL; i += 2) {
				if(strcasecmp((char*)__dictionary[i], name) == 0) {
					if(strcasecmp(subcommand, "set_header_handler") == 0) {
						if(((struct W3*)__dictionary[i + 1])->tcl_header != NULL) free(((struct W3*)__dictionary[i + 1])->tcl_header);
						((struct W3*)__dictionary[i + 1])->tcl_header = __W3_Strdup(handler);
						W3_On(__dictionary[i + 1], "header", tcl_w3_header_handler);
					} else if(strcasecmp(subcommand, "set_status_handler") == 0) {
						if(((struct W3*)__dictionary[i + 1])->tcl_status != NULL) free(((struct W3*)__dictionary[i + 1])->tcl_status);
						((struct W3*)__dictionary[i + 1])->tcl_status = __W3_Strdup(handler);
						W3_On(__dictionary[i + 1], "status", tcl_w3_status_handler);
					} else {
						if(((struct W3*)__dictionary[i + 1])->tcl_data != NULL) free(((struct W3*)__dictionary[i + 1])->tcl_data);
						((struct W3*)__dictionary[i + 1])->tcl_data = __W3_Strdup(handler);
						W3_On(__dictionary[i + 1], "data", tcl_w3_data_handler);
					}
					Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
					return TCL_OK;
				}
			}
			const char* errmsg = "argument error: a client named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		}
	} else if(strcasecmp(subcommand, "send_request") == 0) {
		if(objc != 3) {
			const char* errmsg = "argument error: arguments should be \"w3 send_request name\"";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		}
		char* name = Tcl_GetString(objv[2]);
		if(__dictionary == NULL) {
			const char* errmsg = "argument error: a client named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		} else {
			int i;
			for(i = 0; __dictionary[i] != NULL; i += 2) {
				if(strcasecmp((char*)__dictionary[i], name) == 0) {
					W3_Send_Request(__dictionary[i + 1]);
					Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
					return TCL_OK;
				}
			}
			const char* errmsg = "argument error: a client named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		}
	} else if(strcasecmp(subcommand, "destroy") == 0) {
		if(objc != 3) {
			const char* errmsg = "argument error: arguments should be \"w3 destroy name\"";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		}
		char* name = Tcl_GetString(objv[2]);
		if(__dictionary == NULL) {
			const char* errmsg = "argument error: a client named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		} else {
			int i;
			int except = -1;
			int len = 0;
			for(i = 0; __dictionary[i] != NULL; i += 2) {
				if(strcasecmp((char*)__dictionary[i], name) == 0) {
					W3_Free(__dictionary[i + 1]);
					free(__dictionary[i]);
					except = i;
					break;
				}
			}
			for(len = 0; __dictionary[len] != NULL; len++)
				;
			if(except != -1) {
				void** olddict = __dictionary;
				__dictionary = malloc(sizeof(*__dictionary) * (len - 1));
				int incr = 0;
				for(i = 0; olddict[i] != NULL; i += 2) {
					if(except == i) continue;
					__dictionary[incr] = olddict[incr];
					__dictionary[incr + 1] = olddict[incr + 1];
					incr += 2;
				}
				__dictionary[incr] = NULL;
				free(olddict);
				Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
				return TCL_OK;
			}
			const char* errmsg = "argument error: a client named that does not exist";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		}
	} else if(strcasecmp(subcommand, "debug") == 0) {
		if(objc != 3) {
			const char* errmsg = "argument error: arguments should be \"w3 debug bool\"";
			Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
			return TCL_ERROR;
		}
		W3_Do_Debug(atoi(Tcl_GetString(objv[2])));
		Tcl_SetObjResult(interp, Tcl_NewIntObj(0));
		return TCL_OK;
	} else {
		const char* errmsg = "argument error: invalid subcommand";
		Tcl_SetObjResult(interp, Tcl_NewStringObj(errmsg, strlen(errmsg)));
		return TCL_ERROR;
	}
	return TCL_OK;
}

int W3_Init(Tcl_Interp* interp) {
	__W3_Debug("Tcl", "Initializing the binding");
	W3_Library_Init();
	__dictionary = malloc(sizeof(*__dictionary));
	__dictionary[0] = NULL;
	__files = malloc(sizeof(*__files));
	__files[0] = NULL;
	Tcl_CreateObjCommand(interp, "w3", Tcl_W3Cmd, NULL, NULL);
	Tcl_CreateObjCommand(interp, "w3fio", Tcl_W3FIOCmd, NULL, NULL);
	__W3_Debug("Tcl", "Initialized the binding");
	return TCL_OK;
}
