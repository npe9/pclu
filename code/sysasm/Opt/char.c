
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: char.c,v 1.4 91/06/06 13:26:54 root Exp $";
#endif 
/* $Log:	char.c,v $
 * Revision 1.4  91/06/06  13:26:54  root
 * added copyright notice
 * 
 * Revision 1.3  91/05/31  12:13:13  root
 * fixed aggregate initialization in ops_actual
 * added missing signal(ERR_ok) to _gcd
 * added result variables to pstream calls
 * removed unused variable
 * corrected char value fetch in print
 * 
 * Revision 1.2  91/02/11  11:24:33  dcurtis
 * char$c2i: add "& 0xff" so that result would never be negative
 * 
 * Revision 1.1  91/02/04  15:49:40  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			char			*/
/*						*/


#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode charOPprint();

errcode charOPi2c(i, ans)
CLUREF i;
CLUREF *ans;
{

	if (i.num > 255 || i.num < 0) signal(ERR_illegal_char); 
	ans->ch = i.num;
	signal(ERR_ok);
	}

errcode charOPc2i(c, ans)
CLUREF c;
CLUREF *ans;
{
long temp;

	temp = (c.ch) & 0xff;
	ans->num = temp;
	signal(ERR_ok);
}

errcode charOPlt(c1, c2, ans)
CLUREF c1;
CLUREF c2;
CLUREF *ans;
{
	if ((unsigned char)c1.ch < (unsigned char)c2.ch) {ans->tf = true; signal(ERR_ok);}
	ans->tf = false; signal(ERR_ok);
	}

errcode charOPle(c1, c2, ans)
CLUREF c1;
CLUREF c2;
CLUREF *ans;
{
	if ((unsigned char)c1.ch <= (unsigned char)c2.ch) {ans->tf = true; signal(ERR_ok);}
	ans->tf = false; signal(ERR_ok);
	}


errcode charOPge(c1, c2, ans)
CLUREF c1;
CLUREF c2;
CLUREF *ans;
{
	if ((unsigned char) c1.ch >= (unsigned char)c2.ch) {ans->tf = true; signal(ERR_ok);}
	ans->tf = false; signal(ERR_ok);
	}

errcode charOPgt(c1, c2, ans)
CLUREF c1;
CLUREF c2;
CLUREF *ans;
{
	if ((unsigned char)c1.ch > (unsigned char)c2.ch) {ans->tf = true; signal(ERR_ok);}
	ans->tf = false; signal(ERR_ok);
	}

errcode charOPequal(c1, c2, ans)
CLUREF c1;
CLUREF c2;
CLUREF *ans;
{
	if ((unsigned char)c1.ch == (unsigned char)c2.ch) {ans->tf = true; signal(ERR_ok);}
	ans->tf = false; signal(ERR_ok);
	}

errcode charOPsimilar(c1, c2, ans)
CLUREF c1;
CLUREF c2;
CLUREF *ans;
{
	if ((unsigned char)c1.ch == (unsigned char)c2.ch) {ans->tf = true; signal(ERR_ok);}
	ans->tf = false; signal(ERR_ok);
	}

errcode charOPcopy(c, ans)
CLUREF c;
CLUREF *ans;
{
	ans->num = c.ch; signal(ERR_ok);
	}

errcode charOPdebug_print(c, pst)
CLUREF c, pst;
{
errcode err;
	err = charOPprint(c, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode charOPprint(c, pst)
CLUREF c, pst;
{
CLUREF temp;
errcode err;
CLUREF temp_str, temp_ref;
static bool init = false;
static CLUREF t1, t2, t3, t4, t5, ans;
bool done = false;

	if (init == 0) {
		err = stringOPcons("'\\^", CLU_1, CLU_3, &t1);
		err = stringOPcons("'\\!", CLU_1, CLU_3, &t2);
		err = stringOPcons("'\\",  CLU_1, CLU_2, &t3);
		err = stringOPcons("'\\&", CLU_1, CLU_3, &t4);
		err = stringOPcons("'",    CLU_1, CLU_1, &t5);
		init = true;
		}
	temp.ch = c.ch & 0x7f;
	if (temp.ch == '\177') {
		temp_str = t1;
		if (c.ch != temp.ch) temp_str = t2;
		temp.ch = '?';
		done = true;
		}
	if (!done && (temp.ch == '\'' || temp.ch == '\\')) {
		temp_str = t3;
		if (temp.ch != c.ch) temp_str = t4;
		done = true;
		}
	if (!done && (' ' <= temp.ch)) {
		temp_str = t5;
		if (temp.ch != c.ch) temp_str = t4;
		done = true;
		}
	if (!done && (temp.ch != c.ch)) {
		temp_str = t2;
		temp.ch += 64;
		done = true;
		}
	if (!done && (temp.ch == '\n')) {
		temp_str = t3;
		temp.ch = 'n';
		done = true;
		}
	if (!done && (temp.ch == '\t')) {
		temp_str = t3;
		temp.ch = 't';
		done = true;
		}
	if (!done && (temp.ch == '\f')) {
		temp_str = t3;
		temp.ch = 'p';
		done = true;
		}
	if (!done && (temp.ch == '\b')) {
		temp_str = t3;
		temp.ch = 'b';
		done = true;
		}
	if (!done && (temp.ch == '\r')) {
		temp_str = t3;
		temp.ch = 'r';
		done = true;
		}
	if (!done && (temp.ch == '\v')) {
		temp_str = t3;
		temp.ch = 'v';
		done = true;
		}
	if (!done) {
		temp_str = t1;
		temp.ch += 64;
		}
        err = pstreamOPtext(pst, temp_str, &ans);
        err = pstreamOPtextc(pst, temp, &ans);
        temp_ref.ch = '\'';
        err = pstreamOPtextc(pst, temp_ref, &ans);
	signal(ERR_ok);
	}

errcode charOPencode(c, ist)
CLUREF c, ist;
{
errcode err;

        err = istreamOPputi(ist, c);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
        signal(ERR_ok);
	}

errcode charOPdecode(ist, ans)
CLUREF ist, *ans;
{
errcode err;
CLUREF temp;

        err = istreamOPgeti(ist, &temp);
        if (err == ERR_not_possible || err == ERR_end_of_file) signal(err);
        if (err != ERR_ok) resignal(err);
        if (temp.num < 256) {
                ans->num = temp.num;
                signal(ERR_ok);
                }
        else {
                elist[0].str = bad_format_STRING;
                signal(ERR_not_possible);
                }
	}

errcode charOP_gcd(c, tab, ans)
CLUREF c;
CLUREF tab;
CLUREF *ans;
{
	ans->num = -1;
	signal(ERR_ok);
	}

typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[14];
} char_OPS;

OWN_ptr char_own_init = {1, 0};

CLU_proc char_oe_i2c = {{0,0,0,0}, charOPi2c, &char_own_init, &char_own_init};
CLU_proc char_oe_c2i = {{0,0,0,0}, charOPc2i, &char_own_init, &char_own_init};
CLU_proc char_oe_lt = {{0,0,0,0}, charOPlt, &char_own_init, &char_own_init};
CLU_proc char_oe_le = {{0,0,0,0}, charOPle, &char_own_init, &char_own_init};
CLU_proc char_oe_ge = {{0,0,0,0}, charOPge, &char_own_init, &char_own_init};
CLU_proc char_oe_gt = {{0,0,0,0}, charOPgt, &char_own_init, &char_own_init};
CLU_proc char_oe_equal = {{0,0,0,0}, charOPequal, &char_own_init, &char_own_init};
CLU_proc char_oe_similar = {{0,0,0,0}, charOPsimilar, &char_own_init, &char_own_init};
CLU_proc char_oe_copy = {{0,0,0,0}, charOPcopy, &char_own_init, &char_own_init};
CLU_proc char_oe_print = {{0,0,0,0}, charOPprint, &char_own_init, &char_own_init};
CLU_proc char_oe_encode = {{0,0,0,0}, charOPencode, &char_own_init, &char_own_init};
CLU_proc char_oe_decode = {{0,0,0,0}, charOPdecode, &char_own_init, &char_own_init};
CLU_proc char_oe__gcd = {{0,0,0,0}, charOP_gcd, &char_own_init, &char_own_init};
CLU_proc char_oe_debug_print = {{0,0,0,0}, charOPdebug_print, &char_own_init, &char_own_init};

char_OPS char_ops_actual = {14,
    &char_own_init,
    &char_own_init, {
{&char_oe_equal, "equal"},
{&char_oe_similar, "similar"},
{&char_oe_copy, "copy"},
{&char_oe_print, "print"},
{&char_oe_encode, "encode"},
{&char_oe_decode, "decode"},
{&char_oe__gcd, "_gcd"},
{&char_oe_debug_print, "debug_print"},
{&char_oe_i2c, "i2c"},
{&char_oe_c2i, "c2i"},
{&char_oe_lt, "lt"},
{&char_oe_le, "le"},
{&char_oe_ge, "ge"},
{&char_oe_gt, "gt"}}
};

char_OPS *char_ops = &char_ops_actual;
