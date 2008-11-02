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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "document.h"
#include "eprintf.h"
#include "fileutils.h"

int main(int argc, char **argv){
  DocumentManager *dm;
  int doc_num;
  int i = 100;
  Document doc;

  srand(time(NULL));

  if (argc == 1){
    fprintf(stderr,"Usage\ndocuemnt-test document_name1 document_name2 ... document_nameN\n");
    exit(1);
  }
  setprogname(argv[0]);

  /* write the document names to the data file */
  dm = open_document_manager("document.dat",FM_WRITE);  
  while (--argc > 0){
    printf("Writing %s\n", argv[argc]);
    write_document(dm,argv[argc]);
  }
  doc_num = dm->doc_id;
  close_document_manager(dm);
  free(dm);
 
  
  /* Read in random documents */
  dm = open_document_manager("document.dat", FM_READ);
  while(i--){
    int n = rand() % doc_num;
    get_document(dm,&doc,n);
    printf("Document %.4d: %s\n", n, doc.data);
  }

  return 0;
}
