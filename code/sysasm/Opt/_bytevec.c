
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _bytevec.c,v 1.5 91/06/06 13:23:04 root Exp $";
#endif
/* $Log:	_bytevec.c,v $
 * Revision 1.5  91/06/06  13:23:04  root
 * added copyright notice
 * 
 * Revision 1.4  91/05/31  11:41:04  root
 * fixed aggregate initialization in ops_actual
 * 
 * Revision 1.3  91/05/07  14:17:57  root
 * switched from clu_alloc to clu_alloc_atomic
 * 
 * Revision 1.2  91/02/14  16:06:01  dcurtis
 * fixed indexv and nc_indexv to not signal bounds if the 2nd bytevec is empty
 * 
 * Revision 1.1  91/02/04  15:49:30  mtv
 * Initial revision
 * 
 * Revision 1.1  91/02/04  15:44:48  mtv
 * Initial revision
 * 
 */

/*							*/
/*		BYTEVEC IMPLEMENTATION			*/
/*							*/

/* TODO: comp, build, move_rl, move_b (not referenced) 	*/

#include <stdio.h>
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _bytevecOPcreate(sz, ans)
CLUREF sz;
CLUREF *ans;
{
CLUREF temp;

	if (sz.num > MAX_STR) signal(ERR_toobig);
	clu_alloc_atomic(((sz.num+1+CLUREFSZ-1)/CLUREFSZ)*CLUREFSZ + sizeof(CLU_string) -1, &temp);
	temp.str->size = sz.num;
	temp.str->typ.val = CT_STRING;
	temp.str->typ.mark = 0;
	temp.str->typ.refp = 0;

	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode _bytevecOPcopy(bv1, bv2)
CLUREF bv1;
CLUREF *bv2;
{
CLUREF temp;
long i, count;

	clu_alloc_atomic(((bv1.str->size+1+CLUREFSZ-1)/CLUREFSZ)*CLUREFSZ + 
					sizeof(CLU_string) - CLUREFSZ, &temp);
	temp.str->size = bv1.str->size;
	temp.str->typ.val = CT_STRING;
	temp.str->typ.mark = 0;
	temp.str->typ.refp = 0;

	for (count = 0, i = 0; count < bv1.str->size; i++, count++) {
		temp.str->data[i] = bv1.str->data[i];
		}
		temp.str->data[i] = '\000';
	bv2->str = temp.str;
	signal(ERR_ok);
	}

errcode _bytevecOPfetch(bv, i, ans)
CLUREF bv;
CLUREF i;
CLUREF *ans;
{
	if (i.num < 1) signal(ERR_bounds);
	if (i.num > bv.str->size) signal(ERR_bounds);
	ans->ch = bv.str->data[i.num-1];
	signal(ERR_ok);
	}	

errcode _bytevecOPstore(bv, i, c)
CLUREF bv;
CLUREF i;
CLUREF c;
{
	if (i.num < 1) signal(ERR_bounds);
	if (i.num > bv.str->size) signal(ERR_bounds);
	bv.str->data[i.num-1] = c.ch;
	signal(ERR_ok);
	}

errcode _bytevecOPsize(bv, ans)
CLUREF bv;
CLUREF *ans;
{
	ans->num = bv.str->size;
	signal(ERR_ok);
	}

errcode _bytevecOPequal(bv1, bv2, ans)
CLUREF bv1;
CLUREF bv2;
CLUREF *ans;
{
	if (bv1.str == bv2.str) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _bytevecOPindexc(c, bv, first, ans)
CLUREF c, bv, first, *ans;
{
long i;
	if (first.num < 1) signal(ERR_bounds);
	if (first.num > (bv.str->size+1)) signal(ERR_bounds); 
	for (i = first.num; i <= bv.str->size; i++) {
		if (bv.str->data[i-1] == c.ch) {
			ans->num = i;
			signal(ERR_ok);
			}
		}
	ans->num = 0;
	signal(ERR_ok);
	}

/* find bv1 in bv2; start looking at bv2[first] ; */
/* some way to use matchc and/or locc ? */

errcode _bytevecOPindexv(bv1, bv2, first, ans)
CLUREF bv1, bv2, first, *ans;
{
bool found;
long i,j;
	if (first.num < 1) signal(ERR_bounds);	
	if (first.num > (bv2.str->size + 1)) signal(ERR_bounds);
	for (i = first.num; i + bv1.str->size - 1 <= bv2.str->size; i++) {
		found = true;
		for (j = 1; j <= bv1.str->size; j++) {
			if (bv2.str->data[i-1+j-1] != bv1.str->data[j-1]) {
				found = false;
				break;
				}
			}
		if (found == true) {
			ans->num = i;
			signal(ERR_ok);
			}
		}
	ans->num = 0;
	signal(ERR_ok);
	}

/* find bv1 in bv2; start looking at bv2[first] ; */
/* case doesn't matter */

errcode _bytevecOPnc_indexv(bv1, bv2, first, ans)
CLUREF bv1, bv2, first, *ans;
{
bool found;
long i,j;
char temp_c1, temp_c2;
	if (first.num < 1) signal(ERR_bounds);	
	if (first.num > (bv2.str->size + 1)) signal(ERR_bounds);
	for (i = first.num; i + bv1.str->size - 1 <= bv2.str->size; i++) {
		found = true;
		for (j = 1; j <= bv1.str->size; j++) {
			temp_c1 = bv1.str->data[j-1];
			if (temp_c1 >= MIN_CAP_CHAR && temp_c1 <= MAX_CAP_CHAR)
				temp_c1 += OFF_CHAR;
			temp_c2 = bv2.str->data[i-1+j-1];
			if (temp_c2 >= MIN_CAP_CHAR && temp_c2 <= MAX_CAP_CHAR)
				temp_c2 += OFF_CHAR;
			if (temp_c1 != temp_c2) {
				found = false;
				break;
				}
			}
		if (found == true) {
			ans->num = i;
			signal(ERR_ok);
			}
		}
	ans->num = 0;
	signal(ERR_ok);
	}

/* original code tries to use more efficient instructions 
	if the bytevecs are not identical,
	if they are the same and the destination is less than the source,
	if they are the same and the dest is gtr than the source and the
		size of the dest is greater than the copycount
	if a single character is being propagated through a bytevec
*/
errcode _bytevecOPcomp(v1, v2, ans)
CLUREF v1, v2, *ans;
{
	fprintf(stderr, "_bytevec$comp not implemented\n");
	signal(ERR_failure);
	}

errcode _bytevecOPmove_rl(v1, s1, v2, s2, len)
CLUREF v1, s1, v2, s2, len;
{
	fprintf(stderr, "_bytevec$move_rl not implemented\n");
	signal(ERR_failure);
	}

errcode _bytevecOPmove_b(v1, s1, v2, s2, len)
CLUREF v1, s1, v2, s2, len;
{
	fprintf(stderr, "_bytevec$move_b not implemented\n");
	signal(ERR_failure);
	}

errcode _bytevecOPbuild(v1, ans)
CLUREF v1, *ans;
{
	fprintf(stderr, "_bytevec$build not implemented\n");
	signal(ERR_failure);
	}

errcode _bytevecOPmove_lr(bv1, s1, bv2, s2, len)
CLUREF bv1, s1, bv2, s2, len;
{
long copycount;
long i;
	if (len.num < 0) signal(ERR_negative_size);
	if (len.num == 0) signal(ERR_ok);
	if (s1.num < 1) signal(ERR_bounds);
	if (s1.num > bv1.str->size) signal(ERR_bounds);
	if (s2.num < 1) signal(ERR_bounds);
	if (s2.num >bv2.str->size) signal(ERR_bounds);
	copycount = len.num;
	if (copycount + s1.num > bv1.str->size) copycount = bv1.str->size - s1.num + 1;
	if (copycount + s2.num > bv2.str->size) copycount = bv2.str->size - s2.num + 1;
	for (i = 0; i < copycount; i++) {
		bv2.str->data[s2.num-1+i] = bv1.str->data[s1.num-1+i];
		}
	signal(ERR_ok);
	}

/* moves chars from a sequence of chars to a bytevec */

errcode _bytevecOPmove_v(qc1, s1, bv2, s2, len)
CLUREF qc1, s1, bv2, s2, len;
{
long copycount;
long i;
	if (len.num < 0) signal(ERR_negative_size);
	if (s1.num < 1) signal(ERR_bounds);
	if (s1.num > qc1.vec->size) signal(ERR_bounds);
	if (s2.num < 1) signal(ERR_bounds);
	if (s2.num >bv2.str->size) signal(ERR_bounds);
	copycount = len.num;
	if (copycount + s1.num > qc1.vec->size) copycount = qc1.vec->size - s1.num + 1;
	if (copycount + s2.num > bv2.str->size) copycount = bv2.str->size - s2.num + 1;
	for (i = 0; i < copycount; i++) {
		bv2.str->data[s2.num-1+i] = qc1.vec->data[s1.num-1+i];
		}
	signal(ERR_ok);
	}

errcode _bytevecOP_gcd(bv, tab, ans)
CLUREF bv, tab, *ans;
{
errcode err;
CLUREF temp_oneof;

        err = oneofOPnew(1, 0, &temp_oneof);
        if (err != ERR_ok) resignal(err);
/*                 16 for type + size, 1 for trailing 0, +3 &~3 to round up */
        err = gcd_tabOPinsert(tab, 2*CLUREFSZ + 
			(bv.str->size + CLUREFSZ) &~(CLUREFSZ-1),
                        temp_oneof, bv, ans);
        if (err != ERR_ok) resignal(err);
        signal(ERR_ok);
        }

errcode _bytevecOPdebug_print(bv, ps)
CLUREF bv, ps;
{
errcode err;

        err = stringOPdebug_print(bv, ps);
        if (err != ERR_ok) resignal(err);
        signal(ERR_ok);
        }


typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[13];
} _bytevec_OPS;

OWN_ptr _bytevec_own_init = {1, 0};

CLU_proc _bytevec_oe_create = {{0,0,0,0}, _bytevecOPcreate, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_copy = {{0,0,0,0}, _bytevecOPcopy, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_fetch = {{0,0,0,0}, _bytevecOPfetch, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_store = {{0,0,0,0}, _bytevecOPstore, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_size = {{0,0,0,0}, _bytevecOPsize, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_equal = {{0,0,0,0}, _bytevecOPequal, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_indexc = {{0,0,0,0}, _bytevecOPindexc, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_indexv = {{0,0,0,0}, _bytevecOPindexv, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_nc_indexv = {{0,0,0,0}, _bytevecOPnc_indexv, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_move_lr = {{0,0,0,0}, _bytevecOPmove_lr, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_move_v = {{0,0,0,0}, _bytevecOPmove_v, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe__gcd = {{0,0,0,0}, _bytevecOP_gcd, &_bytevec_own_init, &_bytevec_own_init};
CLU_proc _bytevec_oe_debug_print = {{0,0,0,0}, _bytevecOPdebug_print, &_bytevec_own_init, &_bytevec_own_init};

_bytevec_OPS _bytevec_ops_actual = {13,
    &_bytevec_own_init,
    &_bytevec_own_init, {
{&_bytevec_oe_create, "create"},
{&_bytevec_oe_copy, "copy"},
{&_bytevec_oe_fetch, "fetch"},
{&_bytevec_oe_store, "store"},
{&_bytevec_oe_size, "size"},
{&_bytevec_oe_equal, "equal"},
{&_bytevec_oe_indexc, "indexc"},
{&_bytevec_oe_indexv, "indexv"},
{&_bytevec_oe_nc_indexv, "nc_indexv"},
{&_bytevec_oe_move_lr, "move_lr"},
{&_bytevec_oe_move_v, "move_v"},
{&_bytevec_oe__gcd, "_gcd"},
{&_bytevec_oe_debug_print, "debug_print"}}
};

_bytevec_OPS *_bytevec_ops = &_bytevec_ops_actual;
