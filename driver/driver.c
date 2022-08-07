/*
 * Portable CLU driver.
 */

/* 1/26/93 dwc: use lpclu_opt for optimized library;
		removed CLU_DEBUG_SUPPORT;
		removed -g from CFLAGS_DEBUG;
		added -x to link command;
   11/7/16 sjg: added -lgc -lpthread to CLU_OTHER_LIBS
*/

#include "acconfig.h"

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "proto.h"

/*
 * Collect up various compile time options.
 */
#ifndef CLUHOME
#define CLUHOME "/usr/local/lib/pclu"
#endif
#ifndef C_COMPILER
#define C_COMPILER "cc"
#endif
#ifndef CFLAGS_NORMAL
#define CFLAGS_NORMAL ""
#endif
#ifndef CFLAGS_DEBUG
#define CFLAGS_DEBUG ""
#endif
#ifndef CFLAGS_OPTIMIZE
#define CFLAGS_OPTIMIZE "-O"
#endif

/*
 * Directories relative to CLUHOME.
 */
#ifndef CLU_COMPILER
#define CLU_COMPILER "exe/pclu"
#endif
#ifndef CLU_INCLUDE
#define CLU_INCLUDE "include"
#endif
#ifndef CLU_DEBUG_INCLUDE
#define CLU_DEBUG_INCLUDE "include/debug"
#endif
#ifndef CLU_LIBDIR
#define CLU_LIBDIR "code"
#endif

/*
 * Various libraries.
 */
#ifndef CLU_LIB
#define CLU_LIB "-lpclu_opt"
#endif
#ifndef CLU_DEBUG_LIB
#define CLU_DEBUG_LIB "-lpclu"
#endif
#ifndef CLU_DEBUG_SUPPORT
#define CLU_DEBUG_SUPPORT ""
#endif
#ifndef CLU_OTHER_LIBS
#define CLU_OTHER_LIBS "-lm -lgc -lpthread"
#endif

#include "bool.h"
#include "buffer.h"
#include "error.h"
#include "proto.h"
#include "wordlist.h"

/*
 * Option structure.
 * REP INVARIANT
 *	1. At most one of compile, link, create.
 *	2. If create, then spec.
 *	3. If neither spec, create, or link, then compile.
 *	3. At most one of debug or optimize.
 *	4. No obj_files except when link.
 *	5. No equ_files, clu_files, and use files when link.
 */

typedef struct {
    bool	spec;		/* Spec files? */
    bool	compile;	/* Compile files? */
    bool	link;		/* Link together a program? */
    bool	create;		/* Create a library? */

    bool	debug;		/* Debugging on? */
    bool	optimize;	/* Optimization on? */
    bool	prof;		/* Profiling on? */

    bool	verbose;	/* Be verbose about invoked programs? */
    bool	leave;		/* Leave intermediate files behind */

    char*	program;	/* Program to link together */
    char*	library;	/* Library to create */

    /* Arrays of file names */
    WordList*	use;		/* List of libraries to use */
    WordList*	clu_files;	/* List of clu files */
    WordList*	equ_files;	/* List of equate files */
    WordList*	obj_files;	/* List of object files */

    /* Generated commands */
    Buffer*	clu_cmd;	/* Clu compiler command */
    Buffer*	cc_cmd;		/* C compile command */
    Buffer*	ld_cmd;		/* Link command */

    /* Various parameters */
    char*	cluhome;	/* CLU home location */
} Options;

bool parse_options ARGS((int argc, char** argv, Options* options));
    /*
     * REQUIRES	argv[0..argc-1] are arguments
     * MODIFIES	options
     * EFFECTS	Returns true iff arguments are successfully parsed.
     *		options is filled in correctly on return.
     */

bool has_suffix ARGS((char* str, char* suffix));
    /*
     * EFFECTS  Returns true iff str ends with suffix.
     */

char* change_suffix ARGS((char* str, char* old_suffix, char* new_suffix));
    /*
     * REQUIRES	str has old_suffix as a suffix.
     * EFFECTS  Returns new string with old_suffix in str replaced by
     *		new_suffix.
     */

int  clu_compile ARGS((Options* options));
    /*
     * EFFECTS	Compile clu_files with the specified options.
     *		Return non-zero on error, 0 otherwise.
     */

int  clu_link ARGS((Options* options));
    /*
     * EFFECTS	Link obj_files with the specified options.
     *		Return non-zero on error, 0 otherwise.
     */

int  clu_create ARGS((Options* options));
    /*
     * EFFECTS	Spec check clu_files with the specified options
     *		and write out to the specified library.
     *		Return non-zero on error, 0 otherwise.
     */

int  clu_spec ARGS((Options* options));
    /*
     * EFFECTS	Spec check clu_files with the specified options.
     *		Return non-zero on error, 0 otherwise.
     */

int clu_compile_file ARGS((Options* options, char* file));
    /*
     * EFFECTS	Runs the CLU compiler and the C compiler on the specified
     *		file using the supplied options.
     *		Returns non-zero on error, 0 otherwise.
     */

Buffer* clu_compile_command ARGS((Options* options));
    /*
     * REQUIRES Either spec or compile.
     * EFFECTS  Returns a buffer that contains the CLU compiler invocation
     *		command specified by options. Contains equates, required
     *		libraries, and any required debug and optimize flags.
     */

Buffer* cc_compile_command ARGS((Options* options));
    /*
     * REQUIRES compile.
     * EFFECTS  Returns a buffer that contains the C compiler invocation
     *		command.
     */

Buffer* ld_compile_command ARGS((Options* options));
    /*
     * REQUIRES link.
     * EFFECTS  Returns a buffer that contains the linker invocation
     *		command.
     */

int run_command ARGS((Options* options, Buffer* command));
    /*
     * EFFECTS  Runs command specified by the string in the supplied buffer.
     *		Returns non-zero on error, 0 otherwise.
     */

void bprint ARGS((Buffer* buffer));
    /*
     * EFFECTS  Prints buffer contents followed by newline.
     */

int
main(argc, argv)
     int    argc;
     char** argv;
{
    Options options;

    error_set_program(argv[0]);

    if (! parse_options(argc, argv, &options)) {
	error_usage();
	return 1;
    }

    if (options.compile) {
	return clu_compile(&options);
    }

    if (options.link) {
	return clu_link(&options);
    }

    if (options.create) {
	return clu_create(&options);
    }

    /*
     * This must come after clu_create() because create implies spec
     * and we do not want to do specs twice.
     */
    if (options.spec) {
	return clu_spec(&options);
    }

    return 0;
}

bool parse_options(argc, argv, options)
     int      argc;
     char**   argv;
     Options* options;
{
    int i;

    /*
     * Clear out options.
     */
    options->spec	  = FALSE;
    options->compile	  = FALSE;
    options->link	  = FALSE;
    options->create	  = FALSE;
    options->debug	  = FALSE;
    options->optimize	  = FALSE;
    options->prof	  = FALSE;
    options->verbose	  = FALSE;
    options->leave	  = FALSE;

    options->program	  = NULL;
    options->library	  = NULL;

    options->use	  = WordList_create();
    options->clu_files	  = WordList_create();
    options->equ_files	  = WordList_create();
    options->obj_files	  = WordList_create();

    options->clu_cmd	  = NULL;
    options->cc_cmd	  = NULL;
    options->ld_cmd	  = NULL;
    options->cluhome	  = NULL;

    /*
     * Parse options.
     */
    i = 1;
    while (i < argc) {
	char* arg = argv[i++];

	if (strcmp(arg, "-debug") == 0) {
	    options->debug = TRUE;
	    continue;
	}

	if (strcmp(arg, "-optimize") == 0) {
	    options->optimize = TRUE;
	    continue;
	}

	if (strcmp(arg, "-prof") == 0) {
	    options->prof = TRUE;
	    continue;
	}

	if (strcmp(arg, "-verbose") == 0) {
	    options->verbose = TRUE;
	    continue;
	}

	if (strcmp(arg, "-cfile") == 0) {
	    options->leave = TRUE;
	    continue;
	}

	if (strcmp(arg, "-spec") == 0) {
	    options->spec = TRUE;
	    continue;
	}

	if (strcmp(arg, "-compile") == 0) {
	    options->compile = TRUE;
	    continue;
	}

	if (strcmp(arg, "-link") == 0) {
	    if (i < argc) {
		options->link = TRUE;
		options->program = argv[i++];
		continue;
	    }
	    error1("missing option to -link");
	    return FALSE;
	}

	if (strcmp(arg, "-create") == 0) {
	    if (i < argc) {
		options->create = TRUE;
		options->library = argv[i++];
		continue;
	    }
	    error1("missing option to -create");
	    return FALSE;
	}

	if (strcmp(arg, "-use") == 0) {
	    if (i < argc) {
		WordList_append(options->use, argv[i++]);
		continue;
	    }
	    error1("missing option to -use");
	    return FALSE;
	}

	if (has_suffix(arg, ".equ")) {
	    WordList_append(options->equ_files, arg);
	    continue;
	}

	if (has_suffix(arg, ".clu")) {
	    WordList_append(options->clu_files, arg);
	    continue;
	}

	if (has_suffix(arg, ".o")) {
	    WordList_append(options->obj_files, arg);
	    continue;
	}

	if (has_suffix(arg, ".a")) {
	    WordList_append(options->obj_files, arg);
	    continue;
	}

	if (has_suffix(arg, ".lib")) {
	    WordList_append(options->use, arg);
	    continue;
	}

	error2(arg, (arg[0] == '-') ? "unkown option" : "unknown file type");
	return FALSE;
    }

    /*
     * Check for conflicting options.
     */
    if (options->spec && options->compile) {
	error_conflict("spec", "compile");
	return FALSE;
    }

    if (options->spec && options->link) {
	error_conflict("spec", "link");
	return FALSE;
    }

    if (options->compile && options->link) {
	error_conflict("compile", "link");
	return FALSE;
    }

    if (options->compile && options->create) {
	error_conflict("compile", "create");
	return FALSE;
    }

    if (options->create && options->link) {
	error_conflict("create", "link");
	return FALSE;
    }

    if (options->debug && options->optimize) {
	error_conflict("debug", "optimize");
	return FALSE;
    }

    /*
     * Set-up defaults if necessary.
     */
    if ((!options->spec) && (!options->link) && (!options->create)) {
	/* Compile by default */
	options->compile = TRUE;
    }

    /*
     * Check for wrong file types.
     */
    if ((!options->link) && (!WordList_empty(options->obj_files))) {
	error2(WordList_first(options->obj_files),
	       "can only be specified with -link");
	return FALSE;
    }

    if (options->link && (!WordList_empty(options->equ_files))) {
	error2(WordList_first(options->equ_files),
	       "cannot be specified with -link");
	return FALSE;
    }

    if (options->link && (!WordList_empty(options->clu_files))) {
	error2(WordList_first(options->clu_files),
	       "cannot be specified with -link");
	return FALSE;
    }

    if (options->link && (!WordList_empty(options->use))) {
	error2(WordList_first(options->use),
	       "cannot be specified with -link");
	return FALSE;
    }

    /*
     * Check for minimal specification.
     */
    if (options->link && WordList_empty(options->obj_files)) {
	error1("no object files specified for linking");
	return FALSE;
    }

    if (options->compile && WordList_empty(options->clu_files)) {
	error1("no clu files specified for compiling");
	return FALSE;
    }

    if (options->spec && WordList_empty(options->clu_files)) {
	error1("no clu files specified for spec checking");
	return FALSE;
    }

    /* Create implies spec */
    if (options->create) {
	options->spec = TRUE;
    }

    /* Set-up CLU home */
    options->cluhome = getenv("CLUHOME");
    if (options->cluhome == NULL) {
	/* Use compiled in value */
	options->cluhome = CLUHOME;

	/* Set-up environment */
	if (setenv("CLUHOME", options->cluhome, TRUE) < 0) {
	    error1("setenv: out of memory");
	    return FALSE;
	}
    }

    /* Create various commands */
    options->clu_cmd = clu_compile_command(options);
    options->cc_cmd  = cc_compile_command(options);
    options->ld_cmd  = ld_compile_command(options);

    return TRUE;
}

bool has_suffix(str, suffix)
     char* str;
     char* suffix;
{
    int str_length, suffix_length;

    str_length = strlen(str);
    suffix_length = strlen(suffix);

    if (str_length < suffix_length) {
	return FALSE;
    }

    return (strcmp((str + (str_length - suffix_length)), suffix) == 0);
}

char* change_suffix(str, old_suffix, new_suffix)
     char* str;
     char* old_suffix;
     char* new_suffix;
{
    int str_length, old_length, new_length;
    char* new_string;

    str_length = strlen(str);
    old_length = strlen(old_suffix);
    new_length = strlen(new_suffix);

    new_string = (char*) malloc(sizeof(char) *
				(1 + str_length + new_length - old_length));
    strncpy(new_string, str, str_length - old_length);
    strcpy(new_string + str_length - old_length, new_suffix);

    return new_string;
}

int clu_compile(options)
     Options* options;
{
    WordIter* i;
    int result = 0;

    for (i = WordIter_start(options->clu_files);
	 WordIter_valid(i);
	 WordIter_next(i)) {
	result = clu_compile_file(options, WordIter_element(i));
	if (result != 0) break;
    }
    WordIter_stop(i);

    return result;
}

int clu_compile_file(options, file)
     Options* options;
     char* file;
{
    char* c_file;
    int result;

    Buffer* command = Buffer_copy(options->clu_cmd);
    if (options->leave) {
        Buffer_append(command, " -cfile ");
	}

    Buffer_append(command, " -compile ");
    Buffer_append(command, file);

    result = run_command(options, command);
    Buffer_destroy(command);

    if (result != 0) {
	/* Error */
	return result;
    }
/*
    command = Buffer_copy(options->cc_cmd);

    c_file = change_suffix(file, ".clu", ".c");
    Buffer_append(command, " -c ");
    Buffer_append(command, c_file);

    result = run_command(options, command);
    Buffer_destroy(command);

    if (! options->leave) {
	unlink(c_file);
    }

    free(c_file);
*/

    return result;
}

int clu_link(options)
     Options* options;
{
    return run_command(options, options->ld_cmd);
}

int clu_create(options)
     Options* options;
{
    int result;
    Buffer* command = Buffer_copy(options->clu_cmd);

    if (! WordList_empty(options->clu_files)) {
	WordIter* i;

	Buffer_append(command, " -spec");

	for (i = WordIter_start(options->clu_files);
	     WordIter_valid(i);
	     WordIter_next(i)) {
	    Buffer_append(command, " ");
	    Buffer_append(command, WordIter_element(i));
	}
	WordIter_stop(i);
    }

    Buffer_append(command, " -dump ");
    Buffer_append(command, options->library);

    result = run_command(options, command);
    Buffer_destroy(command);
    return result;
}

int clu_spec(options)
     Options* options;
{
    WordIter* i;
    int result;
    Buffer* command = Buffer_copy(options->clu_cmd);

    Buffer_append(command, " -spec");

    for (i = WordIter_start(options->clu_files);
	 WordIter_valid(i);
	 WordIter_next(i)) {
	Buffer_append(command, " ");
	Buffer_append(command, WordIter_element(i));
    }
    WordIter_stop(i);

    result = run_command(options, command);
    Buffer_destroy(command);
    return result;
}

Buffer* clu_compile_command(options)
     Options* options;
{
    Buffer* buffer = Buffer_create();

    Buffer_append(buffer, options->cluhome);
    Buffer_append(buffer, "/");
    Buffer_append(buffer, CLU_COMPILER);
    Buffer_append(buffer, " -ext false");

    /* Merge libraries if any */
    if (! WordList_empty(options->use)) {
	WordIter* i;

	Buffer_append(buffer, " -merge");

	for (i = WordIter_start(options->use);
	     WordIter_valid(i);
	     WordIter_next(i)) {
	    Buffer_append(buffer, " ");
	    Buffer_append(buffer, WordIter_element(i));
	}
	WordIter_stop(i);
    }

    /* Use equates if any */
    if (! WordList_empty(options->equ_files)) {
	WordIter* i;

	Buffer_append(buffer, " -ce");

	for (i = WordIter_start(options->equ_files);
	     WordIter_valid(i);
	     WordIter_next(i)) {
	    Buffer_append(buffer, " ");
	    Buffer_append(buffer, WordIter_element(i));
	}
	WordIter_stop(i);
    }

    /* Add optimize if necessary */
    if (options->optimize || (! options->debug)) {
	Buffer_append(buffer, " -optimize");
    }

    return buffer;
}

Buffer* cc_compile_command(options)
     Options* options;
{
    Buffer* buffer = Buffer_create();

    Buffer_append(buffer, C_COMPILER);

    /* Debug/Optimize options */
    if (options->debug) {
	Buffer_append(buffer, " ");
	Buffer_append(buffer, CFLAGS_DEBUG);
	Buffer_append(buffer, " -I");
	Buffer_append(buffer, options->cluhome);
	Buffer_append(buffer, "/");
	Buffer_append(buffer, CLU_DEBUG_INCLUDE);
    }
    else if (options->optimize) {
	Buffer_append(buffer, " ");
	Buffer_append(buffer, CFLAGS_OPTIMIZE);
	Buffer_append(buffer, " -I");
	Buffer_append(buffer, options->cluhome);
	Buffer_append(buffer, "/");
	Buffer_append(buffer, CLU_INCLUDE);
    }
    else {
	Buffer_append(buffer, " ");
	Buffer_append(buffer, CFLAGS_NORMAL);
	Buffer_append(buffer, " -I");
	Buffer_append(buffer, options->cluhome);
	Buffer_append(buffer, "/");
	Buffer_append(buffer, CLU_INCLUDE);
    }

    return buffer;
}

Buffer* ld_compile_command(options)
     Options* options;
{
    Buffer* buffer = Buffer_create();

    Buffer_append(buffer, C_COMPILER);
#if !defined(i386) && !defined(__NetBSD)
    if (!options->prof) Buffer_append(buffer, " -x ");
#else
    if (!options->prof) ;
#endif
    else Buffer_append(buffer, " -p ");
    Buffer_append(buffer, " -o ");
    Buffer_append(buffer, options->program);

    /* Add object files */
    if (! WordList_empty(options->obj_files)) {
	WordIter* i;

	for (i = WordIter_start(options->obj_files);
	     WordIter_valid(i);
	     WordIter_next(i)) {
	    Buffer_append(buffer, " ");
	    Buffer_append(buffer, WordIter_element(i));
	}
	WordIter_stop(i);
    }

    /* Add debugging support if necessary */
/*
    if (options->debug) {
	Buffer_append(buffer, " ");
	Buffer_append(buffer, options->cluhome);
	Buffer_append(buffer, "/");
	Buffer_append(buffer, CLU_DEBUG_SUPPORT);
    }
*/

    /* Add library */
    Buffer_append(buffer, " -L");
    Buffer_append(buffer, options->cluhome);
    Buffer_append(buffer, "/");
    Buffer_append(buffer, CLU_LIBDIR);
    Buffer_append(buffer, " ");

    if (options->debug) {
	Buffer_append(buffer, CLU_DEBUG_LIB);
    }
    else {
	Buffer_append(buffer, CLU_LIB);
    }

    /* Other libraries */
    Buffer_append(buffer, " ");
    Buffer_append(buffer, CLU_OTHER_LIBS);

    return buffer;
}

int run_command(options, command)
     Options* options;
     Buffer* command;
{
    int result;
    char* str = Buffer_extract(command);

    if (options->verbose) {
	bprint(command);
    }

    result = system(str);
#ifndef __alpha
    result = WEXITSTATUS(*(union wait*)(&result));
#endif

    free(str);
    return result;
}

void bprint(buffer)
     Buffer* buffer;
{
    char* str = Buffer_extract(buffer);
    printf("%s\n", str);
    free(str);
}

/*
 * Define setenv in case the system does not provide it.
 */
#ifdef NO_SETENV

int setenv(name, value, overwrite)
     char* name;
     char* value;
     int   overwrite;
{
    Buffer* buffer;
    char* binding;

    if (!overwrite && (getenv(name) != NULL)) {
	/* Nothing to change */
	return 0;
    }

    /* Construct binding string */
    buffer = Buffer_create();
    Buffer_append(buffer, name);
    Buffer_append(buffer, "=");
    Buffer_append(buffer, value);
    binding = Buffer_extract(buffer);
    Buffer_destroy(buffer);

    if (putenv(binding) != 0) {
	/* Error */
	free(binding);
	return -1;
    }

    /* Success - do NOT free binding */
    return 0;
}

#endif /* NO_SETENV */
