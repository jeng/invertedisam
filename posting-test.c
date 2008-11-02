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
 * Created: 02-November-2008 
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "posting.h"
#include "eprintf.h"

int main(int argc, char **argv){
  int i = 100;
  Posting *list;
  
  setprogname(argv[0]);

  srand(time(NULL));

  list = NULL;
  while(i--){
    int n = rand() % 100;
    Posting *newp;
    newp = emalloc(sizeof(*newp));
    newp->frequency = rand() % 100;
    newp->docid = i;
    list = add_front(list,newp);
  }

  print_posting_list(list);

  return 0;
}
