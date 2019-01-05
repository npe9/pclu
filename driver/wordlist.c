#include <malloc.h>
#include "wordlist.h"

/*
 * Rep is a doubly linked circular list with dummy initial element.
 * initial_element->next is the first element in the list.
 */

struct WordList {
    char*	word;
    WordList*	next;
    WordList*	prev;
    int		size;
};

WordList* WordList_create() {
    WordList* head = (WordList*) malloc(sizeof(WordList));
    head->next = head;
    head->prev = head;
    head->size = 0;
    return head;
}

void WordList_destroy(list)
     WordList* list;
{
    /* Clear out all elements */

    WordList* index = list->next;
    while (index != list) {
	WordList* tmp = index;
	index = index->next;
	free((char*) tmp);
    }

    free((char*) list);
}

void WordList_append(list, word)
     WordList* list;
     char*     word;
{
    WordList* cell = (WordList*) malloc(sizeof(WordList));
    cell->word = word;

    cell->prev = list->prev;
    list->prev->next = cell;

    cell->next = list;
    list->prev = cell;

    list->size++;
}

bool WordList_empty(list)
     WordList* list;
{
    return (list->next == list);
}

int WordList_size(list)
     WordList* list;
{
    return list->size;
}

char* WordList_first(list)
     WordList* list;
{
    return list->next->word;
}

struct WordIter {
    WordList* list;
    WordList* index;
};

WordIter* WordIter_start(list)
     WordList* list;
{
    WordIter* iter = (WordIter*) malloc(sizeof(WordIter));
    iter->list = list;
    iter->index = list->next;
    return iter;
}

void WordIter_stop(iter)
     WordIter* iter;
{
    free((char*) iter);
}

bool WordIter_valid(iter)
     WordIter* iter;
{
    return iter->index != iter->list;
}

void WordIter_next(iter)
     WordIter* iter;
{
    iter->index = iter->index->next;
}

char* WordIter_element(iter)
     WordIter* iter;
{
    return iter->index->word;
}
