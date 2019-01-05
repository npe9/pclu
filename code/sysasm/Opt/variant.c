
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/variant.c,v 1.6 91/08/29 14:55:03 dcurtis Exp $";
#endif
/* $Log:	variant.c,v $
 * Revision 1.6  91/08/29  14:55:03  dcurtis
 * fixed oneofOPnew call's arguments to conform (removed elist)
 * 
 * Revision 1.5  91/06/06  13:32:37  root
 * added copyright notice
 * 
 * Revision 1.4  91/06/05  13:27:19  root
 * added elist to sequence$new2
 * 
 * Revision 1.3  91/06/03  14:29:26  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.2  91/05/31  12:57:34  root
 * fixed aggregate initialization in ops_actual
 * fixed ints vs CLUREFS in copy & copy1
 * added start & length arguments to string$cons calls
 * added result values to pstream calls
 * 
 * Revision 1.1  91/02/04  15:49:58  mtv
 * Initial revision
 * 
 */

/*							*/
/*		VARIANT IMPLEMENTATION			*/
/*							*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode variantOPprint();

errcode variantOPnew(tag, val, ans)
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

errcode variantOPmake_1(val, ans)
CLUREF val, *ans;
{
CLUREF temp;

	clu_alloc(sizeof(CLU_cell), &temp);
	temp.cell->typ.val = CT_VAR;
	temp.cell->tag = 1;
	temp.cell->value = val.num;
	ans->cell = temp.cell;
	signal(ERR_ok);
	}


errcode variantOPmake_2(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_2, val, ans);
	resignal(err);
	}


errcode variantOPmake_3(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_3, val, ans);
	resignal(err);
	}


errcode variantOPmake_4(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_4, val, ans);
	resignal(err);
	}


errcode variantOPmake_5(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_5, val, ans);
	resignal(err);
	}


errcode variantOPmake_6(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_6, val, ans);
	resignal(err);
	}


errcode variantOPmake_7(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_7, val, ans);
	resignal(err);
	}


errcode variantOPmake_8(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_8, val, ans);
	resignal(err);
	}


errcode variantOPmake_9(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_9, val, ans);
	resignal(err);
	}


errcode variantOPmake_10(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_10, val, ans);
	resignal(err);
	}


errcode variantOPmake_11(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_11, val, ans);
	resignal(err);
	}


errcode variantOPmake_12(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_12, val, ans);
	resignal(err);
	}


errcode variantOPmake_13(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_13, val, ans);
	resignal(err);
	}


errcode variantOPmake_14(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_14, val, ans);
	resignal(err);
	}


errcode variantOPmake_15(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_15, val, ans);
	resignal(err);
	}


errcode variantOPmake_16(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_16, val, ans);
	resignal(err);
	}


errcode variantOPmake_17(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_17, val, ans);
	resignal(err);
	}


errcode variantOPmake_18(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_18, val, ans);
	resignal(err);
	}


errcode variantOPmake_19(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_19, val, ans);
	resignal(err);
	}


errcode variantOPmake_20(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_20, val, ans);
	resignal(err);
	}


errcode variantOPmake_21(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_21, val, ans);
	resignal(err);
	}


errcode variantOPmake_22(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_22, val, ans);
	resignal(err);
	}


errcode variantOPmake_23(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_23, val, ans);
	resignal(err);
	}


errcode variantOPmake_24(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_24, val, ans);
	resignal(err);
	}


errcode variantOPmake_25(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_25, val, ans);
	resignal(err);
	}


errcode variantOPmake_26(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_26, val, ans);
	resignal(err);
	}


errcode variantOPmake_27(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_27, val, ans);
	resignal(err);
	}


errcode variantOPmake_28(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_28, val, ans);
	resignal(err);
	}


errcode variantOPmake_29(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_29, val, ans);
	resignal(err);
	}


errcode variantOPmake_30(val, ans)
CLUREF val, *ans;
{
errcode err;
	err = variantOPnew(CLU_30, val, ans);
	resignal(err);
	}

errcode variantOPvalue_1(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 1) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_2(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 2) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_3(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 3) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_4(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 4) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_5(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 5) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_6(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 6) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_7(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 7) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_8(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 8) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_9(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 9) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_10(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 10) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_11(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 11) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_12(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 12) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_13(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 13) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_14(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 14) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_15(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 15) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_16(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 16) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_17(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 17) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_18(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 18) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_19(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 19) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_20(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 20) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_21(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 21) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_22(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 22) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_23(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 23) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_24(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 24) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_25(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 25) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_26(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 26) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_27(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 27) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_28(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 28) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_29(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 29) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}


errcode variantOPvalue_30(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 30) {
		ans->num = variant.cell->value;
		signal(ERR_ok);
		}
	else signal(ERR_wrong_tag);
	}

errcode variantOPchange(variant, tag, val)
CLUREF variant, tag, val;
{
	variant.cell->tag = tag.num;
	variant.cell->value = val.num;
	signal(ERR_ok);
	}

errcode variantOPchange_1(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_1, val);
	resignal(err);
	}


errcode variantOPchange_2(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_2, val);
	resignal(err);
	}


errcode variantOPchange_3(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_3, val);
	resignal(err);
	}


errcode variantOPchange_4(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_4, val);
	resignal(err);
	}


errcode variantOPchange_5(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_5, val);
	resignal(err);
	}


errcode variantOPchange_6(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_6, val);
	resignal(err);
	}


errcode variantOPchange_7(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_7, val);
	resignal(err);
	}


errcode variantOPchange_8(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_8, val);
	resignal(err);
	}


errcode variantOPchange_9(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_9, val);
	resignal(err);
	}


errcode variantOPchange_10(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_10, val);
	resignal(err);
	}


errcode variantOPchange_11(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_11, val);
	resignal(err);
	}


errcode variantOPchange_12(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_12, val);
	resignal(err);
	}


errcode variantOPchange_13(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_13, val);
	resignal(err);
	}


errcode variantOPchange_14(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_14, val);
	resignal(err);
	}


errcode variantOPchange_15(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_15, val);
	resignal(err);
	}


errcode variantOPchange_16(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_16, val);
	resignal(err);
	}


errcode variantOPchange_17(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_17, val);
	resignal(err);
	}


errcode variantOPchange_18(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_18, val);
	resignal(err);
	}


errcode variantOPchange_19(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_19, val);
	resignal(err);
	}


errcode variantOPchange_20(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_20, val);
	resignal(err);
	}


errcode variantOPchange_21(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_21, val);
	resignal(err);
	}


errcode variantOPchange_22(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_22, val);
	resignal(err);
	}


errcode variantOPchange_23(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_23, val);
	resignal(err);
	}


errcode variantOPchange_24(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_24, val);
	resignal(err);
	}


errcode variantOPchange_25(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_25, val);
	resignal(err);
	}


errcode variantOPchange_26(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_26, val);
	resignal(err);
	}


errcode variantOPchange_27(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_27, val);
	resignal(err);
	}


errcode variantOPchange_28(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_28, val);
	resignal(err);
	}


errcode variantOPchange_29(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_29, val);
	resignal(err);
	}


errcode variantOPchange_30(variant, val)
CLUREF variant, val;
{
errcode err;
	err = variantOPchange(variant, CLU_30, val);
	resignal(err);
	}

errcode variantOPis_1(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 1) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_2(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 2) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_3(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 3) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_4(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 4) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_5(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 5) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_6(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 6) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_7(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 7) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_8(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 8) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_9(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 9) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_10(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 10) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_11(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 11) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_12(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 12) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_13(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 13) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_14(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 14) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_15(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 15) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_16(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 16) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_17(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 17) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_18(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 18) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_19(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 19) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_20(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 20) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_21(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 21) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_22(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 22) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_23(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 23) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_24(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 24) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_25(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 25) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_26(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 26) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_27(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 27) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_28(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 28) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_29(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 29) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode variantOPis_30(variant, ans)
CLUREF variant, *ans;
{
	if (variant.cell->tag == 30) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode variantOPcopy(variant, ans)
CLUREF variant, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to copy fcns */
errcode err;
CLUREF tag, value, temp;

	tag.num = variant.cell->tag;
	value.num = variant.cell->value;
	err = variantOPnew(tag, value, &temp);
	if (err != ERR_ok) resignal(err); 
	CUR_PROC_VAR.proc = (CLUPROC)table[variant.cell->tag - 1];
	err = table[variant.cell->tag - 1]->proc(value, &temp.cell->value);
	if (err != ERR_ok) resignal(err);
	ans->cell = temp.cell;
	signal(ERR_ok);
	}

errcode variantOPsimilar(vnt1, vnt2, ans)
CLUREF vnt1, vnt2, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to similar fcns */
errcode err;
CLUREF value1, value2;

	if (vnt1.cell->tag != vnt2.cell->tag) {
			ans->tf = false;
			signal(ERR_ok);
			}
	value1.num = vnt1.cell->value;
	value2.num = vnt2.cell->value;
	CUR_PROC_VAR.proc = (CLUPROC)table[vnt1.cell->tag - 1];
	err = table[vnt1.cell->tag - 1]->proc(value1, value2, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode variantOP_gcd(vnt, tab, ans)
CLUREF vnt, tab, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to _gcd fcns */
errcode err;
long i;
CLUREF temp_oneof, temp_seq, sz;

	sz.num = vnt.cell->tag;
        sequenceOPnew2(sz, &temp_seq);
        for (i = 0; i < vnt.cell->tag; i++) {
                temp_seq.vec->data[i] = (long)table[i];
                }

	err = oneofOPnew(CLU_4, temp_seq, &temp_oneof);
	if (err != ERR_ok) resignal(err);
	sz.num = 2*CLUREFSZ + GCD_REF_SIZE;
	err = gcd_tabOPinsert(tab, sz, temp_oneof, vnt, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode variantOPequal(vnt1, vnt2, ans)
CLUREF vnt1, vnt2, *ans;
{
	if (vnt1.ref == vnt2.ref) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode variantOPdebug_print(vnt, pst)
CLUREF vnt, pst;
{
errcode err;
	err = variantOPprint(vnt, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

extern char**find_names();
errcode variantOPprint(vnt, pst)
CLUREF vnt, pst;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to print fcns */
char**table2; /* ptr to print fcns */
char *nm;
errcode err;
CLUREF temp_str, temp_st2, tag, value, size, ans;

	err = stringOPcons("<", CLU_1, CLU_1, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPstart(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) {
		err = pstreamOPstop(pst, temp_str, &ans);
		if (err != ERR_ok) resignal(err);
		signal(ERR_ok);
		}

#ifdef sparc
	tag.num = vnt.cell->tag;
	err = intOPprint(tag, pst);
	if (err != ERR_ok) resignal(err);
#else
	table2 = find_names(table); /* ptr to print fcns */
	nm = table2[vnt.cell->tag-1];
	size.num = strlen(nm);
	err = stringOPcons(nm, CLU_1, size, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPtext(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
#endif

	err = stringOPcons(": ", CLU_1, CLU_2, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPtext(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);

	value.num = vnt.cell->value;
	CUR_PROC_VAR.proc = (CLUPROC)table[vnt.cell->tag - 1];
	err = table[vnt.cell->tag - 1]->proc(value, pst);
	if (err != ERR_ok) resignal(err);

	err = stringOPcons(">", CLU_1, CLU_1, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPstop(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode variantOPencode(vnt, ist)
CLUREF vnt, ist;
{
errcode err;
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to encode fcns */
CLUREF tag, value;

	tag.num = vnt.cell->tag;
	value.num = vnt.cell->value;
	err = istreamOPputi(ist, tag);
	if (err != ERR_ok) resignal(ERR_ok);
	CUR_PROC_VAR.proc = (CLUPROC)table[vnt.cell->tag - 1];
	err = table[vnt.cell->tag - 1]->proc(value, ist);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode variantOPdecode(ist, ans)
CLUREF ist, *ans;
{
errcode err;
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to decode fcns */
CLUREF tag, value, temp;

	err = istreamOPgeti(ist, &tag);
	if (err != ERR_ok) resignal(ERR_ok);

	/* error checking on tag value */

	CUR_PROC_VAR.proc = (CLUPROC)table[tag.num - 1];
	err = table[tag.num - 1]->proc(ist, &value);
	if (err == ERR_not_possible) signal(err);
	if (err == ERR_end_of_file) signal(err);
	if (err != ERR_ok) resignal(err);
	err = variantOPnew(tag, value, &temp);
	if (err != ERR_ok) resignal(err);
	ans->cell = temp.cell;
	signal(ERR_ok);
	}

errcode variantOPv_gets_v(vnt1, vnt2)
CLUREF vnt1, vnt2;
{
	vnt1.cell->tag = vnt2.cell->tag;
	vnt1.cell->value = vnt2.cell->value;
	signal(ERR_ok);
	}

errcode variantOPv_gets_o(vnt, one)
CLUREF vnt, one;
{
	vnt.cell->tag = one.cell->tag;
	vnt.cell->value = one.cell->value;
	signal(ERR_ok);
	}


errcode variantOPcopy1(vnt, ans)
CLUREF vnt, *ans;
{
errcode err;
CLUREF tag, value, temp;

	tag.num = vnt.cell->tag;
	value.num = vnt.cell->value;
	err = variantOPnew(tag, value, &temp);
	if (err != ERR_ok) resignal(err);
	ans->cell = temp.cell;
	signal(ERR_ok);
	}

errcode variantOPsimilar1(vnt1, vnt2, ans)
CLUREF vnt1, vnt2, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to equal fcns */
errcode err;
CLUREF value1, value2;

	if (vnt1.cell->tag != vnt2.cell->tag) {
			ans->tf = false;
			signal(ERR_ok);
			}
	value1.num = vnt1.cell->value;
	value2.num = vnt2.cell->value;
	CUR_PROC_VAR.proc = (CLUPROC)table[vnt1.cell->tag - 1];
	err = table[vnt1.cell->tag - 1]->proc(value1, value2, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);

	}


typedef struct 
{
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
struct OP_ENTRY copy1;
struct OP_ENTRY similar1;
	} selector_TABLE;

OWN_ptr variant_own_init = {1, 0};

CLU_proc variant_oe_equal = {{0,0,0,0}, variantOPequal, &variant_own_init, &variant_own_init};
CLU_proc variant_oe_copy = {{0,0,0,0}, variantOPcopy, &variant_own_init, &variant_own_init};
CLU_proc variant_oe_similar = {{0,0,0,0}, variantOPsimilar, &variant_own_init, &variant_own_init};
CLU_proc variant_oe_print = {{0,0,0,0}, variantOPprint, &variant_own_init, &variant_own_init};
CLU_proc variant_oe_encode = {{0,0,0,0}, variantOPencode, &variant_own_init, &variant_own_init};
CLU_proc variant_oe_decode = {{0,0,0,0}, variantOPdecode, &variant_own_init, &variant_own_init};
CLU_proc variant_oe__gcd = {{0,0,0,0}, variantOP_gcd, &variant_own_init, &variant_own_init};
CLU_proc variant_oe_debug_print = {{0,0,0,0}, variantOPdebug_print, &variant_own_init, &variant_own_init};

selector_OPS variant_ops_actual = {8, 
    &variant_own_init,
    &variant_own_init, {
{&variant_oe_equal, "equal"},
{&variant_oe_copy, "copy"}, 
{&variant_oe_similar, "similar"},
{&variant_oe_print, "print"},
{&variant_oe_encode, "encode"},
{&variant_oe_decode, "decode"},
{&variant_oe__gcd, "_gcd"},
{&variant_oe_debug_print, "debug_print"}
}};

selector_OPS *variant_ops = &variant_ops_actual;

