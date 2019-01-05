#ifndef _ERROR_H
#define _ERROR_H

#include "proto.h"

extern void error_set_program ARGS((char* msg));
    /*
     * EFFECTS  Set program name. Used in reporting errors.
     */

extern void error1 ARGS((char* msg));
    /*
     * EFFECTS	Print message on standard error.
     */

extern void error2 ARGS((char* msg1, char* msg2));
    /*
     * EFFECTS	Print two part message on standard error.
     */

extern void error_conflict ARGS((char* option1, char* option2));
    /*
     * EFFECTS	Print conflicting option message on standard error.
     */

extern void error_usage ARGS(());
    /*
     * EFFECTS  Print usage information on standard error.
     */

#endif /* _ERROR_H */
