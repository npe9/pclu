#ifndef _BUFFER_H
#define _BUFFER_H

/*
 * Dynamic buffer of characters.
 */

#include "proto.h"

typedef struct Buffer Buffer;

Buffer* Buffer_create ARGS(());
    /*
     * EFFECTS  Returns new empty buffer.
     */

Buffer* Buffer_copy ARGS((Buffer* buffer));
    /*
     * EFFECTS  Returns a new copy of buffer.
     */

void Buffer_destroy ARGS((Buffer* buffer));
    /*
     * EFFECTS  Destroys a buffer.
     */

void Buffer_append ARGS((Buffer* buffer, char* str));
    /*
     * MODIFIES buffer
     * EFFECTS  appends contents of str to buffer
     */

char* Buffer_extract ARGS((Buffer* buffer));
    /*
     * EFFECTS  Returns contents of buffer as a null-terminated string
     */

#endif /* _BUFFER_H */
