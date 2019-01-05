
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _unparse_real.c,v 1.2 91/06/06 13:25:21 root Exp $";
#endif 
/* $Log:	_unparse_real.c,v $
 * Revision 1.2  91/06/06  13:25:21  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:35  mtv
 * Initial revision
 * 
 */

/*						*/
/*		Unparse a Real Number		*/
/* given a string representation of the mantissa, */
/*	 & number of desired digits		*/
/*						*/

#include <stdio.h>
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _unparse_real(r, ans1, ans2, ans3)
CLUREF r;
CLUREF *ans1, *ans2, *ans3;
{
errcode err;
int uerr;
char temp[100];
CLUREF minus, exp, temp2, size;
float mant;

	if (r.real == 0.0) {
		err = stringOPcons("0", CLU_1, CLU_1, &temp2);
		ans1->tf = false;
		ans2->str = temp2.str;
		ans3->num = 0;
		signal(ERR_ok);
		}
	if (r.real < 0.0) {minus.tf = true; r.real = -r.real;} 
		else {minus.tf = false;}
	exp.num = 0;
	mant = r.real;
	for(; mant >= 1.0;) {
		mant = mant/10.0;
		exp.num += 1;
		}
	for(; mant < 0.1;) {
		mant = mant*10.0;
		exp.num -= 1;
		}
	uerr = sprintf(temp, "%11.9f", (double)mant);
	if (uerr == -1) signal(ERR_bad_format);
	size.num = strlen(temp) - 2;
	err = stringOPcons(temp, CLU_3, size, &temp2);
	if (err != ERR_ok) resignal(err);
	ans1->tf = minus.tf;
	ans2->str = temp2.str;
	ans3->num = exp.num;
	signal(ERR_ok);
	}

