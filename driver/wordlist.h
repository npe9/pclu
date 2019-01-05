#ifndef _WORDLIST_H
#define _WORDLIST_H

#include "bool.h"
#include "proto.h"

/*
 * Dynamic list of words.
 */

typedef struct WordList WordList;
typedef struct WordIter WordIter;

WordList* WordList_create ARGS(());
    /*
     * EFFECTS  Returns a new empty list.
     */

void WordList_destroy ARGS((WordList* list));
    /*
     * EFFECTS  Destroy the list.
     */

void WordList_append ARGS((WordList* list, char* word));
    /*
     * EFFECTS  Append word to list.
     */

bool WordList_empty ARGS((WordList* list));
    /*
     * EFFECTS  Returns TRUE iff list is empty.
     */

int WordList_size ARGS((WordList* list));
    /*
     * EFFECTS  Returns number of elements in list
     */

char* WordList_first ARGS((WordList* list));
    /*
     * REQUIRES list is not empty.
     * EFFECTS  Returns first element of list.
     */

WordIter* WordIter_start ARGS((WordList* list));
    /*
     * EFFECTS  Returns a new iterator for scanning through list.
     */

void WordIter_stop ARGS((WordIter* iter));
    /*
     * REQUIRES list has not been modified since iter was created
     * EFFECTS  Destroys iter.
     */

bool WordIter_valid ARGS((WordIter* iter));
    /*
     * REQUIRES list has not been modified since iter was created
     * EFFECTS  Returns TRUE iff iteration is not finished yet.
     */

void WordIter_next ARGS((WordIter* iter));
    /*
     * REQUIRES list has not been modified since iter was created
     * MODIFIES iter
     * EFFECTS  iter is advanced to next element in list.
     */

char* WordIter_element ARGS((WordIter* iter));
    /*
     * REQUIRES list has not been modified since iter was created
     * EFFECTS  Returns the current iteration element.
     */

#endif /* _WORDLIST_H */
