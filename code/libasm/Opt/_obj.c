
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _obj.c,v 1.3 91/06/06 13:49:10 dcurtis Exp $";
#endif
/* $Log:	_obj.c,v $
 * Revision 1.3  91/06/06  13:49:10  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  13:08:32  root
 * fixed aggregate initialization in ops_actual
 * 
 * Revision 1.1  91/02/04  23:20:48  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_obj			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern int heaplo;
extern int heaphi;
extern errcode _rtn_infoOPis_proc();
extern errcode _rtn_infoOPis_iter();
extern errcode intOPprint();

/* may return false positives */

errcode _objOPis_ref(x, ans)
CLUREF x, *ans;
{
	if (x.num >= heaplo) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _objOPis_int_(x, ans)
CLUREF x, *ans;
{
	ans->tf = true;
	signal(ERR_ok);
	}

errcode _objOPis_real_(x, ans)
CLUREF x, *ans;
{
	ans->tf = false;
	signal(ERR_ok);
	}

errcode _objOPis_uninit_(x, ans)
CLUREF x, *ans;
{
	if (x.num == 0x80808080) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _objOPis_proc_(x, ans)
CLUREF x, *ans;
{
errcode err;

	if (x.num <= heaphi || x.vec->typ.val != CT_PROC) {
		ans->tf = false;
		signal(ERR_ok);
		}
	err = _rtn_infoOPis_proc(x, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}


errcode _objOPis_iter_(x, ans)
CLUREF x, *ans;
{
errcode err;

	if (x.num <= heaphi || x.vec->typ.val != CT_PROC) {
		ans->tf = false;
		signal(ERR_ok);
		}
	err = _rtn_infoOPis_iter(x, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}


errcode _objOPis_routine_(x, ans)
CLUREF x, *ans;
{
	if (x.num <= heaphi || x.vec->typ.val != CT_PROC) {
		ans->tf = false;
		signal(ERR_ok);
		}
	ans->tf = true;
	signal(ERR_ok);
	}

errcode _objOPis_vec(x, ans)
CLUREF x, *ans;
{
	if (x.num < heaplo || x.vec->typ.val != CT_AGG) {
		ans->tf = false;
		signal(ERR_ok);
		}
	ans->tf = true;
	signal(ERR_ok);
	}


errcode _objOPis_bvec(x, ans)
CLUREF x, *ans;
{
	if (x.num < heaplo || x.vec->typ.val != CT_STRING) {
		ans->tf = false;
		signal(ERR_ok);
		}
	ans->tf = true;
	signal(ERR_ok);
	}

errcode _objOPis_cell(x, ans)
CLUREF x, *ans;
{
	if (x.num < heaplo || x.vec->typ.val != CT_VAR) {
		ans->tf = false;
		signal(ERR_ok);
		}
	ans->tf = true;
	signal(ERR_ok);
	}

/* need to distinguish any's from variants */

errcode _objOPis_anyize(x, ans)
CLUREF x, *ans;
{
	if (x.num < heaplo || x.vec->typ.val != CT_VAR) {
		ans->tf = false;
		signal(ERR_ok);
		}
	ans->tf = true;
	signal(ERR_ok);
	}

/* check array vs dope vector definition */

errcode _objOPis_adv(x, ans)
CLUREF x, *ans;
{
	if (x.num < heaplo || x.vec->typ.val != CT_ARRAY) {
		ans->tf = false;
		signal(ERR_ok);
		}
	ans->tf = true;
	signal(ERR_ok);
	}

/* have no idea what an entry vector is */

errcode _objOPis_entry_vector(x, ans)
CLUREF x, *ans;
{
	if (x.num <= heaphi || x.vec->typ.val != CT_PROC) {
		ans->tf = false;
		signal(ERR_ok);
		}
	/* is it a closure: look for movl/jump instrs */
	ans->tf = true;
	signal(ERR_ok);
	}


errcode _objOPget_addr(x, ans)
CLUREF x, *ans;
{
	ans->num = x.num;
	signal(ERR_ok);
	}

errcode _objOPget_left(x, ans)
CLUREF x, *ans;
{
	ans->num = (x.num >> 16) & 0xffff;
	signal(ERR_ok);
	}

errcode _objOPget_right(x, ans)
CLUREF x, *ans;
{
	ans->num = x.num & 0xffff;
	signal(ERR_ok);
	}

errcode _objOPlt(x, y, ans)
CLUREF x, y, *ans;
{
	if ((unsigned)x.num < (unsigned)y.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _objOPle(x, y, ans)
CLUREF x, y, *ans;
{
	if ((unsigned)x.num <= (unsigned)y.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _objOPge(x, y, ans)
CLUREF x, y, *ans;
{
	if ((unsigned)x.num >= (unsigned)y.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _objOPgt(x, y, ans)
CLUREF x, y, *ans;
{
	if ((unsigned)x.num > (unsigned)y.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _objOPequal(x, y, ans)
CLUREF x, y, *ans;
{
	if (x.num == y.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _objOPsimilar(x, y, ans)
CLUREF x, y, *ans;
{
	if (x.num == y.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _objOPcopy(x, ans)
CLUREF x, *ans;
{
	ans->num = x.num;
	signal(ERR_ok);
	}


errcode _objOPprint(x, ps)
CLUREF x, ps;
{
errcode err;

	err = intOPprint(x, ps);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}


typedef struct {
int count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[9];
} _obj_OPS;

OWN_ptr _obj_own_init = {1, 0};

CLU_proc _obj_oe_copy = {{0,0,0,0}, _objOPcopy, 0};
CLU_proc _obj_oe_equal = {{0,0,0,0}, _objOPequal, 0};
CLU_proc _obj_oe_similar = {{0,0,0,0}, _objOPsimilar, 0};
CLU_proc _obj_oe_print = {{0,0,0,0}, _objOPprint, 0};
CLU_proc _obj_oe_lt = {{0,0,0,0}, _objOPlt, 0};
CLU_proc _obj_oe_le = {{0,0,0,0}, _objOPle, 0};
CLU_proc _obj_oe_ge = {{0,0,0,0}, _objOPge, 0};
CLU_proc _obj_oe_gt = {{0,0,0,0}, _objOPgt, 0};

_obj_OPS _obj_ops_actual = {9,
    &_obj_own_init,
    &_obj_own_init, {
{&_obj_oe_copy, "copy"},
{&_obj_oe_equal, "equal"},
{&_obj_oe_similar, "similar"},
{&_obj_oe_print, "debug_print"},
{&_obj_oe_print, "print"},
{&_obj_oe_lt, "lt"},
{&_obj_oe_le, "le"},
{&_obj_oe_ge, "ge"},
{&_obj_oe_gt, "gt"}}
};

_obj_OPS *_obj_ops = &_obj_ops_actual;
