#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdarg.h>
#include <unistd.h>
#include <libio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

typedef struct _IO_FILE FILE;

typedef int (*OpenFunction)(const char* fn, int flags, ...);
typedef FILE* (*FopenFunction)(char* fn, char *mode);
OpenFunction real_open;
FopenFunction real_fopen;

int file_contain_string(const char *fn) {
    char dest_string[1024], dest_path[1024];
    char real_path[1024];
    int i = 0,j = 0;
    int file_flag = 1, string_flag = 0;

    FILE *fp = ((FILE* (*)(char*, char*))(dlsym(RTLD_NEXT, "fopen")))(MONITOR_CONF, "r");
    if (!fp) return 0;
    
    char* (*fgets_ptr)(char*, int, FILE*)=dlsym(RTLD_NEXT, "fgets");
    char s[4096];
    while((fgets_ptr(s, 4096, fp))!=NULL) {
	if (strstr(s, "string=")) {
	    strcpy(dest_string, strstr(s, "string=")+7);
	    dest_string[strlen(dest_string)-1]=0;
	}
	if (strstr(s, "path=")) {
	    strcpy(dest_path, strstr(s, "path=")+5);
	    dest_path[strlen(dest_path)-1]=0;
	}
    }

    realpath(fn, real_path);
    while (i<strlen(dest_path) && j<strlen(real_path)) {
	char tmp1[256] = "",tmp2[256] = "";
	int k = 0;
	while (dest_path[i] && dest_path[i]!='/') tmp1[k++]=dest_path[i++];
	k = 0;
	while (real_path[j] && real_path[j]!='/') tmp2[k++]=real_path[j++];
	if (strcmp(tmp1, tmp2) != 0) {
	    file_flag = 0;
	    break;
	}
	i++;
	j++;
    }
    if (!file_flag || i < strlen(dest_path)) return 0;

    FILE *fp1 = ((FILE* (*)(char*, char*))(dlsym(RTLD_NEXT, "fopen")))(real_path, "r"); 
    if (!fp1) return 0;
    while((fgets_ptr(s, 4096, fp1))!=NULL) {
	if (strstr(s, dest_string)) string_flag = 1;
    }
    if (!string_flag) return 0;
    ((int (*)(FILE*))(dlsym(RTLD_NEXT, "fclose")))(fp1); 

    ((int (*)(FILE*))(dlsym(RTLD_NEXT, "fclose")))(fp); 

    return 1;
}

int open(const char* fn, int flags, ...) {
    int r = -1;
    va_list args;

    if (file_contain_string(fn)) {
        char output[1024];
        ((int (*)(char*, const char*, ...))(dlsym(RTLD_NEXT, "sprintf")))(output, "含有敏感词，禁止访问\n", fn);
        write(1, output, strlen(output));
	return -1;
    }

    va_start(args, flags);
    if (!real_open) {
        void** pointer;

        pointer = (void**)&real_open;
        *pointer = dlsym(RTLD_NEXT, "open");
    }
    r = real_open(fn, flags, args);
    va_end(args);
    
    //char output[1024];
    //((int (*)(char*, const char*, ...))(dlsym(RTLD_NEXT, "sprintf")))(output, "try open %s\n", fn);
    //write(1, output, strlen(output));
   
    return r;
}

FILE *fopen(char *fn, char *mode) {
    FILE * fp = NULL;

    if (file_contain_string(fn)) {
        char output[1024];
        ((int (*)(char*, const char*, ...))(dlsym(RTLD_NEXT, "sprintf")))(output, "含有敏感词，禁止访问\n", fn);
        write(1, output, strlen(output));
	return NULL;
    }

    if (!real_fopen) {
        void **pointer;

        pointer = (void**)&real_fopen;
        *pointer = dlsym(RTLD_NEXT, "fopen");
    }
    fp = real_fopen(fn, mode);

    //char output[1024];
    //((int (*)(char*, const char*, ...))(dlsym(RTLD_NEXT, "sprintf")))(output, "try fopen %s\n", fn);
    //write(1, output, strlen(output));

    return fp;
}
