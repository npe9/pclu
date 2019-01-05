
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _wordvec.c,v 1.6 91/06/06 13:52:31 dcurtis Exp $";
#endif
/* $Log:	_wordvec.c,v $
 * Revision 1.6  91/06/06  13:52:31  dcurtis
 * added copyright notice
 * 
 * Revision 1.5  91/06/03  17:32:27  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.4  91/05/31  13:09:10  root
 * fixed aggregate initialization in ops_actual
 * 
 * Revision 1.3  91/02/11  12:05:57  dcurtis
 * added lfetch and lstore
 * fixed bfetch: avoid sign extensions
 * fixed wftetch : case 4 
 * 
 * Revision 1.2  91/02/06  13:57:38  dcurtis
 * no change
 * 
 * Revision 1.1  91/02/04  23:21:09  mtv
 * Initial revision
 * 
 */

/*							*/
/*		WORDVEC IMPLEMENTATION			*/
/*							*/

/* TODO: comp, build, move_rl, move_b (not referenced) 	*/

/*   mutable aggregate of bytes				*/
/*	to hold 64-bit quantities and be accessed in    */
/*		64-bit units				*/

#include <stdio.h>
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _wordvecOPcreate(sz, ans)
CLUREF sz;
CLUREF *ans;
{
errcode err;
CLUREF temp;
int size = sz.num * 8;
int rounded_size;
int i;

	if (size > MAX_STR) signal(ERR_toobig);
	rounded_size = ((size+7)/8)*8 + sizeof(CLU_string);
	clu_alloc(rounded_size, &temp);
	temp.str->size = size;
	temp.str->typ.val = CT_STRING;
	temp.str->typ.mark = 0;
	temp.str->typ.refp = 0;

	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode _wordvecOPcopy(wv1, wv2)
CLUREF wv1;
CLUREF *wv2;
{
errcode err;
CLUREF temp;
int i;

	clu_alloc(((wv1.str->size+7)/8)*8 + sizeof(CLU_string), &temp);
	temp.str->size = wv1.str->size;
	temp.str->typ.val = CT_STRING;
	temp.str->typ.mark = 0;
	temp.str->typ.refp = 0;

	for (i = 0; i < wv1.str->size; i++) {
		temp.str->data[i] = wv1.str->data[i];
		}
		temp.str->data[i] = '\000';
	wv2->str = temp.str;
	signal(ERR_ok);
	}

errcode _wordvecOPfetch(wv, i, ans)
CLUREF wv;
CLUREF i;
CLUREF *ans;
{
int bi;
/* 1/10/91 : modified wrt gc_dump */

	bi = (i.num-1)*8;
	if (bi < 0) signal(ERR_bounds);
	if (bi > (wv.str->size)) signal(ERR_bounds);
	ans->num = wv.vec->data[i.num-1];
/*	ans->num = (wv.str->data[bi] & 0xff)
			+ ((wv.str->data[bi+1] << 8) & 0xff00)
			+ ((wv.str->data[bi+2] << 16)  & 0xff0000)
			+ ((wv.str->data[bi+3] << 24) & 0xff000000);
*/
	signal(ERR_ok);
	}	

errcode _wordvecOPstore(wv, i, w)
CLUREF wv;
CLUREF i;
CLUREF w;
{
int bi;
/* 1/10/91 : modified wrt gc_dump */
	bi = (i.num-1)*8;
	if (bi < 0) signal(ERR_bounds);
	if (bi > wv.str->size) signal(ERR_bounds);
	wv.vec->data[i.num-1] = w.num;
/*	wv.str->data[bi] = w.num & 0xff;
	wv.str->data[bi+1] = (w.num >> 8)  & 0xff;
	wv.str->data[bi+2] = (w.num >> 16) & 0xff;
	wv.str->data[bi+3] = (w.num >> 24) & 0xff;
*/
	signal(ERR_ok);
	}

errcode _wordvecOPfetch2(wv, i, ans1, ans2)
CLUREF wv;
CLUREF i;
CLUREF *ans1, *ans2;
{
int bi, temp;

	bi = (i.num-1)*8;
	if (bi < 0) signal(ERR_bounds);
	if (bi > (wv.str->size)) signal(ERR_bounds);
	temp = wv.vec->data[i.num - 1];
	ans1->num = (temp & 0xffff0000) >> 16;
	ans2->num = temp & 0xffff;
/*
	ans1->num = (wv.str->data[bi+1] << 8) || wv.str->data[bi];
	ans2->num = (wv.str->data[bi+3] << 8) || wv.str->data[bi+2];
*/
	signal(ERR_ok);
	}	

errcode _wordvecOPstore2(wv, i, l, r)
CLUREF wv;
CLUREF i;
CLUREF l, r;
{
int bi, temp;

	bi = (i.num-1)*8;
	if (bi < 0) signal(ERR_bounds);
	if (bi > wv.str->size) signal(ERR_bounds);
	wv.vec->data[i.num-1] = (r.num & 0xffff) | ((l.num & 0xffff) << 16);
/*
	wv.str->data[bi] = r.num & 0xff;
	wv.str->data[bi+1] = (r.num >> 8) & 0xff;
	wv.str->data[bi+2] = l.num & 0xff;
	wv.str->data[bi+3] = (l.num >> 8) & 0xff;
*/
	signal(ERR_ok);
	}

errcode _wordvecOPxstore(wv, i, b, l)
CLUREF wv;
CLUREF i;
CLUREF b, l;
{
int bi;

	bi = (i.num-1)*8;
	if (bi < 0) signal(ERR_bounds);
	if (bi > wv.str->size) signal(ERR_bounds);
	wv.vec->data[i.num-1] = l.num & 0xffffff | ((b.num & 0xff) << 24);
/*
	wv.str->data[bi+1] = (l.num >> 8) & 0xff;
	wv.str->data[bi+2] = (l.num >> 16) & 0xff;
	wv.str->data[bi+3] = b.num & 0xff;
*/
	signal(ERR_ok);
	}

errcode _wordvecOPbfetch(wv, i, ans)
CLUREF wv;
CLUREF i;
CLUREF *ans;
{
int bi, wi, sub_index, temp;

	bi = (i.num-1);
	if (bi < 0) signal(ERR_bounds);
	if (bi > (wv.str->size)) signal(ERR_bounds);
	wi = bi/8;
	sub_index = bi - (wi*8) + 1;
	temp = wv.vec->data[wi];
	switch (sub_index) {
		case 1:
			temp = temp & 0xff;
			break;
		case 2:
			temp = temp >> 8;
			temp = temp & 0xff;
			break;
		case 3:
			temp = temp >> 16;
			temp = temp & 0xff;
			break;
		case 4:
			temp = temp >> 24;
			temp = temp & 0xff;
			break;
		}
	ans->num = temp;
	signal(ERR_ok);
/*
	ans->num = wv.str->data[bi];
	signal(ERR_ok);
*/
	}	

errcode _wordvecOPbstore(wv, i, c)
CLUREF wv;
CLUREF i;
CLUREF c;
{
int bi, wi, sub_index, temp;

	bi = (i.num-1);
	if (bi < 0) signal(ERR_bounds);
	if (bi > wv.str->size) signal(ERR_bounds);
	c.num = c.num & 0xff;
	wi = bi/8;
	sub_index = bi - (wi*8) + 1;
	temp = wv.vec->data[wi];
	switch (sub_index) {
		case 1:
			temp = temp & ~0xff;
			temp = temp | c.num;
			break;
		case 2:
			temp = temp & ~0xff00;
			temp = temp | (c.num << 8);
			break;
		case 3:
			temp = temp & ~0xff0000;
			temp = temp | (c.num << 16);
			break;
		case 4:
			temp = temp & ~0xff000000;
			temp = temp | (c.num << 24);
			break;
		}
	wv.vec->data[wi] = temp;
/*
	temp.num = c.num & 0xff;
	wv.str->data[bi] = (char)temp.num;
*/
	signal(ERR_ok);
	}


errcode _wordvecOPwfetch(wv, i, ans)
CLUREF wv;
CLUREF i;
CLUREF *ans;
{
int bi, temp, temp2, wi, sub_index;

	bi = (i.num-1);
	if (bi < 0) signal(ERR_bounds);
	if (bi >= (wv.str->size-1)) signal(ERR_bounds);
	wi = bi/8;
	sub_index = bi - (wi*8) + 1;
	temp = wv.vec->data[wi];
	switch (sub_index) {
		case 1:
			temp = temp & 0xffff;
			ans->num = temp;
			break;
		case 2:
			temp = temp & 0xffff00;
			temp = temp >> 8;
			ans->num = temp;
			break;
		case 3:
			temp = temp & 0xffff0000;
			temp = temp >> 16;
			ans->num = temp;
			break;
		case 4:
			temp = temp & ~0xff000000;
			temp = temp >> 24;
			temp2 = wv.vec->data[wi+1];
			temp2 = (temp2 & 0xff) << 8;
			temp = temp2 | temp;
			ans->num = temp;
			break;
		}
/*
	ans->num = (wv.str->data[bi+1] << 8) + wv.str->data[bi];
*/
	signal(ERR_ok);
	}	

errcode _wordvecOPwstore(wv, i, n)
CLUREF wv;
CLUREF i;
CLUREF n;
{
int bi, temp, wi, sub_index;

	bi = (i.num-1);
	if (bi < 0) signal(ERR_bounds);
	if (bi >= wv.str->size-1) signal(ERR_bounds);
	n.num = n.num & 0xffff;
	wi = bi/8;
	sub_index = bi - (wi*8) + 1;
	temp = wv.vec->data[wi];
	switch (sub_index) {
		case 1:
			temp = temp & ~0xffff;
			temp = temp | n.num;
			wv.vec->data[wi] = temp;
			break;
		case 2:
			temp = temp & ~0xffff00;
			temp = temp | (n.num << 8);
			wv.vec->data[wi] = temp;
			break;
		case 3:
			temp = temp & ~0xffff0000;
			temp = temp | (n.num << 16);
			wv.vec->data[wi] = temp;
			break;
		case 4:
			temp = temp & ~0xff000000;
			temp = temp | (n.num << 24);
			wv.vec->data[wi] = temp;
			temp = wv.vec->data[wi+1];
			temp = temp & ~0x000000ff;
			temp = temp | ((n.num & 0xff00) >> 8);
			wv.vec->data[wi+1] = temp;
			break;
		}
/*
	.str->data[bi] = n.num & 0xff;
	wv.str->data[bi + 1] = n.num & 0xff00 >> 8;
*/
	signal(ERR_ok);
	}


errcode _wordvecOPlfetch(wv, i, ans)
CLUREF wv;
CLUREF i;
CLUREF *ans;
{
int bi, temp, temp2, wi, sub_index;

	bi = (i.num-1);
	if (bi < 0) signal(ERR_bounds);
	if (bi >= (wv.str->size-1)) signal(ERR_bounds);
	wi = bi/8;
	sub_index = bi - (wi*8) + 1;
	temp = wv.vec->data[wi];
	switch (sub_index) {
		case 1:
			ans->num = temp;
			break;
		case 2:
			temp = temp & 0xffffff00;
			temp = temp >> 8;
			temp2 = wv.vec->data[wi+1];
			temp2 = (temp2 & 0xff) << 24;
			ans->num = temp | temp2;
			break;
		case 3:
			temp = temp & 0xffff0000;
			temp = temp >> 16;
			temp2 = wv.vec->data[wi+1];
			temp2 = (temp2 & 0xffff) << 16;
			ans->num = temp | temp2;
			break;
		case 4:
			temp = temp & ~0xff000000;
			temp = temp >> 24;
			temp2 = wv.vec->data[wi+1];
			temp2 = (temp2 & 0xff) << 8;
			ans->num = temp | temp2;
			break;
		}
	signal(ERR_ok);
	}	

errcode _wordvecOPlstore(wv, i, n)
CLUREF wv;
CLUREF i;
CLUREF n;
{
int bi, temp, wi, sub_index;

	bi = (i.num-1);
	if (bi < 0) signal(ERR_bounds);
	if (bi >= wv.str->size-1) signal(ERR_bounds);
	wi = bi/8;
	sub_index = bi - (wi*8) + 1;
	temp = wv.vec->data[wi];
	switch (sub_index) {
		case 1:
			wv.vec->data[wi] = n.num;
			break;
		case 2:
			temp = temp & ~0xffffff00;
			temp = temp | (n.num << 8);
			wv.vec->data[wi] = temp;
			temp = wv.vec->data[wi+1];
			temp = temp & ~0xff;
			temp = temp | ((n.num & 0xff000000) >> 24);
			wv.vec->data[wi+1] = temp;
			break;
		case 3:
			temp = temp & ~0xffff0000;
			temp = temp | (n.num << 16);
			wv.vec->data[wi] = temp;
			temp = wv.vec->data[wi+1];
			temp = temp & ~0xffff;
			temp = temp | ((n.num & 0xffff0000) >> 16);
			wv.vec->data[wi+1] = temp;
			break;
		case 4:
			temp = temp & ~0xff000000;
			temp = temp | (n.num << 24);
			wv.vec->data[wi] = temp;
			temp = wv.vec->data[wi+1];
			temp = temp & ~0x00ffffff;
			temp = temp | ((n.num & 0xffffff00) >> 8);
			wv.vec->data[wi+1] = temp;
			break;
		}
	signal(ERR_ok);
	}

errcode _wordvecOPsize(wv, ans)
CLUREF wv;
CLUREF *ans;
{
	ans->num = wv.str->size/8;
	signal(ERR_ok);
	}

errcode _wordvecOPequal(wv1, wv2, ans)
CLUREF wv1;
CLUREF wv2;
CLUREF *ans;
{
	if (wv1.str == wv2.str) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _wordvecOPindexc(c, wv, first, ans)
CLUREF c, wv, first, *ans;
{
int i;
	if (first.num < 1) signal(ERR_bounds);
	if (first.num > wv.str->size) signal(ERR_bounds);
	for (i = first.num; i <= wv.str->size; i++) {
		if (wv.str->data[i-1] == c.ch) {
			ans->num = i;
			signal(ERR_ok);
			}
		}
	ans->num = 0;
	signal(ERR_ok);
	}

/* find wv1 in wv2; start looking at wv2[first] ; */
/* some way to use matchc and/or locc ? */

errcode _wordvecOPindexv(wv1, wv2, first, ans)
CLUREF wv1, wv2, first, *ans;
{
bool found;
int i,j;
	if (first.num < 1) signal(ERR_bounds);	
	if (first.num > wv2.str->size) signal(ERR_bounds);
	for (i = first.num; i + wv1.str->size - 1 <= wv2.str->size; i++) {
		found = true;
		for (j = 1; j <= wv1.str->size; j++) {
			if (wv2.str->data[i-1+j-1] != wv1.str->data[j-1]) {
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

/* find wv1 in wv2; start looking at wv2[first] ; */
/* case doesn't matter */

errcode _wordvecOPnc_indexv(wv1, wv2, first, ans)
CLUREF wv1, wv2, first, *ans;
{
bool found;
int i,j;
char temp_c1, temp_c2;
	if (first.num < 1) signal(ERR_bounds);	
	if (first.num > wv2.str->size) signal(ERR_bounds);
	for (i = first.num; i + wv1.str->size - 1 <= wv2.str->size; i++) {
		found = true;
		for (j = 1; j <= wv1.str->size; j++) {
			temp_c1 = wv1.str->data[j-1];
			if (temp_c1 >= MIN_CAP_CHAR && temp_c1 <= MAX_CAP_CHAR)
				temp_c1 += OFF_CHAR;
			temp_c2 = wv2.str->data[i-1+j-1];
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
	if the wordvecs are not identical,
	if they are the same and the destination is less than the source,
	if they are the same and the dest is gtr than the source and the
		size of the dest is greater than the copycount
	if a single character is being propagated through a wordvec
*/

errcode _wordvecOPmove_lr(wv1, s1, wv2, s2, len)
CLUREF wv1, s1, wv2, s2, len;
{
int copycount;
int i;
	if (len.num < 0) signal(ERR_negative_size);
	if (s1.num < 1) signal(ERR_bounds);
	if (s1.num > wv1.str->size) signal(ERR_bounds);
	if (s2.num < 1) signal(ERR_bounds);
	if (s2.num >wv2.str->size) signal(ERR_bounds);
	copycount = len.num;
	if (copycount + s1.num > wv1.str->size) copycount = wv1.str->size - s1.num + 1;
	if (copycount + s2.num > wv2.str->size) copycount = wv2.str->size - s2.num + 1;
	for (i = 0; i < copycount; i++) {
		wv2.str->data[s2.num-1+i] = wv1.str->data[s1.num-1+i];
		}
	signal(ERR_ok);
	}

/* moves chars from a sequence of chars to a wordvec */

errcode _wordvecOPmove_v(qc1, s1, wv2, s2, len)
CLUREF qc1, s1, wv2, s2, len;
{
int copycount;
int i;
	if (len.num < 0) signal(ERR_negative_size);
	if (s1.num < 1) signal(ERR_bounds);
	if (s1.num > qc1.vec->size) signal(ERR_bounds);
	if (s2.num < 1) signal(ERR_bounds);
	if (s2.num >wv2.str->size) signal(ERR_bounds);
	copycount = len.num;
	if (copycount + s1.num > qc1.vec->size) copycount = qc1.vec->size - s1.num + 1;
	if (copycount + s2.num > wv2.str->size) copycount = wv2.str->size - s2.num + 1;
	for (i = 0; i < copycount; i++) {
		wv2.str->data[s2.num-1+i] = qc1.vec->data[s1.num-1+i];
		}
	signal(ERR_ok);
	}

errcode _wordvecOPmove_rl(w1, s1, w2, s2, len)
CLUREF w1, s1, w2, s2, len;
{
	fprintf(stderr, "_wordvec$move_rl not implemented\n");
	signal(ERR_failure);
	}

errcode _wordvecOPget_byte(wv, wrd, bit, len, ans)     /* Is this every used? */
CLUREF wv, wrd, bit, len, *ans;
{
int n1, result, mask;

	if (len.num <= 0) signal(ERR_illegal_size);
	if (len.num > 32) signal(ERR_illegal_size);    /* 32 or 64?*/
	n1 = wrd.num << 2;
	if (n1 <= 0) signal(ERR_bounds);
	if (n1 > wv.str->size) signal(ERR_bounds);
	if (len.num > bit.num) signal(ERR_bounds);
	result = wv.vec->data[wrd.num - 1];
	result = result >> (32 - bit.num);             /* 32 or 64? */
	mask =  (1 << len.num) - 1;
	/* printf("before %X %X\n", mask, result); */
	result = result & mask;
	/* printf("after %X %X\n", mask, result); */
	ans->num = result;
	signal(ERR_ok);
	}

errcode _wordvecOPset_byte(wv, byte, wrd, bit, len)              /* Is this every used? */
CLUREF wv, byte, wrd, bit, len;
{
int n1;
int slot;
int new_field;
int field_mask;

	if (len.num <= 0) signal(ERR_illegal_size);
	if (len.num > 32) signal(ERR_illegal_size);              /* 32 or 64? */
	n1 = wrd.num << 2;
	if (n1 <= 0) signal(ERR_bounds);
	if (n1 > wv.str->size) signal(ERR_bounds);
	if (len.num > bit.num) signal(ERR_bounds);
	field_mask = (1 << len.num) - 1;
	new_field = (byte.num & field_mask) << (32 - bit.num);  /* 32 or 64? */
	field_mask = field_mask << (32 - bit.num);              /* 32 or 64? */
	slot = wv.vec->data[wrd.num - 1];
	slot = slot & ~field_mask;
	slot |= new_field;
	wv.vec->data[wrd.num - 1] = slot;
	signal(ERR_ok);
	}


errcode _wordvecOPbyte_size(ans)
CLUREF *ans;
{
	ans->num = 8;
	signal(ERR_ok);
	}

errcode _wordvecOPword_size(ans)
CLUREF *ans;
{
	ans->num = 64;
	signal(ERR_ok);
	}

errcode _wordvecOPbytes_per_word(ans)
CLUREF *ans;
{
	ans->num = 8;
	signal(ERR_ok);
	}

/* moves bytes from a _bytevec to a wordvec */

errcode _wordvecOPmove_b2w(b, bpos, w, wpos, n)
CLUREF b, bpos, w, wpos, n;
{
int copycount;
int i;
	if (n.num < 0) signal(ERR_negative_size); /* interface change */
	if (n.num == 0) signal(ERR_ok);
	if (bpos.num < 1) signal(ERR_bounds);
	if (bpos.num > b.str->size) signal(ERR_bounds);
	if (wpos.num < 1) signal(ERR_bounds);
	if (wpos.num > w.str->size) signal(ERR_bounds);
	copycount = n.num;
	if (bpos.num + n.num > b.str->size) 
		copycount = b.str->size - bpos.num + 1;
	if (wpos.num + copycount > w.str->size)
		copycount = w.str->size - wpos.num + 1;
	for (i = 0; i < copycount; i++) {
		w.str->data[wpos.num-1+i] = b.str->data[bpos.num-1+i];
		}
	signal(ERR_ok);
	}

/* moves bytes from a _wordvec to a _bytevec */

errcode _wordvecOPmove_w2b(w, wpos, b, bpos, n)
CLUREF b, bpos, w, wpos, n;
{
int copycount;
int i;
	if (n.num < 0) signal(ERR_negative_size); /* interface change */
	if (n.num == 0) signal(ERR_ok);
	if (bpos.num < 1) signal(ERR_bounds);
	if (bpos.num > b.str->size) signal(ERR_bounds);
	if (wpos.num < 1) signal(ERR_bounds);
	if (wpos.num > w.str->size) signal(ERR_bounds);
	copycount = n.num;
	if (wpos.num + copycount > w.str->size)
		copycount = w.str->size - wpos.num + 1;
	if (bpos.num + copycount > b.str->size) 
		copycount = b.str->size - bpos.num + 1;
	for (i = 0; i < copycount; i++) {
		b.str->data[bpos.num-1+i] = w.str->data[wpos.num-1+i];
		}
	signal(ERR_ok);
	}

errcode _wordvecOP_gcd(w, tab, ans)
CLUREF w, tab, *ans;
{
errcode err;
CLUREF temp_oneof, sz;

        err = oneofOPnew(CLU_1, CLU_0, &temp_oneof);
        if (err != ERR_ok) resignal(err);
/*                    8 for type + size, 1 for trailing 0, +7 &~7 to round up */
	sz.num = 8 + (w.str->size+1 + 7)&~7;
        err = gcd_tabOPinsert(tab, sz, temp_oneof, w, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode _wordvecOPprint(w, pst)
CLUREF w, pst;
{
errcode err;
	err = stringOPprint(w, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode _wordvecOPdebug_print(w, pst)
CLUREF w, pst;
{
errcode err;
	err = stringOPprint(w, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

typedef struct {
int count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[13];
} _wordvec_OPS;
CLU_proc _wordvec_oe_create = {{0,0,0,0}, _wordvecOPcreate, 0};
CLU_proc _wordvec_oe_copy = {{0,0,0,0}, _wordvecOPcopy, 0};
CLU_proc _wordvec_oe_fetch = {{0,0,0,0}, _wordvecOPfetch, 0};
CLU_proc _wordvec_oe_store = {{0,0,0,0}, _wordvecOPstore, 0};
CLU_proc _wordvec_oe_size = {{0,0,0,0}, _wordvecOPsize, 0};
CLU_proc _wordvec_oe_equal = {{0,0,0,0}, _wordvecOPequal, 0};
CLU_proc _wordvec_oe_indexc = {{0,0,0,0}, _wordvecOPindexc, 0};
CLU_proc _wordvec_oe_indexv = {{0,0,0,0}, _wordvecOPindexv, 0};
CLU_proc _wordvec_oe_nc_indexv = {{0,0,0,0}, _wordvecOPnc_indexv, 0};
CLU_proc _wordvec_oe_move_lr = {{0,0,0,0}, _wordvecOPmove_lr, 0};
CLU_proc _wordvec_oe_move_v = {{0,0,0,0}, _wordvecOPmove_v, 0};
CLU_proc _wordvec_oe_print = {{0,0,0,0}, _wordvecOPprint, 0};
CLU_proc _wordvec_oe_debug_print = {{0,0,0,0}, _wordvecOPdebug_print, 0};

OWN_ptr _wordvec_own_init = {1,0};

_wordvec_OPS _wordvec_ops_actual = {13,
    &_wordvec_own_init,
    &_wordvec_own_init, {
{&_wordvec_oe_create, "create"},
{&_wordvec_oe_copy, "copy"},
{&_wordvec_oe_fetch, "fetch"},
{&_wordvec_oe_store, "store"},
{&_wordvec_oe_size, "size"},
{&_wordvec_oe_equal, "equal"},
{&_wordvec_oe_indexc, "indexc"},
{&_wordvec_oe_indexv, "indexv"},
{&_wordvec_oe_nc_indexv, "nc_indexv"},
{&_wordvec_oe_move_lr, "move_lr"},
{&_wordvec_oe_move_v, "move_v"},
{&_wordvec_oe_print, "print"},
{&_wordvec_oe_debug_print, "debug_print"}
}};

_wordvec_OPS *_wordvec_ops = &_wordvec_ops_actual;
