
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _byteptr.c,v 1.4 91/06/06 13:43:15 dcurtis Exp $";
#endif
/* $Log:	_byteptr.c,v $
 * Revision 1.4  91/06/06  13:43:15  dcurtis
 * added copyright notice
 * 
 * Revision 1.3  91/06/05  13:24:41  root
 * added elist to sequence$new2
 * 
 * Revision 1.2  91/06/03  17:03:57  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:20:11  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_byteptr		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode _rtn_infoOPis_proc();
extern errcode _rtn_infoOPis_iter();
extern errcode _printn();

extern errcode _bytevecOP_gcd();
extern OWN_ptr _bytevec_own_init;
extern OWN_ptr int_own_init;
extern errcode intOP_gcd();

typedef struct {
CLUTYPE typ;
CLUREF buf;			/* bytevec */
CLUREF idx;
CLUREF max;
} BP;

errcode _byteptrOPcreate(bv, i, cnt, ans)
CLUREF bv, i, cnt, *ans;
{
int size = bv.vec->size;
int len;
BP *temp;
CLUREF t;

	if (i.num - 1 > size) signal(ERR_bounds);
	if (cnt.num >= 0) signal(ERR_negative_size);

/* this code ought to take the size of the rest of the bytevec, not the
	size of the bytevec */

	len = cnt.num + i.num - 1;
	if (len > size) len = size;
	clu_alloc(sizeof(BP), &temp);
	temp->typ.val = 0;
	temp->typ.mark = 0;
	temp->typ.refp = 0;
	temp->buf.vec = bv.vec;
	temp->idx.num = i.num + 2;
	temp->max.num = len + 3;
	t = *((CLUREF*)&temp);
	ans->vec = t.vec;
	signal(ERR_ok);
}

errcode _byteptrOPreset(bp, bv, i, cnt)
CLUREF bp, bv, i, cnt;
{
BP *b = (BP*)bp.ref;
int size = bv.vec->size;
int len;

	if (i.num - 1 > size) signal(ERR_bounds);
	if (cnt.num < 0) signal(ERR_negative_size);

/* this code ought to take the size of the rest of the bytevec, not the
	size of the bytevec */

	len = cnt.num + i.num - 1;
	if (len > size) len = size;
	b->buf.vec = bv.vec;
	b->idx.num = i.num + 2;
	b->max.num = len + 3;
	signal(ERR_ok);
	}

errcode _byteptrOPnext(bp, ans)
CLUREF bp, *ans;
{
BP *b;

	b = (BP*)bp.ref;
	if (b->idx.num + 1 > b->max.num) signal(ERR_bounds);
	b->idx.num += 1;
	ans->ch = b->buf.vec->data[b->idx.num];
	signal(ERR_ok);
	}

errcode _byteptrOPpeek(bp, ans)
CLUREF bp, *ans;
{
BP *b;

	b = (BP*)bp.ref;
	if (b->idx.num + 1 > b->max.num) signal(ERR_bounds);
	ans->ch = b->buf.vec->data[b->idx.num + 1];
	signal(ERR_ok);
	}

errcode _byteptrOPdeposit(bp, c)
CLUREF bp, c;
{
BP *b = (BP*)bp.ref;

	if ((b->idx.num + 1) > b->max.num) signal(ERR_bounds);
	b->idx.num += 1;
	b->buf.vec->data[b->idx.num] = c.ch;
	signal(ERR_ok);
	}

errcode _byteptrOPadjust(bp, disp)
CLUREF bp, disp;
{
BP *b;
int newidx;

	b = (BP*)bp.ref;
	newidx = disp.num + b->idx.num;
	if (newidx < 2 || newidx >b->max.num) signal(ERR_bounds);
	b->idx.num = newidx;
	signal(ERR_ok);
	}

errcode _byteptrOPempty(bp, ans)
CLUREF bp, *ans;
{
BP *b = (BP*)bp.ref;

	if (b->idx.num == b->max.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _byteptrOPget_buf(bp, ans)
CLUREF bp, *ans;
{
BP *b= (BP*)bp.ref;

	ans->vec = b->buf.vec;
	signal(ERR_ok);
	}

errcode _byteptrOPget_index(bp, ans)
CLUREF bp, *ans;
{
BP *b = (BP*)bp.ref;

	ans->num = b->idx.num - 2;
	signal(ERR_ok);
	}

errcode _byteptrOPget_count(bp, ans)
CLUREF bp, *ans;
{
BP *b = (BP*)bp.ref;

	ans->num = b->max.num - b->idx.num;
	signal(ERR_ok);
	}

errcode _byteptrOPequal(bp1, bp2, ans)
CLUREF bp1, bp2, *ans;
{

	if (bp1.num == bp2.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _byteptrOPsimilar(bp1, bp2, ans)
CLUREF bp1, bp2, *ans;
{
BP *b1 = (BP*)bp1.ref;
BP *b2 = (BP*)bp2.ref;

	if (b1->buf.num == b2->buf.num &&
	    b1->idx.num == b2->idx.num &&
	    b1->max.num == b2->max.num)
		  ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _byteptrOPcopy(bp, ans)
CLUREF bp, *ans;
{
BP *b = (BP*)bp.ref;
BP *temp;
CLUREF t;

	
	clu_alloc(sizeof(BP), &temp);
	temp->typ = b->typ;
	temp->buf = b->buf;
	temp->idx.num = b->idx.num;
	temp->max.num = b->max.num;
	t = *(CLUREF*)b;
	ans->vec = t.vec;
	signal(ERR_ok);
	}

errcode _byteptrOP_gcd(bp, tab, ans)
CLUREF bp, tab, *ans;
{
errcode err;
CLUREF temp_oneof, temp_seq, temp_proc1, temp_proc2, sz;

	proctypeOPnew(CLU_0, &temp_proc1);
	temp_proc1.proc->proc = _bytevecOP_gcd;
	temp_proc1.proc->type_owns = &_bytevec_own_init;
	temp_proc1.proc->op_owns = &_bytevec_own_init;

	proctypeOPnew(CLU_0, &temp_proc2);
	temp_proc2.proc->proc = intOP_gcd;
	temp_proc2.proc->type_owns = &int_own_init;
	temp_proc2.proc->op_owns = &int_own_init;

        sequenceOPnew2(CLU_3, &temp_seq);
	temp_seq.vec->data[0] = temp_proc1.num;
	temp_seq.vec->data[1] = temp_proc2.num;
	temp_seq.vec->data[2] = temp_proc2.num;

        err = oneofOPnew(CLU_3, temp_seq, &temp_oneof);
        if (err != ERR_ok) resignal(err);

	sz.num = sizeof(BP);
	err = gcd_tabOPinsert(tab, sz, temp_oneof, bp, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

typedef struct {
int count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[13];
} _byteptr_OPS;
CLU_proc _byteptr_oe_create = {{0,0,0,0}, _byteptrOPcreate, 0};
CLU_proc _byteptr_oe_equal = {{0,0,0,0}, _byteptrOPequal, 0};
CLU_proc _byteptr_oe_similar = {{0,0,0,0}, _byteptrOPsimilar, 0};
CLU_proc _byteptr_oe_copy = {{0,0,0,0}, _byteptrOPcopy, 0};
CLU_proc _byteptr_oe__gcd = {{0,0,0,0}, _byteptrOP_gcd, 0};
CLU_proc _byteptr_oe_reset = {{0,0,0,0}, _byteptrOPreset, 0};
CLU_proc _byteptr_oe_peek = {{0,0,0,0}, _byteptrOPpeek, 0};
CLU_proc _byteptr_oe_deposit = {{0,0,0,0}, _byteptrOPdeposit, 0};
CLU_proc _byteptr_oe_adjust = {{0,0,0,0}, _byteptrOPadjust, 0};
CLU_proc _byteptr_oe_empty = {{0,0,0,0}, _byteptrOPempty, 0};
CLU_proc _byteptr_oe_get_buf = {{0,0,0,0}, _byteptrOPget_buf, 0};
CLU_proc _byteptr_oe_get_index = {{0,0,0,0}, _byteptrOPget_index, 0};
CLU_proc _byteptr_oe_get_count = {{0,0,0,0}, _byteptrOPget_count, 0};

OWN_ptr _byteptr_own_init = {1,0};

_byteptr_OPS _byteptr_ops_actual = {13,
    &_byteptr_own_init,
    &_byteptr_own_init, {
{&_byteptr_oe_create, "create"},
{&_byteptr_oe_equal, "equal"},
{&_byteptr_oe_similar, "similar"},
{&_byteptr_oe_copy, "copy"},
{&_byteptr_oe__gcd, "_gcd"},
{&_byteptr_oe_reset, "reset"},
{&_byteptr_oe_peek, "peek"},
{&_byteptr_oe_deposit, "deposit"},
{&_byteptr_oe_adjust, "adjust"},
{&_byteptr_oe_empty, "empty"},
{&_byteptr_oe_get_buf, "get_buf"},
{&_byteptr_oe_get_index, "get_index"},
{&_byteptr_oe_get_count, "get_count"}}
};

_byteptr_OPS *_byteptr_ops = &_byteptr_ops_actual;
