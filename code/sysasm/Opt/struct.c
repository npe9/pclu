
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/struct.c,v 1.8 91/08/29 16:05:02 dcurtis Exp $";
#endif
/* $Log:	struct.c,v $
 * Revision 1.8  91/08/29  16:05:02  dcurtis
 * fixed casting on arguments to bcopy
 * 
 * Revision 1.7  91/08/29  14:58:41  dcurtis
 * removed elist from oneofOPnew call
 * 
 * Revision 1.6  91/07/25  08:34:52  root
 * result should be a *pointer* to a CLUREF in _gcd
 * 	necessary for sparc: lp/freeze,thaw
 * 
 * Revision 1.5  91/06/06  13:31:31  root
 * added copyright notice
 * 
 * Revision 1.4  91/06/05  13:26:37  root
 * added elist to sequence$new2
 * 
 * Revision 1.3  91/06/03  15:04:33  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.2  91/05/31  13:00:54  root
 * fixed aggregate initialization in ops_actual
 * fixed 2 instances of int vs CLUREF
 * added result argument to pstream calls
 * added start & length arguments to string$cons calls
 * 
 * Revision 1.1  91/02/04  15:49:54  mtv
 * Initial revision
 * 
 */

/*							*/
/*		STRUCT IMPLEMENTATION			*/
/*							*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode structOPblit();
extern errcode structOPprint();

errcode structOPnew(size, ans)
CLUREF size;
CLUREF *ans;
{
CLUREF temp;

	clu_alloc(size.num*CLUREFSZ + sizeof(CLU_sequence) - CLUREFSZ, &temp);
	temp.vec->typ.mark = 0;
	temp.vec->typ.val = CT_REC;
	temp.vec->typ.refp = 0;
	temp.vec->typ.spare = 0;
	temp.vec->size = size.num;
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode structOPget_1(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[0];
	signal(ERR_ok);
	}


errcode structOPget_2(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[1];
	signal(ERR_ok);
	}


errcode structOPget_3(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[2];
	signal(ERR_ok);
	}


errcode structOPget_4(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[3];
	signal(ERR_ok);
	}


errcode structOPget_5(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[4];
	signal(ERR_ok);
	}


errcode structOPget_6(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[5];
	signal(ERR_ok);
	}


errcode structOPget_7(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[6];
	signal(ERR_ok);
	}


errcode structOPget_8(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[7];
	signal(ERR_ok);
	}


errcode structOPget_9(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[8];
	signal(ERR_ok);
	}


errcode structOPget_10(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[9];
	signal(ERR_ok);
	}


errcode structOPget_11(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[10];
	signal(ERR_ok);
	}


errcode structOPget_12(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[11];
	signal(ERR_ok);
	}


errcode structOPget_13(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[12];
	signal(ERR_ok);
	}


errcode structOPget_14(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[13];
	signal(ERR_ok);
	}


errcode structOPget_15(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[14];
	signal(ERR_ok);
	}


errcode structOPget_16(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[15];
	signal(ERR_ok);
	}


errcode structOPget_17(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[16];
	signal(ERR_ok);
	}


errcode structOPget_18(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[17];
	signal(ERR_ok);
	}


errcode structOPget_19(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[18];
	signal(ERR_ok);
	}


errcode structOPget_20(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[19];
	signal(ERR_ok);
	}


errcode structOPget_21(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[20];
	signal(ERR_ok);
	}


errcode structOPget_22(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[21];
	signal(ERR_ok);
	}


errcode structOPget_23(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[22];
	signal(ERR_ok);
	}


errcode structOPget_24(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[23];
	signal(ERR_ok);
	}


errcode structOPget_25(strct, ans)
CLUREF strct, *ans;
{
	ans->num = strct.vec->data[24];
	signal(ERR_ok);
	}

errcode structOPreplace_1(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[0] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_2(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[1] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_3(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[2] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_4(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[3] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_5(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[4] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_6(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[5] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_7(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[6] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_8(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[7] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_9(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[8] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_10(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[9] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_11(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[10] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_12(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[11] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_13(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[12] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_14(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[13] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_15(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[14] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_16(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[15] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_17(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[16] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_18(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[17] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_19(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[18] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_20(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[19] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_21(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[20] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_22(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[21] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_23(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[22] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_24(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[23] = val.num;
	signal(ERR_ok);
	}


errcode structOPreplace_25(strct, val, ans)
CLUREF strct, val, *ans;
{
CLUREF temp;
errcode err;
	err = structOPblit(strct, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	ans->vec->data[24] = val.num;
	signal(ERR_ok);
	}

errcode structOPblit(strct, ans)
CLUREF strct, *ans;
{
CLUREF temp;

	clu_alloc(((strct.vec->size - 1)*sizeof(CLUREF))
			 + sizeof(CLU_sequence), &temp);
	temp.vec->typ.mark = 0;
	temp.vec->typ.val = CT_REC;
	temp.vec->typ.refp = 0;
	temp.vec->typ.spare = 0;
	temp.vec->size = strct.vec->size;
	bcopy((char*)strct.vec->data, (char*)temp.vec->data, strct.vec->size*CLUREFSZ);
	ans->vec = temp.vec;

	signal(ERR_ok);
	}

errcode structOPcopy(strct, ans)
CLUREF strct, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to copy fcns */
errcode err;
CLUREF temp, elt;
long i;

	clu_alloc(((strct.vec->size - 1) * sizeof(CLUREF))
		 + sizeof(CLU_sequence), &temp);
	temp.vec->typ.mark = 0;
	temp.vec->typ.val = CT_REC;
	temp.vec->typ.refp = 0;
	temp.vec->typ.spare = 0;
	temp.vec->size = strct.vec->size;
	for (i = 0; i < strct.vec->size; i++) {
		elt.num = strct.vec->data[i];
		CUR_PROC_VAR.proc = (CLUPROC)table[i];
		err = table[i]->proc(elt, &temp.vec->data[i]);
		if (err != ERR_ok) resignal(err);
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode structOPsimilar(rec1, rec2, ans)
CLUREF rec1, rec2, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to similar fcns */
long i;
errcode err;
CLUREF elt1, elt2;

	if (rec1.vec->size != rec2.vec->size) {
			ans->tf = false;
			signal(ERR_ok);
			}
	for (i = 0; i < rec1.vec->size; i++) {
		elt1.num = rec1.vec->data[i];
		elt2.num = rec2.vec->data[i];
		CUR_PROC_VAR.proc = (CLUPROC)table[i];
		err = table[i]->proc(elt1, elt2, ans);
			if (ans->tf == false) signal(ERR_ok);
			}
	signal(ERR_ok);
	}

errcode structOPequal(str1, str2, ans)
CLUREF str1, str2, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to equal fcns */
long i;
errcode err;
CLUREF elt1, elt2;

	if (str1.vec->size != str2.vec->size) {
			ans->tf = false;
			signal(ERR_ok);
			}
	for (i = 0; i < str1.vec->size; i++) {
		elt1.num = str1.vec->data[i];
		elt2.num = str2.vec->data[i];
		CUR_PROC_VAR.proc = (CLUPROC)table[i];
		err = table[i]->proc(elt1, elt2, ans);
			if (ans->tf == false) signal(ERR_ok);
			}
	signal(ERR_ok);
	}

errcode structOP_gcd(str, tab, ans)
CLUREF str, tab, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to _gcd fcns */
errcode err;
long i;
CLUREF temp_oneof, temp_seq, sz;

	sz.num = str.vec->size;
        sequenceOPnew2(sz, &temp_seq);
        for (i = 0; i < str.vec->size; i++) {
                temp_seq.vec->data[i] = (long)table[i];
                }

	err = oneofOPnew(CLU_3, temp_seq, &temp_oneof);
	if (err != ERR_ok) resignal(err);
	sz.num = 2*CLUREFSZ + (str.vec->size * GCD_REF_SIZE);
	err = gcd_tabOPinsert(tab, sz, temp_oneof, str, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode structOPs2r(str, ans)
CLUREF str, *ans;
{
errcode err;
CLUREF temp, sz;

	sz.num = str.vec->size;
	err = recordOPnew(sz, &temp);
	if (err != ERR_ok) resignal(err);
	bcopy((char*)str.vec->data, (char*)temp.vec->data, str.vec->size*CLUREFSZ);
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode structOPr2s(rec, ans)
CLUREF rec, *ans;
{
errcode err;
CLUREF temp, sz;

	sz.num = rec.vec->size;
	err = structOPnew(sz, &temp);
	if (err != ERR_ok) resignal(err);
	bcopy((char*)rec.vec->data, (char*)temp.vec->data, rec.vec->size*CLUREFSZ);
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode structOPdebug_print(str, pst)
CLUREF str, pst;
{
errcode err;
	err = structOPprint(str, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode structOPprint(str, pst)
CLUREF str, pst;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to print fcns */
char**table2 = (char**)(table + str.vec->size); /* ptr to field names */
long i;
errcode err;
CLUREF temp_str, temp_str2, temp_str3, ref, size, ans;

	err = stringOPcons("{", CLU_1, CLU_1, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPstart(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;
	err = stringOPcons(",", CLU_1, CLU_1, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = stringOPcons(": ", CLU_1, CLU_2, &temp_str2);
	if (err != ERR_ok) resignal(err);
	for (i = 0; i < str.vec->size; i++) {
		if (i != 0) {
			err = pstreamOPpause(pst, temp_str, &ans);
			if (err != ERR_ok) resignal(err);
			if (ans.tf == false) break;
			}
		size.num = strlen(table2[i]);
		stringOPcons(table2[i], CLU_1, size, &temp_str3);
		err = pstreamOPtext(pst, temp_str3, &ans);
		if (err != ERR_ok) resignal(err);
		err = pstreamOPtext(pst, temp_str2, &ans);
		if (err != ERR_ok) resignal(err);
		ref.num = str.vec->data[i];
		CUR_PROC_VAR.proc = (CLUPROC)table[i];
		err = table[i]->proc(ref, pst);
		if (err != ERR_ok) resignal(err);
		}
done:
	err = stringOPcons("}", CLU_1, CLU_1, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPstop(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode structOPencode(str, ist)
CLUREF str, ist;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to encode fcns */
long i;
errcode err;
CLUREF ref;

        err = istreamOPputi(ist, str.vec->size);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
	for (i = 0; i < str.vec->size; i++) {
		ref.num = str.vec->data[i];
		CUR_PROC_VAR.proc = (CLUPROC)table[i];
		err = table[i]->proc(ref, ist);
		if (err == ERR_not_possible) signal(err);
		if (err != ERR_ok) resignal(err);
		}
	signal(ERR_ok);
	}

errcode structOPdecode(ist, ans)
CLUREF ist, *ans;
{
CLUSELPROC *table = (CLUSELPROC*)CUR_PROC_VAR.proc->op_owns->info; /* ptr to decode fcns */
long i;
errcode err;
CLUREF temp, size;

        err = istreamOPgeti(ist, &size);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
	err = structOPnew(size, &temp);
	if (err != ERR_ok) resignal(err);
	for (i = 0; i < temp.vec->size; i++) {
		CUR_PROC_VAR.proc = (CLUPROC)table[i];
		err = table[i]->proc(ist, &temp.vec->data[i]);
		if (err == ERR_not_possible) signal(err);
		if (err == ERR_end_of_file) signal(err);
		if (err != ERR_ok) resignal(err);
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

typedef struct {
PROC *fcn;
char *name;
} SEL_ENTRY;

typedef struct {
long count;
OWNPTR	type_owns;
OWNPTR  op_owns;
struct OP_ENTRY entry[8];
} selector_OPS;

typedef struct {
long count;
OWNPTR	type_owns;
OWNPTR  op_owns;
struct OP_ENTRY equal;
struct OP_ENTRY copy;
struct OP_ENTRY similar;
struct OP_ENTRY print;
struct OP_ENTRY encode;
struct OP_ENTRY decode;
struct OP_ENTRY _gcd;
struct OP_ENTRY debug_print;
} selector_TABLE;

OWN_ptr struct_own_init = {0, 0};

CLU_proc struct_oe_equal = {{0,0,0,0}, structOPequal, &struct_own_init, &struct_own_init};
CLU_proc struct_oe_copy = {{0,0,0,0}, structOPcopy, &struct_own_init, &struct_own_init};
CLU_proc struct_oe_similar = {{0,0,0,0}, structOPsimilar, &struct_own_init, &struct_own_init};
CLU_proc struct_oe_print = {{0,0,0,0}, structOPprint, &struct_own_init, &struct_own_init};
CLU_proc struct_oe_encode = {{0,0,0,0}, structOPencode, &struct_own_init, &struct_own_init};
CLU_proc struct_oe_decode = {{0,0,0,0}, structOPdecode, &struct_own_init, &struct_own_init};
CLU_proc struct_oe__gcd = {{0,0,0,0}, structOP_gcd, &struct_own_init, &struct_own_init};
CLU_proc struct_oe_debug_print = {{0,0,0,0}, structOPdebug_print, &struct_own_init, &struct_own_init};

selector_OPS struct_ops_actual = {8, 
&struct_own_init,
&struct_own_init, {
{&struct_oe_equal, "equal"},
{&struct_oe_copy, "copy"}, 
{&struct_oe_similar, "similar"},
{&struct_oe_print, "print"},
{&struct_oe_encode, "encode"},
{&struct_oe_decode, "decode"},
{&struct_oe__gcd, "_gcd"},
{&struct_oe_debug_print, "debug_print"}
}};

selector_OPS *struct_ops = &struct_ops_actual;

