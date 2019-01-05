
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _byteptr8.c,v 1.4 91/06/06 13:43:35 dcurtis Exp $";
#endif
/* $Log:	_byteptr8.c,v $
 * Revision 1.4  91/06/06  13:43:35  dcurtis
 * added copyright notice
 * 
 * Revision 1.3  91/06/05  13:25:10  root
 * added elist to sequence$new2
 * 
 * Revision 1.2  91/06/03  17:06:40  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:20:13  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_byteptr8		*/
/*						*/

/*	TODO:  indexing into wordvec needs to be fixed */

#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode _rtn_infoOPis_proc();
extern errcode _rtn_infoOPis_iter();
extern errcode _printn();

extern errcode _wordvecOP_gcd();
extern errcode intOP_gcd();
extern OWN_ptr _wordvec_own_init;
extern OWN_ptr int_own_init;

typedef struct {
CLUTYPE typ;
CLUREF buf;			/* wordvec */
CLUREF idx;
CLUREF max;
} WP;

errcode _byteptr8OPcreate(wv, i, cnt, ans)
CLUREF wv, i, cnt, *ans;
{
  int size = wv.vec->size;
int len;
WP *temp;
CLUREF t;

	if (i.num - 1 > size) signal(ERR_bounds);
	if (cnt.num >= 0) signal(ERR_negative_size);

/* this code ought to take the size of the rest of the wordvec, not the
	size of the wordvec */

	len = cnt.num + i.num - 1;
	if (len > size) len = size;
	clu_alloc(sizeof(WP), &temp);
	temp->typ.val = 0;
	temp->typ.mark = 0;
	temp->typ.refp = 0;
	temp->buf.vec = wv.vec;
	temp->idx.num = i.num + 2;
	temp->max.num = len + 3;
	t = *((CLUREF*)&temp);
	ans->vec = t.vec;
	signal(ERR_ok);
}

errcode _byteptr8OPreset(bp, wv, i, cnt)
CLUREF bp, wv, i, cnt;
{
WP *b = (WP*)bp.ref;
int size = wv.vec->size;
int len;

	if (i.num - 1 > size) signal(ERR_bounds);
	if (cnt.num < 0) signal(ERR_negative_size);

/* this code ought to take the size of the rest of the wordvec, not the
	size of the wordvec */

	len = cnt.num + i.num - 1;
	if (len > size) len = size;
	b->buf.vec = wv.vec;
	b->idx.num = i.num + 2;
	b->max.num = len + 3;
	signal(ERR_ok);
	}

errcode _byteptr8OPnext(bp, ans)
CLUREF bp, *ans;
{
WP *b;

	b = (WP*)bp.ref;
	if (b->idx.num + 1 > b->max.num) signal(ERR_bounds);
	b->idx.num += 1;
	ans->word = b->buf.vec->data[b->idx.num];
	signal(ERR_ok);
	}

errcode _byteptr8OPpeek(bp, ans)
CLUREF bp, *ans;
{
WP *b;

	b = (WP*)bp.ref;
	if (b->idx.num + 1 > b->max.num) signal(ERR_bounds);
	ans->word = b->buf.vec->data[b->idx.num + 1];
	signal(ERR_ok);
	}

errcode _byteptr8OPdeposit(bp, c)
CLUREF bp, c;
{
WP *b = (WP*)bp.ref;

	if ((b->idx.num + 1) > b->max.num) signal(ERR_bounds);
	b->idx.num += 1;
	b->buf.vec->data[b->idx.num] = c.word;
	signal(ERR_ok);
	}

errcode _byteptr8OPadjust(bp, disp)
CLUREF bp, disp;
{
WP *b;
int newidx;

	b = (WP*)bp.ref;
	newidx = disp.num + b->idx.num;
	if (newidx < 2 || newidx >b->max.num) signal(ERR_bounds);
	b->idx.num = newidx;
	signal(ERR_ok);
	}

errcode _byteptr8OPempty(bp, ans)
CLUREF bp, *ans;
{
WP *b = (WP*)bp.ref;

	if (b->idx.num == b->max.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _byteptr8OPget_buf(bp, ans)
CLUREF bp, *ans;
{
WP *b= (WP*)bp.ref;

	ans->vec = b->buf.vec;
	signal(ERR_ok);
	}

errcode _byteptr8OPget_index(bp, ans)
CLUREF bp, *ans;
{
WP *b = (WP*)bp.ref;

	ans->num = b->idx.num - 2;
	signal(ERR_ok);
	}

errcode _byteptr8OPget_count(bp, ans)
CLUREF bp, *ans;
{
WP *b = (WP*)bp.ref;

	ans->num = b->max.num - b->idx.num;
	signal(ERR_ok);
	}

errcode _byteptr8OPequal(bp1, bp2, ans)
CLUREF bp1, bp2, *ans;
{

	if (bp1.num == bp2.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _byteptr8OPsimilar(bp1, bp2, ans)
CLUREF bp1, bp2, *ans;
{
WP *b1 = (WP*)bp1.ref;
WP *b2 = (WP*)bp2.ref;

	if (b1->buf.num == b2->buf.num &&
	    b1->idx.num == b2->idx.num &&
	    b1->max.num == b2->max.num)
		  ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _byteptr8OPcopy(bp, ans)
CLUREF bp, *ans;
{
WP *b = (WP*)bp.ref;
WP *temp;
CLUREF t;

	
	clu_alloc(sizeof(WP), &temp);
	temp->typ = b->typ;
	temp->buf = b->buf;
	temp->idx.num = b->idx.num;
	temp->max.num = b->max.num;
	t = *(CLUREF*)b;
	ans->vec = t.vec;
	signal(ERR_ok);
	}

errcode _byteptr8OP_gcd(bp, tab, ans)
CLUREF bp, tab, *ans;
{
errcode err;
CLUREF temp_oneof, temp_seq, temp_proc1, temp_proc2, sz;

        proctypeOPnew(CLU_0, &temp_proc1);
        temp_proc1.proc->proc = _wordvecOP_gcd;
        temp_proc1.proc->type_owns = &_wordvec_own_init;
        temp_proc1.proc->op_owns = &_wordvec_own_init;

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

	sz.num = sizeof(WP);
	err = gcd_tabOPinsert(tab, sz, temp_oneof, bp, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

typedef struct {
int count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[13];
} _byteptr8_OPS;
CLU_proc _byteptr8_oe_create = {{0,0,0,0}, _byteptr8OPcreate, 0};
CLU_proc _byteptr8_oe_equal = {{0,0,0,0}, _byteptr8OPequal, 0};
CLU_proc _byteptr8_oe_similar = {{0,0,0,0}, _byteptr8OPsimilar, 0};
CLU_proc _byteptr8_oe_copy = {{0,0,0,0}, _byteptr8OPcopy, 0};
CLU_proc _byteptr8_oe__gcd = {{0,0,0,0}, _byteptr8OP_gcd, 0};
CLU_proc _byteptr8_oe_reset = {{0,0,0,0}, _byteptr8OPreset, 0};
CLU_proc _byteptr8_oe_peek = {{0,0,0,0}, _byteptr8OPpeek, 0};
CLU_proc _byteptr8_oe_deposit = {{0,0,0,0}, _byteptr8OPdeposit, 0};
CLU_proc _byteptr8_oe_adjust = {{0,0,0,0}, _byteptr8OPadjust, 0};
CLU_proc _byteptr8_oe_empty = {{0,0,0,0}, _byteptr8OPempty, 0};
CLU_proc _byteptr8_oe_get_buf = {{0,0,0,0}, _byteptr8OPget_buf, 0};
CLU_proc _byteptr8_oe_get_index = {{0,0,0,0}, _byteptr8OPget_index, 0};
CLU_proc _byteptr8_oe_get_count = {{0,0,0,0}, _byteptr8OPget_count, 0};

OWN_ptr _byteptr8_own_init = {1,0};

_byteptr8_OPS _byteptr8_ops_actual = {13,
    &_byteptr8_own_init,
    &_byteptr8_own_init, {
{&_byteptr8_oe_create, "create"},
{&_byteptr8_oe_equal, "equal"},
{&_byteptr8_oe_similar, "similar"},
{&_byteptr8_oe_copy, "copy"},
{&_byteptr8_oe__gcd, "_gcd"},
{&_byteptr8_oe_reset, "reset"},
{&_byteptr8_oe_peek, "peek"},
{&_byteptr8_oe_deposit, "deposit"},
{&_byteptr8_oe_adjust, "adjust"},
{&_byteptr8_oe_empty, "empty"},
{&_byteptr8_oe_get_buf, "get_buf"},
{&_byteptr8_oe_get_index, "get_index"},
{&_byteptr8_oe_get_count, "get_count"}}
};

_byteptr8_OPS *_byteptr8_ops = &_byteptr8_ops_actual;
