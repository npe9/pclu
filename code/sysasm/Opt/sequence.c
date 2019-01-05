
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/sequence.c,v 1.11 91/08/29 16:03:46 dcurtis Exp $";
#endif
/* $Log:	sequence.c,v $
 * Revision 1.11  91/08/29  16:03:46  dcurtis
 * fixed casting on arguments to bcopy
 * 
 * Revision 1.10  91/08/29  15:00:20  dcurtis
 * removed elist from oneofOPnew call
 * 
 * Revision 1.9  91/07/25  08:36:31  root
 * fixed non-elist args to _gcd to be CLUREFs
 * 	probably necessary for sparcs
 * 
 * Revision 1.8  91/06/06  13:31:01  root
 * added copyright notice
 * 
 * Revision 1.7  91/06/05  13:28:01  root
 * added elist to sequence$new2
 * 
 * Revision 1.6  91/06/03  12:36:50  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.5  91/05/31  12:47:21  root
 * added result variable to pstream calls
 * 
 * Revision 1.4  91/05/31  12:44:51  root
 * fixed aggregate initialization in ops_actual
 * changed int argument to CLUREF argument
 * fixed calls to new: added elist
 * 
 * Revision 1.3  91/02/04  15:31:35  mtv
 * added rcsid string
 * 
 */

/*							*/
/*		SEQUENCE IMPLEMENTATION			*/
/*							*/

/* somewhat unfortunate to be using 'num' throughout */

#include "pclu_err.h"
#include "pclu_sys.h"

errcode extern sequenceOPinternal_print();

typedef struct {
long count;
struct REQ_ENTRY entry[8];
} sequence_of_t_REQS;

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
} sequence_of_t_TABLE;

sequence_of_t_REQS sequence_of_t_reqs_actual = 
		{8, "equal", "copy", "similar", "print", "encode", 
		"decode", "_gcd", "debug_print"};
sequence_of_t_REQS *sequence_of_t_reqs = &sequence_of_t_reqs_actual;


errcode sequenceOPnew(ans)
CLUREF *ans;
{
static CLUREF empty_sequence;
static bool init = false;

	if (!init) {
		clu_alloc(sizeof(CLU_sequence) -sizeof(CLUREF), &empty_sequence);
		empty_sequence.vec->size = 0;
		empty_sequence.vec->typ.val = CT_AGG;
		empty_sequence.vec->typ.mark = 0;
		empty_sequence.vec->typ.refp = 0;
		init = true;
		}
	ans->vec = empty_sequence.vec;
	signal(ERR_ok);
	}

errcode sequenceOPnew2(size, ans)
CLUREF size;
CLUREF *ans;
{
CLUREF temp;
	clu_alloc(sizeof(CLU_sequence) + 
			(size.num - 1) *sizeof(CLUREF), &temp);
	temp.vec->size = size.num;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPcons(x, ans)
CLUREF x, *ans;
{
	ans->vec = x.vec;
	signal(ERR_ok);
	}

errcode sequenceOPe2s(x, ans)
CLUREF x, *ans;
{
CLUREF temp;
	clu_alloc(sizeof(CLU_sequence), &temp);
	temp.vec->size = 1;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;
	temp.vec->data[0] = x.num;

	ans->vec = temp.vec;
	signal(ERR_ok);
	}
	
errcode sequenceOPsize(x, ans)
CLUREF x, *ans;
{
	ans->num = x.vec->size;
	signal(ERR_ok);
	}

errcode sequenceOPsubseq(s, first, length, ans)
CLUREF s, first, length, *ans;
{
errcode err;
CLUREF temp;
long i, copycount;

	if (first.num < 1) signal(ERR_bounds);
	if (first.num == 1 && length.num == s.vec->size) {
			ans->vec = s.vec;
			signal(ERR_ok);
			}
	if (first.num > (s.vec->size + 1)) signal(ERR_bounds);
	if (length.num < 0) signal(ERR_negative_size);
	if (length.num == 0) {
			err = sequenceOPnew(&temp);
			if (err != ERR_ok) resignal(err);
			ans->vec = temp.vec;
			signal(ERR_ok);
			}
	copycount = length.num;
	if (copycount + first.num - 1 > s.vec->size)
			copycount = s.vec->size - first.num + 1;
	clu_alloc((copycount-1)*sizeof(CLUREF) + sizeof(CLU_sequence),
				 &temp);
	temp.vec->size = copycount;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	for (i = 0; i < copycount; i++) {
		temp.vec->data[i] = s.vec->data[first.num-1+i];
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}
	
errcode sequenceOPfill(length, x, ans)
CLUREF length, x, *ans;
{
errcode err;
CLUREF temp;
long i;
	if (length.num < 0) signal(ERR_negative_size);
	if (length.num == 0) {
		err = sequenceOPnew(&temp);
		ans->vec = temp.vec;
		signal(ERR_ok);
		}
	if (length.num > MAX_SEQ) {
			elist[0].str = huge_allocation_request_STRING;
			signal(ERR_failure);
			}
	clu_alloc(length.num*CLUREFSZ + sizeof(CLU_sequence) - CLUREFSZ, &temp);
	temp.vec->size = length.num;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	for (i = 0; i < length.num; i++)
		temp.vec->data[i] = x.num;
		
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPfill_copy(length, x, ans)
CLUREF length, x, *ans;
{
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
errcode err;
CLUREF temp;
long i;
	if (length.num < 1) signal(ERR_negative_size);
	if (length.num == 0) {
		err = sequenceOPnew(&temp);
		ans->vec = temp.vec;
		signal(ERR_ok);
		}
	if (length.num > MAX_SEQ) {
			elist[0].str = huge_allocation_request_STRING;
			signal(ERR_failure);
			}
	clu_alloc(length.num*CLUREFSZ + sizeof(CLU_sequence) - CLUREFSZ, &temp);
	temp.vec->size = length.num;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	for (i = 0; i < length.num; i++) {
		CUR_PROC_VAR.proc = table->copy.fcn;
		err = table->copy.fcn->proc(x, &(temp.vec->data[i]));
		if (err != ERR_ok) resignal(err);
		}
	
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPfetch(s, i, ans)
CLUREF s, i, *ans;
{
	if (i.num <= 0) signal(ERR_bounds);
	if (i.num > s.vec->size) signal(ERR_bounds);
	ans->num = s.vec->data[i.num-1];
	signal(ERR_ok);
	}

errcode sequenceOPbottom(s, ans)
CLUREF s, *ans;
{
	if (s.vec->size == 0) signal(ERR_bounds);
	ans->num = s.vec->data[0];
	signal(ERR_ok);
	}

errcode sequenceOPtop(s, ans)
CLUREF s, *ans;
{
long top;
	if (s.vec->size == 0) signal(ERR_bounds);
	top = s.vec->size;
	ans->num = s.vec->data[top-1];
	signal(ERR_ok);
	}

errcode sequenceOPreplace(s, ind, x, ans)
CLUREF s, ind, x, *ans;
{
errcode err;
CLUREF temp;
long i, size;
	if (ind.num <= 0) signal(ERR_bounds);
	size = s.vec->size;
	if (ind.num > size) signal(ERR_bounds);
	
	clu_alloc(size*sizeof(CLUREF) + sizeof(CLU_sequence) -sizeof(CLUREF), &temp);
	temp.vec->size = size;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	for (i = 0; i < size; i++){
		temp.vec->data[i] = s.vec->data[i];
		}		/* original does copying in 10000 byte chunks */

	temp.vec->data[ind.num-1] = x.num;
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPaddh(s, x, ans)
CLUREF s, x, *ans;
{
errcode err;
CLUREF temp;
long size;
long i;
	size = s.vec->size;
	if ((size + 1) * sizeof(CLUREF) > MAX_SEQ) {
			elist[0].str = huge_allocation_request_STRING;
			signal(ERR_failure);	
			}
	
	clu_alloc((size + 1) * sizeof(CLUREF) + sizeof(CLU_sequence) - sizeof(CLUREF),
					 &temp);
	temp.vec->size = size + 1;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	bcopy((char*)&s.vec->data[0], (char*)temp.vec->data, size*CLUREFSZ);
/*
	for (i = 0; i < size; i++)
		temp.vec->data[i] = s.vec->data[i];
*/
	temp.vec->data[size] = x.num;
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPaddl(s, x, ans)
CLUREF s, x, *ans;
{
errcode err;
CLUREF temp;
long size;
long i;
	size = s.vec->size;
	if ((size + 1) * sizeof(CLUREF) > MAX_SEQ) {
			elist[0].str = huge_allocation_request_STRING;
			signal(ERR_failure);	
			}
	
	clu_alloc((size + 1) * sizeof(CLUREF) + sizeof(CLU_sequence) - sizeof(CLUREF),
					 &temp);
	temp.vec->size = size + 1;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	temp.vec->data[0] = x.num;
	for (i = 0; i < size; i++)
		temp.vec->data[i+1] = s.vec->data[i];
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPremh(s, ans)
CLUREF s, *ans;
{
long size;
long i;
errcode err;
CLUREF temp;
	size = s.vec->size;
	if (size == 0) signal (ERR_bounds);
	if (size == 1) {
		err = sequenceOPnew(&temp);
		if (err != ERR_ok) resignal(err);
		ans->vec = temp.vec;
		signal(ERR_ok);
		}

	clu_alloc((size - 1) * sizeof(CLUREF) +
			sizeof(CLU_sequence) - sizeof(CLUREF), &temp);
	temp.vec->size = size - 1;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;
	
	for (i = 0; i < size - 1; i++)
		temp.vec->data[i] = s.vec->data[i];

	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPreml(s, ans)
CLUREF s, *ans;
{
long size;
long i;
errcode err;
CLUREF temp;
	size = s.vec->size;
	if (size == 0) signal (ERR_bounds);
	if (size == 1) {
		err = sequenceOPnew(&temp);
		if (err != ERR_ok) resignal(err);
		ans->vec = temp.vec;
		signal(ERR_ok);
		}

	clu_alloc((size - 1) * sizeof(CLUREF) +
			sizeof(CLU_sequence) - sizeof(CLUREF), &temp);
	temp.vec->size = size - 1;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;
	
	for (i = 0; i < size - 1; i++)
		temp.vec->data[i] = s.vec->data[i + 1];

	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPconcat(s1, s2, ans)
CLUREF s1, s2, *ans;
{
errcode err;
CLUREF temp;
long size, i, j;
	if (s1.vec->size == 0) {
		ans->vec = s2.vec;
		signal(ERR_ok);
		}
	if (s2.vec->size == 0) {
		ans->vec = s1.vec;
		signal(ERR_ok);
		}
	size = s1.vec->size + s2.vec->size;
	if (size > MAX_SEQ) {
		elist[0].str = huge_allocation_request_STRING;
		signal(ERR_failure);
		}
	clu_alloc(size * sizeof(CLUREF) + sizeof(CLU_sequence) -sizeof(CLUREF), &temp);
	temp.vec->size = size;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	for (i = 0; i < s1.vec->size; i++)
		temp.vec->data[i] = s1.vec->data[i];
	for (j = 0, i = s1.vec->size; j < s2.vec->size; j++, i++)
		temp.vec->data[i] = s2.vec->data[j];
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOPa2s(a, ans)
CLUREF a, *ans;
{
errcode err;
CLUREF temp;
long i, j;

	clu_alloc(sizeof(CLU_sequence) + (a.array->ext_size - 1) * sizeof(CLUREF), &temp);
	temp.vec->size = a.array->ext_size;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = a.array->typ.refp;
	/*
	for (j = 0, i = a.array->int_low ; j < a.array->ext_size; j++, i++) {
		temp.vec->data[j] = a.array->store->data[i];
		}
	*/
	bcopy((char*)&a.array->store->data[a.array->int_low], 
				(char*)temp.vec->data,
				a.array->ext_size * CLUREFSZ);  /* new 6/25/90 */
	ans->vec = temp.vec;
	signal(ERR_ok);
}

errcode sequenceOPs2a(s, ans)
CLUREF s, *ans;
{
errcode err;
CLUREF temp;
long i;

	err = arrayOPOPnewdesc(&temp);
	if (err != ERR_ok) resignal(err);
	err = arrayOPOPnewstore(temp, s.vec->size);
	if (err != ERR_ok) resignal(err);
	for (i = 0 ; i < temp.array->int_size; i++) {
		temp.array->store->data[i] = s.vec->data[i];
		}
	temp.array->ext_low = 1;
	temp.array->ext_size = s.vec->size;	
	temp.array->ext_high = s.vec->size;
	ans->vec = temp.vec;
	signal(ERR_ok);
}

errcode sequenceOPempty(s, ans)
CLUREF s, *ans;
{
	if (s.vec->size == 0) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode sequenceOPequal(s1, s2, ans)
CLUREF s1, s2, *ans;
{
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
long i;
CLUREF elt1, elt2, temp;
errcode err;
	if (s1.vec->size != s2.vec->size) {
		ans->tf = false;
		signal(ERR_ok);
		}
	if (s1.vec->size == 0) {
		ans->tf = true;
		signal(ERR_ok);
		}
	for (i = 0; i < s1.vec->size; i++) {
		elt1.num = s1.vec->data[i];
		elt2.num = s2.vec->data[i];
		CUR_PROC_VAR.proc = table->equal.fcn;
		err = table->equal.fcn->proc(elt1, elt2, &temp);
		if (err != ERR_ok) resignal(err);
		if (temp.tf != true) {  
			ans->tf = false;
			signal(ERR_ok);
			}
		}
	ans->tf = true;
	signal(ERR_ok);
	}


errcode sequenceOPsimilar(s1, s2, ans)
CLUREF s1, s2, *ans;
{
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
long i;
errcode err;
CLUREF elt1, elt2, temp;
	if (s1.vec->size != s2.vec->size) {
		ans->tf = false;
		signal(ERR_ok);
		}
	if (s1.vec->size == 0) {
		ans->tf = true;
		signal(ERR_ok);
		}
	for (i = 0; i < s1.vec->size; i++) {
		elt1.num = s1.vec->data[i];
		elt2.num = s2.vec->data[i];
		CUR_PROC_VAR.proc = table->similar.fcn;
		err = table->similar.fcn->proc(elt1, elt2, &temp);
		if (err != ERR_ok) resignal(err);
		if (temp.tf != true) {   /* t$similar */
			ans->tf = false;
			signal(ERR_ok);
			}
		}
	ans->tf = true;
	signal(ERR_ok);
	}

errcode sequenceOPcopy(s1, s2)
CLUREF s1, *s2;
{
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
errcode err;
CLUREF temp, elt1;
long i;
	if (s1.vec->size == 0) {
		s2->vec = s1.vec;
		signal(ERR_ok);
		}
	clu_alloc(s1.vec->size * sizeof(CLUREF) +
			 sizeof(CLU_sequence) -sizeof(CLUREF), &temp);
	temp.vec->size = s1.vec->size;
	temp.vec->typ.val = CT_AGG;
	temp.vec->typ.mark = 0;
	temp.vec->typ.refp = 0;

	for (i = 0; i < s1.vec->size; i++) {
		elt1.num = s1.vec->data[i];
		CUR_PROC_VAR.proc = table->copy.fcn;
		err = table->copy.fcn->proc(elt1, &(temp.vec->data[i]));
		if (err != ERR_ok) resignal(err);
		}

	s2->vec = temp.vec;
	signal(ERR_ok);
	}


errcode sequenceOPindexes(s, iterbod, locals, ecode2)
CLUREF s;
char *locals;
errcode (*iterbod)();
errcode *ecode2;
{
errcode err;
errcode ecode;
CLUREF iv_1;

   if (s.vec->size == 0) signal(ERR_ok);
   iv_1.num = 1;
   while (true) {
	err = iterbod(iv_1, locals, ecode2);
	if (err == ERR_ok) {
		iv_1.num += 1;
		if (iv_1.num <= s.vec->size) continue;
		if (iv_1.num > s.vec->size) signal(ERR_ok);
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

errcode sequenceOPelements(s, iterbod, locals, ecode2)
CLUREF s;
char **locals;
errcode (*iterbod)();
errcode *ecode2;
{
errcode err;
errcode ecode;
CLUREF iv_1;
long index;

   if (s.vec->size == 0) signal(ERR_ok);
   index = 1;
   while (true) {
	iv_1.num = s.vec->data[index-1];
	err = iterbod(iv_1, locals, ecode2);
	if (err == ERR_ok) {
		index += 1;
		if (index <= s.vec->size) continue;
		if (index > s.vec->size) signal(ERR_ok);
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

errcode sequenceOPdebug_print(s, pst)
CLUREF s, pst;
{
errcode err;
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
CLUREF pfcn;

	pfcn.proc = table->debug_print.fcn;
	err = sequenceOPinternal_print(s, pst, pfcn);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode sequenceOPprint(s, pst)
CLUREF s, pst;
{
errcode err;
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
CLUREF pfcn;

	pfcn.proc = table->print.fcn;
	err = sequenceOPinternal_print(s, pst, pfcn);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode sequenceOPinternal_print(s, pst, pfcn)
CLUREF s, pst, pfcn;
{
errcode err;
CLUREF ans;
long i;
static bool init = false;
static CLUREF str1, str2, str3, elt;

	if (init == false) {
		err = stringOPcons("[", CLU_1, CLU_1, &str1);	
		err = stringOPcons(",", CLU_1, CLU_1, &str2);	
		err = stringOPcons("]", CLU_1, CLU_1, &str3);	
		init = true;
		}

	err = pstreamOPstart(pst, str1, &ans);
	if (err != ERR_ok) resignal(err);
	if ((ans.tf == false) || (s.vec->size == 0)) {
		err = pstreamOPstop(pst, str3, &ans);
		if (err != ERR_ok) resignal(err);
		signal(ERR_ok);
		}

	for (i = 0; i < s.vec->size; i++) {
		if (i != 0) {
			err = pstreamOPpause(pst, str2, &ans);
			if (err != ERR_ok) resignal(err);
			if (ans.tf == false) break;
			}
		elt.num = s.vec->data[i];
/*		CUR_PROC_VAR.proc = table->print.fcn; */
		CUR_PROC_VAR.proc = pfcn.proc;
		err = pfcn.proc->proc(elt, pst);
		if (err != ERR_ok) resignal(err);
		}

	err = pstreamOPstop(pst, str3, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode sequenceOPencode(s, ist)		/* use t$encode */
CLUREF s, ist;
{
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
errcode err;
long i;
CLUREF sz, elt;

	sz.num = s.vec->size;
	err = istreamOPputi(ist, sz);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	if (s.vec->size == 0) {
		signal(ERR_ok);
		}

	for (i = 0; i < s.vec->size; i++) {
		elt.num = s.vec->data[i];
		CUR_PROC_VAR.proc = table->encode.fcn;
		err = table->encode.fcn->proc(elt, ist);
		if (err == ERR_not_possible) signal(err);
		if (err != ERR_ok) resignal(err);
		}
	signal(ERR_ok);
	}

errcode sequenceOPdecode(ist, ans)		/* use t$decode */
CLUREF ist, *ans;
{
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
errcode err;
long i;
CLUREF elt, temp, size;

	err = istreamOPgeti(ist, &size);
	if (err == ERR_end_of_file) signal(err);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);

	err = sequenceOPnew2(size, &temp);
	if (err != ERR_ok) resignal(err);

	if (size.num == 0) {
		ans->vec = temp.vec;
		signal(ERR_ok);
		}

	for (i = 0; i < size.num; i++) {
		CUR_PROC_VAR.proc = table->decode.fcn;
		err = table->decode.fcn->proc(ist, &elt);
		if (err == ERR_not_possible) signal(err);
		if (err != ERR_ok) resignal(err);
		temp.vec->data[i] = elt.num;
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode sequenceOP_gcd(s, tab, ans)
CLUREF s;
CLUREF tab;
CLUREF *ans;
{
sequence_of_t_TABLE *table = (sequence_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0]; /* ptr to _gcd fcns */
errcode err;
CLUREF temp_oneof, sz, fcn;

	fcn.proc = table->_gcd.fcn;
	err = oneofOPnew(CLU_2, fcn, &temp_oneof);
	if (err != ERR_ok) resignal(err);
	sz.num = s.vec->size * GCD_REF_SIZE + 2*CLUREFSZ;
	err = gcd_tabOPinsert(tab, sz, temp_oneof, s, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
}


typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[29];
} sequence_OPS;

OWN_ptr sequence_own_init = {0, 0};

OWN_req sequence_ownreqs = {2*UNIT, 1};

CLU_proc sequence_oe_new = {{0,0,0,0}, sequenceOPnew, 0,0};
CLU_proc sequence_oe_cons = {{0,0,0,0}, sequenceOPcons, 0,0};
CLU_proc sequence_oe_e2s = {{0,0,0,0}, sequenceOPe2s, 0,0};
CLU_proc sequence_oe_size = {{0,0,0,0}, sequenceOPsize, 0,0};
CLU_proc sequence_oe_subseq = {{0,0,0,0}, sequenceOPsubseq, 0,0};
CLU_proc sequence_oe_fill = {{0,0,0,0}, sequenceOPfill, 0,0};
CLU_proc sequence_oe_fill_copy = {{0,0,0,0}, sequenceOPfill_copy, 0,0};
CLU_proc sequence_oe_fetch = {{0,0,0,0}, sequenceOPfetch, 0,0};
CLU_proc sequence_oe_bottom = {{0,0,0,0}, sequenceOPbottom, 0,0};
CLU_proc sequence_oe_top = {{0,0,0,0}, sequenceOPtop, 0,0};
CLU_proc sequence_oe_replace = {{0,0,0,0}, sequenceOPreplace, 0,0};
CLU_proc sequence_oe_addh = {{0,0,0,0}, sequenceOPaddh, 0,0};
CLU_proc sequence_oe_addl = {{0,0,0,0}, sequenceOPaddl, 0,0};
CLU_proc sequence_oe_remh = {{0,0,0,0}, sequenceOPremh, 0,0};
CLU_proc sequence_oe_reml = {{0,0,0,0}, sequenceOPreml, 0,0};
CLU_proc sequence_oe_concat = {{0,0,0,0}, sequenceOPconcat, 0,0};
CLU_proc sequence_oe_a2s = {{0,0,0,0}, sequenceOPa2s, 0,0};
CLU_proc sequence_oe_s2a = {{0,0,0,0}, sequenceOPs2a, 0,0};
CLU_proc sequence_oe_empty = {{0,0,0,0}, sequenceOPempty, 0,0};
CLU_proc sequence_oe_elements = {{0,0,0,0}, sequenceOPelements, 0,0};
CLU_proc sequence_oe_indexes = {{0,0,0,0}, sequenceOPindexes, 0,0};
CLU_proc sequence_oe_equal = {{0,0,0,0}, sequenceOPequal, 0,0};
CLU_proc sequence_oe_similar = {{0,0,0,0}, sequenceOPsimilar, 0,0};
CLU_proc sequence_oe_copy = {{0,0,0,0}, sequenceOPcopy, 0,0};
CLU_proc sequence_oe_print = {{0,0,0,0}, sequenceOPprint, 0,0};
CLU_proc sequence_oe_encode = {{0,0,0,0}, sequenceOPencode, 0,0};
CLU_proc sequence_oe_decode = {{0,0,0,0}, sequenceOPdecode, 0,0};
CLU_proc sequence_oe__gcd = {{0,0,0,0}, sequenceOP_gcd, 0,0};
CLU_proc sequence_oe_debug_print = {{0,0,0,0}, sequenceOPdebug_print, 0,0};

sequence_OPS sequence_ops_actual = {29,
    &sequence_own_init,
    &sequence_own_init, {
{&sequence_oe_new, "new"},
{&sequence_oe_cons, "cons"},
{&sequence_oe_e2s, "e2s"},
{&sequence_oe_size, "size"},
{&sequence_oe_subseq, "subseq"},
{&sequence_oe_fill, "fill"},
{&sequence_oe_fill_copy, "fill_copy"},
{&sequence_oe_fetch, "fetch"},
{&sequence_oe_bottom, "bottom"},
{&sequence_oe_top, "top"},
{&sequence_oe_replace, "replace"},
{&sequence_oe_addh, "addh"},
{&sequence_oe_addl, "addl"},
{&sequence_oe_remh, "remh"},
{&sequence_oe_reml, "reml"},
{&sequence_oe_concat, "concat"},
{&sequence_oe_a2s, "a2s"},
{&sequence_oe_s2a, "s2a"},
{&sequence_oe_empty, "empty"},
{&sequence_oe_elements, "elements"},
{&sequence_oe_indexes, "indexes"},
{&sequence_oe_equal, "equal"},
{&sequence_oe_similar, "similar"},
{&sequence_oe_copy, "copy"},
{&sequence_oe_print, "print"},
{&sequence_oe_encode, "encode"},
{&sequence_oe_decode, "decode"},
{&sequence_oe__gcd, "_gcd"},
{&sequence_oe_debug_print, "debug_print"}
}};

sequence_OPS *sequence_ops = &sequence_ops_actual;
