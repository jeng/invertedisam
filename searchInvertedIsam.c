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
 * Created: 09-November-2008 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "posting.h"
#include "words.h"
#include "eprintf.h"
#include "document.h"
#include "fileutils.h"
#include "settings.h"


typedef struct Keyword Keyword;

struct Keyword{
    char *keyword;
    Keyword *next;
};

Keyword *insert(Keyword *keyp, Keyword *newkey){
    if (keyp == NULL){
        return newkey;
    }else{
        Keyword *k = keyp;
        for(; k != NULL; k = k->next){
            if (k->next == NULL){
                k->next = newkey;
                break;
            }
        }
    }
    return keyp;
}

Keyword *new_keyword(char *str){
    Keyword *kw;
    int ln = strlen(str) + 1;
    int i;

    kw = malloc(sizeof(*kw));
    if (kw == NULL){
        fprintf(stderr,"Could not get memory.\n");
        exit(1);
    }
    kw->keyword = calloc(ln, sizeof(char));
    if (kw->keyword == NULL){
        fprintf(stderr,"Could not get memory.\n");
        exit(1);
    }
    for (i = 0; i < ln; i++)
        kw->keyword[i] = tolower(str[i]);

    kw->next = NULL;
    return kw;
}

long int midpoint(long int a, long int b){
    long int m = floor ( (b + a) / 2 );
    return m;
}

/* Return true if the keyword was found. wr will be set to the found
   wordRec */
int search(FILE *fp, char *keyword, IndexWordData *wr){
    int rs = sizeof(*wr); /* record size */
    long int fs; /* File size */
    long int nr; /* number of records in the file */
    long int start, stop;
    int oldmp, mp;

    /* get the file size */
    fseek(fp,0,SEEK_END);
    fs = ftell(fp);
    rewind(fp);

    /* get the number of records in the file */
    nr = floor(fs / rs);
    start = 0;
    stop  = nr;

    mp = -1;
    do{
        int nrr; /* number of records read */
        int cmp;
        oldmp = mp;

        /* get the record in the middle of the start and stop position */
        mp = midpoint(start, stop);
        if (!get_index_recnum(fp,mp,wr)){
            fprintf(stderr,"Could not get recnum %d\n",mp);
            exit(1);
        }
        cmp = strcmp(keyword, wr->word);
        /* if the  keyword is less then the one read */
        /* change stop position continue */
        /* if the keyword is greater then the one read  */
        /* change start position */
        /* if the keyword matches the record  */
        /* stop return match */
        
        if (cmp < 0){
            stop = mp;
        }
        else if (cmp > 0){
            start = mp;
        }
        else {
            return 1;
        }
        /* if start and stop are equal  */
        /* quit no match found */
    }while (mp != oldmp);
    return 0;
}

int search_keywords(char *filename, Keyword *keywords, IndexWordData **iwd){
    IndexWordData wr;
    FILE *fp;
    int i;

    fp = open_binary_file(filename, FM_READ);
    
    i = 0;
    for(;keywords != NULL; keywords = keywords->next){
        if (search(fp, keywords->keyword, &wr)){
            int n = wr.num_docs;
            IndexWordData *p = (*iwd+i);
            memcpy(p,&wr,sizeof(wr));
            /*            printf("Found %s with %d document%s\n",
             *      p->word, p->num_docs,
             *      (n == 1) ? "." : "s."); 
             */

            i++;
        }else{
            printf("%s was not found.\n",keywords->keyword);
        }
    }
    fclose(fp);
    return i;
}

int main(int argc, char **argv){
    setprogname(argv[0]);
    if (argc == 1){
        fprintf(stderr, "Usage:\n%s filename keywords\n",progname());
        exit(1);
    }
    else{
        int i;
        char *filename = argv[1];
        int nkey = argc - 2;
        Keyword *kp;
        IndexWordData *iwd;
        Settings settings;
        int found;

        iwd = emalloc(sizeof(*iwd) * nkey);

        set_output_filenames(&settings,filename, FM_READ);
        
        kp = NULL;
            
        for(i = 0; i < nkey; i++){
            kp = insert(kp,new_keyword(argv[i+2]));
        }
#if 0
        {
            Keyword *k = kp;
            for(; k != NULL; k = k->next)
                printf("%s\n",k->keyword);
            exit(1);
        }
#endif
        found = search_keywords(settings.index_file_name, kp, &iwd);


        /* Display the results with the most frequent occuring first. */
        {
            FILE *pf;
            Posting *p;

            p = NULL;
            pf = open_binary_file(settings.posting_file_name, FM_READ);

            for(i = 0; i < found; i++){
                /*                printf("Word: %.50s num_docs: %.6d offset: %.8x\n",
                                  iwd->word, iwd->num_docs, iwd->posting_offset); */
                append_unique_posting_list(pf, &p, iwd->num_docs, iwd->posting_offset);
                iwd++;

            }

            p = sort_posting_list(p, posting_length(p));

            for (; p != NULL; p = p->next){
                Document doc;
                get_document(settings.dm, &doc, p->data.docid);
                printf("Frequency %.8x Document %s\n",
                       p->data.frequency, doc.data);
            }

            freeall(p);
            fclose(pf);
        }

    }
    return 0;
}
