
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/util.c,v 1.8 91/08/29 16:44:47 dcurtis Exp $";
#endif
/* $Log:	util.c,v $
 * Revision 1.8  91/08/29  16:44:47  dcurtis
 * added definition for find_selops_init function (lint)
 * 
 * Revision 1.7  91/08/29  16:18:16  dcurtis
 * fixed definitions for args to find_ops_init, find_selops_init (lint)
 * 
 * Revision 1.6  91/08/29  14:57:06  dcurtis
 * fixed streamOP_close_all & _chanOP_save_tty calls to include elist argument
 * 
 * Revision 1.5  91/06/06  17:21:25  root
 * made CLU_n exist only on sparc stations
 * 
 * Revision 1.4  91/06/06  13:32:21  root
 * added copyright notice
 * 
 * Revision 1.3  91/06/03  11:47:56  root
 * sparcstation compatibility: int->CLUREF
 * 	also generated unions containing small integers
 * 	and cleaned up returning of function value of main program
 * 
 * Revision 1.2  91/02/14  16:20:48  dcurtis
 * changed #ifdef gc to #ifdef GC, to avoid name conflicts
 * 
 * Revision 1.1  91/02/04  15:49:57  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*		   main				*/
/*    includes several other support routines   */
/*						*/

#define util
#include <stdio.h>
#include "signal.h"
#include <sys/time.h>
#include <sys/resource.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"
#include <string.h>
#include <errno.h>

extern void *gc_malloc();
extern void exit();
extern void find_ops_init();
extern void find_selops_init();
#ifndef LINUX
extern void expand_hp();
extern struct obj * gc_malloc();
#endif
extern void gc_init();
extern char *clu_errlist[];
extern char *clu_uerrlist[];
extern void clu_alloc();
extern errcode clu_err_string_init();
extern errcode clu_int_init();
char **environ;
errlist elist;

CLUREF CUR_PROC_VAR;
CLUREF CLU_string_size;
struct obj **NewObjPtr;
int argc;
char **argv;
char **envp;

CLUREF empty_string;
CLUREF clu_empty_string;
bool print_stat = false;
/* extern errcode string_anal(); */

#undef generic_CLU_proc

struct {
	CLUTYPE typ;
	errcode (*proc)();
	OWNPTR	type_owns;
	OWNPTR  op_owns;
	} generic_CLU_proc;

struct OPS NO_OPS = {0,0,0};
struct OPS NULL_OPS = {0,0,0};
Vlist0 NO_VALS = {0,0};
Slist0 NO_PARMS = {0,0};


long maxheapsize = 0;
/* extern long STACKTOP; */

int main(_argc, _argv, _envp)
int _argc;
char **_argv, **_envp;
{
int err;
/*	STACKTOP = get_sp(); */
/*
	int sz = sizeof(CLUREF);
	int temp1 = INT_MIN;
	int temp2 = LONG_MIN;
	int temp3 = intOPleastint;
	int temp4 = ((long)MIN_ARR_INDEX);
	printf ("size of CLUREF %d\n", sz);
	printf ("INT_MIN %d\n", INT_MIN);
	printf ("LONG_MIN %d\n", LONG_MIN);
	printf ("intOPleastint %d\n", intOPleastint);
	printf ("((long)MIN_ARR_INDEX) %d\n", ((long)MIN_ARR_INDEX));
*/
	err = main_2(_argc, _argv, _envp);
	return(err);
	}

/* Routine to get the ball rolling */
main_2(_argc, _argv, _envp)
int _argc;
char **_argv, **_envp;
{
errcode err, pgmerr;
#ifndef LINUX
struct sigstack ss, oss;
#endif
OWNPTR save_own_ptr;
OWNREQ save_own_req;
void (*save_own_proc)();
OWNPTR *save1, *save2, *save3, *save4;
struct rlimit rlimit;
struct rusage rusage_1;
struct rusage rusage_2;
struct timeval tv1, tv2;
struct timezone tz;

/* make input information available via globals */

	argc = _argc;
	argv = _argv;
	envp = _envp;
	environ = _envp;

	/*printf("%s\n", _envp[0]); */
	/*printf("%s\n", environ[0]); */

/* see what the operating system will give us for heap space */
/* 	and tell it we want the max			     */
/* note that the heap is in the data area for portable clu,  */
/*	while the original vax clu has the heap in P1 (usually the stack) */
	err = getrlimit(RLIMIT_DATA, &rlimit);
	if (err != 0) {
		perror("util failed in getrlimit");
		exit(-10);
		}
	/* fprintf(stderr, "soft data limit: %d\n", rlimit.rlim_cur); */
	/* fprintf(stderr, "hard data limit: %d\n", rlimit.rlim_max); */
	rlimit.rlim_cur = rlimit.rlim_max;
	maxheapsize = rlimit.rlim_max;
	err = setrlimit(RLIMIT_DATA, &rlimit);
	if (err != 0) {
		perror("util failed in setrlimit");
		exit(-10);
		}


/* garbage collector initialization (boehm variant) */

	gc_init();	/* includes expand_hp of 16 hblks => 64Kbytes */
	expand_hp(112); /* expands up to 0.5 Megabytes */

/*
	printf("%s\n", clu_errlist[0]);
	printf("%s\n", clu_errlist[1]);
	printf("%s\n", clu_errlist[2]);
	printf("%s\n", clu_errlist[23]);
*/

/* set up stack for signal handlers */

#ifndef LINUX
	ss.ss_onstack = 1;
	ss.ss_sp = (char *)gc_malloc(100);
	sigstack(&ss, &oss);
#endif

/* initialize storage for ops manager */

	find_ops_init(&save_own_ptr, &save_own_req, &save_own_proc);
	find_selops_init(&save1, &save2, &save3, &save4);

/* set up empty string constants */

	err = stringOPcons("", CLU_1, CLU_0, &empty_string);
	if (err != ERR_ok) fprintf(stderr, "failure: %s\n", elist[0].str->data);
	clu_empty_string = empty_string;

/* set up error string and integer constants */

	clu_err_string_init();
#ifdef sparc
	clu_int_init();
#endif

/* collect statistics on user's program */

	err = gettimeofday(&tv1, &tz);
	if (err != 0) {
		perror("util failed in gettimeofday #1");
		exit(-10);
		}
	err = getrusage(0, &rusage_1);
	if (err != 0) {
		perror("util failed in getruage #1");
		exit(-10);
		}

/* invoke the user's program */

	pgmerr = start_up();
	if (pgmerr != ERR_ok) {
		fflush(stdout);
		fprintf(stderr, "failure: %s\n", elist[0].str->data);
		}

/* collect statistics on user's program */

	err = getrusage(0, &rusage_2);
	if (err != 0) {
		perror("util failed in getruage #2");
		exit(-10);
		}
	err = gettimeofday(&tv2, &tz);
	if (err != 0) {
		perror("util failed in gettimeofday #2");
		exit(-10);
		}
	if (print_stat) {
	fprintf(stderr, "wall clock time %d secs\n", tv2.tv_sec - tv1.tv_sec);
	fprintf(stderr, "user time %d, system time %d, user+system time %d secs\n",
			rusage_2.ru_utime.tv_sec - rusage_1.ru_utime.tv_sec,
			rusage_2.ru_stime.tv_sec - rusage_1.ru_stime.tv_sec,
			rusage_2.ru_stime.tv_sec - rusage_1.ru_stime.tv_sec +
			rusage_2.ru_utime.tv_sec - rusage_1.ru_utime.tv_sec);
	fprintf(stderr, "max resident set sizes (start & end), %d & %d Kbytes\n",
			rusage_1.ru_maxrss, rusage_2.ru_maxrss);
	fprintf(stderr, "minor page faults %d, major page faults %d\n",
			rusage_2.ru_minflt - rusage_1.ru_minflt,
			rusage_2.ru_majflt - rusage_1.ru_majflt);
	fprintf(stderr, "number of times swapped out %d\n",
			rusage_2.ru_nswap - rusage_1.ru_nswap);
	fprintf(stderr, "input via file system %d, output via file system %d\n",
			rusage_2.ru_inblock - rusage_1.ru_inblock,
			rusage_2.ru_oublock - rusage_1.ru_oublock);
	fprintf(stderr, "IPC input %d, IPC output %d\n",
			rusage_2.ru_msgrcv - rusage_1.ru_msgrcv,
			rusage_2.ru_msgsnd - rusage_1.ru_msgsnd);
	fprintf(stderr, "number of signals delivered %d\n",
			rusage_2.ru_nsignals - rusage_1.ru_nsignals);
	fprintf(stderr, "number of voluntary give ups %d\n",
			rusage_2.ru_nvcsw - rusage_1.ru_nvcsw);
	fprintf(stderr, "number of involuntary give ups %d\n",
			rusage_2.ru_nivcsw - rusage_1.ru_nivcsw);
/*	string_anal(); */
	}


/* exit */

	streamOP_close_all();	/* replace by call to _cleanup_ when gprof in */
	_chanOP_save_tty();
	if (pgmerr == ERR_ok) return(0);
	else return(-1);
	}

/*	Routine to compare user-defined signals (strings)
		internal signals are integers between 0 and MAX_INTERNAL_ERROR
*/

#undef errcmp
#ifndef LINUX
bool errcmp(s1, s2)
char *s1, *s2;
{
	if ((long)s1 > 0 && (long)s1 < MAX_INTERNAL_ERROR) return (false);
	if ((long)s2 > 0 && (long)s2 < MAX_INTERNAL_ERROR) return (false);
	return (!strcmp(s1, s2));
	}
#else
bool errcmp(s1, s2)
char *s1, *s2;
{
long longs1 = (long)s1;
long longs2 = (long)s2;
	if (longs1 < 0 && longs1 > -MAX_INTERNAL_ERROR) {
		longs1 = -longs1;
		s1 = (char *)longs1;
	}
	if (longs2 < 0 && longs2 > -MAX_INTERNAL_ERROR) {
		longs2 = -longs2;
		s2 = (char *)longs2;
	}
	// if ((long)s1 < 0 && (long)s1 > -MAX_INTERNAL_ERROR) (long)s1 = -(long)s1;
	// if ((long)s2 < 0 && (long)s2 > -MAX_INTERNAL_ERROR) (long)s2 = -(long)s2;
	if ((long)s1 > 0 && (long)s1 < MAX_INTERNAL_ERROR) return (false);
	if ((long)s2 > 0 && (long)s2 < MAX_INTERNAL_ERROR) return (false);
	return (!strcmp(s1, s2));
	}
#endif

extern int sys_nerr;
extern long clu_nerr;

/* Routine to convert a clu signal to a string */

CLUREF _pclu_erstr(sig)
long sig;
{
char *temp;
CLUREF temp_str, sz;

	if (sig < 0 && sig > -MAX_INTERNAL_ERROR) {
	    sig = -sig;
	    if (sig < MAX_INTERNAL_ERROR)  {
		if (sig <= clu_nerr) {
			temp = clu_errlist[sig];
			}
		else {
			temp = clu_uerrlist[sig - UMINERR];
			}
		sz.num = strlen(temp);
		stringOPcons(temp, CLU_1, sz, &temp_str);
		return(temp_str);
		}
	    }
	else {
/*					1, formerly */
		sz.num = strlen((char*) sig) - 4;
		stringOPcons((char*)sig, CLU_5, sz, &temp_str);
		return(temp_str);
		}
	}

/* Routine to produce the unhandled exception string */

void pclu_unhandled(err)
errcode err;
{
CLUREF tempstr;
CLUREF headstr;

	if (err != ERR_failure) tempstr = _pclu_erstr(err);
	else tempstr = elist[0];

	err = stringOPcons("unhandled exception: ", CLU_1, CLU_21, 
				&headstr);
	if (err != ERR_ok) {
		fprintf(stderr, " death in _pclu_unhandled\n");
		exit(-1);
		}
	err = stringOPconcat(headstr, tempstr, &elist[0]);
	if (err != ERR_ok) {
		fprintf(stderr, " death in _pclu_unhandled\n");
		exit(-1);
		}
	return;
	}

/* Routine to convert a unix error to a string */

CLUREF _unix_erstr(num)
long num;
{
const char * temp;
CLUREF temp_str, sz;

        temp = strerror(num);
	sz.num = strlen(temp);
	stringOPcons(temp, CLU_1, sz, &temp_str);
	return(temp_str);
	}

#define CLU_UNERR UMAXERR - UMINERR + 1

char *clu_uerrlist[CLU_UNERR] = {
	"missing",
	"already_flattened"
	};

#define CLU_NERR 85

long clu_nerr = CLU_NERR - 1;
char *clu_errlist[CLU_NERR] = {
	"ok",
	"overflow",
	"negative_exponent",
	"zero_divide",
	"break",
	"iterbodyexit",
	"iterbodysignal",
	"iterbodyreturn",
	"bounds",
	"failure",
	"illegal_char",
	"badfmt",
	"negative_size",
	"heap_exhausted",
	"not_possible",
	"toobig",
	"not_found",
	"bad_format",
	"end_of_file",
	"permanent",
	"illegal_stream",
	"no_limit",
	"negative_field_width",
	"wrong_tag",
	"wrong_type",
	"found",
	"illegal_field_width",
	"undefined",
	"insufficient_field_width",
	"script_failed",
	"iterforbodyexit",
	"iterforbodysignal",
	"iterforbodyreturn",
	"none",
	"illegal_size",
	"bad_code",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"unallocated",
	"a_cons",
	"abstract",
	"anyize",
	"bad",
	"cont",
	"eof",
	"error",
	"exists",
	"extra",
	"finish",
	"illegal",
	"include_failed",
	"local",
	"no_directive",
	"no_du",
	"open_failed",
	"recons",
	"specs_exist",
	"toohard",
	"underflow",
	"negative_time",
	"negative_multiplier",
	"bad_divisor",
	"bad_size",
	"change",
	"empty",
	"invalid_format",
	"illegal_signal",
	"no_server",
	"timeout",
	"bad_address",
	"complex_result",
	"unterminated_quote",
	"specs_missing",
	"duplicate"
	};

#define CLU_ERR_STRING 17

long clu_err_str = CLU_ERR_STRING - 1;
char *clu_err_string_list[CLU_ERR_STRING] = {
	"illegal access mode",
	"cannot write to this _chan",
	"unknown error",
	"cannot read from this _chan",
	"cannot duplicate _chan",
	"_chan is closed",
	"not a terminal _chan",
	"speeds unknown",
	"array bounds overflow",
	"cannot get connected directory",
	"bad format",
	"huge allocation request",
	"cannot read from input _chan",
	"cannot write to output _chan",
	"cannot write to error _chan",
	"no return values",
	"array bounds exception"
	};

CLUSTRING 	illegal_access_mode_STRING;
CLUSTRING	cannot_write_to_this__chan_STRING;
CLUSTRING	unknown_error_STRING;
CLUSTRING	cannot_read_from_this__chan_STRING;
CLUSTRING	cannot_duplicate__chan_STRING;
CLUSTRING	_chan_is_closed_STRING;
CLUSTRING	not_a_terminal__chan_STRING;
CLUSTRING	speeds_unknown_STRING;
CLUSTRING	array_bounds_overflow_STRING;
CLUSTRING	cannot_get_connected_directory_STRING;
CLUSTRING	bad_format_STRING;
CLUSTRING	huge_allocation_request_STRING;
CLUSTRING	cannot_read_from_input__chan_STRING;
CLUSTRING	cannot_write_to_output__chan_STRING;
CLUSTRING	cannot_write_to_error__chan_STRING;
CLUSTRING	no_return_values_STRING;
CLUSTRING	array_bounds_exception_STRING;

CLUSTRING *clu_glob_err_string_list[CLU_ERR_STRING] = {
	&illegal_access_mode_STRING,
	&cannot_write_to_this__chan_STRING,
	&unknown_error_STRING,
	&cannot_read_from_this__chan_STRING,
	&cannot_duplicate__chan_STRING,
	&_chan_is_closed_STRING,
	&not_a_terminal__chan_STRING,
	&speeds_unknown_STRING,
	&array_bounds_overflow_STRING,
	&cannot_get_connected_directory_STRING,
	&bad_format_STRING,
	&huge_allocation_request_STRING,
	&cannot_read_from_input__chan_STRING,
	&cannot_write_to_output__chan_STRING,
	&cannot_write_to_error__chan_STRING,
	&no_return_values_STRING,
	&array_bounds_exception_STRING
	};

errcode clu_err_string_init()
{
long i;
char * ith;
CLUSTRING *ithdest;
CLUREF sz;

	for (i = 0; i < CLU_ERR_STRING; i++) {
		ith = clu_err_string_list[i];
		ithdest = clu_glob_err_string_list[i];
		sz.num = strlen(ith);
		stringOPcons(ith, CLU_1, sz, ithdest);
		}
	signal(ERR_ok);
	}

#ifdef sparc
CLUREF CLU_0;
CLUREF CLU_1;
CLUREF CLU_2;
CLUREF CLU_3;
CLUREF CLU_4;
CLUREF CLU_5;
CLUREF CLU_6;
CLUREF CLU_7;
CLUREF CLU_8;
CLUREF CLU_9;
CLUREF CLU_10;
CLUREF CLU_11;
CLUREF CLU_12;
CLUREF CLU_13;
CLUREF CLU_14;
CLUREF CLU_15;
CLUREF CLU_16;
CLUREF CLU_17;
CLUREF CLU_18;
CLUREF CLU_19;
CLUREF CLU_20;
CLUREF CLU_21;
CLUREF CLU_22;
CLUREF CLU_23;
CLUREF CLU_24;
CLUREF CLU_25;
CLUREF CLU_26;
CLUREF CLU_27;
CLUREF CLU_28;
CLUREF CLU_29;
CLUREF CLU_30;
CLUREF CLU_31;
CLUREF CLU_32;
CLUREF CLU_33;
CLUREF CLU_34;
CLUREF CLU_35;
CLUREF CLU_36;
CLUREF CLU_37;
CLUREF CLU_38;
CLUREF CLU_39;
CLUREF CLU_40;
CLUREF CLU_41;
CLUREF CLU_42;
CLUREF CLU_43;
CLUREF CLU_44;
CLUREF CLU_45;
CLUREF CLU_46;
CLUREF CLU_47;
CLUREF CLU_48;
CLUREF CLU_49;
CLUREF CLU_50;

errcode clu_int_init()
{
	CLU_0.num = 0;
	CLU_1.num = 1;
	CLU_2.num = 2;
	CLU_3.num = 3;
	CLU_4.num = 4;
	CLU_5.num = 5;
	CLU_6.num = 6;
	CLU_7.num = 7;
	CLU_8.num = 8;
	CLU_9.num = 9;
	CLU_10.num = 10;
	CLU_11.num = 11;
	CLU_12.num = 12;
	CLU_13.num = 13;
	CLU_14.num = 14;
	CLU_15.num = 15;
	CLU_16.num = 16;
	CLU_17.num = 17;
	CLU_18.num = 18;
	CLU_19.num = 19;
	CLU_20.num = 20;
	CLU_21.num = 21;
	CLU_22.num = 22;
	CLU_23.num = 23;
	CLU_24.num = 24;
	CLU_25.num = 25;
	CLU_26.num = 26;
	CLU_27.num = 27;
	CLU_28.num = 28;
	CLU_29.num = 29;
	CLU_30.num = 30;
	CLU_31.num = 31;
	CLU_32.num = 32;
	CLU_33.num = 33;
	CLU_34.num = 34;
	CLU_35.num = 35;
	CLU_36.num = 36;
	CLU_37.num = 37;
	CLU_38.num = 38;
	CLU_39.num = 39;
	CLU_40.num = 40;
	CLU_41.num = 41;
	CLU_42.num = 42;
	CLU_43.num = 43;
	CLU_44.num = 44;
	CLU_45.num = 45;
	CLU_46.num = 46;
	CLU_47.num = 47;
	CLU_48.num = 48;
	CLU_49.num = 49;
	CLU_50.num = 50;
	signal(ERR_ok);
	}
#endif

char * mystrcat(s1, s2)
char *s1, *s2;
{
long size, i, j, len1, len2;
const char *temp;

	len1 = strlen(s1);
	len2 = strlen(s2);
	size = len1 + len2 + 1;
	temp = (char *)gc_malloc(size);
	bcopy(s1, temp, len1);
	bcopy(s2, &temp[len1], len2+1);
/*
	for (i = 0; i < len1; i++) {
		temp[i] = s1[i];
		}
	for (j = 0; j < len2; j++) {
		temp[i] = s2[j]; i++;
		}
	temp[size-1] = '\0';
*/
	return(temp);
	}

char * mystrcat2(s1, len1, s2, len2)
char *s1, *s2;
{
long size, i, j;
char *temp;

	size = len1 + len2 + 1;
	temp = (char *)gc_malloc(size);
	for (i = 0; i < len1; i++) {
		temp[i] = s1[i];
		}
	for (j = 0; j < len2; j++) {
		temp[i] = s2[j]; i++;
		}
	temp[size-1] = '\0';
	return(temp);
	}
/* OBSOLETE
errcode build_parm_table(reqs, ops, table)
struct REQS * reqs;
struct OPS * ops;
struct OPS ** table;
{
int i,j;
char *name, *name1;
struct OPS * temp;
errcode err;

/ * REQS -> OPS                        \/   	3/29/90 * /
	clu_alloc(sizeof(struct OPS) + 
		sizeof(struct OP_ENTRY) * (reqs->count - 1), &temp);
	temp->count = reqs->count;
	temp->type_owns = ops->type_owns;
	temp->op_owns = ops->op_owns;
	if (ops == 0) {*table = temp; signal(ERR_ok);}
	for (i = 0; i < reqs->count; i++) {
		name = reqs->entry[i].name;
		for (j = 0; j < ops->count; j++) {
			name1 = ops->entry[j].name;
			if (name1 == 0 || name1[0] != name[0]) continue;
			if (!(strcmp(name1, name))) {
				temp->entry[i].name = name1;
				temp->entry[i].fcn = ops->entry[j].fcn;
				break;}
			}
		}
	*table = temp;
	signal(ERR_ok);
	}
 */
