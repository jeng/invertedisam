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
 * Created: 07-November-2008 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "stop.h"
#include "words.h"
#include "eprintf.h"
#include "posting.h"
#include "document.h"
#include "fileutils.h"

typedef enum ParamType ParamType;
enum ParamType { DIRECTORY, OUTPUT_FILE, FILE_EXTENSION, STOP_LIST, VERBOSE};

typedef struct Param Param;
struct Param {
    ParamType pt;
    char *value;
    Param *next;
};

void new_param_node(Param **params, Param **p, ParamType pt){
    if (*params == NULL){
        *params = emalloc(sizeof(Param));
        *p = *params;
    }
    else{
        (*p)->next = emalloc(sizeof(Param));
        *p = (*p)->next;
        (*p)->next = NULL;
    }
    (*p)->pt = pt;
    (*p)->value = NULL;
}

void usage(){
    fprintf(stderr,"Usage:\n%s -d directory -o output [-e extension] [-s stop list file] [-v]\n", progname());
    exit(1);
}


/* Parse the command line arguments */
Param* parse_params(int argc, char **argv){
    Param *params;
    Param *p;
    int i, getval;
    params = NULL;

    getval = 0;
    /* XXX: I'm sure there is a library that will do all of this for
       me. I'm just don't fill like looking it up right now. */
    for(i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            if (getval){
                fprintf(stderr,
                        "%s: A value was expected after the last flag.\n",
                        progname());
                exit(1);
            }
            else if (strcmp(argv[i],"-d") == 0){
                /* We need to scan a directory */
                getval = 1;
                new_param_node(&params, &p, DIRECTORY);                    
            }
            else if (strcmp(argv[i],"-o") == 0){
                /* We need to scan a directory */
                getval = 1;
                new_param_node(&params, &p, OUTPUT_FILE);                    
            }
            else if (strcmp(argv[i],"-s") == 0){
                /* We need to scan a directory */
                getval = 1;
                new_param_node(&params, &p, STOP_LIST);                    
            }
            else if (strcmp(argv[i],"-e") == 0){                
                /* We need to look for file of this extension */
                getval = 1;
                new_param_node(&params, &p, FILE_EXTENSION);
            }
            else if (strcmp(argv[i], "-v") == 0){
                /* No value comes after verbose */
                new_param_node(&params, &p, VERBOSE);
            }                    
            else {
                /* We don't know what you want to do */
                fprintf(stderr,"%s: Unknown command %s\n", 
                        progname(),argv[i]);
                exit(1);
            }
        }
        else{
            if (getval){
                /* This is the value part of the -d or -e command */
                getval = 0;
                if (p == NULL){
                    fprintf(stderr, "%s: NULL Parameter node!", argv[i]);
                    exit(1);
                }
                else{
                    int n = strlen(argv[i]) + 1;
                    p->value = calloc(sizeof(char), n);
                    strncpy(p->value,argv[i],n);
                }
            }
            else{
                usage();
            }
        }
    }
    if (getval){
        fprintf(stderr,
                "%s: A value was expected after the last flag.\n",
                progname());
        exit(1);
    }
    
    
#if 0 /* XXX: Debug the parameter parser. */
    for(p = params; p != NULL; p = p->next){
        printf("Type: %d Value %s\n",p->pt, p->value);
    }
    exit(1);
#endif

    return params;
}


Param *find_param(Param *params, ParamType pt){
    for (; params != NULL && params->pt != pt; params = params->next)
        ;
    return params;
}
        
int verbose_messages(Param *params){
    return (find_param(params, VERBOSE) != NULL);
}

int read_file(settings *settings, Nameval **treep, char *filename){
    const int MAX_BUF = 0xFF;
    char buf[MAX_BUF];
    FILE *fp;
    int i;

    if ((fp = fopen(filename,"r")) == NULL){
        fprintf(stderr,"%s: Could not open file %s\n", 
                progname, filename);
        return 0;
    }
    if (verbose){
        printf("Reading file %s\n",filename);
    }
    for(i = 0;!feof(fp);){
        char c = fgetc(fp);
        if (isalnum(c)){
            buf[i++] = tolower(c);
        }
        else{
            buf[i] = '\0';
            i = 0;
            if (strlen(buf) > 1){
               *treep = insert(*treep, add_node(buf, 1));
            }
        }
        if (i == MAX_BUF){
            fprintf(stderr,"%s: Word is to long\n", progname);
            exit(1);
        }
    }
    fclose(fp);
    return 1;
}

int main(int argc, char **argv){
    Param *params;
    Param *directory;
    Param *output;
    Param *stop;
    Param *extp = get_next_ext(p);
    StopWord *stopword;
    Settings settings;
    struct stat st;

    setprogname(argv[0]);
    if (argc == 1) usage();
    params = parse_params(argc,argv);
    directory = find_param(params,DIRECTORY);
    output = find_param(params,OUTPUT_FILE);
    stop   = find_param(params,STOP_LIST);
    settings.verbose = verbose_messages(params);
    extp = find_param(params,FILE_EXTENSION);

    stopword = NULL;

    if (directory == NULL){
        fprintf(stderr,"%s: Could not find a directory.\n", progname());
        usage();
    }else{
        if (settings.verbose){
            printf("We are going to search directory: %s\n",directory->value);
        }
    }

    if (output == NULL){
        fprintf(stderr,"%s: Could not find an output name.\n", progname());
        usage();
    }else{
        set_output_filenames(&settings, output->value);

        if (settings.verbose){
            printf("We are going to use output name: %s\n",output->value);
            printf("Document filename: %s\n",settings.document_file_name);
            printf("Posting  filename: %s\n",settings.posting_file_name);
            printf("Index    filename: %s\n",settings.index_file_name);
        }
    }

    if (stop != NULL){
        stopword = create_stop_tree(&settings, stop->value);
    }

    if(!stat (directory->value, &st) &&
       S_ISDIR(st.st_mode)){
        DIR *dir;
        struct dirent *dentry;
        
        dir = opendir(directory->value);
        if (! dir){
            fprintf(stderr, "%s: Could not open directory\n", progname());
            exit(1);
        }
        if (settings.verbose){
            printf("Reading Directory %s\n", directory->value);
        }
        while((dentry = readdir(dir))){
            char *fullpath;
            int pn = strlen(directory->value);
            int dn = strlen(dentry->d_name);
            int fn = pn + dn + 1;
            fullpath = calloc(sizeof(char), fn);
            if (fullpath == NULL){
                fprintf("%s: output of memory.\n",progname());
                exit(1);
            }
            strncpy(fullpath, directory->value, pn + 1);
            if (fullpath[pn] != '/'){
                fullpath[pn] = '/';
            }
            if (!strcat(fullpath,dentry->d_name)){
                fprintf(stderr,"%s: Could not build complete filename.\n", progname());
                exit(1);
            }
            if (extp != NULL){
                /* Read File for a certain extension */
                int n = strlen(fullpath);
                int en = strlen(extp->value);
                if (n > en && !strcasecmp(fullpath + n - en, extp->value)){
                    if (!read_file(&setting, &treep, fullpath)){
                        fprintf(stderr,"%s: Could not read file.", progname());
                    }
                }
            }
            else{
                if (!read_file(&setting, &treep, fullpath)){
                    fprintf(stderr,"%s: Could not read file.", progname());
                }
            }
        }
    }
    else {
        fprintf(stderr,"%s: Unknown directory %s\n",
                progname(), p->value);
    }
    
    return 1;
}
