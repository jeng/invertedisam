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

#ifndef __POSTING_H__
#define __POSTING_H__

#include <stdint.h>

typedef struct PostingData PostingData;
struct PostingData{
    uint32_t frequency;
    uint32_t docid;
};

typedef struct Posting Posting;
struct Posting {
    PostingData data;
    Posting *next;
};

extern Posting *add_front(Posting *listp, Posting *newp);
extern void apply(Posting *listp,
           void (*fn)(Posting*, void*), void *arg);
extern void print_posting_list(Posting *p);
extern void write_posting_list(Posting *p, FILE *fp);
extern void freeall(Posting *p);
extern Posting *get_posting_list(FILE *fp, int num_postings, int offset);

#endif /* __POSTING_H_ */

