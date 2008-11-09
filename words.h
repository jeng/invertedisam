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

#ifndef __WORDS_H__
#define __WORDS_H__

#include <stdint.h>
#include <stdio.h>
#include "posting.h"

enum{

    /* index words greater then this length will be excluded from the
       index file. */       
    MAX_WORD_LEN = 50 
};

typedef struct IndexWordData IndexWordData;
struct IndexWordData{
    char word[MAX_WORD_LEN];
    uint32_t num_docs;
    uint32_t posting_offset;
};

typedef struct IndexWord IndexWord;
struct IndexWord {
    IndexWordData data;
    Posting *posting;
    IndexWord *left; /* lesser */
    IndexWord *right; /* greater */
};

extern IndexWord *new_index_word(char *word);
extern IndexWord *insert_word(IndexWord *treep, IndexWord *newp, uint32_t docid);
extern void print_word_tree(IndexWord *treep);
extern void write_index_file(IndexWord *treep, char *index_filename, char *posting_filename);
extern int get_index_recnum(FILE *fp, int recno, IndexWordData *wr);


#endif /*__WORDS_H__ */
