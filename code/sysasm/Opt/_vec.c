
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _vec.c,v 1.6 91/06/06 13:26:15 root Exp $";
#endif
/* $Log:	_vec.c,v $
 * Revision 1.6  91/06/06  13:26:15  root
 * added copyright notice
 * 
 * Revision 1.5  91/06/03  12:15:43  root
 * sparcestation compatibility: int->CLUREF
 * 
 * Revision 1.4  91/05/31  11:58:33  root
 * fixed aggregate initialization in ops_actual
 * changed int arguement to CLUREF argument in copy operation
 * 
 * Revision 1.3  91/02/14  16:37:44  dcurtis
 * fixed move_rl, move_lr off by one in index in copy
 * 
 * Revision 1.2  91/02/14  16:15:39  dcurtis
 * fixed move_rl, move_lr to signal ok if length = 0
 * fixed move_rl, move_lr to copy between vec args, rather than size args
 * 
 * Revision 1.1  91/02/04  15:49:38  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_vec			*/
/*    sort of a mutable sequence		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

typedef struct {
int count;
struct REQ_ENTRY entry[2];
} _vec_of_t_REQS;

typedef struct {
int count;
OWNPTR type_owns;
OWNPTR op_owns;
struct OP_ENTRY _gcd;
struct OP_ENTRY debug_print;
} _vec_of_t_TABLE;

_vec_of_t_REQS _vec_of_t_reqs_actual = {2, "_gcd", "debug_print"};
_vec_of_t_REQS *_vec_of_t_reqs = &_vec_of_t_reqs_actual;


errcode _vecOPcreate(sz, ans)
CLUREF sz, *ans;
{
errcode err;
CLUREF temp;
int i;
	if (sz.num < 0) signal(ERR_negative_size);

/* Using sequenceOPnew for empty _vec's is NOT A GOOD IDEA:
	sequences are immutable, but _vec's are used
	for array stores by gc_read and there is but a single empty
	sequence in the world and mtv's array$addh frees stores that
	are being replaced.  7/23/93 dcurtis

	if (sz.num == 0) {
		err = sequenceOPnew(&temp);
		ans->vec = temp.vec;
		signal(ERR_ok);
		}
*/
	if (sz.num > MAX_SEQ) signal(ERR_toobig);

	clu_alloc((sz.num-1)*sizeof(CLUREF) + sizeof(CLU_sequence), &temp);
	temp.vec->size = sz.num;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	for (i = 0; i < sz.num; i++) {
		temp.vec->data[i] = 0;
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode _vecOPcopy(v, ans)
CLUREF v, *ans;
{
errcode err;
CLUREF temp, sz;
int i;

	sz.num = v.vec->size;
	err = _vecOPcreate(sz, &temp);
	if (err != ERR_ok) resignal(err);
	for (i = 0; i < v.vec->size; i++) {
		temp.vec->data[i] = v.vec->data[i];
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode _vecOPfetch(v, i, ans)
CLUREF v, i, *ans;
{
	if (i.num < 1) signal(ERR_bounds);
	if (i.num > v.vec->size) signal(ERR_bounds);
	ans->num = v.vec->data[i.num-1];
	signal(ERR_ok);
	}

errcode _vecOPstore(v, i, val)
CLUREF v, i, val;
{
	if (i.num < 1) signal(ERR_bounds);
	if (i.num > v.vec->size) signal(ERR_bounds);
	v.vec->data[i.num-1] = val.num;
	signal(ERR_ok);
	}

errcode _vecOPsize(v, ans)
CLUREF v, *ans;
{
	ans->num = v.vec->size;
	signal(ERR_ok);
	}

errcode _vecOPequal(v1, v2, ans)
CLUREF v1, v2, *ans;
{
	if (v1.ref == v2.ref) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _vecOPmove_lr(v1, s1, v2, s2, len)
CLUREF v1, s1, v2, s2, len;
{
int copycount, i;

	if (len.num < 0) signal(ERR_negative_size);
	if (len.num == 0) signal(ERR_ok);
	if (s1.num > v1.vec->size) signal(ERR_bounds);
	if (s2.num > v2.vec->size) signal(ERR_bounds);
	copycount = len.num;
	if (s1.num + copycount > v1.vec->size) 
		copycount = v1.vec->size - s1.num + 1;
	if (s2.num + copycount > v2.vec->size) 
		copycount = v2.vec->size - s2.num + 1;
	for (i = 0; i < copycount; i++) {
		v2.vec->data[s2.num + i - 1] = v1.vec->data[s1.num + i - 1];
		}
	signal(ERR_ok);
	}

errcode _vecOPmove_rl(v1, s1, v2, s2, len)
CLUREF v1, s1, v2, s2, len;
{
int copycount, i;

	if (len.num < 0) signal(ERR_negative_size);
	if (len.num == 0) signal(ERR_ok);
	if (s1.num > v1.vec->size) signal(ERR_bounds);
	if (s2.num > v2.vec->size) signal(ERR_bounds);
	copycount = len.num;
	if (s1.num + copycount > v1.vec->size) 
		copycount = v1.vec->size - s1.num + 1;
	if (s2.num + copycount > v2.vec->size) 
		copycount = v2.vec->size - s2.num + 1;
	for (i = copycount - 1; i >= 0; i--) {
		v2.vec->data[s2.num + i - 1] = v1.vec->data[s1.num + i - 1];
		}
	signal(ERR_ok);
	}

errcode _vecOP_gcd(v, tab, ans)
CLUREF v, tab, *ans;
{
_vec_of_t_TABLE *table = (_vec_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0]; /* ptr to _gcd fcns */
errcode err;
CLUREF temp_oneof, sz, fcn;

	fcn.proc = table->_gcd.fcn;
	err = oneofOPnew(CLU_2, fcn, &temp_oneof);
	if (err != ERR_ok) resignal(err);
	sz.num = v.vec->size * GCD_REF_SIZE + 8;
	err = gcd_tabOPinsert(tab, sz, temp_oneof, v, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode _vecOPdebug_print(v, pst)
CLUREF v, pst;
{
errcode err;
_vec_of_t_TABLE *table = (_vec_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
CLUREF pfcn;

	pfcn.proc = table->debug_print.fcn;
	err = sequenceOPinternal_print(v, pst, pfcn);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}


OWN_req _vec_ownreqs = {2*UNIT,1};

typedef struct{
    int count;
    OWNPTR type_owns;
    OWNPTR op_owns;
    struct OP_ENTRY entry[10];
} _vec_OPS;

OWN_ptr _vec_own_init = {0, 0};

CLU_proc _vec_oe__gcd = {{0,0,0,0}, _vecOP_gcd, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_debug_print = {{0,0,0,0}, _vecOPdebug_print, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_create = {{0,0,0,0}, _vecOPcreate, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_equal = {{0,0,0,0}, _vecOPequal, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_copy = {{0,0,0,0}, _vecOPcopy, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_fetch = {{0,0,0,0}, _vecOPfetch, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_store = {{0,0,0,0}, _vecOPstore, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_size = {{0,0,0,0}, _vecOPsize, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_move_lr = {{0,0,0,0}, _vecOPmove_lr, &_vec_own_init, &_vec_own_init};
CLU_proc _vec_oe_move_rl = {{0,0,0,0}, _vecOPmove_rl, &_vec_own_init, &_vec_own_init};


_vec_OPS _vec_ops_actual = {10,
    &_vec_own_init,
    &_vec_own_init, {
{&_vec_oe_create, "create"},
{&_vec_oe_equal, "equal"},
{&_vec_oe_copy, "copy"},
{&_vec_oe_fetch, "fetch"},
{&_vec_oe_store, "store"},
{&_vec_oe_size, "size"},
{&_vec_oe_move_lr, "move_lr"},
{&_vec_oe_move_rl, "move_rl"},
{&_vec_oe__gcd, "_gcd"},
{&_vec_oe_debug_print, "debug_print"}}
};

_vec_OPS *_vec_ops = (_vec_OPS *)&_vec_ops_actual;


