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
#include <stdio.h>
#include "words.h"
#include "posting.h"
#include "eprintf.h"
#include "fileutils.h"

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

void print_word_tree(IndexWord *treep){
    if (treep == NULL){
        return;
    }
    else{
        print_word_tree(treep->left);
        printf("Num Docs %6d Word %s\n", 
               treep->data.num_docs, 
               treep->data.word);
        printf("Postings:\n");
        print_posting_list(treep->posting);
        printf("------------------------------------------------------------\n");
        print_word_tree(treep->right);
    }
}

static void write_files(IndexWord *treep, FILE *ifp, FILE *pfp){
    int outsz;
    if (treep == NULL)
        return;    
    write_files(treep->left, ifp, pfp);
    /* Output index word and the update the posting file */
    treep->data.posting_offset = ftell(pfp);
    outsz = fwrite(&(treep->data), sizeof(treep->data), 1, ifp);
    if (outsz != 1){
        fprintf(stderr,"%s: An error occured while writing to the index file.\n", progname());
        exit(1);
    }
    write_posting_list(treep->posting, pfp);
    write_files(treep->right, ifp, pfp);
}


void write_index_file(IndexWord *treep, char *index_filename, char *posting_filename){
    FILE *ifp;
    FILE *pfp;
    
    ifp = open_binary_file(index_filename, FM_WRITE);
    pfp = open_binary_file(posting_filename, FM_WRITE);

    write_files(treep,ifp,pfp);

    fclose(ifp);
    fclose(pfp);
}

/* get_index_recnum: Set wr to the record refered to by recnum. Return
   true if the recnum was found false otherwise. */
int get_index_recnum(FILE *fp, int recno, IndexWordData *wr){
    int n;
    long int sz = sizeof(*wr);

    fseek(fp, recno * sz, SEEK_SET);

    if ((n = fread(wr,sz,1,fp)) != 1){
        return 0;
    }
    
    return 1;
}
