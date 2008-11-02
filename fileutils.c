/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil;  -*-
 *
 * Copyright (c) 2008 Jeremy English <jhe@jeremyenglish.org>
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 * 
 * Created: 02-November-2008 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileutils.h"

int has_extension(char *filename){
    for(;*filename != '\0';filename++){
        if (*filename == '.'){
            return 1;
        }
    }
    return 0;
}

void change_extension(char **filename,char *ext){
    int ln = strlen(*filename);
    int ext_ln = 0;
    int i;
    int nl;

    if (has_extension(*filename)){
        for(i = ln-1; i > 0; i--){
            if ((*filename)[i] == '.'){
                break;
            }
        }
        ext_ln = ln - i;
        if (ext_ln > strlen(ext) + 1){
            for(;*ext != '\0';ext++){
                (*filename)[i++] = *ext;
            }
            (*filename)[i] = '\0';
        }
        else{ /* we going to have to increase the filename */
            nl = ln + ((strlen(ext) + 1) - ext_ln);
            *filename = realloc(*filename,nl);
            if (*filename == NULL){
                fprintf(stderr, "Could not change the filename extension\n.");
                exit(1);
            }
            for(;*ext != '\0';ext++){
                (*filename)[i++] = *ext;
            }	
            (*filename)[i] = '\0';
        }
    }
    /* Does not have an extension */
    else {
        nl = ln + strlen(ext) + 1;
        *filename = realloc(*filename, ln + strlen(ext) + 1);
        if (*filename == NULL){
            fprintf(stderr, "Could not change the filename extension\n.");
            exit(1);
        }
        i = ln;
        for(;*ext != '\0';ext++){
            (*filename)[i++] = *ext;
        }
        (*filename)[i] = '\0';
    }
}	    

FILE *open_binary_file(char *filename, BinaryFileMode mode){
    FILE *fp;
    char fmode[3];

    if (mode == FM_READ){
        strncpy(fmode,"rb\0", 3);
    }else if (mode == FM_WRITE){
        strncpy(fmode,"wb\0", 3);
    }else{
        strncpy(fmode,"ab\0", 3);
    }
    if ((fp = fopen(filename,fmode)) == NULL){
        fprintf(stderr,"%s: Could not open file %s\n", 
                progname(), filename);
        exit(1);
    }
    return fp;
}  
