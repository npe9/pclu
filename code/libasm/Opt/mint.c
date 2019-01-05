
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: mint.c,v 1.3 91/07/09 15:18:13 root Exp $";
#endif
/* $Log:	mint.c,v $
 * Revision 1.3  91/07/09  15:18:13  root
 * == -> = (caught by lint)
 * 
 * Revision 1.2  91/06/06  13:55:13  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:35  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			mint			*/
/*						*/

#include <sys/types.h>
#include <sys/dir.h>
#include <limits.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

/* MODIFIED BY ROBERT G. FERMIER */

#if defined(sparc) || defined(linux)
#define WORD_BIT 32
#endif

extern CLUREF clu_empty_string;
extern char **environ;
extern int wrpipe;
#include <errno.h>

errcode i_and(i, j, ans)
CLUREF i, j, *ans;
{
	ans->num = i.num & j.num;
	signal(ERR_ok);
	}

errcode i_or(i, j, ans)
CLUREF i, j, *ans;
{
	ans->num = i.num | j.num;
	signal(ERR_ok);
	}


errcode i_xor(i, j, ans)
CLUREF i, j, *ans;
{
	ans->num = i.num ^ j.num;
	signal(ERR_ok);
	}

errcode i_not(i, ans)
CLUREF i, *ans;
{
	ans->num = ~i.num;
	signal(ERR_ok);
	}

errcode i_shift(i, cnt, ans)
CLUREF i, cnt, *ans;
{
int temp_res;
	/* handle the case where the number to be shifted is zero */
	if (i.num == 0) {
		ans->num = 0;
		signal(ERR_ok);
		}
	/* handler the case where the number doesn't get shifted at all */
	if (cnt.num == 0) {
		ans->num = i.num;
		signal(ERR_ok);
		}
	if (cnt.num > 0) {
		/* we are multiplying by a power of 2 */
		/* |result| should be greater than |i| */
		/* else overflow */
		if (cnt.num >= WORD_BIT) signal(ERR_overflow);
		temp_res = i.num << cnt.num;
	       	if (i.num > 0 && temp_res <= i.num) signal(ERR_overflow);
       		if (i.num < 0 && temp_res >= i.num) signal(ERR_overflow);
		ans->num = temp_res;
		signal(ERR_ok);
		}
	else {
		/* we are dividing by a power of 2 */
		/* |result| should be less than |i| */
		/* else result is 0 for positive input, -1 for negative input */
		if (-cnt.num >= WORD_BIT) {
			if (i.num > 0) temp_res = 0;
			else temp_res = -1;
			}
		else {
			temp_res = i.num >> -cnt.num;
			if (i.num > 0 && temp_res >= i.num) temp_res = 0;
			if (i.num < 0 && temp_res <= i.num) temp_res = -1;
			}
		ans->num = temp_res;
		signal(ERR_ok);
		}
	}

errcode i_rotate(input, cnt, ans)
CLUREF input, cnt, *ans;
{
int i,j;
int temp, cnt2;

/* Probably a table lookup system would be faster */

	cnt2 = cnt.num;
	if (cnt.num < 0) cnt2 = WORD_BIT + cnt.num;
	temp = 0;
	for (i = 0; i < WORD_BIT; i++) {
		if (input.num & 1 << i) {
			j = (i + cnt2) % WORD_BIT;
			temp |= 1 << j;
			}
		}
	ans->num = temp;
	signal(ERR_ok);
	}

errcode i_get (i, bit, cnt, ans)
CLUREF i, bit, cnt, *ans;
{
int temp;
int j,k;

	if (cnt.num <= 0) signal(ERR_illegal_size);
	if (bit.num >= WORD_BIT) signal(ERR_bounds);
	if ((bit.num - (cnt.num - 1)) < 0) signal(ERR_bounds);
	temp = 0;
	for (j = (bit.num - (cnt.num - 1)), k = 0; k <= bit.num; j++, k++) {
		if (i.num & (1 << (j))) {
			temp |= (1 << k);
			}
		}
	ans->num = temp;
	signal(ERR_ok);
	}

errcode i_set (i, bit, cnt, j, ans)
CLUREF i, bit, cnt, j, *ans;
{
int temp;
int j1, k;

	if (cnt.num <= 0) signal(ERR_illegal_size);
	if (bit.num >= WORD_BIT) signal(ERR_bounds);
	if (cnt.num - bit.num - 1 < 0) signal(ERR_bounds);
	temp = i.num;
	for (j1 = bit.num, k = 0; j1 <= cnt.num; j1++, k++) {
		if (j.num & (1 << (j1))) temp |= (1 << k);
		else temp &= ~(1 << k);
		}
	ans->num = temp;
	signal(ERR_ok);
	}

/* return the value of the bit'th bit in i */
/* return values are 0 or 1 */
/* min value for bit is 0, max value is 31 */
errcode i_get1(i, bit, ans)
CLUREF i, bit, *ans;
{
	if (bit.num >= WORD_BIT) signal(ERR_bounds);
	if (i.num & (1 << (bit.num))) ans->num = 1;
	else ans->num = 0;
	signal(ERR_ok);
	}

/* change the value of the bit'th bit in i to b */
/* min value for bit is 0, max value is 31 */
errcode i_set1(i, bit, b, ans)
CLUREF i, bit, b, *ans;
{
int temp;

	if (bit.num >= WORD_BIT) signal(ERR_bounds);
	temp = i.num;
	if (b.tf) ans->tf = temp | (1 << (bit.num));
	else ans->tf = temp & ~(1 << (bit.num));
	signal(ERR_ok);
	}


/* return index of most significant bit that is set */
/* min index = 0, max index = 31 */
errcode i_first1(i, ans)
CLUREF i, *ans;
{
int j;
	if (i.num > 0) {
		for (j = 31; j > -1; j--) {
			if (i.num & (1 << j)) break;
			}
		ans->num = j;
		signal(ERR_ok);
		}
	if (i.num < 0) {
		ans->num = 31;
		}
	signal(ERR_none);
	}

/* return index of least significant bit that is set */
/* min index = 0, max index = 31 */
errcode i_last1(i, ans)
CLUREF i, *ans;
{
int j;

	if (i.num == 0) signal(ERR_none);
	for (j = 0; j < WORD_BIT; j++) {
		if (i.num & (1 << j)) break;
		}
	ans->num = j;    /* removed +1 6/4/90 */
	signal(ERR_ok);
	}


