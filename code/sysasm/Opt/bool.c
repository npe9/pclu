
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: bool.c,v 1.3 91/06/06 13:26:43 root Exp $";
#endif
/* $Log:	bool.c,v $
 * Revision 1.3  91/06/06  13:26:43  root
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  12:10:03  root
 * fixed aggregate initialization in ops_actual
 * fixed usage of true_STRING vs false_STRING
 * added result variable to pstream calls
 * 
 * Revision 1.1  91/02/04  15:49:40  mtv
 * Initial revision
 * 
 */


#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode boolOPprint();

errcode boolOPand(b1, b2, ans)
CLUREF b1, b2;
CLUREF *ans;
{
	ans->tf = b1.tf && b2.tf;
	signal(ERR_ok);
	}

errcode boolOPor(b1, b2, ans)
CLUREF b1,b2;
CLUREF *ans;
{
	ans->tf = b1.tf || b2.tf;
	signal(ERR_ok);
	}

errcode boolOPnot(b1, ans)
CLUREF b1;
CLUREF *ans;
{
	ans->tf = ~b1.tf & 1;
	signal(ERR_ok);
	}


errcode boolOPequal(b1, b2, ans)
CLUREF b1, b2;
CLUREF *ans;
{
	ans->tf = ((b1.tf & 1) == (b2.tf & 1));
	signal(ERR_ok);
	}

errcode boolOPsimilar(b1, b2, ans)
CLUREF b1, b2;
CLUREF *ans;
{
	ans->tf = (b1.tf & 1) == (b2.tf & 1);
	signal(ERR_ok);
	}

errcode boolOPcopy(b1, ans)
CLUREF b1;
CLUREF *ans;
{
	ans->tf = b1.tf;
	signal(ERR_ok);
	}

errcode boolOPdebug_print(b, pst)
CLUREF b, pst;
{
errcode err;
	err = boolOPprint(b, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode err;
errcode boolOPprint(b, pst)
CLUREF b, pst;
{
errcode err;
static bool init = false;
static CLUREF true_STRING, false_STRING;
CLUREF ans;

	if (!init) {
		err = stringOPcons("true", CLU_1, CLU_4, &true_STRING);
		err = stringOPcons("false", CLU_1, CLU_5, &false_STRING);
		init = true;
		}

	if (b.tf == true)
		err = pstreamOPtext(pst, true_STRING, &ans);
	else
		err = pstreamOPtext(pst, false_STRING, &ans);
	signal(ERR_ok);
	}

errcode boolOPencode(b, ist)
CLUREF b, ist;
{
errcode err;

        err = istreamOPputi(ist, b);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
        signal(ERR_ok);
	}

errcode boolOPdecode(ist, ans)
CLUREF ist, *ans;
{
errcode err;
CLUREF temp;

        err = istreamOPgeti(ist, &temp);
        if (err == ERR_not_possible || err == ERR_end_of_file) signal(err);
        if (err != ERR_ok) resignal(err);
	if (temp.num == 0 || temp.num == 1) {
		ans->tf = temp.tf;
        	signal(ERR_ok);
		}
	else {
		elist[0].str = bad_format_STRING;
		signal(ERR_not_possible);
		}
	}

errcode boolOP_gcd(b, tab, ans)
CLUREF b, tab;
CLUREF *ans;
{
	ans->num = -1;
	signal(ERR_ok);
	}

typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[11];
} bool_OPS;

OWN_ptr bool_own_init = {1, 0};

CLU_proc bool_oe_and = {{0,0,0,0}, boolOPand, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_or = {{0,0,0,0}, boolOPor, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_not = {{0,0,0,0}, boolOPnot, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_equal = {{0,0,0,0}, boolOPequal, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_similar = {{0,0,0,0}, boolOPsimilar, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_copy = {{0,0,0,0}, boolOPcopy, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_print = {{0,0,0,0}, boolOPprint, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_encode = {{0,0,0,0}, boolOPencode, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_decode = {{0,0,0,0}, boolOPdecode, &bool_own_init, &bool_own_init};
CLU_proc bool_oe__gcd = {{0,0,0,0}, boolOP_gcd, &bool_own_init, &bool_own_init};
CLU_proc bool_oe_debug_print = {{0,0,0,0}, boolOPdebug_print, &bool_own_init, &bool_own_init};

bool_OPS bool_ops_actual = {11,
    &bool_own_init,
    &bool_own_init, {
{&bool_oe_equal, "equal"},
{&bool_oe_similar, "similar"},
{&bool_oe_copy, "copy"},
{&bool_oe_print, "print"},
{&bool_oe_encode, "encode"},
{&bool_oe_decode, "decode"},
{&bool_oe__gcd, "_gcd"},
{&bool_oe_debug_print, "debug_print"},
{&bool_oe_and, "and"},
{&bool_oe_or, "or"},
{&bool_oe_not, "not"}}
};

bool_OPS *bool_ops = &bool_ops_actual;
