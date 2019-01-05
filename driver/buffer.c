#include <malloc.h>
#include <string.h>
#include "buffer.h"
#include "wordlist.h"

struct Buffer {
    WordList* list;
};

Buffer* Buffer_create() {
    Buffer* buffer = (Buffer*) malloc(sizeof(Buffer));
    buffer->list = WordList_create();
    return buffer;
}

Buffer* Buffer_copy(orig)
     Buffer* orig;
{
    char* contents;
    Buffer* buffer;

    buffer = Buffer_create();
    contents = Buffer_extract(orig);
    Buffer_append(buffer, contents);

    return buffer;
}

void Buffer_destroy(buffer)
     Buffer* buffer;
{
    WordList_destroy(buffer->list);
    free((char*) buffer);
}

void Buffer_append(buffer, str)
     Buffer* buffer;
     char*   str;
{
    WordList_append(buffer->list, str);
}

char* Buffer_extract(buffer)
     Buffer* buffer;
{
    WordIter* i;
    int length;
    char* str;
    int   index;

    /*
     * Compute total length.
     */
    length = 0;
    for (i = WordIter_start(buffer->list);
	 WordIter_valid(i);
	 WordIter_next(i)) {
	length += strlen(WordIter_element(i));
    }
    WordIter_stop(i);

    str = (char*) malloc((length + 1) * sizeof(char));
    index = 0;
    for (i = WordIter_start(buffer->list);
	 WordIter_valid(i);
	 WordIter_next(i)) {
	char* word = WordIter_element(i);
	strcpy(str+index, word);
	index += strlen(word);
    }
    WordIter_stop(i);
    str[index] = '\0';

    return str;
}
