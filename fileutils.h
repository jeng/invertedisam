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

#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

typedef enum{
    FM_WRITE,
    FM_READ,
    FM_APPEND
} BinaryFileMode;

extern int has_extension(char *filename);
extern void change_extension(char **filename,char *ext);
extern FILE *open_binary_file(char *filename, BinaryFileMode mode);
#endif /*  __FILEUTILS_H__ */
