
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _tagcell.c,v 1.4 91/06/06 13:26:02 root Exp $";
#endif
/* $Log:	_tagcell.c,v $
 * Revision 1.4  91/06/06  13:26:02  root
 * added copyright notice
 * 
 * Revision 1.3  91/06/03  12:09:43  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.2  91/05/31  11:54:21  root
 * fixed aggregate initialization in ops_actual
 * changed int arguement to CLUREF in copy operation
 * 
 * Revision 1.1  91/02/04  15:49:37  mtv
 * Initial revision
 * 
 */

 /*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_tagcell		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

typedef struct {
long count;
struct REQ_ENTRY entry[1];
} _tagcell_of_t_REQS;

typedef struct {
long count;
OWNPTR type_owns;
OWNPTR op_owns;
struct OP_ENTRY _gcd;
} _tagcell_of_t_TABLE;

_tagcell_of_t_REQS _tagcell_of_t_reqs_actual = {1, "_gcd"};
_tagcell_of_t_REQS *_tagcell_of_t_reqs = &_tagcell_of_t_reqs_actual;



errcode _tagcellOPcreate(tg, val, ans)
CLUREF tg, val, *ans;
{
errcode err;
CLUREF temp;

	if (tg.num >= cellOPmaxtag) signal(ERR_toobig);
	clu_alloc(sizeof(CLU_cell), &temp);
	temp.cell->typ.mark = 0;
	temp.cell->typ.val = CT_TAG;
	temp.cell->typ.refp = 0;
	temp.cell->typ.spare = 0;
	temp.cell->tag = tg.num;
	temp.cell->value = val.num;
	ans->cell = temp.cell;
	signal(ERR_ok);
	}


errcode _tagcellOPcopy(tt, ans)
CLUREF tt, *ans;
{
errcode err;
CLUREF tg, val, temp;

	tg.num = tt.cell->tag;
	val.num = tt.cell->value;
	err = _tagcellOPcreate(tg, val, &temp);
	if (err != ERR_ok) resignal(err);
	ans->cell = temp.cell;
	signal(ERR_ok);	
	}

errcode _tagcellOPget(tt, ans1, ans2)
CLUREF tt, *ans1, *ans2;
{
	ans1->num = tt.cell->tag;
	ans2->num = tt.cell->value;
	signal(ERR_ok);
	}

errcode _tagcellOPset(tt, tg, val)
CLUREF tt, tg, val;
{
	if (tg.num > cellOPmaxtag) signal(ERR_toobig);
	tt.cell->tag = tg.num;
	tt.cell->value = val.num;
	signal(ERR_ok);
	}

errcode _tagcellOPset_value(tt, val)
CLUREF tt, val;
{
	tt.cell->value = val.num;
	signal(ERR_ok);
	}

errcode _tagcellOPequal(t1, t2, ans)
CLUREF t1, t2, *ans;
{
	if (t1.ref == t2.ref) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _tagcellOP_gcd(tt, tab, ans)
CLUREF tt, tab, *ans;
{
_tagcell_of_t_TABLE *table = (_tagcell_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0]; /* ptr to _gcd fcns */
errcode err;
CLUREF temp_oneof, sz, fcn;

	fcn.proc = table->_gcd.fcn;
	err = oneofOPnew(CLU_5, fcn, &temp_oneof);
	if (err != ERR_ok) resignal(err);
	sz.num = 2*CLUREFSZ + GCD_REF_SIZE;
	err = gcd_tabOPinsert(tab,  sz, temp_oneof, tt, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

typedef struct{
    long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
    struct OP_ENTRY entry[7];
} _tagcell_OPS;


OWN_req _tagcell_ownreqs = {2*UNIT,1};

OWN_ptr _tagcell_own_init = {0, 0};

CLU_proc _tagcell_oe_create = {{0,0,0,0}, _tagcellOPcreate, &_tagcell_own_init, &_tagcell_own_init};
CLU_proc _tagcell_oe_equal = {{0,0,0,0}, _tagcellOPequal, &_tagcell_own_init, &_tagcell_own_init};
CLU_proc _tagcell_oe_copy = {{0,0,0,0}, _tagcellOPcopy, &_tagcell_own_init, &_tagcell_own_init};
CLU_proc _tagcell_oe_get = {{0,0,0,0}, _tagcellOPget, &_tagcell_own_init, &_tagcell_own_init};
CLU_proc _tagcell_oe_set = {{0,0,0,0}, _tagcellOPset, &_tagcell_own_init, &_tagcell_own_init};
CLU_proc _tagcell_oe_set_value = {{0,0,0,0}, _tagcellOPset_value, &_tagcell_own_init, &_tagcell_own_init};
CLU_proc _tagcell_oe__gcd = {{0,0,0,0}, _tagcellOP_gcd, &_tagcell_own_init, &_tagcell_own_init};

_tagcell_OPS _tagcell_ops_actual = {7,
    &_tagcell_own_init,
    &_tagcell_own_init, {
    {&_tagcell_oe_create, "create"},
    {&_tagcell_oe_equal, "equal"},
    {&_tagcell_oe_copy, "copy"},
    {&_tagcell_oe_get, "get"},
    {&_tagcell_oe_set, "set"},
    {&_tagcell_oe_set_value, "set_value"},
    {&_tagcell_oe__gcd, "_gcd"}}
};

struct OPS *_tagcell_ops = (struct OPS *)&_tagcell_ops_actual;
