
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: itertype.c,v 1.3 91/06/06 13:28:17 root Exp $";
#endif
/* $Log:	itertype.c,v $
 * Revision 1.3  91/06/06  13:28:17  root
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  12:19:47  root
 * fixed aggregate initialization in ops_actual
 * 
 * Revision 1.1  91/02/04  15:49:44  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*	 	    ITERTYPE			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode itertypeOPnew(nops, ans)
CLUREF nops;
CLUREF *ans;
{
CLUREF temp;

	clu_alloc(sizeof(CLU_proc) + (nops.num - 1)*sizeof(struct OPS *), &temp);
	temp.proc->typ.val = CT_PROC;
	temp.proc->typ.mark = 0;
	temp.proc->typ.refp = 0;
	ans->proc = temp.proc;
	signal(ERR_ok);
	}

errcode itertypeOPequal(x1, x2, ans)
CLUREF x1, x2, *ans;
{
	if (x1.proc == x2.proc) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode itertypeOPsimilar(x1, x2, ans)
CLUREF x1, x2, *ans;
{
	if (x1.proc == x2.proc) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode itertypeOPcopy(x1, ans)
CLUREF x1, *ans;
{
	ans->proc = x1.proc;
	signal(ERR_ok);
	}

errcode itertypeOPdebug_print(x1, ps)
CLUREF x1, ps;
{
errcode err;
CLUREF ans, str;

	stringOPcons("itertype", 1, 8, &str);
	err = pstreamOPtextc(ps, str, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}



typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[4];
} itertype_OPS;

OWN_ptr itertype_own_init = {1, 0};

CLU_proc itertype_oe_copy = {{0,0,0,0}, itertypeOPcopy, &itertype_own_init, &itertype_own_init};
CLU_proc itertype_oe_equal = {{0,0,0,0}, itertypeOPequal, &itertype_own_init, &itertype_own_init};
CLU_proc itertype_oe_similar = {{0,0,0,0}, itertypeOPsimilar, &itertype_own_init, &itertype_own_init};
CLU_proc itertype_oe_debug_print = {{0,0,0,0}, itertypeOPdebug_print, &itertype_own_init, &itertype_own_init};

itertype_OPS itertype_ops_actual = {4,
    &itertype_own_init,
    &itertype_own_init, {
{&itertype_oe_copy, "copy"},
{&itertype_oe_equal, "equal"},
{&itertype_oe_similar, "similar"},
{&itertype_oe_debug_print, "debug_print"}}
};

itertype_OPS *itertype_ops = &itertype_ops_actual;
