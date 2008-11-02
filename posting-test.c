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
#include "fileutils.h"

int main(int argc, char **argv){
  int i = 10;
  Posting *list;
  FILE *fp;
  int of1, of2, of3;
  
  setprogname(argv[0]);

  fp = open_binary_file("posting.dat", FM_WRITE);

  srand(time(NULL));

  list = NULL;
  while(i--){
    int n = rand() % 100;
    Posting *newp;
    newp = emalloc(sizeof(*newp));
    newp->data.frequency = rand() % 100;
    newp->data.docid = i;
    list = add_front(list,newp);
  }

  printf("Posting list 1\n");
  print_posting_list(list);
  of1 = ftell(fp);
  write_posting_list(list, fp);

  freeall(list);
  list = NULL;

  i = 20;
  while(i--){
    int n = rand() % 100;
    Posting *newp;
    newp = emalloc(sizeof(*newp));
    newp->data.frequency = rand() % 100;
    newp->data.docid = i;
    list = add_front(list,newp);
  }
  printf("Posting list 2\n");
  print_posting_list(list);
  of2 = ftell(fp);
  write_posting_list(list, fp);

  freeall(list);
  list = NULL;

  i = 15;
  while(i--){
    int n = rand() % 100;
    Posting *newp;
    newp = emalloc(sizeof(*newp));
    newp->data.frequency = rand() % 100;
    newp->data.docid = i;
    list = add_front(list,newp);
  }

  printf("Posting list 3\n");
  print_posting_list(list);
  of3 = ftell(fp);
  write_posting_list(list,fp);
  freeall(list);
  list = NULL;    
  fclose(fp);

  /* Reading postings from file */
  printf("Reading postings from postings.dat\n");
  fp = open_binary_file("posting.dat", FM_READ);
  list = get_posting_list(fp,20,of2);
  printf("Posting List 2\n");
  print_posting_list(list);
  freeall(list);

  list = get_posting_list(fp,15,of3);
  printf("Posting List 3\n");
  print_posting_list(list);
  freeall(list);

  list = get_posting_list(fp,10,of1);
  printf("Posting List 1\n");
  print_posting_list(list);
  freeall(list);

  return 0;
}
