#ifndef PCLU_ERR
#define PCLU_ERR

/*						  */
/*	err.h				          */
/*						  */
/*	Defines error codes and signalling macros */

typedef long errcode;
/*typedef char * errlist[10]; */

/* Negative numbers for LINUX */

#define ERR_ok			0
#define ERR_overflow 		-1
#define ERR_negative_exponent 	-2
#define ERR_zero_divide 	-3
#define ERR_break		-4
#define ERR_iterbodyexit	-5
#define ERR_iterbodysignal	-6
#define ERR_iterbodyreturn	-7
#define ERR_forbodyexit		-5
#define ERR_forbodysignal	-6
#define ERR_forbodyreturn	-7
#define ERR_bounds		-8
#define ERR_failure		-9
#define ERR_illegal_char	-10
#define ERR_badfmt		-11
#define ERR_negative_size	-12
#define ERR_heap_exhausted	-13
#define ERR_not_possible	-14
#define ERR_toobig		-15
#define ERR_not_found		-16
#define ERR_bad_format		-17
#define ERR_end_of_file		-18
#define ERR_permanent		-19
#define ERR_illegal_stream	-20
#define ERR_no_limit		-21
#define ERR_negative_field_width -22
#define ERR_wrong_tag		-23
#define ERR_wrong_type		-24
#define ERR_found		-25
#define ERR_illegal_field_width -26
#define ERR_undefined		-27
#define ERR_insufficient_field_width -28
#define ERR_script_failed	-29
#define ERR_iteriterbodyexit	-30
#define ERR_iteriterbodysignal	-31
#define ERR_iteriterbodyreturn	-32
#define ERR_iterforbodyexit	-30
#define ERR_iterforbodysignal	-31
#define ERR_iterforbodyreturn	-32
#define ERR_none		-33
#define ERR_illegal_size	-34
#define ERR_bad_code		-35

#define ERR_a_cons		-50
#define ERR_abstract		-51
#define ERR_anyize		-52
#define ERR_bad			-53
#define ERR_cont		-54
#define ERR_eof			-55
#define ERR_error		-56
#define ERR_exists		-57
#define ERR_extra		-58
#define ERR_finish		-59
#define ERR_illegal		-60
#define ERR_include_failed	-61
#define ERR_local		-62
#define ERR_no_directive	-63
#define ERR_no_du		-64
#define ERR_open_failed		-65
#define ERR_recons		-66
#define ERR_specs_exist		-67
#define ERR_toohard		-68
#define ERR_underflow		-69

#define ERR_negative_time	-70
#define ERR_negative_multiplier -71
#define ERR_bad_divisor		-72
#define ERR_bad_size		-73
#define ERR_change		-74
#define ERR_empty		-75
#define ERR_invalid_format	-76
#define ERR_illegal_signal	-77
#define ERR_no_server		-78
#define ERR_timeout		-79
#define ERR_bad_address		-80
#define ERR_complex_result	-81
#define ERR_unterminated_quote	-82
#define ERR_specs_missing     -83
#define ERR_duplicate         -84


#define UMINERR 502
#define ERR_missing		-502
#define ERR_already_flattened   -503
#define UMAXERR 503
#define MAX_INTERNAL_ERROR	1000

/* 2/26/01 dwc: nuked off the following old-style defn */
/* extern int strcmp(); */
#define errcmp(s1, s2) \
	(((long) (s1) < 0 && (long)(s1) > -MAX_INTERNAL_ERROR) ? false : \
	(((long) (s2) < 0 && (long)(s2) > -MAX_INTERNAL_ERROR) ? false : \
		(!strcmp(s1, s2))))
#if defined(sun) && defined(sparc) /* errcmp macro breaks -O on Sparcs */
#undef errcmp
#endif

#ifndef errcmp
extern long errcmp();
#endif
#define ecodeOPlookup _pclu_erstr

#endif
