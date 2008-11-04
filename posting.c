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

#include <stdlib.h>
#include <stdio.h>
#include "posting.h"
#include "eprintf.h"

/* add_front: Cons and element onto the first of a list */
Posting *add_front(Posting *listp, Posting *newp){
    newp->next = listp;
    return newp;
}

/* apply: execute fn for each element of listp */
void apply(Posting *listp,
           void (*fn)(Posting*, void*), void *arg) {
    for (; listp != NULL; listp = listp->next)
        (*fn)(listp, arg); /* call the function */
}
        
/* print_posting: print the frequency and the docid */
static void print_posting(Posting *p, void *arg){
    char *fmt;
    
    fmt = (char *) arg;
    printf(fmt, p->data.frequency, p->data.docid);
}

static void write_posting(Posting *p, void *arg){
    FILE *fp;
    fp = (FILE *) arg;
    fwrite(&(p->data), sizeof(p->data), 1, fp);
}

/* print_posting_list: Applies the function print_posting to each
   element of the posting list */
void print_posting_list(Posting *p){
    apply(p,print_posting, "Frequency: %d; Document Id: %d\n");
}

/* write_posting_list: Write all of the posting nodes out to a binary
   file */           
void write_posting_list(Posting *p, FILE *fp){
    apply(p,write_posting,fp);
}

/* freeall: Free each element of the list */
void freeall(Posting *p){
    Posting *next;
    for(; p != NULL; p = next){
        next = p->next;
        free(p);
    }
}

/* get_posting_list: Read a number of posting from a certain offset
   from the beginning of a file. */
Posting *get_posting_list(FILE *fp, int num_postings, int offset){
    int sz = sizeof(PostingData);
    PostingData pd;
    Posting *list;
    int n,i;

    list = NULL;

    fseek(fp, offset, SEEK_SET);

    for (i = 0; i < num_postings; i++){
        if ((n = fread(&pd,sz,1,fp)) != 1){
            int errcode = ferror(fp);
            fprintf(stderr,"%s Could not read recorda at offset %d. Error code %d\n", 
                    progname(), offset, errcode);
            exit(1);
        }
        else {
            Posting *p;
            p = emalloc(sizeof(*p));
            p->data.frequency = pd.frequency;
            p->data.docid = pd.docid;
            list = add_front(list,p);
        }
    }
    return list;
}

Posting *find_posting(Posting *listp, uint32_t docid){
    for (; listp != NULL; listp = listp->next){
        if (listp->data.docid == docid){
            return listp;
        }
    }
    return NULL;
}
