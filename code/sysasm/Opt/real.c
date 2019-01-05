
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/real.c,v 1.8 91/10/08 18:27:10 dcurtis Exp $";
#endif
/* $Log:	real.c,v $
 * Revision 1.8  91/10/08  18:27:10  dcurtis
 * fixed string$cons call for sun compatibility
 * 
 * Revision 1.7  91/09/25  14:48:25  dcurtis
 * nothing done
 * 
 * Revision 1.6  91/09/25  14:17:33  dcurtis
 * updated exponent and mantissa to work
 * 
 * Revision 1.5  91/09/09  13:29:20  dcurtis
 * added extern declarations for pow, drem. logb
 * improved overflow detection for power
 * 
 * Revision 1.4  91/08/30  14:25:24  dcurtis
 * various fixes: realOPsub needed to use floats throughout
 *                realOPi2r & realOPr2i were backwards
 *                realOPtrunc needed to look at arg as float
 * 
 * Revision 1.3  91/06/06  13:30:06  root
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  12:27:51  root
 * fixed aggregate initialization in ops_actual
 * fixed OPS entry for parse (formerly mapped to unparse)
 * added result variable to pstream call
 * 
 * Revision 1.1  91/02/04  15:49:50  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*	unreal	IMPLEMENTATION OF		*/
/*			REAL			*/
/*						*/

#include <stdio.h>
#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode realOPprint();

#include <errno.h>
extern double pow();
extern double drem();
extern double logb();

/* This is the real (real cluster).  Many of these procedures
   should be inlined.  They are all here for reference.  Full
   reals are implemented: no bits are left for tags.
*/

/*
real = cluster is add, sub, mul, minus, div, power, abs, min, max,
		 i2r, r2i, trunc, exponent, mantissa, parse, unparse,
		 lt, le, ge, gt,
		 equal, similar, copy, print, encode, decode, _gcd

*/

errcode realOPadd(r1, r2, ans)
CLUREF r1, r2, *ans;
{
float temp;

	temp = r1.real + r2.real;
	ans->real = temp;
	signal(ERR_ok);
}

errcode realOPsub(r1, r2, ans)
CLUREF r1, r2, *ans;
{
float temp;
	temp = r1.real - r2.real;
	ans->real = temp;
	signal(ERR_ok);
}	


errcode realOPmul(r1, r2, ans)
CLUREF r1, r2, *ans;
{
float temp;

	temp = r1.real * r2.real;
	ans->real = temp; 
	signal(ERR_ok);
}

errcode realOPminus(r1, ans)
CLUREF r1, *ans;
{
	ans->real = -r1.real;
	signal(ERR_ok);
}

errcode realOPdiv(num, den, ans)
CLUREF num, den, *ans;
{
	if (den.real == 0) signal(ERR_zero_divide);
	ans->real = num.real/den.real;
	signal(ERR_ok);
}

errcode realOPpower(r, exp, ans)
CLUREF r, exp, *ans;
{
double temp, dr, dexp;

	dr = (double) r.real;
	dexp = (double) exp.real;
	temp = pow(dr, dexp);
	if (isnan(temp)) signal(ERR_overflow);
	ans->real = temp; 
	signal(ERR_ok);
}

errcode realOPabs(r, ans)
CLUREF r, *ans;
{
	if (r.real >= 0) {ans->real = r.real; signal(ERR_ok);}
	ans->real = -r.real;
	signal(ERR_ok);
}

errcode realOPmin(r1, r2, ans)
CLUREF r1, r2, *ans;
{
	if (r1.real <= r2.real) ans->real = r1.real;
	else ans->real = r2.real;
	signal(ERR_ok);
}

errcode realOPmax(r1, r2, ans)
CLUREF r1, r2, *ans;
{
	if (r1.real >= r2.real) ans->real = r1.real;
	else ans->real = r2.real;
	signal(ERR_ok);
}

errcode realOPi2r(i, ans)
CLUREF i, *ans;
{
	ans->real = (float)i.num;
	signal(ERR_ok);
	}

errcode realOPr2i(r, ans)
CLUREF r, *ans;
{
	ans->num = (long)r.real;
	signal(ERR_ok);
	}

errcode realOPtrunc(x, ans)
CLUREF x, *ans;
{
long temp;

	if (x.real > intOPmaxint) signal(ERR_overflow);
	temp = (long) x.real;
	ans->num = temp;
	signal(ERR_ok);
	}

errcode realOPexponent(r, ans)
CLUREF r, *ans;
{
CLUREF exp;
float mant;

	if (r.real == 0.0) {
		signal(ERR_undefined);
		}
	if (r.real < 0.0) r.real = -r.real;
	exp.num = 0;
	mant = r.real;
	for(; mant > 10.0;) {
		mant = mant/10.0;
		exp.num += 1;
		}
	for(; mant < 1.0;) {
		mant = mant*10.0;
		exp.num -= 1;
		}
	ans->num = exp.num;
	signal(ERR_ok);
	}

errcode realOPmantissa(r, ans)
CLUREF r, *ans;
{
float mant;
bool minus;

	if (r.real == 0.0) {
		ans->real = 0.0;
		}
	if (r.real < 0.0) {minus = true; r.real = -r.real;}
		else {minus = false;}
	mant = r.real;
	for(; mant > 10.0;) {
		mant = mant/10.0;
		}
	for(; mant < 1.0;) {
		mant = mant*10.0;
		}
	if (minus == false) ans->real = mant;
	else ans->real  = -mant;
	signal(ERR_ok);
	}

errcode realOPparse(s, ans)
CLUREF s, *ans;
{
float temp;
int cnt;

	cnt = sscanf(s.str->data, "%f", &temp);
	if (cnt == 0) signal(ERR_bad_format);
	ans->real = temp;
	signal(ERR_ok);
}

errcode realOPunparse(r, ans)
CLUREF r, *ans;
{
errcode err;
int uerr;
char temp[100];
CLUREF temp2, size;

	uerr = sprintf(temp, "%e", (double)r.real);
	if (uerr == -1) signal(ERR_bad_format);
	size.num = strlen(temp);
	err = stringOPcons(temp, CLU_1, size, &temp2);
	if (err != ERR_ok) resignal(err);
	ans->str = temp2.str;
	signal(ERR_ok);
	}

errcode realOPlt(r1, r2, ans)
CLUREF r1, r2, *ans;
{
	ans->tf = false;
	if (r1.real < r2.real) ans->tf = true;
	signal(ERR_ok);
}

errcode realOPle(r1, r2, ans)
CLUREF r1, r2, *ans;
{
	ans->tf = false;
	if (r1.real <= r2.real) ans->tf = true;
	signal(ERR_ok);
}

errcode realOPgt(r1, r2, ans)
CLUREF r1, r2, *ans;
{
	ans->tf = false;
	if (r1.real > r2.real) ans->tf = true;
	signal(ERR_ok);
}

errcode realOPge(r1, r2, ans)
CLUREF r1, r2, *ans;
{
	ans->tf = false;
	if (r1.real >= r2.real) ans->tf = true;
	signal(ERR_ok);
}

errcode realOPequal(r1, r2, ans)
CLUREF r1, r2, *ans;
{
	ans->tf = false;
	if (r1.real == r2.real) ans->tf = true;
	signal(ERR_ok);
}

errcode realOPsimilar(r1, r2, ans)
CLUREF r1, r2, *ans;
{
	ans->tf = false;
	if (r1.real == r2.real) ans->tf = true;
	signal(ERR_ok);
}

errcode realOPcopy(r, ans)
CLUREF r, *ans;
{
	ans->real = r.real;
	signal(ERR_ok);
}

errcode realOPdebug_print(r, pst)
CLUREF r, pst;
{
errcode err;
	err = realOPprint(r, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode realOPprint(r, pst)
CLUREF r, pst;
{
errcode err;
CLUREF temp, ans;

	err = realOPunparse(r, &temp);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPtext(pst, temp, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode realOPencode(r, ist)
CLUREF r, ist;
{
errcode err;

	err = istreamOPputi(ist, r);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode realOPdecode(ist, ans)
CLUREF ist, *ans;
{
errcode err;

	err = istreamOPgeti(ist, ans);
	if (err == ERR_not_possible || err == ERR_end_of_file) signal(err);
	if (err != ERR_ok) resignal(err);
/* add a sanity check here??? */
	signal(ERR_ok);
	}

errcode realOP_gcd(r, tab, ans)
CLUREF r, tab, *ans;
{
	ans->num = -1;
	signal(ERR_ok);
}

typedef struct {
long count;
OWNPTR	type_owns;
OWNPTR  op_owns;
struct OP_ENTRY entry[23];
} real_OPS;

OWN_ptr real_own_init = {1, 0};

CLU_proc real_oe_add = {{0,0,0,0}, realOPadd,&real_own_init, &real_own_init};
CLU_proc real_oe_sub = {{0,0,0,0}, realOPsub,&real_own_init, &real_own_init};
CLU_proc real_oe_mul = {{0,0,0,0}, realOPmul,&real_own_init, &real_own_init};
CLU_proc real_oe_minus = {{0,0,0,0}, realOPminus,&real_own_init, &real_own_init};
CLU_proc real_oe_div = {{0,0,0,0}, realOPdiv,&real_own_init, &real_own_init};
CLU_proc real_oe_power = {{0,0,0,0}, realOPpower,&real_own_init, &real_own_init};
CLU_proc real_oe_abs = {{0,0,0,0}, realOPabs,&real_own_init, &real_own_init};
CLU_proc real_oe_min = {{0,0,0,0}, realOPmin,&real_own_init, &real_own_init};
CLU_proc real_oe_max = {{0,0,0,0}, realOPmax,&real_own_init, &real_own_init};
CLU_proc real_oe_parse = {{0,0,0,0}, realOPparse,&real_own_init, &real_own_init};
CLU_proc real_oe_unparse = {{0,0,0,0}, realOPunparse,&real_own_init, &real_own_init};
CLU_proc real_oe_lt = {{0,0,0,0}, realOPlt,&real_own_init, &real_own_init};
CLU_proc real_oe_le = {{0,0,0,0}, realOPle,&real_own_init, &real_own_init};
CLU_proc real_oe_gt = {{0,0,0,0}, realOPgt, &real_own_init, &real_own_init};
CLU_proc real_oe_ge = {{0,0,0,0}, realOPge, &real_own_init, &real_own_init};
CLU_proc real_oe_equal = {{0,0,0,0}, realOPequal, &real_own_init, &real_own_init};
CLU_proc real_oe_similar = {{0,0,0,0}, realOPsimilar, &real_own_init, &real_own_init};
CLU_proc real_oe_copy = {{0,0,0,0}, realOPcopy, &real_own_init, &real_own_init};
CLU_proc real_oe_print = {{0,0,0,0}, realOPprint, &real_own_init, &real_own_init};
CLU_proc real_oe_encode = {{0,0,0,0}, realOPencode, &real_own_init, &real_own_init};
CLU_proc real_oe_decode = {{0,0,0,0}, realOPdecode, &real_own_init, &real_own_init};
CLU_proc real_oe__gcd = {{0,0,0,0}, realOP_gcd, &real_own_init, &real_own_init};
CLU_proc real_oe_debug_print = {{0,0,0,0}, realOPdebug_print, &real_own_init, &real_own_init};
real_OPS real_ops_actual = {23,
&real_own_init,
&real_own_init, {
{&real_oe_add, "add"},
{&real_oe_sub, "sub"},
{&real_oe_mul, "mul"},
{&real_oe_minus, "minus"},
{&real_oe_div, "div"},
{&real_oe_power, "power"},
{&real_oe_abs, "abs"},
{&real_oe_min, "min"},
{&real_oe_max, "max"},
{&real_oe_parse, "parse"},
{&real_oe_unparse, "unparse"},
{&real_oe_lt, "lt"},
{&real_oe_le, "le"},
{&real_oe_gt, "gt"},
{&real_oe_ge, "ge"},
{&real_oe_equal, "equal"},
{&real_oe_similar, "similar"},
{&real_oe_copy, "copy"},
{&real_oe_print, "print"},
{&real_oe_encode, "encode"},
{&real_oe_decode, "decode"},
{&real_oe__gcd, "_gcd"},
{&real_oe_debug_print, "debug_print"}
}};

real_OPS *real_ops = &real_ops_actual;

/* to add:		 i2r, r2i, trunc, exponent, mantissa, */
