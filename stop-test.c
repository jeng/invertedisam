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
 * p
 * Created: 01-November-2008 
 */

#include <stdlib.h>
#include <stdio.h>
#include "stop.h"
#include "eprintf.h"

int main(int argc, char **argv){
  Settings *settings;
  StopWord *stw;
  
  if (argc == 1){
    fprintf(stderr,"Usage\nstop-test stop-list-file keywords1 keywords2 ... keywordsN\n");
    exit(1);
  }
  setprogname(argv[0]);

  settings = emalloc(sizeof(*settings));
  
  settings->verbose = 1;
  
  stw = create_stop_tree(settings, argv[1]);
  
  while (--argc > 1){
    printf("%s %s a keyword\n", argv[argc], 
	   (is_stop_word(stw,argv[argc])) ? "is" : "is not");
  }
  return 0;
}
