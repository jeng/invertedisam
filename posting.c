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
    printf(fmt, p->frequency, p->docid);
}

/* print_posting_list: Applies the function print_posting to each
   element of the posting list */
void print_posting_list(Posting *p){
    apply(p,print_posting, "Frequency: %d; Document Id: %d\n");
}
           
