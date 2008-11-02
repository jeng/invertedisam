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
 * Created: 01-November-2008 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stop.h"
#include "settings.h"
#include "eprintf.h"

/* Returns a stopword node with the string set */
static StopWord* add_node(char *str){
    StopWord *newp = emalloc(sizeof(StopWord));
    int n = strlen(str) + 1;
    newp->left = NULL;
    newp->right = NULL;
    newp->word = emalloc(sizeof(char) * n);
    strncpy(newp->word,str,n);
    return newp;
}

/* insert: insert newp in treep, return treep */
static StopWord *insert(StopWord *treep, StopWord *newp) {
    int cmp;
  
    if (treep == NULL)
        return newp;
    cmp = strcmp(newp->word, treep->word);
    if (cmp == 0)
        ;
    else if (cmp < 0)
        treep->left = insert(treep->left, newp);
    else
        treep->right = insert(treep->right, newp);
    return treep;
}

static int read_file(char *progname,int verbose, StopWord **treep, char *filename){
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
            if (strlen(buf) > 0){
               *treep = insert(*treep, add_node(buf));
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


StopWord *create_stop_tree(Settings *settings, char *input_file){
    StopWord *stw;
    stw = NULL;
    read_file(progname(), settings->verbose,
              &stw, input_file);
    return stw;
}


/* Check to see if the keyword is in the stop tree. Return 1 if it is
   found 0 otherwise. */
int is_stop_word(StopWord *swt, char *keyword){
    int cmp;
    
    while (swt != NULL){
        cmp = strcmp(keyword, swt->word);
        if (cmp == 0)
            return 1;
        else if (cmp < 0)
            swt = swt->left;
        else
            swt = swt->right;
    }
    return 0;
}


