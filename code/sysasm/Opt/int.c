
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: int.c,v 1.6 91/06/06 13:28:05 root Exp $";
#endif
/* $Log:	int.c,v $
 * Revision 1.6  91/06/06  13:28:05  root
 * added copyright notice
 * 
 * Revision 1.5  91/06/03  13:01:15  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.4  91/05/31  12:17:36  root
 * fixed aggregate initialization in ops_actual
 * added result variable to pstream calls
 * removed unused variables
 * fixed printing of negative numbers
 * 
 * Revision 1.3  91/02/14  16:19:11  dcurtis
 * fixed power to handle exp <= 0 properly
 * 
 * Revision 1.2  91/02/11  11:19:40  dcurtis
 * changed badfmt signals to bad_format signals
 * /
 * 
 * Revision 1.1  91/02/04  15:49:43  mtv
 * Initial revision
 * 
 */

/* Copyright Massachusetts Institute of Technology 1989 */


/* This is the int (integer cluster).  Many of these procedures
   should be inlined.  They are all here for reference.  Full
   ints are implemented: no bits are left for tags.
*/
/*
int = cluster is add, sub, mul, minus, div, power, mod, abs, min, max,
		 from_to_by, from_to, parse, unparse, lt, le, ge, gt,
		 equal, similar, copy, print, encode, decode, _gcd

*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode intOPprint();

#include <errno.h>
extern errcode stringOPcons();

errcode intOPadd(i1, i2, ans)
CLUREF i1, i2;
CLUREF *ans;
{
long temp;
	temp = i1.num + i2.num;
	if (i1.num < 0 && i2.num < 0 && temp >= 0) signal(ERR_overflow);
	if (i1.num > 0 && i2.num > 0 && temp <= 0) signal(ERR_overflow);
	ans->num = temp;
	signal(ERR_ok);
}

errcode intOPsub(i1, i2, ans)
CLUREF i1, i2;
CLUREF *ans;
{
long temp;
	temp = i1.num - i2.num;
	if (i1.num < 0 && (-i2.num) < 0 && temp >= 0) signal(ERR_overflow);
	if (i1.num > 0 && (-i2.num) > 0 && temp <= 0) signal(ERR_overflow);
	ans->num = temp;
	signal(ERR_ok);
}	


errcode intOPmul(i1, i2, ans)
CLUREF i1, i2;
CLUREF *ans;
{
long temp;
long abs_temp;
long abs_i1;
long abs_i2;

/* be careful: leastint*0 is 0, leastint*1 is leastint,
			leastint*n (n != 1,0) => overflow */
	 

/* handle zero first */
	if (i1.num == 0 || i2.num == 0) {ans->num = 0; signal(ERR_ok);}

/* handle one */
	if (i1.num == 1) {ans->num = i2.num; signal(ERR_ok);}
	if (i2.num == 1) {ans->num = i1.num; signal(ERR_ok);}

/* deal with cases where one of the numbers is the smallest integer */
	if (i1.num == intOPleastint || i2.num == intOPleastint) signal(ERR_overflow);

/* form absolute values */

	if (i1.num >= 0) abs_i1 = i1.num;
		else abs_i1 = -i1.num;
	if (i2.num >= 0) abs_i2 = i2.num;
		else abs_i2 = -i2.num;

/* if both absolute values are less than
	 square root of max value then all is ok */

	if (abs_i1 < intOPsqrtmax && abs_i2 < intOPsqrtmax)
		{ans->num = i1.num*i2.num; signal(ERR_ok);}

/* if both integers greater than square root of max value then no good */
	if (abs_i1 > intOPsqrtmax && abs_i2 > intOPsqrtmax)
		{signal(ERR_overflow);}

/* now just multiply and check that result is greater than components */
	temp = i1.num * i2.num;
	if (temp != intOPleastint) {
		if (temp >= 0) abs_temp = temp;
			else abs_temp = -temp;
		if (abs_temp >= abs_i1
				&& abs_temp >= abs_i2)
			{ans->num = temp; signal(ERR_ok);}
		signal(ERR_overflow);
		}
	else {
		if (i1.num < 0 && i2.num > 0) signal(ERR_ok);
		if (i1.num > 0 && i2.num < 0) signal(ERR_ok);
		signal(ERR_overflow);
		}
		
}

errcode intOPminus(i1, ans)
CLUREF i1;
CLUREF *ans;
{
	if (i1.num == intOPleastint) signal(ERR_overflow);
	ans->num = -i1.num;
	signal(ERR_ok);
}

/* CLU: num/den = ans + remainder where 0 <= remainder < |den| */
/*	i.e. remainder is always positive		       */
/*	=> -20/6 = -4 not -3				       */
/* C: -20/6 = -3					       */

errcode intOPdiv(num, den, ans)
CLUREF num;
CLUREF den;
CLUREF *ans;
{
bool adjust;
long temp;

	if (den.num == 0) signal(ERR_zero_divide);

	if (num.num < 0) {adjust = true;}
	else adjust = false;

	temp = num.num/den.num;
	if (temp * den.num != num.num) {
		if (adjust && (temp < 0)) temp--;
		if (adjust && (temp > 0)) temp++;
		if (adjust && (temp == 0) && (den.num < 0)) temp++;
		if (adjust && (temp == 0) && (den.num > 0)) temp--;
		}
	ans->num = temp;
	signal(ERR_ok);
}

extern double pow();

errcode intOPpower(i, exp, ans)
CLUREF i;
CLUREF exp;
CLUREF *ans;
{
double temp, di, dexp;
long temp2;
/* these tests may be redundant with pow */
	if (exp.num <= 0)
		{if (exp.num == 0) {ans->num = 1; signal(ERR_ok);}
		 signal(ERR_negative_exponent);}
	errno = 0;
	di = (double) i.num;
	dexp = (double) exp.num;
	temp = pow(di, dexp);
	temp2 = temp;
	if (errno == 0) {ans->num = temp2; signal(ERR_ok);}
	signal(ERR_overflow);
}

/* 
   CLU int$mod: i mod base = res
		0 <= res < |base|
		i = base * q + res    for some number q
*/

/* 
   C % : (a/b)*b + a%b = a
*/

errcode intOPmod(i, base, ans)
CLUREF i;
CLUREF base;
CLUREF *ans;
{
long temp;
bool flip;

	if (base.num == 0) signal(ERR_zero_divide);
	errno = 0;

	if ( i.num < 0 ) { i.num = -i.num; flip = true; }
	else { flip = false; }

	if ( base.num < 0 ) { base.num = -base.num;}

	temp = i.num % base.num;
	if (errno == 0) {
		if (flip && temp != 0) temp = base.num - temp;
		ans->num = temp; 
		signal(ERR_ok);
		}
	signal(ERR_overflow);
}

errcode intOPabs(i, ans)
CLUREF i;
CLUREF *ans;
{
	if (i.num >= 0) {ans->num = i.num; signal(ERR_ok);}
	if (i.num == intOPleastint) {signal(ERR_overflow);}
	ans->num = -i.num;
	signal(ERR_ok);
}

errcode intOPmin(i1, i2, ans)
CLUREF i1;
CLUREF i2;
CLUREF *ans;
{
	if (i1.num <= i2.num) ans->num = i1.num;
	else ans->num = i2.num;
	signal(ERR_ok);
}

errcode intOPmax(i1, i2, ans)
CLUREF i1;
CLUREF i2;
CLUREF *ans;
{
	if (i1.num >= i2.num) ans->num = i1.num;
	else ans->num = i2.num;
	signal(ERR_ok);
}


errcode intOPparse(s, ans)
CLUREF s;
CLUREF *ans;
{
CLUREF count, start, index;
CLUREF firstchar, thischar;
int i;
long total = 0;
errcode err;

	err = stringOPsize(s, &count);
	if (count.num == 0) signal(ERR_bad_format);
	start.num = 1;
	err = stringOPfetch(s, start, &firstchar);
	if (firstchar.ch == '-' || firstchar.ch == '+')
		{start.num = start.num + 1;
		 count.num = count.num - 1;
		 if (count.num == 0) signal(ERR_bad_format);}
	else firstchar.ch = '+';
	index.num = start.num;
	for (i = 0; i < count.num; i++) {
		err = stringOPfetch(s, index, &thischar);
		if (thischar.ch >= '0' && thischar.ch <= '9') {
			total = total * 10 - (thischar.ch - '0');
			if (total > 0) signal(ERR_overflow);
			index.num += 1;
			}
		else
			signal(ERR_bad_format);
		}
	if (firstchar.ch == '+') {
		if (total == intOPleastint) {signal(ERR_overflow);}
		total = -total;
		}
	ans->num = total;
	signal(ERR_ok);
}

errcode intOPunparse(i, ans)
CLUREF i;
CLUREF *ans;
{
CLUREF beg, len;
char buf[intOPmaxdigits+1]; 
char sign;
bool least = false;
int ind;
int first = 0;
int last = intOPmaxdigits;
char digit;
errcode err;

	if (i.num == intOPleastint) {
		i.num++; least = true;
		}
	sign = '+';
	if (i.num < 0) {sign = '-'; i.num = -i.num;}
	for (ind = last; ind >= first; ind--) {
		digit = i.num % 10;
		i.num = i.num/10;
		if (least == true) {
			digit++;
			least = false;
			if (digit == 10) {
				digit = 0;
				i.num++;
				}
			}
		digit += '0';
		buf[ind] = digit;
		if (i.num == 0) {ind--; break;}
		}
	if (sign == '-') {buf[ind] = sign; ind--;}
	ind++;
	beg.num = ind + 1;
	len.num = last - ind + 1;
	err = stringOPcons(buf, beg, len, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}


errcode intOPlt(i1, i2, ans)
CLUREF i1;
CLUREF i2;
CLUREF *ans;
{
	ans->tf = false;
	if (i1.num < i2.num) ans->tf = true;
	signal(ERR_ok);
}

errcode intOPle(i1, i2, ans)
CLUREF i1;
CLUREF i2;
CLUREF *ans;
{
	ans->tf = false;
	if (i1.num <= i2.num) ans->tf = true;
	signal(ERR_ok);
}

errcode intOPgt(i1, i2, ans)
CLUREF i1;
CLUREF i2;
CLUREF *ans;
{
	ans->tf = false;
	if (i1.num > i2.num) ans->tf = true;
	signal(ERR_ok);
}

errcode intOPge(i1, i2, ans)
CLUREF i1;
CLUREF i2;
CLUREF *ans;
{
	ans->tf = false;
	if (i1.num >= i2.num) ans->tf = true;
	signal(ERR_ok);
}

errcode intOPequal(i1, i2, ans)
CLUREF i1;
CLUREF i2;
CLUREF *ans;
{
	ans->tf = false;
	if (i1.num == i2.num) ans->tf = true;
	signal(ERR_ok);
}

errcode intOPsimilar(i1, i2, ans)
CLUREF i1;
CLUREF i2;
CLUREF *ans;
{
	ans->tf = false;
	if (i1.num == i2.num) ans->tf = true;
	signal(ERR_ok);
}

errcode intOPcopy(i, ans)
CLUREF i;
CLUREF *ans;
{
	ans->num = i.num;
	signal(ERR_ok);
}

errcode intOPdebug_print(i, pst)
CLUREF i, pst;
{
errcode err;

	err = intOPprint(i, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode err;
errcode intOPprint(i, pst)
CLUREF i, pst;
{
errcode err;
bool positive;
long temp, j, k;
char digits[20];
CLUREF ch, ans;

	temp = i.num;
	if (i.num < 0 ) {positive = false; temp = -i.num;}
	else positive = true;

	for (j = 0 ; ; j++) {
		digits[j] = temp % 10;
		temp = temp / 10;
		if (temp == 0) break;
		}
	if (positive == false) {
		ch.ch = '-';
		err = pstreamOPtextc(pst, ch, &ans);
		if (err != ERR_ok) resignal(err);
		}
	for (k = j; k > -1 ; k--) {
		ch.ch = digits[k] + '0';
		err = pstreamOPtextc(pst, ch, &ans);
		if (err != ERR_ok) resignal(err);
		}
	signal(ERR_ok);
	}

errcode intOPencode(i, ist)
CLUREF i, ist;
{
errcode err;

	err = istreamOPputi(ist, i);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode intOPdecode(ist, ans)
CLUREF ist, *ans;
{
errcode err;

	err = istreamOPgeti(ist, ans);
	if (err == ERR_not_possible || err == ERR_end_of_file) signal(err);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode intOPfrom_to_by(from, to, by, proc, user_locals, iecode)
CLUREF from, to, by;
char *user_locals;
errcode (*proc)();
errcode *iecode;
{
errcode err;
CLUREF iv_1;

/*
struct {
 char **user_locals;
 errcode (*proc)();
 errcode err;
 errcode ecode2;
 CLUREF from;
 CLUREF to;
	} locals;

   locals.from = from;
   locals.to = to;
   locals.user_locals = user_locals;
   locals.proc = proc;
*/

   if (from.num > to.num && by.num > 0) signal(ERR_ok);
   if (from.num < to.num && by.num < 0) signal(ERR_ok);
   iv_1 = from;
   while (true) {
	err = (*proc)(iv_1, user_locals, iecode);
	if (err == ERR_ok) {
		iv_1.num += by.num;
		if (by.num >= 0 && iv_1.num <= to.num) continue;
		if (by.num >= 0 && iv_1.num > to.num) signal(ERR_ok);
		if (by.num < 0 && iv_1.num >= to.num) continue;
		if (by.num < 0 && iv_1.num < to.num) signal(ERR_ok);
		}
	if (err == ERR_iterbodyreturn) signal(err);
	/* 9/9/93 dwc: just propagate break, do not convert to ok */
	if (err == ERR_break) signal(ERR_break);
	if (err == ERR_iterbodyexit) signal(err);
	if (err == ERR_iterbodysignal) 	signal(err);
	if (err == ERR_iteriterbodysignal) signal(err);
	if (err == ERR_iteriterbodyexit) signal(err);
	if (err == ERR_iteriterbodyreturn) signal(err);
   }
}


errcode intOPfrom_to(from, to, iterbod, locals, ecode2)
CLUREF from, to;
char *locals;
errcode (*iterbod)();
errcode *ecode2;
{
errcode err;
CLUREF iv_1;

   if (from.num > to.num) signal(ERR_ok);
   iv_1 = from;
   while (true) {
	err = iterbod(iv_1, locals, ecode2);
	if (err == ERR_ok) {
		iv_1.num += 1;
		if (iv_1.num <= to.num) continue;
		if (iv_1.num > to.num) signal(ERR_ok);
		}
	if (err == ERR_iterbodyreturn) signal(err);
	/* 9/9/93 dwc: just propagate break, do not convert to ok */
	if (err == ERR_break) signal(ERR_break);
	if (err == ERR_iterbodyexit) signal(err);
	if (err == ERR_iterbodysignal) signal(err);
	if (err == ERR_iteriterbodysignal) signal(err);
	if (err == ERR_iteriterbodyexit) signal(err);
	if (err == ERR_iteriterbodyreturn) signal(err);
   }
}

errcode intOP_gcd(i, tab, ans)
CLUREF i, tab, *ans;
{
	ans->num = -1;
	signal(ERR_ok);
}

typedef struct {
long count;
OWNPTR	type_owns;
OWNPTR  op_owns;
struct OP_ENTRY entry[26];
} int_OPS;

OWN_ptr int_own_init = {1, 0};

CLU_proc int_oe_add = {{0,0,0,0}, intOPadd, &int_own_init,&int_own_init};
CLU_proc int_oe_sub = {{0,0,0,0}, intOPsub, &int_own_init,&int_own_init};
CLU_proc int_oe_mul = {{0,0,0,0}, intOPmul, &int_own_init,&int_own_init};
CLU_proc int_oe_minus = {{0,0,0,0}, intOPminus, &int_own_init,&int_own_init};
CLU_proc int_oe_div = {{0,0,0,0}, intOPdiv, &int_own_init,&int_own_init};
CLU_proc int_oe_power = {{0,0,0,0}, intOPpower, &int_own_init,&int_own_init};
CLU_proc int_oe_mod = {{0,0,0,0}, intOPmod, &int_own_init,&int_own_init};
CLU_proc int_oe_abs = {{0,0,0,0}, intOPabs, &int_own_init,&int_own_init};
CLU_proc int_oe_min = {{0,0,0,0}, intOPmin, &int_own_init,&int_own_init};
CLU_proc int_oe_max = {{0,0,0,0}, intOPmax, &int_own_init,&int_own_init};
CLU_proc int_oe_unparse = {{0,0,0,0}, intOPunparse, &int_own_init,&int_own_init};
CLU_proc int_oe_parse = {{0,0,0,0}, intOPparse, &int_own_init,&int_own_init};
CLU_proc int_oe_lt = {{0,0,0,0}, intOPlt, &int_own_init,&int_own_init};
CLU_proc int_oe_le = {{0,0,0,0}, intOPle, &int_own_init,&int_own_init};
CLU_proc int_oe_gt = {{0,0,0,0}, intOPgt, &int_own_init,&int_own_init};
CLU_proc int_oe_ge = {{0,0,0,0}, intOPge, &int_own_init,&int_own_init};
CLU_proc int_oe_equal = {{0,0,0,0}, intOPequal, &int_own_init,&int_own_init};
CLU_proc int_oe_similar = {{0,0,0,0}, intOPsimilar, &int_own_init,&int_own_init};
CLU_proc int_oe_copy = {{0,0,0,0}, intOPcopy, &int_own_init,&int_own_init};
CLU_proc int_oe_print = {{0,0,0,0}, intOPprint , &int_own_init,&int_own_init};
CLU_proc int_oe_encode = {{0,0,0,0}, intOPencode, &int_own_init,&int_own_init};
CLU_proc int_oe_decode = {{0,0,0,0}, intOPdecode, &int_own_init,&int_own_init};
CLU_proc int_oe_from_to = {{0,0,0,0}, intOPfrom_to, &int_own_init,&int_own_init};
CLU_proc int_oe_from_to_by = {{0,0,0,0}, intOPfrom_to_by, &int_own_init,&int_own_init};
CLU_proc int_oe__gcd = {{0,0,0,0}, intOP_gcd, &int_own_init,&int_own_init};
CLU_proc int_oe_debug_print = {{0,0,0,0}, intOPdebug_print , &int_own_init,&int_own_init};
int_OPS int_ops_actual = {26,
&int_own_init,
&int_own_init, {
{&int_oe_equal, "equal"},
{&int_oe_similar, "similar"},
{&int_oe_copy, "copy"},
{&int_oe_print, "print"},
{&int_oe_encode, "encode"},
{&int_oe_decode, "decode"},
{&int_oe__gcd, "_gcd"},
{&int_oe_debug_print, "debug_print"},
{&int_oe_add, "add"},
{&int_oe_sub, "sub"},
{&int_oe_mul, "mul"},
{&int_oe_minus, "minus"},
{&int_oe_div, "div"},
{&int_oe_power, "power"},
{&int_oe_mod, "mod"},
{&int_oe_abs, "abs"},
{&int_oe_min, "min"},
{&int_oe_max, "max"},
{&int_oe_parse, "parse"},
{&int_oe_unparse, "unparse"},
{&int_oe_lt, "lt"},
{&int_oe_le, "le"},
{&int_oe_gt, "gt"},
{&int_oe_ge, "ge"},
{&int_oe_from_to, "from_to"},
{&int_oe_from_to_by, "from_to_by"}}
};

int_OPS *int_ops = &int_ops_actual;
