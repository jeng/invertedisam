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

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "document.h"
#include "stop.h"

typedef struct Settings Settings;

struct Settings{
    int verbose;
    char *index_file_name;
    char *posting_file_name;
    DocumentManager *dm;
    StopWord *stop_words;
};

extern void set_output_filenames(Settings *settings,char *basename, BinaryFileMode mode);

#endif /* __SETTINGS_H__ */
