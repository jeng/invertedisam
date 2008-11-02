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
#include "document.h"
#include "eprintf.h"


DocumentManager *open_document_manager(char *output_file, 
                                       DocumentManagerMode mode){
    DocumentManager *dm;
    char *fmode = (mode == DM_READ) ? "rb" : "wb";

    dm = emalloc(sizeof(*dm));
    dm->filename = estrdup(output_file);
    dm->doc_id = 0;
    
    if ((dm->fp = fopen(dm->filename,fmode)) == NULL){
        fprintf(stderr,"%s: Could not open file %s\n", 
                progname(), dm->filename);
        exit(1);
    }

    return dm;
}

void write_document(DocumentManager *dm, char *filename){
    Document doc;
    int i,j;
    
    if (strlen(filename) > MAX_DOCUMENT_NAME){
        fprintf(stderr,
                "%s: Filename is to long. Cannot write to the document file.\n",
                progname());
    }

    j = 0;
    for (i = 0; i < MAX_DOCUMENT_NAME; i++){
        if (filename[j] != '\0'){
            doc.data[i] = filename[j++];
        }
        else{
            doc.data[i] = '\0';
        }
    }
    fwrite(&doc, sizeof(doc), 1, dm->fp);
    dm->doc_id++;
}

void close_document_manager(DocumentManager *dm){
    free(dm->filename);
    dm->filename = NULL;
    fclose(dm->fp);
}

void get_document(DocumentManager *dm, Document *doc, int docid){
    int sz = sizeof(Document);
    int offset = docid * sz;
    int n;
    
    fseek(dm->fp, offset, SEEK_SET);

    if ((n = fread(doc,sz,1,dm->fp)) != 1){
        int errcode = ferror(dm->fp);
        fprintf(stderr,"%s Could not read record %d. Error code %d\n", 
                progname(), docid, errcode);
        exit(1);
    }
}
