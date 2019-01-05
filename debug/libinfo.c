
/*	debugging information so that system functions */
/*		can be invoked			       */

#include "pclu_err.h"
#include "pclu_sys.h"
typedef struct {
	long	hdr;
	long	count;
	char  *name;
	char  *fname;
	bool   	is_iter;
	long    	is_parmd;
	errcode	(*addr)();
	long	nargs;
	Vlist0   *vals; 
	siglist0 *sigs;
	Vlist0 *locals;
	Vlist0 *owns;
	Vlist0 *ptowns;
	Vlist0 *popowns;
	Slist0 *type_formals;
	Slist0 *op_formals;
	} dbg_info_generic;
#include "libinfo.h"
