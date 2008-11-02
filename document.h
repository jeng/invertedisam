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

enum{
    MAX_DOCUMENT_NAME = 255
};

typedef struct Document Document;

struct Document{
    char name[MAX_DOCUMENT_NAME];
};

typedef struct DocumentManager DocumentManager;

struct DocumentManager{
    uint32_t doc_id;   /* Current Document Id */
    Document document; /* Current Document Name */
    char filename[MAX_DOCUMENT_NAME]; /* Output file */
};


#endif /* __DOCUMENT_H__ */
