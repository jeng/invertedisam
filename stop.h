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

#ifndef __STOP_H__
#define __STOP_H__

#include "settings.h"

typedef struct StopWord StopWord;
struct StopWord {
    char *word;
    StopWord *left; /* lesser */
    StopWord *right; /* greater */
};

/* Returns a stop tree that can latter be search for keywords */
extern StopWord* create_stop_tree(Settings *settings, char *filename);

/* Is the keyword passed in the stop tree */
extern int is_stop_word(StopWord *swt, char *keyword);

#endif /*__STOP_H__ */
