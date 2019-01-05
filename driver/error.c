#include <stdio.h>
#include "error.h"

static char* programName = "PCLU";

void error_set_program(name)
     char* name;
{
    programName = name;
}

void error1(msg)
     char* msg;
{
    fprintf(stderr, "%s: %s\n", programName, msg);
}

void error2(msg1, msg2)
     char* msg1;
     char* msg2;
{
    fprintf(stderr, "%s: %s: %s\n", programName, msg1, msg2);
}

void error_conflict(option1, option2)
     char* option1;
     char* option2;
{
    fprintf(stderr, "%s: conflicting options %s and %s\n",
	    programName, option1, option2);
}

void error_usage() {
    fprintf(stderr, "Usage: %s [options] [files]\n", programName);

    fprintf(stderr, "  Options\n");
    fprintf(stderr, "    -debug          ; Debugging on\n");
    fprintf(stderr, "    -optimize       ; Optimization on\n");
    fprintf(stderr, "    -prof           ; Profiling on\n");
    fprintf(stderr, "    -spec           ; Spec check only\n");
    fprintf(stderr, "    -compile        ; Compile named files\n");
    fprintf(stderr, "    -link   <prog>  ; Link files and make <prog>\n");
    fprintf(stderr, "    -create <lib>   ; Dump collected specs to <lib>\n");
    fprintf(stderr, "    -use    <lib>   ; Use specs from <lib>\n");
    fprintf(stderr, "    -verbose        ; Print commands before execution\n");
    fprintf(stderr, "    -cfile          ; Leave intermediate files behind\n");

    fprintf(stderr, "  Files\n");
    fprintf(stderr, "    <file>.equ      ; Equate file\n");
    fprintf(stderr, "    <file>.clu      ; CLU file\n");
    fprintf(stderr, "    <file>.o        ; Object file\n");
    fprintf(stderr, "    <file>.lib      ; Library file\n");
}
