
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/oneof.c,v 1.6 91/08/29 15:02:07 dcurtis Exp $";
#endif 
/* $Log:	oneof.c,v $
 * Revision 1.6  91/08/29  15:02:07  dcurtis
 * removed elist from oneofOPnew call
 * 
 * Revision 1.5  91/06/06  13:29:43  root
 * added copyright notice
 * 
 * Revision 1.4  91/06/05  13:25:42  root
 * added elist to sequence$new2
 * 
 * Revision 1.3  91/06/03  14:10:47  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.2  91/05/31  12:37:37  root
 * fixed aggregate initialization in ops_actual
 * added start & len to string$cons's
 * added result variable to pstream calls
 * 
 * Revision 1.1  91/02/04  15:49:48  mtv
 * Initial revision
 * 
 */

/*							*/
/*		ONEOF IMPLEMENTATION			*/
/*							*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode oneofOPprint();

errcode oneofOPnew(tag, val, ans)
CLUREF tag, val;
CLUREF *ans;
{
CLUREF temp;

	clu_alloc(sizeof(CLU_cell), &temp);
	temp.cell->typ.mark = 0;
	temp.cell->typ.val = CT_VAR;
	temp.cell->typ.refp = 0;
	temp.cell->typ.spare = 0;
	temp.cell->tag = tag.num;
	temp.cell->value = val.num;
	ans->cell = temp.cell;
	signal(ERR_ok);
	}

errcode oneofOPmake_1(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_1, val, ans);
	resignal(err);
	}

errcode oneofOPmake_2(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_2, val, ans);
	resignal(err);
	}


errcode oneofOPmake_3(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_3, val, ans);
	resignal(err);
	}


errcode oneofOPmake_4(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_4, val, ans);
	resignal(err);
	}


errcode oneofOPmake_5(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_5, val, ans);
	resignal(err);
	}


errcode oneofOPmake_6(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_6, val, ans);
	resignal(err);
	}


errcode oneofOPmake_7(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_7, val, ans);
	resignal(err);
	}


errcode oneofOPmake_8(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_8, val, ans);
	resignal(err);
	}


errcode oneofOPmake_9(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_9, val, ans);
	resignal(err);
	}


errcode oneofOPmake_10(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_10, val, ans);
	resignal(err);
	}


errcode oneofOPmake_11(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_11, val, ans);
	resignal(err);
	}


errcode oneofOPmake_12(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_12, val, ans);
	resignal(err);
	}


errcode oneofOPmake_13(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_13, val, ans);
	resignal(err);
	}


errcode oneofOPmake_14(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_14, val, ans);
	resignal(err);
	}


errcode oneofOPmake_15(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_15, val, ans);
	resignal(err);
	}


errcode oneofOPmake_16(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_16, val, ans);
	resignal(err);
	}


errcode oneofOPmake_17(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_17, val, ans);
	resignal(err);
	}


errcode oneofOPmake_18(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_18, val, ans);
	resignal(err);
	}


errcode oneofOPmake_19(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_19, val, ans);
	resignal(err);
	}


errcode oneofOPmake_20(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_20, val, ans);
	resignal(err);
	}


errcode oneofOPmake_21(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_21, val, ans);
	resignal(err);
	}


errcode oneofOPmake_22(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_22, val, ans);
	resignal(err);
	}


errcode oneofOPmake_23(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_23, val, ans);
	resignal(err);
	}


errcode oneofOPmake_24(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_24, val, ans);
	resignal(err);
	}


errcode oneofOPmake_25(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_25, val, ans);
	resignal(err);
	}


errcode oneofOPmake_26(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_26, val, ans);
	resignal(err);
	}


errcode oneofOPmake_27(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_27, val, ans);
	resignal(err);
	}


errcode oneofOPmake_28(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_28, val, ans);
	resignal(err);
	}


errcode oneofOPmake_29(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_29, val, ans);
	resignal(err);
	}


errcode oneofOPmake_30(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_30, val, ans);
	resignal(err);
	}

errcode oneofOPmake_31(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_31, val, ans);
	resignal(err);
	}

errcode oneofOPmake_32(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_32, val, ans);
	resignal(err);
	}

errcode oneofOPmake_33(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_33, val, ans);
	resignal(err);
	}

errcode oneofOPmake_34(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_34, val, ans);
	resignal(err);
	}

errcode oneofOPmake_35(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_35, val, ans);
	resignal(err);
	}

errcode oneofOPmake_36(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_36, val, ans);
	resignal(err);
	}

errcode oneofOPmake_37(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_37, val, ans);
	resignal(err);
	}

errcode oneofOPmake_38(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_38, val, ans);
	resignal(err);
	}

errcode oneofOPmake_39(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_39, val, ans);
	resignal(err);
	}

errcode oneofOPmake_40(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_40, val, ans);
	resignal(err);
	}


errcode oneofOPmake_41(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = oneofOPnew(CLU_41, val, ans);
	resignal(err);
	}

/* 
   added for compiler use only:
	context is tag x, y (v:vtype)
*/
errcode oneofOPvalue(oneof, ans)	
CLUREF oneof, *ans;
{
	ans->num = oneof.cell->value;
	signal(ERR_ok);
	}

errcode oneofOPvalue_1(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 1) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_2(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 2) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_3(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 3) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_4(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 4) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_5(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 5) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_6(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 6) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_7(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 7) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_8(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 8) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_9(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 9) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_10(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 10) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_11(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 11) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_12(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 12) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_13(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 13) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_14(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 14) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_15(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 15) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_16(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 16) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_17(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 17) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_18(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 18) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_19(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 19) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_20(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 20) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_21(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 21) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_22(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 22) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_23(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 23) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_24(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 24) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_25(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 25) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_26(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 26) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_27(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 27) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_28(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 28) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_29(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 29) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode oneofOPvalue_30(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 30) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_31(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 31) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_32(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 32) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_33(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 33) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_34(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 34) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_35(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 35) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_36(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 36) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_37(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 37) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_38(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 38) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_39(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 39) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_40(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 40) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPvalue_41(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 41) {
		ans->num = oneof.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode oneofOPis_1(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 1) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_2(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 2) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_3(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 3) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_4(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 4) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_5(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 5) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_6(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 6) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_7(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 7) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_8(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 8) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_9(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 9) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_10(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 10) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_11(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 11) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_12(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 12) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_13(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 13) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_14(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 14) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_15(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 15) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_16(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 16) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_17(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 17) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_18(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 18) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_19(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 19) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_20(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 20) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_21(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 21) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_22(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 22) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_23(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 23) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_24(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 24) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_25(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 25) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_26(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 26) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_27(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 27) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_28(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 28) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_29(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 29) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode oneofOPis_30(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 30) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_31(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 31) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_32(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 32) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_33(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 33) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_34(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 34) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_35(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 35) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_36(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 36) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_37(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 37) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_38(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 38) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_39(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 39) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_40(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 40) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPis_41(oneof, ans)
CLUREF oneof, *ans;
{
	if (oneof.cell->tag == 41) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode oneofOPcopy(one, ans)
CLUREF one, *ans;
{
errcode err;
CLUREF temp, tag, value;
long i;
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to copy fcns */

	tag.num = one.cell->tag;
	value.num = one.cell->value;
	err = oneofOPnew(tag, value, &temp);
	if (err != ERR_ok) resignal(err);
	CUR_PROC_VAR.proc = (CLUPROC)table[one.cell->tag - 1];
	err = table[one.cell->tag - 1]->proc(value, &temp.cell->value);
	if (err != ERR_ok) resignal(err);
	ans->cell = temp.cell;
	signal(ERR_ok);
	}

errcode oneofOPo2v(one, ans)
CLUREF one, *ans;
{
errcode err;
CLUREF temp;

	err = variantOPnew(one.cell->tag, one.cell->value, &temp);
	if (err != ERR_ok) resignal(err);
	ans->cell = temp.cell;
	signal(ERR_ok);
	}	

errcode oneofOPv2o(vnt, ans)
CLUREF vnt, *ans;
{
errcode err;
CLUREF temp;

	err = oneofOPnew(vnt.cell->tag, vnt.cell->value, &temp);
	if (err != ERR_ok) resignal(err);
	ans->cell = temp.cell;
	signal(ERR_ok);
	}

errcode oneofOPsimilar(one1, one2, ans)
CLUREF one1, one2, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to similar fcns */
long i;
errcode err;
CLUREF value1, value2;

	if (one1.cell->tag != one2.cell->tag) {
			ans->tf = false;
			signal(ERR_ok);
			}
	value1.num = one1.cell->value;
	value2.num = one2.cell->value;
	CUR_PROC_VAR.proc = (CLUPROC)table[one1.cell->tag - 1];
	err = table[one1.cell->tag - 1]->proc(value1, value2, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode oneofOP_gcd(one, tab, ans)
CLUREF one, tab, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to _gcd fcns */
errcode err;
long i;
CLUREF temp_oneof, temp_seq, sz, tag;

	tag.num = one.cell->tag;
        sequenceOPnew2(tag, &temp_seq);
        for (i = 0; i < one.cell->tag; i++) {
                temp_seq.vec->data[i] = (long)table[i];
                }
	err = oneofOPnew(CLU_4, temp_seq, &temp_oneof);
	if (err != ERR_ok) resignal(err);
	sz.num = 2*CLUREFSZ + GCD_REF_SIZE;
	err = gcd_tabOPinsert(tab, sz, temp_oneof, one, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode oneofOPencode(one, ist)
CLUREF one, ist;
{
errcode err;
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to encode fcns */
CLUREF temp;

	temp.num = one.cell->tag;
	err = istreamOPputi(ist, temp);
	if (err != ERR_ok) resignal(ERR_ok);
	temp.num = one.cell->value;
	CUR_PROC_VAR.proc = (CLUPROC)table[one.cell->tag - 1];
	err = table[one.cell->tag - 1]->proc(temp, ist);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode oneofOPdecode(ist, ans)
CLUREF ist, *ans;
{
errcode err;
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to decode fcns */
CLUREF tag, val, temp;

	err = istreamOPgeti(ist, &tag);
	if (err != ERR_ok) resignal(ERR_ok);

	/* error checking on tag value */

	CUR_PROC_VAR.proc = (CLUPROC)table[tag.num - 1];
	err = table[tag.num - 1]->proc(ist, &val);
	if (err == ERR_not_possible) signal(err);
	if (err == ERR_end_of_file) signal(err);
	if (err != ERR_ok) resignal(err);
	err = oneofOPnew(tag, val, &temp);
	if (err != ERR_ok) resignal(err);
	ans->cell = temp.cell;
	signal(ERR_ok);
	}

errcode oneofOPequal(one1, one2, ans)
CLUREF one1, one2, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to equal fcns */
errcode err;
CLUREF value1, value2;

	if (one1.cell->tag != one2.cell->tag) {
			ans->tf = false;
			signal(ERR_ok);
			}
	value1.num = one1.cell->value;
	value2.num = one2.cell->value;
	CUR_PROC_VAR.proc = (CLUPROC)table[one1.cell->tag - 1];
	err = table[one1.cell->tag - 1]->proc(value1, value2, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

char**find_names();

errcode oneofOPdebug_print(one, pst)
CLUREF one, pst;
{
errcode err;
	err = oneofOPprint(one, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode oneofOPprint(one, pst)
CLUREF one, pst;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to print fcns */
char **table2;
char *nm;
errcode err;
CLUREF temp_str, temp_st2, tag, value, size, ans;

	err = stringOPcons("<", CLU_1, CLU_1, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPstart(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;
#ifdef sparc
	tag.num = one.cell->tag;
	err = intOPprint(tag, pst);
	if (err != ERR_ok) resignal(err);
#else
	table2 = find_names(table);
	nm = table2[one.cell->tag-1];
	size.num = strlen(nm);
	stringOPcons(nm, CLU_1, size, &temp_str);
	err = pstreamOPtext(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
#endif

	err = stringOPcons(": ", CLU_1, CLU_2, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPtext(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);

	value.num = one.cell->value;
	CUR_PROC_VAR.proc = (CLUPROC)table[one.cell->tag - 1];
	err = table[one.cell->tag - 1]->proc(value, pst);
	if (err != ERR_ok) resignal(err);
done:
	err = stringOPcons(">", CLU_1, CLU_1, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPstop(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);

}

char **find_names(table)
CLUSELPROC *table;
{
long i, *j;

	for (i = 0; i < 41; i++) {
		j = (long*)table[i];
		if (j == 0) continue;
		if (*j == 0) continue;
		if ((table[i]->typ.val == CT_PROC) 
			&& (table[i]->typ.mark == 0) 
			&& (table[i]->typ.refp == 0) 
			&& (table[i]->typ.spare == 0)) continue;
		return ((char**)&table[i]);
		}
	}

typedef struct {
PROC *fcn;
char *name;
} SEL_ENTRY;

typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[8];
} selector_OPS;

typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY equal;
struct OP_ENTRY copy;
struct OP_ENTRY similar;
struct OP_ENTRY print;
struct OP_ENTRY encode;
struct OP_ENTRY decode;
struct OP_ENTRY _gcd;
struct OP_ENTRY debug_print;
} selector_TABLE;


OWN_ptr oneof_own_init = {0, 0};

CLU_proc oneof_oe_equal = {{0,0,0,0}, oneofOPequal, &oneof_own_init, &oneof_own_init};
CLU_proc oneof_oe_copy = {{0,0,0,0}, oneofOPcopy, &oneof_own_init, &oneof_own_init};
CLU_proc oneof_oe_similar = {{0,0,0,0}, oneofOPsimilar, &oneof_own_init, &oneof_own_init};
CLU_proc oneof_oe_print = {{0,0,0,0}, oneofOPprint, &oneof_own_init, &oneof_own_init};
CLU_proc oneof_oe_encode = {{0,0,0,0}, oneofOPencode, &oneof_own_init, &oneof_own_init};
CLU_proc oneof_oe_decode = {{0,0,0,0}, oneofOPdecode, &oneof_own_init, &oneof_own_init};
CLU_proc oneof_oe__gcd = {{0,0,0,0}, oneofOP_gcd, &oneof_own_init, &oneof_own_init};
CLU_proc oneof_oe_debug_print = {{0,0,0,0}, oneofOPdebug_print, &oneof_own_init, &oneof_own_init};

selector_OPS oneof_ops_actual = {8, 
    &oneof_own_init,
    &oneof_own_init, {
{&oneof_oe_equal, "equal"},
{&oneof_oe_copy, "copy"}, 
{&oneof_oe_similar, "similar"},
{&oneof_oe_print, "print"},
{&oneof_oe_encode, "encode"},
{&oneof_oe_decode, "decode"},
{&oneof_oe__gcd, "_gcd"},
{&oneof_oe_print, "debug_print"}
}};

selector_OPS *oneof_ops = &oneof_ops_actual;

