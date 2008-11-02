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

#ifndef __DOCUMENT_H__
#define __DOCUMENT_H__

#include <stdint.h>

enum{
    MAX_DOCUMENT_NAME = 255
};

typedef enum{
    DM_READ,
    DM_WRITE
} DocumentManagerMode;

typedef struct Document  Document;

struct Document{
    char data [MAX_DOCUMENT_NAME];
};

typedef struct DocumentManager DocumentManager;

struct DocumentManager{
    uint32_t doc_id;   /* Current Document Id */
    char *filename;    /* Data file */
    FILE *fp;          /* File pointer */
};

extern DocumentManager *open_document_manager(char *output_file, DocumentManagerMode mode);
extern void write_document(DocumentManager *dm, char *filename);
extern void close_document_manager(DocumentManager *dm);
extern void get_document(DocumentManager *dm, Document *doc, int docid);

#endif /* __DOCUMENT_H__ */
