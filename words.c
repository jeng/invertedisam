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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "words.h"
#include "posting.h"
#include "eprintf.h"

/* insert: insert newp in treep, return treep */
IndexWord *insert_word(IndexWord *treep, IndexWord *newp, uint32_t docid) {
    int cmp;
  
    if (treep == NULL){
        Posting *p;
        p = emalloc(sizeof(*p));
        p->data.frequency = 1;
        p->data.docid = docid;
        newp->posting = add_front(newp->posting, p);
        newp->data.num_docs++;     
        return newp;
    }
    cmp = strcmp(newp->data.word, treep->data.word);
    if (cmp == 0){
        /* Check to see if we already have a posting for this document */
        Posting *p = find_posting(treep->posting, docid);
        /* If we do then get the posting */
        if (p != NULL){
            /* update the postings frequency */
            p->data.frequency++;
        }
        else{
            /* add a new posting */
            p = emalloc(sizeof(*p));
            p->data.frequency = 1;
            p->data.docid = docid;
            treep->posting = add_front(treep->posting, p);
            /* update num_docs */
            treep->data.num_docs++;
        }
    }
    else if (cmp < 0)
        treep->left = insert_word(treep->left, newp, docid);
    else
        treep->right = insert_word(treep->right, newp, docid);
    return treep;
}

IndexWord *new_index_word(char *word){
    IndexWord *iw;
    int i, j;
    int ln = (strlen(word) < MAX_WORD_LEN-1) ? strlen(word) : MAX_WORD_LEN-1;
    iw = emalloc(sizeof(*iw));
    iw->posting = NULL;
    iw->left = NULL;
    iw->right = NULL;
    iw->data.num_docs = 0;
    iw->data.posting_offset = 0;
    j = 0;
    for (i = 0; i < MAX_WORD_LEN; i++){
        if (j < ln)
            iw->data.word[i] = word[j++];
        else
            iw->data.word[i] = '\0';
    }
    return iw;
}

void print_tree(IndexWord *treep){
    if (treep == NULL){
        return;
    }
    else{
        print_tree(treep->left);
        printf("Num Docs %6d Word %s\n", 
               treep->data.num_docs, 
               treep->data.word);
        printf("Postings:\n");
        print_posting_list(treep->posting);
        printf("------------------------------------------------------------\n");
        print_tree(treep->right);
    }
}
