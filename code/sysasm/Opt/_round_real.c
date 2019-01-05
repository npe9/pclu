
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _round_real.c,v 1.2 91/06/06 13:25:21 root Exp $";
#endif
/* $Log:	_round_real.c,v $
 * Revision 1.2  91/06/06  13:25:21  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:35  mtv
 * Initial revision
 * 
 */

/*						*/
/*		Round a Real Number		*/
/* given a string representation of the mantissa, */
/*	 & number of desired digits		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _round_real(digs, num, ans1, ans2)
CLUREF digs, num;
CLUREF *ans1, *ans2;
{
errcode err;
CLUREF temp1, temp2;
int nz, i, carry;
bool done;

	if (num.num > 9) num.num = 9;
	if (num.num <= 0) num.num = 1;
	done = false;
	if (digs.str->data[num.num] >= '5') {
		for (i = num.num-1; i > -1; i--) {
			if (digs.str->data[i] == '9') {
				digs.str->data[i] = '0';
				}
			else {
				digs.str->data[i] = digs.str->data[i] + 1;
				done = true;
				break;
				}
			} /* for */
		if (done == false) {
                        digs.str->data[0] = '1';
			ans1->num = 1;
			ans2->num = 1;
			signal(ERR_ok);
			}
		}
	nz = num.num;
	for (i = num.num-1; i > -1; i--) {
		if (digs.str->data[i] != '0') break;
		nz -= 1;
		}
	ans1->num = 0;
	ans2->num = nz;
	signal(ERR_ok);
	}
