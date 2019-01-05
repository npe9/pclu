
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _adv.c,v 1.9 91/06/06 13:22:40 root Exp $";
#endif
/* $Log:	_adv.c,v $
 * Revision 1.9  91/06/06  13:22:40  root
 * *** empty log message ***
 * 
 * Revision 1.8  91/06/06  13:22:11  root
 * *** empty log message ***
 * 
 * Revision 1.7  91/06/06  13:21:24  root
 * added copyright notice
 * 
 * Revision 1.6  91/06/03  11:56:26  root
 * ditto
 * 
 * Revision 1.5  91/06/03  11:54:39  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.4  91/05/31  11:39:12  root
 * fixed aggregate initialization in ops_actual
 * 
 * Revision 1.3  91/02/19  14:14:20  dcurtis
 * made symmetric change to set_start (see last change to get_start)
 * 
 * Revision 1.2  91/02/14  16:04:06  dcurtis
 * fixed get_start to return int_low + 1, rather than int_low
 * 
 * Revision 1.1  91/02/04  15:49:29  mtv
 * Initial revision
 * 
 * Revision 1.1  91/02/04  15:44:09  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_adv			*/
/*	a clone of the array type		*/
/*		with more intimate operations	*/
/*						*/
/* TODO: make pred fields real			*/

#include "pclu_err.h"
#include "pclu_sys.h"


typedef struct {
long count;
struct REQ_ENTRY entry[2];
} _adv_of_t_REQS;

_adv_of_t_REQS _adv_of_t_reqs_actual = {2,
    "_gcd", "debug_print"};

_adv_of_t_REQS *_adv_of_t_reqs = &_adv_of_t_reqs_actual;

typedef struct {
    long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
    struct OP_ENTRY _gcd;
    struct OP_ENTRY debug_print;
} _adv_of_t_TABLE;

/**** BEGIN CLUSTER _adv ****/

extern CLUREF CUR_PROC_VAR;

/**** BEGIN PROCEDURE create ****/

errcode _advOPcreate(low, pred, v, ret_1)
CLUREF low;
CLUREF pred;
CLUREF v;
CLUREF *ret_1;
{
CLUREF temp;
errcode err;

	if (low.num < MIN_ARR_INDEX) signal(ERR_bounds);
	err = arrayOPOPnewdesc(&temp);
	if (err != ERR_ok) goto ex_0;
	temp.array->ext_low = low.num;
	temp.array->ext_size = v.vec->size;
	temp.array->ext_high = low.num + v.vec->size - 1;
	temp.array->int_low = 0;
	temp.array->int_size = v.vec->size;
	temp.array->store = v.store;
	ret_1->array = temp.array;
	signal(ERR_ok);
    ex_0:
        {
            if (err == ERR_failure) signal(ERR_failure);
            elist[0] = _pclu_erstr(err);
            signal(ERR_failure);
        }
    }

    /**** END PROCEDURE create ****/


/**** BEGIN PROCEDURE equal ****/

errcode _advOPequal(adv1, adv2, ret_1)
CLUREF adv1;
CLUREF adv2;
CLUREF *ret_1;
    {
    if (adv1.num == adv2.num) ret_1->tf = true;
    else ret_1->tf = false;
    signal(ERR_ok);
    }

    /**** END PROCEDURE equal ****/


/**** BEGIN PROCEDURE get_low ****/

errcode _advOPget_low(adv, ret_1)
CLUREF adv;
CLUREF *ret_1;
{
    ret_1->num = adv.array->ext_low;
    signal(ERR_ok);
    }

    /**** END PROCEDURE get_low ****/

/**** BEGIN PROCEDURE set_low ****/

errcode _advOPset_low(adv, low)
CLUREF adv;
CLUREF low;
    {
	if (low.num < MIN_ARR_INDEX || low.num + adv.array->ext_size > MAX_ARR_INDEX) {
		signal(ERR_bounds);}
	adv.array->ext_low = low.num;
	adv.array->ext_high = low.num + adv.array->ext_size - 1;
	signal(ERR_ok);

    }

    /**** END PROCEDURE set_low ****/

/**** BEGIN PROCEDURE get_size ****/

errcode _advOPget_size(adv, ret_1)
CLUREF adv;
CLUREF *ret_1;
    {
	ret_1->num = adv.array->ext_size;
	signal(ERR_ok);
    }

    /**** END PROCEDURE get_size ****/


/**** BEGIN PROCEDURE set_size ****/

errcode _advOPset_size(adv, size)
CLUREF adv;
CLUREF size;
    {
	if (size.num < 0) signal(ERR_bounds);
	if (adv.array->ext_size + size.num > MAX_ARR_INDEX) signal(ERR_bounds);
	if (adv.array->store->size - adv.array->int_low - size.num < 0)
			signal(ERR_bounds);
	adv.array->ext_size = size.num;
	adv.array->ext_high = size.num + adv.array->ext_low - 1;
	signal(ERR_ok);
    }

    /**** END PROCEDURE set_size ****/


/**** BEGIN PROCEDURE get_start ****/

errcode _advOPget_start(adv, ret_1)
CLUREF adv;
CLUREF *ret_1;
    {
	ret_1->num = adv.array->int_low + 1;
	signal(ERR_ok);
    }

    /**** END PROCEDURE get_start ****/

/**** BEGIN PROCEDURE set_start ****/

errcode _advOPset_start(adv, strt)
CLUREF adv;
CLUREF strt;
    {
long new_size;
long new_strt;

	if (strt.num < 1) signal(ERR_bounds);
	new_strt = strt.num - 1;
	new_size = adv.array->int_low - new_strt + adv.array->ext_size;
	if (new_size < 0) signal(ERR_bounds);
	if (adv.array->ext_low + new_size > MAX_ARR_INDEX) signal(ERR_bounds);
	adv.array->ext_size = new_size;
	adv.array->int_low = new_strt;
	adv.array->ext_high = adv.array->ext_low + new_size - 1;
	signal(ERR_ok);	/* trim back store as well? */
    }

    /**** END PROCEDURE set_start ****/


/**** BEGIN PROCEDURE get_predict ****/

errcode _advOPget_predict(adv, ret_1)
CLUREF adv;
CLUREF *ret_1;
    {
	ret_1->num = adv.array->int_size;
	signal(ERR_ok);
    }

    /**** END PROCEDURE get_predict ****/


/**** BEGIN PROCEDURE set_predict ****/

errcode _advOPset_predict(adv, pred)
CLUREF adv;
CLUREF pred;
    {
	signal(ERR_ok);
    }

    /**** END PROCEDURE set_predict ****/


/**** BEGIN PROCEDURE get_vector ****/

errcode _advOPget_vector(adv, ret_1)
CLUREF adv;
CLUREF *ret_1;
    {
	ret_1->store = adv.array->store;
	signal(ERR_ok);
    }

    /**** END PROCEDURE get_vector ****/


/**** BEGIN PROCEDURE set_vector ****/

errcode _advOPset_vector(adv, v)
CLUREF adv;
CLUREF v;
    {
	if(adv.array->ext_low + v.vec->size > MAX_ARR_INDEX) signal(ERR_bounds);
	adv.array->int_size = v.vec->size;
	adv.array->ext_size = v.vec->size;
	adv.array->ext_high = adv.array->ext_low + adv.array->ext_size - 1;
	adv.array->store = (CLUSTORE)v.vec;
	adv.array->int_low = 0;
	signal(ERR_ok);
    }

    /**** END PROCEDURE set_vector ****/


/**** BEGIN PROCEDURE _gcd ****/

errcode _advOP_gcd(adv, tab, ans)
CLUREF adv;
CLUREF tab;
CLUREF *ans;
    {
_adv_of_t_TABLE *table = (_adv_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0]; /* ptr to _gcd fcns */
errcode err;
CLUREF temp_oneof, temp_oneof_2, sz, fcn;

	fcn.proc = table->_gcd.fcn;
	err = oneofOPnew(CLU_7, fcn, &temp_oneof);
	if (err != ERR_ok) resignal(err);
	err = oneofOPnew(CLU_6, temp_oneof, &temp_oneof_2);
	if (err != ERR_ok) resignal(err);
	sz.num = 6*CLUREFSZ + GCD_REF_SIZE;
	err = gcd_tabOPinsert(tab, sz, temp_oneof_2,
			adv, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
    ex_0:
        {
            if (err == ERR_failure) signal(ERR_failure);
            elist[0] = _pclu_erstr(err);
            signal(ERR_failure);
        }
    end_0:;
    }

    /**** END PROCEDURE _gcd ****/

/**** BEGIN PROCEDURE debug_print ****/

errcode _advOPdebug_print(adv, ps)
CLUREF adv;
CLUREF ps;
    {
_adv_of_t_TABLE *table = (_adv_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0]; /* ptr to debug_print fcns */
errcode err;
CLUREF pfcn;

	pfcn.proc = table->debug_print.fcn;
	err = arrayOPinternal_print(adv, ps, pfcn);
	if (err != ERR_ok) resignal(err);	
	signal(ERR_ok);
    }

    /**** END PROCEDURE debug_print ****/

typedef struct{
    long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
    struct OP_ENTRY entry[14];
} _adv_OPS;


OWN_ptr _adv_own_init = {0, 0};

OWN_req _adv_ownreqs = {2*UNIT,1};

CLU_proc _adv_oe_create = {{0,0,0,0}, _advOPcreate, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_equal = {{0,0,0,0}, _advOPequal, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_get_low = {{0,0,0,0}, _advOPget_low, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_get_predict = {{0,0,0,0}, _advOPget_predict, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_get_size = {{0,0,0,0}, _advOPget_size, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_get_start = {{0,0,0,0}, _advOPget_start, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_get_vector = {{0,0,0,0}, _advOPget_vector, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_set_low = {{0,0,0,0}, _advOPset_low, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_set_predict = {{0,0,0,0}, _advOPset_predict, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_set_size = {{0,0,0,0}, _advOPset_size, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_set_start = {{0,0,0,0}, _advOPset_start, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_set_vector = {{0,0,0,0}, _advOPset_vector, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe__gcd = {{0,0,0,0}, _advOP_gcd, &_adv_own_init, &_adv_own_init};
CLU_proc _adv_oe_debug_print = {{0,0,0,0}, _advOPdebug_print, &_adv_own_init, &_adv_own_init};

_adv_OPS _adv_ops_actual = {14,
    &_adv_own_init,
    &_adv_own_init, {
    {&_adv_oe_create, "create"},
    {&_adv_oe_equal, "equal"},
    {&_adv_oe_get_low, "get_low"},
    {&_adv_oe_get_predict, "get_predict"},
    {&_adv_oe_get_size, "get_size"},
    {&_adv_oe_get_start, "get_start"},
    {&_adv_oe_get_vector, "get_vector"},
    {&_adv_oe_set_low, "set_low"},
    {&_adv_oe_set_predict, "set_predict"},
    {&_adv_oe_set_size, "set_size"},
    {&_adv_oe_set_start, "set_start"},
    {&_adv_oe_set_vector, "set_vector"},
    {&_adv_oe__gcd, "_gcd"},
    {&_adv_oe_debug_print, "debug_print"}}
};

struct OPS *_adv_ops = (struct OPS *)&_adv_ops_actual;

/**** END CLUSTER _adv ****/
