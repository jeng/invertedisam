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
 * Created: 07-November-2008 
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdint.h>
#include "settings.h"
#include "eprintf.h"
#include "fileutils.h"

void set_output_filenames(Settings *settings,char *basename, BinaryFileMode mode){
    char *doc;
    settings->index_file_name = estrdup(basename);
    change_extension(&(settings->index_file_name), ".if");
    settings->posting_file_name = estrdup(basename);
    change_extension(&(settings->posting_file_name), ".pf");
    doc = estrdup(basename);
    change_extension(&doc, ".df");
    settings->dm = open_document_manager(doc,mode);
    free(doc);
}
