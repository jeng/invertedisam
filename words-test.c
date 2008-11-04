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
 * Created: 04-November-2008 
 */

#include <stdlib.h>
#include <stdio.h>
#include "words.h"

int main(int argc, char **argv){
    IndexWord *treep;
    treep = NULL;
    
    treep = insert_word(treep, new_index_word("this"), 1);
    treep = insert_word(treep, new_index_word("this"), 1);
    treep = insert_word(treep, new_index_word("this"), 2);
    treep = insert_word(treep, new_index_word("this"), 3);
    treep = insert_word(treep, new_index_word("this"), 3);
    treep = insert_word(treep, new_index_word("this"), 3);
    treep = insert_word(treep, new_index_word("is"), 1);
    treep = insert_word(treep, new_index_word("is"), 1);
    treep = insert_word(treep, new_index_word("a"), 2);
    treep = insert_word(treep, new_index_word("a"), 2);
    treep = insert_word(treep, new_index_word("a"), 3);
    treep = insert_word(treep, new_index_word("a"), 4);
    treep = insert_word(treep, new_index_word("a"), 3);
    treep = insert_word(treep, new_index_word("a"), 3);
    treep = insert_word(treep, new_index_word("test"), 1);

    printf("Printing Word Tree\n");
    printf("------------------------------------------------------------\n");
    print_tree(treep);

    return 1;
}
