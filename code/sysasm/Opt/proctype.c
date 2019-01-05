
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: proctype.c,v 1.3 91/06/06 13:29:54 root Exp $";
#endif
/* $Log:	proctype.c,v $
 * Revision 1.3  91/06/06  13:29:54  root
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  12:20:22  root
 * fixed aggregate initialization in ops_actual
 * 
 * Revision 1.1  91/02/04  15:49:49  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*	 	    PROCTYPE			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode proctypeOPnew(nops, ans)
CLUREF nops;
CLUREF *ans;
{
CLUREF temp;
long size;

	size = sizeof(CLU_proc);
/*	if (nops.num > 1) size += (nops.num - 1)*sizeof(struct OPS *); */
	clu_alloc(size, &temp);
	temp.proc->typ.val = CT_PROC;
	temp.proc->typ.mark = 0;
	temp.proc->typ.refp = 0;
	ans->proc = temp.proc;
	signal(ERR_ok);
	}

/* totally wrong */
errcode proctypeOPequal(x1, x2, ans)
CLUREF x1, x2, *ans;
{
	if (x1.proc->proc == x2.proc->proc) {
		ans->tf = true;
		}
	else ans->tf = false;
	signal(ERR_ok);
	}

/* totally wrong */
errcode proctypeOPsimilar(x1, x2, ans)
CLUREF x1, x2, *ans;
{
	if (x1.proc == x2.proc) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode proctypeOPcopy(x1, ans)
CLUREF x1, *ans;
{
	ans->proc = x1.proc;
	signal(ERR_ok);
	}

errcode proctypeOPdebug_print(x1, ps)
CLUREF x1, ps;
{
errcode err;
CLUREF ans, str;

	stringOPcons("proctype", 1, 8, &str);
	err = pstreamOPtextc(ps, str, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}



typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[4];
} proctype_OPS;

OWN_ptr proctype_own_init = {1, 0};

CLU_proc proctype_oe_copy = {{0,0,0,0}, proctypeOPcopy,&proctype_own_init, &proctype_own_init};
CLU_proc proctype_oe_equal = {{0,0,0,0}, proctypeOPequal,&proctype_own_init, &proctype_own_init};
CLU_proc proctype_oe_similar = {{0,0,0,0}, proctypeOPsimilar,&proctype_own_init, &proctype_own_init};
CLU_proc proctype_oe_debug_print = {{0,0,0,0}, proctypeOPdebug_print,&proctype_own_init, &proctype_own_init};

proctype_OPS proctype_ops_actual = {4,
    &proctype_own_init,
    &proctype_own_init, {
{&proctype_oe_copy, "copy"},
{&proctype_oe_equal, "equal"},
{&proctype_oe_similar, "similar"},
{&proctype_oe_debug_print, "debug_print"}}
};

proctype_OPS *proctype_ops = &proctype_ops_actual;
