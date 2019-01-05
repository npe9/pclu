
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: null.c,v 1.4 91/07/09 13:11:21 root Exp $";
#endif 
/* $Log:	null.c,v $
 * Revision 1.4  91/07/09  13:11:21  root
 * changed args to stringOPcons to CLUREFs (sun compatibility)
 * 
 * Revision 1.3  91/06/06  13:29:28  root
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  12:23:03  root
 * fixed aggregate initialization in ops_actual
 * removed unused variables
 * fixed printing: consed CLU_string, rather than using C string
 * 
 * Revision 1.1  91/02/04  15:49:48  mtv
 * Initial revision
 * 
 */

/*								*/
/*		NULL IMPLEMENTATION				*/
/*								*/


#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode nullOPprint();

errcode nullOPequal(nn1, nn2, ans)
CLUREF nn1, nn2;
CLUREF *ans;
{
	ans->tf = true;
	signal(ERR_ok);
	}

errcode nullOPsimilar(nn1, nn2, ans)
CLUREF nn1, nn2;
CLUREF *ans;
{
	ans->tf = true;	
	signal(ERR_ok);
	}

errcode nullOPcopy(n, ans)
CLUREF n;
CLUREF *ans;
{
	ans->num = n.num;
	signal(ERR_ok);
	}

errcode nullOPdebug_print(n, pst)
CLUREF n, pst;
{
errcode err;
	err = nullOPprint(n, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}


errcode nullOPprint(n, pst)
CLUREF n, pst;
{
errcode err;
CLUREF temp_str, ans;

	err = stringOPcons("nil", CLU_1, CLU_3, &temp_str);
	err = pstreamOPtext(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);	
	}

errcode nullOPencode(n, ist)
CLUREF n, ist;
{
errcode err;

	err = istreamOPputi(ist, n);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode nullOPdecode(ist, ans)
CLUREF ist, *ans;
{
errcode err;
CLUREF temp;

	err = istreamOPgeti(ist, &temp);
	if (err == ERR_end_of_file) signal(err);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	if (temp.num != 0) {
		elist[0].str = bad_format_STRING;
		signal(ERR_not_possible);
		}
	ans->num = 0;
	signal(ERR_ok);
	}

errcode nullOP_gcd(s, tab, ans)
CLUREF s, tab, *ans;
{
	ans->num = -1;
	signal(ERR_ok);
	}


typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[8];
} null_OPS;

OWN_ptr null_own_init = {1, 0};

CLU_proc null_oe_copy = {{0,0,0,0}, nullOPcopy, &null_own_init, &null_own_init};
CLU_proc null_oe_equal = {{0,0,0,0}, nullOPequal, &null_own_init, &null_own_init};
CLU_proc null_oe_similar = {{0,0,0,0}, nullOPsimilar, &null_own_init, &null_own_init};
CLU_proc null_oe_print = {{0,0,0,0}, nullOPprint, &null_own_init, &null_own_init};
CLU_proc null_oe_encode = {{0,0,0,0}, nullOPencode, &null_own_init, &null_own_init};
CLU_proc null_oe_decode = {{0,0,0,0}, nullOPdecode, &null_own_init, &null_own_init};
CLU_proc null_oe__gcd = {{0,0,0,0}, nullOP_gcd, &null_own_init, &null_own_init};
CLU_proc null_oe_debug_print = {{0,0,0,0}, nullOPdebug_print, &null_own_init, &null_own_init};

null_OPS null_ops_actual = {8,
    &null_own_init,
    &null_own_init, {
{&null_oe_copy, "copy"},
{&null_oe_equal, "equal"},
{&null_oe_similar, "similar"},
{&null_oe_print, "print"},
{&null_oe_encode, "encode"},
{&null_oe_decode, "decode"},
{&null_oe__gcd, "_gcd"},
{&null_oe_debug_print, "debug_print"}
}};

null_OPS *null_ops = &null_ops_actual;
