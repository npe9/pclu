
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: array.c,v 1.7 91/06/06 13:26:29 root Exp $";
#endif
/* $Log:	array.c,v $
 * Revision 1.7  91/06/06  13:26:29  root
 * added copyright notice
 * 
 * Revision 1.6  91/06/03  15:27:10  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.5  91/05/31  12:08:32  root
 * fixed aggregate initialization in ops_actual
 * removed some unused variable declarations
 * removed _cons2 (unused)
 * 
 * Revision 1.4  91/05/07  14:59:41  root
 * Added mtv's changes to make addh behave better
 * 
 * Revision 1.5  91/05/07  14:51:29  dcurtis
 * Added return value for pstream$text(c) calls
 * Added print function to array_ops
 * 
 * Revision 1.4  91/04/12  17:56:23  mtv
 * Changed remh/reml to zero the removed element to enhance GC.
 * 
 * Revision 1.3  91/04/02  19:34:39  mtv
 * Changed addh to allocate extra storage for future addh's.
 * Changed arrayOPOPnewstore to increases the size of the allocated CLU_store
 * to denote the padding performed by the storage management system.
 * 
 * Revision 1.2  91/02/11  11:08:34  dcurtis
 * fixed elements to include last element (usually inlined)
 * 
 * Revision 1.1  91/02/04  15:49:39  mtv
 * Initial revision
 */

/*						*/
/*		ARRAY IMPLEMENTATION		*/
/*						*/
/*						*/

/* FIX:  since low can be a negative number and size can be greater than all positive numbers */
/*	 need to change int_low from having a minimum of 0 */

/* Mystery -- how is array bounds (not size) overflow detected?
 * May be necessary to strengthen rep invariant for detecting bounds overflow.
 */

/* NOTA BENE: if rep changes, look at sequence$s2a, a2s    */

#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode arrayOPOPnewdesc();
extern errcode arrayOPOPnewstore();
extern errcode arrayOPOPsize_for_growth();
extern errcode arrayOPOPminimum_size();
extern errcode arrayOPinternal_print();

typedef struct {
long count;
struct REQ_ENTRY entry[8];
} array_of_t_REQS;

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
} array_of_t_TABLE;

array_of_t_REQS array_of_t_reqs_actual = {8, "equal", "copy", "similar", "print", "encode",
"decode", "_gcd", "debug_print"};

array_of_t_REQS *array_of_t_reqs = & array_of_t_reqs_actual;



#ifdef DEBUG_ARRAY
void assertfailed()
{
}

static void check_RI(a)
CLUREF a;
{
  if (! ((a.array->int_size >= 0) &&
	 (a.array->int_low >= 0) &&
	 (a.array->ext_size >= 0) &&
	 (a.array->ext_size == a.array->ext_high - a.array->ext_low + 1) &&
	 (a.array->int_low + a.array->ext_size <= a.array->int_size) &&
	 (a.array->int_size == a.array->store->size)))
    assertfailed();
}
#endif



errcode arrayOPcreate(low, ans)
CLUREF low, *ans;
{
CLUREF temp, store;

/* ODD DWC
	if (low.num < (long)MIN_ARR_INDEX) {
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure);
		}
*/
/* allocate array descriptor */
	clu_alloc(sizeof(CLU_array), &temp);
	temp.array->typ.val = CT_ARRAY;
	temp.array->typ.mark = 0;
	temp.array->typ.refp = 0;
/* allocate storage for data */
	clu_alloc(sizeof(CLU_store), &store);
	store.store->typ.val = CT_STORE;
	store.store->typ.mark = 0;
	store.store->typ.refp = 0;
	store.store->size = 1;
	temp.array->store = store.store;
	temp.array->int_low = 0;
	temp.array->int_size = 1;
	temp.array->ext_low = low.num;
	temp.array->ext_size = 0;
	temp.array->ext_high = low.num - 1;
	ans->array = temp.array;
#ifdef DEBUG_ARRAY
        check_RI(temp);
#endif
	signal(ERR_ok);
}

errcode arrayOPnew(ans)
CLUREF *ans;
{
CLUREF temp, store;

/* allocate array descriptor */
#ifdef athena
	clu_alloc(sizeof(CLU_array), &temp);
#else
	Alloc(CLU_array_sizew, temp);
#endif
	temp.array->typ.val = CT_ARRAY;
	temp.array->typ.mark = 0;
	temp.array->typ.refp = 0; 
/* allocate storage for data */
#ifdef athena
	clu_alloc(sizeof(CLU_store), &store);
#else
	Alloc(CLU_store_sizew, store);
#endif
	store.store->typ.val = CT_STORE;
	store.store->typ.mark = 0;
	store.store->typ.refp = 0;
	store.store->size = 1;
	temp.array->store = store.store;
	temp.array->int_low = 0;
	temp.array->int_size = 1;
	temp.array->ext_low = 1;
	temp.array->ext_size = 0;
	temp.array->ext_high = 0;
	ans->array = temp.array;
#ifdef DEBUG_ARRAY
	check_RI(temp);
#endif
	signal(ERR_ok);
}

errcode arrayOPpredict(low, size, ans)
CLUREF low, size, *ans;
{
CLUREF temp, store;
long s;

/* ODD... DWC
	if (low.num < MIN_ARR_INDEX) {
		printf ("low.num %d\n", low.num);
		printf ("((long)MIN_ARR_INDEX) %d\n", ((long)MIN_ARR_INDEX));
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure);
		}
*/
	if (size.num >= 0) s = size.num;
	else s = - size.num;
	if (s > (long)MAX_ARR_SIZE) {
		s = (long)MAX_ARR_SIZE;
		}

/* NORMALIZE size */
	s = arrayOPOPminimum_size(s);
/* allocate array descriptor */
	clu_alloc(sizeof(CLU_array), &temp);
	temp.array->typ.val = CT_ARRAY;
	temp.array->typ.mark = 0;
	temp.array->typ.refp = 0;
/* allocate storage for data */
	clu_alloc(sizeof(CLU_store) + (s-1) * sizeof(CLUREF), &store);
	store.store->typ.val = CT_STORE;
	store.store->typ.mark = 0;
	store.store->typ.refp = 0;
	store.store->size = s;
	temp.array->store = store.store;
	temp.array->int_low = 0;
	temp.array->int_size = s;
	temp.array->ext_low = low.num;
	temp.array->ext_size = 0;
	temp.array->ext_high = low.num - 1;
	ans->array = temp.array;
	signal(ERR_ok);
}

errcode arrayOPcons(q, ans)
CLUREF q, *ans;
{
errcode err;
CLUREF temp;
long i;

	err = arrayOPOPnewdesc(&temp);
	if (err != ERR_ok) resignal(err);
	err = arrayOPOPnewstore(temp, q.vec->size);
	if (err != ERR_ok) resignal(err);
	for (i = 0 ; i < q.vec->size; i++) {
		temp.array->store->data[i] = q.vec->data[i];
		}
	temp.array->ext_low = 1;
	temp.array->ext_size = q.vec->size;	
	temp.array->ext_high = q.vec->size;
	ans->vec = temp.vec;
#ifdef DEBUG_ARRAY
	check_RI(temp);
#endif
	signal(ERR_ok);
}

errcode arrayOPcons2(low, q, ans)
CLUREF low, q, *ans;
{
errcode err;
CLUREF temp;
long i;

	err = arrayOPOPnewdesc(&temp);
	if (err != ERR_ok) resignal(err);
	err = arrayOPOPnewstore(temp, q.vec->size);
	if (err != ERR_ok) resignal(err);
	for (i = 0 ; i < q.vec->size; i++) {
		temp.array->store->data[i] = q.vec->data[i];
		}
	temp.array->ext_low = low.num;
	temp.array->ext_size = q.vec->size;	
	temp.array->ext_high = low.num + q.vec->size - 1;
	ans->vec = temp.vec;
#ifdef DEBUG_ARRAY
	check_RI(temp);
#endif
	signal(ERR_ok);
}
/*
errcode arrayOP_cons2(low, q, ans)
CLUREF low, q, *ans;
{
}
*/

errcode arrayOPempty(a, ans)
CLUREF a, *ans;
{
	if (a.array->ext_size == 0) 
		ans->tf = true;
	else
		ans->tf = false;
	signal(ERR_ok);
}

errcode arrayOPlow(a, ans)
CLUREF a, *ans;
{
	ans->num = a.array->ext_low;
	signal(ERR_ok);
}

errcode arrayOPhigh(a, ans)
CLUREF a, *ans;
{
	ans->num = a.array->ext_high;
	signal(ERR_ok);
}

errcode arrayOPsize(a, ans)
CLUREF a, *ans;
{
	ans->num = a.array->ext_size;
	signal(ERR_ok);
}

errcode arrayOPset_low(a, low)
CLUREF a, low;
{
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
/*
	if (low.num < (long)MIN_ARR_INDEX || low.num > (long)MAX_ARR_INDEX) {
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure);}
*/
	a.array->ext_low = low.num;
	a.array->ext_high = low.num + a.array->ext_size - 1;
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	signal(ERR_ok);
}

errcode arrayOPtrim(a, low, size)    /* signals bounds, negative_size */
CLUREF a, low, size;
{
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	if (low.num < a.array->ext_low) signal(ERR_bounds);
	if (size.num > a.array->ext_size) signal(ERR_bounds);
	if (size.num < 0) signal(ERR_negative_size);
	a.array->ext_low = low.num;
	if (size.num < a.array->ext_size) a.array->ext_size = size.num;
	a.array->ext_high = low.num + size.num - 1;
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	signal(ERR_ok);
}

errcode arrayOPfill(low, size, elt, ans)     /* signals negative_size */
CLUREF low, size, elt, *ans;
{
CLUREF temp;
errcode err;
long i;

	if (size.num < 0) signal(ERR_negative_size);
/*
	if (low.num < (long)MIN_ARR_INDEX) {
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure); }
*/
	if (low.num + size.num > (long)MAX_ARR_SIZE) {
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure); }
	err = arrayOPOPnewdesc(&temp);
	if (err != ERR_ok) resignal(err);
	err = arrayOPOPnewstore(temp, size.num);
	if (err != ERR_ok) resignal(err);
	temp.array->ext_low = low.num;
	temp.array->ext_size = size.num;
	temp.array->ext_high = low.num + size.num - 1;
	for (i = 0; i < size.num; i++) {
		temp.array->store->data[i] = elt.num; }
	ans->array = temp.array;
#ifdef DEBUG_ARRAY
	check_RI(temp);
#endif
	signal(ERR_ok);
	
}

errcode arrayOPfill_copy(low, size, elt, ans)	/* use t$copy */	/* signals negative_size */
CLUREF low, size, elt, *ans;
{
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
CLUREF temp;
errcode err;
long i;

	if (size.num < 0) signal(ERR_negative_size);
/*
	if (low.num < (long)MIN_ARR_INDEX) {
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure); }
*/
	if (low.num + size.num > (long)MAX_ARR_SIZE) {
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure); }
	err = arrayOPOPnewdesc(&temp);
	if (err != ERR_ok) resignal(err);
	err = arrayOPOPnewstore(temp, size.num);
	if (err != ERR_ok) resignal(err);
	temp.array->ext_low = low.num;
	temp.array->ext_size = size.num;
	temp.array->ext_high = low.num + size.num - 1;
	for (i = 0; i < size.num; i++) {
		CUR_PROC_VAR.proc = table->copy.fcn;
		err = table->copy.fcn->proc(elt, &temp.array->store->data[i]); }
	if (err != ERR_ok) resignal(err);
	ans->array = temp.array;
#ifdef DEBUG_ARRAY
	check_RI(temp);
#endif
	signal(ERR_ok);
	
}

errcode arrayOPfetch(a, idx, ans)			/* signals bounds */
CLUREF a, idx, *ans;
{
long int_index;	/* internal index */

	if (idx.num < a.array->ext_low || idx.num > a.array->ext_high)
			signal(ERR_bounds);
	int_index = idx.num - a.array->ext_low + a.array->int_low;
	ans->num = a.array->store->data[int_index];
	signal(ERR_ok);
}

errcode arrayOPbottom(a, ans)			/* signals bounds */
CLUREF a, *ans;
{
	if (a.array->ext_size == 0) signal(ERR_bounds);
	ans->num = a.array->store->data[a.array->int_low];
	signal(ERR_ok);
}

errcode arrayOPtop(a, ans)			/* signals bounds */
CLUREF a, *ans;
{
	if (a.array->ext_size == 0) signal(ERR_bounds);
	ans->num = a.array->store->data[a.array->int_low + a.array->ext_size - 1];
	signal(ERR_ok);
}

errcode arrayOPstore(a, idx, elt)			/* signals bounds */
CLUREF a, idx, elt;
{
long int_index;	/* internal index */

	if (idx.num < a.array->ext_low || idx.num > a.array->ext_high)
			signal(ERR_bounds);
	int_index = idx.num - a.array->ext_low + a.array->int_low;
	a.array->store->data[int_index] = elt.num; 
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	signal(ERR_ok);
}


/* arrayOPOPsize_for_growth should be tuned */

errcode arrayOPaddh(a, elt)
CLUREF a, elt;
{
long new_size, ext_size, int_low;

#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
        ext_size = a.array->ext_size;
        int_low = a.array->int_low;

	if (ext_size >= (long)MAX_ARR_SIZE) {
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure); }

        if (a.array->ext_size == 0) a.array->int_low = int_low = 0;

        if (a.array->int_size <= ext_size + int_low) {

	  new_size = arrayOPOPsize_for_growth(ext_size + 1);

	  if (new_size <= a.array->int_size) {
	    
	    /* Shift data to low end.  ENHANCE: Consider shrinking a. */
	    
	    bcopy(&a.array->store->data[int_low], 
		  &a.array->store->data[0],
		  ext_size * sizeof(CLUREF));
	    bzero(&a.array->store->data[ext_size], /* trash garbage pointers */
		  int_low * sizeof(CLUREF));
	    a.array->int_low = int_low = 0;

	  } else {

	    /* Allocate new store. */

#define gc_free GC_free
	    extern void gc_free();
	    CLUSTORE old_store = a.array->store;

	    arrayOPOPnewstore(a, new_size);
	    bcopy(&old_store->data[int_low], 
		  &a.array->store->data[0],
		  ext_size * sizeof(CLUREF));
	    gc_free(old_store);
	    int_low = 0;
	  }
	}

	a.array->store->data[int_low + ext_size] = elt.num;
	a.array->ext_size += 1;
	a.array->ext_high += 1;
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	signal(ERR_ok);
}

/* the addl algorithm should be improved to see if the store already has room in it for the new element */

errcode arrayOPaddl(a, elt)
CLUREF a, elt;
{
long new_size;
long old_int_low;
CLUREF oldstore;
errcode err;
long i;

#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	new_size = a.array->ext_size + 1;
	if (new_size > (long)MAX_ARR_SIZE) {
		elist[0].str = array_bounds_overflow_STRING;
		signal(ERR_failure); }
	oldstore.store = a.array->store;
	old_int_low = a.array->int_low;
	err = arrayOPOPnewstore(a, new_size);
	if (err != ERR_ok) resignal(err);
	for (i = 1 ; i < new_size ; i++) {
		a.array->store->data[i] = oldstore.store->data
						[old_int_low + i - 1]; }
	a.array->store->data[0] = elt.num;
	a.array->ext_size = new_size;
	a.array->ext_low -= 1;
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	signal(ERR_ok);
}

errcode arrayOPremh(a, ans)		/* signals bounds */
CLUREF a, *ans;
{
        CLUREF *elem;
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	if (a.array->ext_size == 0) signal(ERR_bounds);
	elem = (CLUREF *)
	  &a.array->store->data[a.array->int_low + a.array->ext_size - 1];
	ans->num = elem->num;
	elem->num = 0;                          /* enhance gc */
	a.array->ext_high -= 1;
	a.array->ext_size -= 1;
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	signal(ERR_ok);
}

errcode arrayOPreml(a, ans)		/* signals bounds */
CLUREF a, *ans;
{
        CLUREF *elem;
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	if (a.array->ext_size == 0) signal(ERR_bounds);
	elem = (CLUREF *) &a.array->store->data[a.array->int_low];
	ans->num = elem->num;
	elem->num = 0;                          /* enhance gc */
	a.array->ext_low += 1;
	a.array->int_low += 1;
	a.array->ext_size -= 1;
#ifdef DEBUG_ARRAY
	check_RI(a);
#endif
	signal(ERR_ok);
}

errcode arrayOPelements(a, iterbod, user_locals, ecode2)
CLUREF a;
char **user_locals;
errcode (*iterbod)();
errcode *ecode2;
{
CLUREF iv_1;
long index, int_index;
struct {
	char **user_locals;
	errcode (*proc)();
	errcode err;
	errcode ecode2;
	CLUREF  a;
	long	limit;
	} locals;

   locals.a = a;
   locals.user_locals = user_locals;
   locals.proc = iterbod;
   locals.limit = a.array->ext_high;

   if (a.array->ext_size == 0) signal(ERR_ok);
   index = a.array->ext_low;
   int_index = a.array->int_low;
   while (true) {
	if (index < a.array->ext_low || index > a.array->ext_high) {
                elist[0].str = array_bounds_exception_STRING;
                signal(ERR_failure);
                }
	iv_1.num = a.array->store->data[int_index];
	locals.err = iterbod(iv_1, user_locals, ecode2);
	if (locals.err == ERR_ok) {
		index += 1;
		int_index += 1;
		if (index <= locals.limit) continue;
		if (index >  locals.limit) signal(ERR_ok);
		}
	if (locals.err == ERR_iterbodyreturn) signal(locals.err);
	/* 9/9/93 dwc: just propagate break, do not convert to ok */
	if (locals.err == ERR_break) signal(ERR_break);
	if (locals.err == ERR_iterbodyexit) signal(locals.err);
	if (locals.err == ERR_iterbodysignal) signal(locals.err);
	if (locals.err == ERR_iteriterbodysignal) signal(locals.err);
	if (locals.err == ERR_iteriterbodyexit) signal(locals.err);
	if (locals.err == ERR_iteriterbodyreturn) signal(locals.err);
   }
}

errcode arrayOPindexes(a, iterbod, locals, ecode2)
CLUREF a;
char *locals;
errcode (*iterbod)();
errcode *ecode2;
{
errcode err;
CLUREF iv_1;
long limit;

   if (a.array->ext_size == 0) signal(ERR_ok);
   limit = a.array->ext_high;   /* capture initial size */
   iv_1.num = a.array->ext_low;
   while (true) {
	err = iterbod(iv_1, locals, ecode2);
	if (err == ERR_ok) {
		iv_1.num += 1;
		if (iv_1.num <= limit) continue;
		if (iv_1.num > limit) signal(ERR_ok);
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

errcode arrayOPequal(a1, a2, ans)
CLUREF a1, a2, *ans;
{
	if (a1.array == a2.array)
		ans->tf = true;
	else
		ans->tf = false;
	signal(ERR_ok);
}

errcode arrayOPsimilar(a1, a2, ans)	/* use t$similar */
CLUREF a1, a2, *ans;
{
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
long i, j, count;
errcode err;
CLUREF ans2, elt1, elt2;

	if (a1.array->ext_size != a2.array->ext_size) {
		ans->tf = false;
		signal(ERR_ok);
		}
	if (a1.array->ext_low != a2.array->ext_low) {
		ans->tf = false;
		signal(ERR_ok);
		}
	if (a1.array->ext_size == 0) {
		ans->tf = true;
		signal(ERR_ok);
		}

/*	ANOTHER CASE TO CHECK FOR BOUNDS? */

	for (i = a1.array->int_low, j = a2.array->int_low, count = a1.array->ext_low;
		count <= a1.array->ext_high; i++, j++, count++) 
		{
		elt1.num = a1.array->store->data[i];
		elt2.num = a2.array->store->data[i];
		CUR_PROC_VAR.proc = table->similar.fcn;
		err = table->similar.fcn->proc(elt1, elt2, &ans2);
		if (err != ERR_ok) resignal(err);
		if (ans2.tf == false) {
			ans->tf = false;
			signal(ERR_ok);
			}
		}
	ans->tf = true;
	signal(ERR_ok);
}

errcode arrayOPsimilar1(a1, a2, ans)	/* use t$equal */
CLUREF a1, a2, *ans;
{
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
long i, j, count;
errcode err;
CLUREF elt1, elt2, ans2;

	if (a1.array->ext_size != a2.array->ext_size) {
		ans->tf = false;
		signal(ERR_ok);
		}
	if (a1.array->ext_low != a2.array->ext_low) {
		ans->tf = false;
		signal(ERR_ok);
		}
	if (a1.array->ext_size == 0) {
		ans->tf = true;
		signal(ERR_ok);
		}

/*	ANOTHER CASE TO CHECK FOR BOUNDS? */

	for (i = a1.array->int_low, j = a2.array->int_low, count = a1.array->ext_low;
		count <= a1.array->ext_high; i++, j++, count++) 
		{
		elt1.num = a1.array->store->data[i];
		elt2.num = a2.array->store->data[i];
		CUR_PROC_VAR.proc = table->equal.fcn;
		err = table->equal.fcn->proc(elt1, elt2, &ans2);
		if (err != ERR_ok) resignal(err);
		if (ans2.tf == false) {
			ans->tf = false;
			signal(ERR_ok);
			}
		}
	ans->tf = true;
	signal(ERR_ok);
}

errcode arrayOPcopy(a1, ans)		/* use t$copy */
CLUREF a1, *ans;
{
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
CLUREF temp, elt;
errcode err;
long i;

#ifdef DEBUG_ARRAY
	check_RI(a1);
#endif
	err = arrayOPOPnewdesc(&temp);
	if (err != ERR_ok) resignal(err);
	err = arrayOPOPnewstore(temp, a1.array->ext_size);
	if (err != ERR_ok) resignal(err);
	temp.array->ext_low = a1.array->ext_low;
	temp.array->ext_size = a1.array->ext_size;
	temp.array->ext_high = a1.array->ext_high;
	for (i = 0; i < a1.array->ext_size; i++) {
		elt.num = a1.array->store->data[i+a1.array->int_low];
		CUR_PROC_VAR.proc = table->copy.fcn;
						/* added _int_low 7/24/90 */
		err = table->copy.fcn->proc(elt,
				&temp.array->store->data[i]);
		if (err != ERR_ok) resignal(err);
		}
	ans->array = temp.array;
#ifdef DEBUG_ARRAY
	check_RI(temp);
#endif
	signal(ERR_ok);
}

errcode arrayOPcopy1(a1, ans)		/* do not use t$copy */
CLUREF a1, *ans;
{
CLUREF temp;
errcode err;
long i,j,k;

#ifdef DEBUG_ARRAY
	check_RI(a1);
#endif
	err = arrayOPOPnewdesc(&temp);
	if (err != ERR_ok) resignal(err);
	err = arrayOPOPnewstore(temp, a1.array->ext_size);
	if (err != ERR_ok) resignal(err);
	temp.array->ext_low = a1.array->ext_low;
	temp.array->ext_size = a1.array->ext_size;
	temp.array->ext_high = a1.array->ext_high;
	j = a1.array->int_low;
	k = temp.array->int_low;
	for (i = 0; i < a1.array->ext_size; i++, j++, k++) {
		temp.array->store->data[k] = a1.array->store->data[j];
		}
	ans->array = temp.array;
#ifdef DEBUG_ARRAY
	check_RI(temp);
#endif
	signal(ERR_ok);
}

errcode arrayOPdebug_print(a, pst)
CLUREF a, pst;
{
errcode err;
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
CLUREF pfcn;

	pfcn.proc = table->debug_print.fcn;
	err = arrayOPinternal_print(a, pst, pfcn);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode arrayOPprint(a, pst)
CLUREF a, pst;
{
errcode err;
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
CLUREF pfcn;

	pfcn.proc = table->print.fcn;
	err = arrayOPinternal_print(a, pst, pfcn);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode arrayOPinternal_print(a, pst, pfcn)		/* use t$print */
CLUREF a, pst, pfcn;
{
errcode err;
CLUREF ans, max, num;
long i,j;
static bool init = false;
static CLUREF str1, str2, str3, str4, str5;

        if (init == false) {
                err = stringOPcons("[", CLU_1, CLU_1, &str1);
                err = stringOPcons("..", CLU_1, CLU_2, &str2);
                err = stringOPcons(": ", CLU_1, CLU_2, &str3);
                err = stringOPcons(",", CLU_1, CLU_1, &str4);
                err = stringOPcons("]", CLU_1, CLU_1, &str5);
                init = true;
                }

        err = pstreamOPtext(pst, str1, &ans);
        if (err != ERR_ok) resignal(err);
	num.num = a.array->ext_low;
	err = intOPprint(num, pst);
        if (err != ERR_ok) resignal(err);
        err = pstreamOPget_max_width(pst, &max);
        if (err != ERR_ok) resignal(err);
	if (a.array->ext_size >= max.num) {
        	err = pstreamOPtext(pst, str2, &ans);
        	if (err != ERR_ok) resignal(err);
		num.num = a.array->ext_high;
		err = intOPprint(num, pst);
        	if (err != ERR_ok) resignal(err);
		}
        err = pstreamOPstart(pst, str3, &ans);
        if (err != ERR_ok) resignal(err);
        if (ans.tf == false || a.array->ext_size == 0) {
                err = pstreamOPstop(pst, str5, &ans);
                if (err != ERR_ok) resignal(err);
                signal(ERR_ok);
                }

/* the original code has a bounds check in the following loop */
        for (i = 0, j = a.array->int_low; i < a.array->ext_size; i++, j++) {
		if (i != 0) {
	                err = pstreamOPpause(pst, str4, &ans);
       		        if (err != ERR_ok) resignal(err);
       		        if (ans.tf == false) break;
			}
		num.num = a.array->store->data[j];
                CUR_PROC_VAR.proc = pfcn.proc;
                err = pfcn.proc->proc(num, pst);
		if (err != ERR_ok) resignal(err);
		}

        err = pstreamOPstop(pst, str5, &ans);
        if (err != ERR_ok) resignal(err);
        signal(ERR_ok);
        }

errcode arrayOPencode(a, ist)		/* use t$encode */	/* signals not_possible(string) */
CLUREF a, ist;
{
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
errcode err;
long i;
CLUREF num;

	num.num = a.array->ext_low;
        err = istreamOPputi(ist, num);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
	num.num = a.array->ext_size;
        err = istreamOPputi(ist, num);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
	num.num = a.array->int_size;
/* removed predict... for mtv 9/93
        err = istreamOPputi(ist, num);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
*/
        if (a.array->ext_size == 0) {
                signal(ERR_ok);
                }

        for (i = 0; i < a.array->ext_size; i++) {
		num.num = a.array->store->data[i];
                CUR_PROC_VAR.proc = table->encode.fcn;
                err = table->encode.fcn->proc(num, ist);
                if (err == ERR_not_possible) signal(err);
                if (err != ERR_ok) resignal(err);
                }
        signal(ERR_ok);
        }

errcode arrayOPdecode(ist, ans)		/* use t$decode *//* signals not_possible(string), end_of_file */
CLUREF ist, *ans;
{
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];
errcode err;
long i;
CLUREF size, low, predict;
CLUREF elt, temp;

        err = istreamOPgeti(ist, &low);
        if (err == ERR_end_of_file) signal(err);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
/*
	if (low.num < (long)MIN_ARR_INDEX) {
		elist[0].str = bad_format_STRING;
		signal(ERR_failure);
		}
*/
        err = istreamOPgeti(ist, &size);
        if (err == ERR_end_of_file) signal(err);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
	if (size.num > (long)MAX_ARR_SIZE) {
		elist[0].str = bad_format_STRING;
		signal(ERR_failure);
		}
	if (size.num + low.num > (long)MAX_ARR_INDEX) {
		elist[0].str = bad_format_STRING;
		signal(ERR_failure);
		}
/*
        err = istreamOPgeti(ist, &predict);
        if (err == ERR_end_of_file) signal(err);
        if (err == ERR_not_possible) signal(err);
        if (err != ERR_ok) resignal(err);
	if (predict.num > (long)MAX_ARR_SIZE) {
		elist[0].str = bad_format_STRING;
		signal(ERR_failure);
		}
*/

        err = arrayOPfill(low, size, CLU_0, &temp);
        if (err != ERR_ok) resignal(err);

        if (size.num == 0) {
                ans->vec = temp.vec;
                signal(ERR_ok);
               }

        for (i = 0; i < size.num; i++) {
                CUR_PROC_VAR.proc = table->decode.fcn;
                err = table->decode.fcn->proc(ist, &elt);
        	if (err == ERR_end_of_file) signal(err);
                if (err == ERR_not_possible) signal(err);
                if (err != ERR_ok) resignal(err);
                temp.array->store->data[i] = elt.num;
                }
        ans->array = temp.array;
        signal(ERR_ok);
        }

errcode arrayOP_gcd(a, tab, ans)		/* use t$_gcd */
CLUREF a, tab, *ans;
{
errcode err;
CLUREF temp_oneof, temp_oneof_2, sz, fcn;
array_of_t_TABLE *table = (array_of_t_TABLE*)CUR_PROC_VAR.proc->type_owns->info[0];

	fcn.proc = table->_gcd.fcn;
	err = oneofOPnew(CLU_7, fcn, &temp_oneof_2);
	if (err != ERR_ok) resignal(err);
	err = oneofOPnew(CLU_6, temp_oneof_2, &temp_oneof);
	if (err != ERR_ok) resignal(err);
	sz.num = 6*CLUREFSZ + GCD_REF_SIZE;
	err = gcd_tabOPinsert(tab, sz, temp_oneof, a, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
}


errcode arrayOPOPnewdesc(ans)
CLUREF *ans;
{
CLUREF temp;
	clu_alloc(sizeof(CLU_array), &temp);
	temp.array->typ.val = CT_ARRAY;
	temp.array->typ.mark = 0;
	temp.array->typ.refp = 0;
	ans->array = temp.array;
	signal(ERR_ok);
}

errcode arrayOPOPnewstore(desc, size)
CLUREF desc;
long size;
{
CLUREF temp;

	size = arrayOPOPminimum_size(size);
	clu_alloc(sizeof(CLU_store) + (size-1) * sizeof(CLUREF), &temp);
	temp.store->typ.val = CT_STORE;
	temp.store->typ.mark = 0;
	temp.store->typ.refp = 0;
	temp.store->size = size;
	desc.array->store = temp.store;
	desc.array->int_low = 0;
	desc.array->int_size = size;
	signal(ERR_ok);
}

/*
 * Computes the minimum size (number of elements) actually allocated for
 * an array of size n.
 * Know secrets about the storage manager.
 */
long arrayOPOPminimum_size(n)
long n;
{
  long store_size = BYTES_TO_WORDS(sizeof(CLU_store) - sizeof(CLUREF));
  long ans;

  ans = n + store_size;
#ifndef LINUX
  if (ans < MAXOBJSZ) {
#ifdef MERGE_SIZES
    extern long size_map[];
    ans = size_map[ans];
#endif
  } else {
    ans = WORDS_TO_BYTES(ans) + sizeof(struct hblkhdr);
    ans = (ans + HBLKMASK) & ~HBLKMASK;
    ans = BYTES_TO_WORDS(ans - sizeof(struct hblkhdr));
  }
#endif
  return(ans - store_size);
}

/*
 * Computes a desirable internal size for an array with n elems.
 * Assumes the array may continue to grow.
 * Ensures result >= n.
 * Know secrets about the storage manager.
 */
long arrayOPOPsize_for_growth(n)
long n;
{
  long pad = n >> 2;
  long max_pad = BYTES_TO_WORDS(HBLKSIZE - sizeof(CLU_store) + sizeof(CLUREF));
  long min_pad = 2;

  if (pad < min_pad) pad = min_pad;
  if (pad > max_pad) pad = max_pad;

  return(n + pad);
}


typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[32];
} array_OPS;

OWN_ptr array_own_init = {0, 0};

CLU_proc array_oe_create = {{0,0,0,0}, arrayOPcreate, &array_own_init , &array_own_init};
CLU_proc array_oe_new = {{0,0,0,0}, arrayOPnew, &array_own_init, &array_own_init};
CLU_proc array_oe_predict = {{0,0,0,0}, arrayOPpredict, &array_own_init, &array_own_init};
CLU_proc array_oe_cons = {{0,0,0,0}, arrayOPcons, &array_own_init, &array_own_init};
CLU_proc array_oe_empty = {{0,0,0,0}, arrayOPempty, &array_own_init, &array_own_init};
CLU_proc array_oe_low = {{0,0,0,0}, arrayOPlow, &array_own_init, &array_own_init};
CLU_proc array_oe_high = {{0,0,0,0}, arrayOPhigh, &array_own_init, &array_own_init};
CLU_proc array_oe_size = {{0,0,0,0}, arrayOPsize, &array_own_init, &array_own_init};
CLU_proc array_oe_set_low = {{0,0,0,0}, arrayOPset_low, &array_own_init, &array_own_init};
CLU_proc array_oe_trim = {{0,0,0,0}, arrayOPtrim, &array_own_init, &array_own_init};
CLU_proc array_oe_fill = {{0,0,0,0}, arrayOPfill, &array_own_init, &array_own_init};
CLU_proc array_oe_fill_copy = {{0,0,0,0}, arrayOPfill_copy, &array_own_init, &array_own_init};
CLU_proc array_oe_fetch = {{0,0,0,0}, arrayOPfetch, &array_own_init, &array_own_init};
CLU_proc array_oe_bottom = {{0,0,0,0}, arrayOPbottom, &array_own_init, &array_own_init};
CLU_proc array_oe_top = {{0,0,0,0}, arrayOPtop, &array_own_init, &array_own_init};
CLU_proc array_oe_store = {{0,0,0,0}, arrayOPstore, &array_own_init, &array_own_init};
CLU_proc array_oe_addh = {{0,0,0,0}, arrayOPaddh, &array_own_init, &array_own_init};
CLU_proc array_oe_addl = {{0,0,0,0}, arrayOPaddl, &array_own_init, &array_own_init};
CLU_proc array_oe_remh = {{0,0,0,0}, arrayOPremh, &array_own_init, &array_own_init};
CLU_proc array_oe_reml = {{0,0,0,0}, arrayOPreml, &array_own_init, &array_own_init};
CLU_proc array_oe_equal = {{0,0,0,0}, arrayOPequal, &array_own_init, &array_own_init};
CLU_proc array_oe_similar = {{0,0,0,0}, arrayOPsimilar, &array_own_init, &array_own_init};
CLU_proc array_oe_copy = {{0,0,0,0}, arrayOPcopy, &array_own_init, &array_own_init};
CLU_proc array_oe_similar1 = {{0,0,0,0}, arrayOPsimilar1, &array_own_init, &array_own_init};
CLU_proc array_oe_copy1 = {{0,0,0,0}, arrayOPcopy1, &array_own_init, &array_own_init};
CLU_proc array_oe_indexes = {{0,0,0,0}, arrayOPindexes, &array_own_init, &array_own_init};
CLU_proc array_oe_elements = {{0,0,0,0}, arrayOPelements, &array_own_init, &array_own_init};
CLU_proc array_oe_encode = {{0,0,0,0}, arrayOPencode, &array_own_init, &array_own_init};
CLU_proc array_oe_decode = {{0,0,0,0}, arrayOPdecode, &array_own_init, &array_own_init};
CLU_proc array_oe__gcd = {{0,0,0,0}, arrayOP_gcd, &array_own_init, &array_own_init};
CLU_proc array_oe_print = {{0,0,0,0}, arrayOPprint, &array_own_init, &array_own_init};
CLU_proc array_oe_debug_print = {{0,0,0,0}, arrayOPdebug_print, &array_own_init, &array_own_init};

OWN_req array_ownreqs = {2*UNIT, 1};


array_OPS array_ops_actual = {32,
    &array_own_init,
    &array_own_init, {
{&array_oe_create, "create"},
{&array_oe_new, "new"},
{&array_oe_predict, "predict"},
{&array_oe_cons, "cons"},
{&array_oe_empty, "empty"},
{&array_oe_low, "low"},
{&array_oe_high, "high"},
{&array_oe_size, "size"},
{&array_oe_set_low, "set_low"},
{&array_oe_trim, "trim"},
{&array_oe_fill, "fill"},
{&array_oe_fill_copy, "fill_copy"},
{&array_oe_fetch, "fetch"},
{&array_oe_bottom, "bottom"},
{&array_oe_top, "top"},
{&array_oe_store, "store"},
{&array_oe_addh, "addh"},
{&array_oe_addl, "addl"},
{&array_oe_remh, "remh"},
{&array_oe_reml, "reml"},
{&array_oe_equal, "equal"},
{&array_oe_similar, "similar"},
{&array_oe_copy, "copy"},
{&array_oe_similar1, "similar1"},
{&array_oe_copy1, "copy1"},
{&array_oe_indexes, "indexes"},
{&array_oe_elements, "elements"},
{&array_oe_encode, "encode"},
{&array_oe_decode, "decode"},
{&array_oe__gcd, "_gcd"},
{&array_oe_print, "print"},
{&array_oe_debug_print, "debug_print"}
}};

array_OPS *array_ops = &array_ops_actual;

