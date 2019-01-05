
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/new_selector.c,v 1.5 91/08/29 15:19:59 dcurtis Exp $";
#endif lint
/* $Log:	new_selector.c,v $
 * Revision 1.5  91/08/29  15:19:59  dcurtis
 * fixed casting (lint)
 * 
 * Revision 1.4  91/08/29  15:04:15  dcurtis
 * added elist to proctypeOPnew calls
 * 
 * Revision 1.3  91/06/06  13:28:45  root
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  12:25:38  root
 * *** empty log message ***
 * 
 * Revision 1.1  91/02/04  15:49:46  mtv
 * Initial revision
 * 
 */

/********************************************************/
/*							*/
/*	IMPLEMENTATION of SELECTORS		        */
/*							*/
/*	    routines common to oneof,variant,		*/
/*			       struct,record  		*/
/*							*/
/********************************************************/

#include <stdio.h>
#include "pclu_err.h"
#include "pclu_sys.h"
#include <string.h>

extern errcode add_sel_ops();
extern errcode find_sel_ops();
extern errcode find_valops();

extern char * mystrcat();
extern errcode missing_print_fcn();

#define MAX_SELECTORS 1000
#define MAX_FIELDS    50

static char *sel_inst_fieldname[MAX_FIELDS];
static long sel_inst_fieldops[MAX_FIELDS];


errcode add_selector_info(field_name, index, ops)
char *field_name;
long index;
struct OPS * ops;
{
	sel_inst_fieldname[index] = field_name;
	sel_inst_fieldops[index] = (long)ops;
	signal(ERR_ok);	
}

extern void sel_ops_counts();
extern void sel_ops_names();
extern void sel_ops_fcns();
extern void sel_ops_restricts();

/********************************************************/
/*							*/
/*	Find_Selector_Ops				*/
/*							*/
/*	allocate storage for table that contains all	*/
/*	the operations that can be performed on a 	*/
/*	selector					*/
/*							*/
/********************************************************/

long generic_field_count;

errcode find_selector_ops(selname, nfields, table)
char *selname;
long nfields;
struct SELOPS **table;
{
errcode err;
struct SELOPS *temp;
long nentries;
long nops;
long i, j, jj, k, ans, index, offset;
CLUREF temp_proc;
long *temp_type_owns, *temp_op_owns;
struct OPS *ops;
char *name, *name1, *field_name;
long *op_own_ptr;
bool found;
static bool init = false;
static CLUREF mpf;

long pf_op_count, parm_op_count, plain_op_count;
char **pf_op_names, **parm_op_names, **plain_op_names;
char **parm_restrict_name;
PROC **pf_op_fcns, **parm_op_fcns, **plain_op_fcns;

/* try to find an existing ops */
	ans = find_sel_ops(selname, nfields, (struct OPS**)table);
	if (ans == true) {
		signal(ERR_ok);
		}

/* initial procedure type object for missing print function */
if (init == false) {
	err = proctypeOPnew(1, &mpf);
	if (err != ERR_ok) resignal(err);
	mpf.proc->proc = missing_print_fcn;
	init = true;
	}

/* create type owns */
	clu_alloc(UNIT, &temp_type_owns);
	temp_type_owns[0] = 1;

/* gather up some initial context */
	generic_field_count = nfields;
        sel_ops_counts(selname, &pf_op_count, &parm_op_count, &plain_op_count);
        sel_ops_names(selname, &pf_op_names, &parm_op_names, &plain_op_names);
        sel_ops_fcns(selname, &pf_op_fcns, &parm_op_fcns, &plain_op_fcns);
	sel_ops_restricts(selname, &parm_restrict_name);

/* create basic ops structure */
	nentries = parm_op_count + plain_op_count + nfields * pf_op_count;
	clu_alloc(sizeof(struct SELOPS) + 
		(nentries-1)*sizeof(struct SELOP_ENTRY), &temp);
	temp->count = nentries;
	temp->type_owns = (OWNPTR)temp_type_owns;
	temp->op_owns = (OWNPTR)temp_type_owns;
/*	temp->op_owns = 0; */

/* set up storage for parameterized operations */
/* 	--- assumes 4th entry (i == 3) is print & allocates storage for names */
/* 	--- assumes 8th entry (i == 7) is debug_print & allocates storage for names */

	for (i = 0; i < parm_op_count; i++) {
		temp->entry[i].name = parm_op_names[i];
		err = proctypeOPnew(1, &temp_proc);
		if (err != ERR_ok) resignal(err);
		temp->entry[i].fcn = temp_proc.selproc;

		if (i == 3 || i == 7) {
			clu_alloc(UNIT + nfields*sizeof(CLUPROC) + 
				nfields*sizeof(char*), &temp_op_owns);
			}
		else {
			clu_alloc(UNIT + nfields*sizeof(CLUPROC), 
				&temp_op_owns);
			}

		temp->entry[i].fcn->proc = parm_op_fcns[i];
		temp->entry[i].fcn->type_owns = (OWNPTR)temp_type_owns;
		temp->entry[i].fcn->op_owns = (OWNPTR)temp_op_owns;
		temp_op_owns[0] = 1;
		}

/* set up storage for plain operations */

	for (j = 0; j < plain_op_count; j++, i++) {
		temp->entry[i].name = plain_op_names[j];
		err = proctypeOPnew(1, &temp_proc);
		if (err != ERR_ok) resignal(err);
		temp->entry[i].fcn = temp_proc.selproc;

		temp->entry[i].fcn->proc = plain_op_fcns[j];
/*		temp->entry[i].fcn->table = NULL; */
		}

/* set up storage for postfixable operations */

	jj = 0;
	for (k = 0; k < nfields; k++) {
	for (j = 0; j < pf_op_count; j++, i++) {
		temp->entry[i].name = pf_op_names[j];
		err = proctypeOPnew(1, &temp_proc);
		if (err != ERR_ok) resignal(err);
		temp->entry[i].fcn = temp_proc.selproc;

/*		temp->entry[i].fcn->proc = plain_op_fcns[j]; */
		temp->entry[i].fcn->proc = pf_op_fcns[jj]; jj++;
/*		temp->entry[i].fcn->table = NULL; */
		}
		}

     for (index = 0; index < nfields; index++) {
	ops = (struct OPS *)sel_inst_fieldops[index];
	field_name = sel_inst_fieldname[index];
	for (i = 0; i < parm_op_count; i++) {
		name = parm_restrict_name[i];
		for (j = 0; j < ops->count; j++) {
			name1 = ops->entry[j].name;
			if (name1 == 0 || name1[0] != name[0]) continue;
			if (!(strcmp(name1, name))) {
				op_own_ptr = (long *)temp->entry[i].fcn->op_owns;
				op_own_ptr[index+1] =
					 (long)ops->entry[j].fcn;
				break;}
			}
		}
	offset = index*pf_op_count + parm_op_count + plain_op_count;
	for (i = 0; i < pf_op_count; i++) {
		temp->entry[offset+i].name = mystrcat(pf_op_names[i], field_name);
		}
	}

/* set up storage for parameterized operations */
/*	& names for postfixable operations */
/* 	--- assumes 4th entry (i == 3) is print & adds field names */
/* 	--- assumes 8th entry (i == 7) is print & adds field names */

     for (index = 0; index < nfields; index++) {
	ops = (struct OPS *)sel_inst_fieldops[index];
	field_name = sel_inst_fieldname[index];
	for (i = 0; i < parm_op_count; i++) {
		name = parm_restrict_name[i];
		found = false;
		for (j = 0; j < ops->count; j++) {
			name1 = ops->entry[j].name;
			if (name1 == 0 || name1[0] != name[0]) continue;
			if (!(strcmp(name1, name))) {
				op_own_ptr = (long *)temp->entry[i].fcn->op_owns;
				op_own_ptr[index+1] =
					 (long)ops->entry[j].fcn;
				if (i == 3 || i == 7) {
					op_own_ptr[index+1+nfields] =
						 (long)field_name;
					}
				found = true; break;}
			}
		if (found == false && (i == 3 || i == 7)) {
			op_own_ptr = (long *)temp->entry[i].fcn->op_owns;
			op_own_ptr[index+1] = (long)mpf.proc;
			op_own_ptr[index+1+nfields] = (long)field_name;
			}
		}
	offset = index*pf_op_count + parm_op_count + plain_op_count;
	for (i = 0; i < pf_op_count; i++) {
		temp->entry[offset+i].name = mystrcat(pf_op_names[i], field_name);
		}
	}

/* save ops for future users */
	add_sel_ops(selname, nfields, (struct OPS*)temp);

/* return created ops */
	*table = temp;
	signal(ERR_ok);
}

errcode missing_print_fcn(val, pst)
CLUREF val, pst;
{
static bool init = false;
static CLUREF msg;
CLUREF ans, mpf;
errcode err;

	if (init == false) {
		err = stringOPcons("no print function", CLU_1, CLU_17, &msg);  
		if (err != ERR_ok) resignal(err);
		init = true;
		}
	err = pstreamOPtext(pst, msg, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}
/* storage for following routine */

OWNPTR	record_opsptr_arr[MAX_SELECTORS]; /* instantiated ops */
OWNPTR	struct_opsptr_arr[MAX_SELECTORS]; /* instantiated ops */
OWNPTR	variant_opsptr_arr[MAX_SELECTORS]; /* instantiated ops */
OWNPTR	oneof_opsptr_arr[MAX_SELECTORS]; /* instantiated ops */

long 	record_field_count [MAX_SELECTORS];
long 	struct_field_count [MAX_SELECTORS];
long 	variant_field_count [MAX_SELECTORS];
long 	oneof_field_count [MAX_SELECTORS];

long *	record_field_vals [MAX_SELECTORS][MAX_FIELDS];
long *	struct_field_vals [MAX_SELECTORS][MAX_FIELDS];
long *	variant_field_vals [MAX_SELECTORS][MAX_FIELDS];
long *	oneof_field_vals [MAX_SELECTORS][MAX_FIELDS];

char *	record_field_names [MAX_SELECTORS][MAX_FIELDS];
char *	struct_field_names [MAX_SELECTORS][MAX_FIELDS];
char *	variant_field_names [MAX_SELECTORS][MAX_FIELDS];
char *	oneof_field_names [MAX_SELECTORS][MAX_FIELDS];

static long record_num_entries = 0;
static long struct_num_entries = 0;
static long variant_num_entries = 0;
static long oneof_num_entries = 0;

/* routine to initialize structures used by find_owns */
void find_selops_init(ans1, ans2, ans3, ans4)
OWNPTR *ans1, *ans2, *ans3, *ans4;
{
long i,j;

/* removed 1/28/91 to speed up start_up dwc
	for (i = 0; i < MAX_SELECTORS; i++) {

		record_opsptr_arr[i] = 0;
		struct_opsptr_arr[i] = 0;
		variant_opsptr_arr[i] = 0;
		oneof_opsptr_arr[i] = 0;

		record_field_count[i] = 0;
		struct_field_count[i] = 0;
		variant_field_count[i] = 0;
		oneof_field_count[i] = 0;
		for (j = 0; j < MAX_FIELDS; j++) {
			record_field_vals[i][j]    = (long*) 0;
			struct_field_vals[i][j]    = (long*) 0;
			variant_field_vals[i][j]    = (long*) 0;
			oneof_field_vals[i][j]    = (long*) 0;
			}
		}
*/
	*ans1 = (OWNPTR)record_opsptr_arr;
	*ans2 = (OWNPTR)struct_opsptr_arr;
	*ans3 = (OWNPTR)variant_opsptr_arr;
	*ans4 = (OWNPTR)oneof_opsptr_arr;
	return;
}

/* routine to find selector ops given selector name, # of fields */
/*	type info for fields is in sel_inst_info arrays */

long find_sel_ops(selname, count, result)
char *selname;
long count;
struct OPS **result;
{
long i, j;
bool found = false;
struct OPS *new_owns;
long *pcount;
OWNPTR *table;
long *(*parm_vals)[MAX_FIELDS];
char *(*parm_names)[MAX_FIELDS];
long *parm_count;


	/* if too many fields, then die */

	if (count >= MAX_FIELDS) {
		fprintf(stderr, 
			"find_sel_ops: too many fields: increase MAX_FIELDS\n");
		exit(-10);
		}

	/* first select which table */
	if (selname[0] == 'r') {
		pcount = &record_num_entries;
		table = record_opsptr_arr;
		parm_count = record_field_count;
		parm_vals = record_field_vals;
		parm_names = record_field_names;
		}
	if (selname[0] == 's') {
		pcount = &struct_num_entries;
		table = struct_opsptr_arr;
		parm_count = struct_field_count;
		parm_vals = struct_field_vals;
		parm_names = struct_field_names;
		}
	if (selname[0] == 'v') {
		pcount = &variant_num_entries;
		table = variant_opsptr_arr;
		parm_count = variant_field_count;
		parm_vals = variant_field_vals;
		parm_names = variant_field_names;
		}
	if (selname[0] == 'o') {
		pcount = &oneof_num_entries;
		table = oneof_opsptr_arr;
		parm_count = oneof_field_count;
		parm_vals = oneof_field_vals;
		parm_names = oneof_field_names;
		}


	found = false;
	for (i = 0 ; i < *pcount; i++) {
		if ((parm_count)[i] != count) continue;
		found = true;
		for (j = 0; j < count; j++) {
			if ((sel_inst_fieldops[j] ==
				(long)(parm_vals)[i][j]) &&
			    ((parm_names)[i][j] ==
				(char*)sel_inst_fieldname[j])) 
			continue;
			else {found = false; break;}
			}
		if (found) break;
		}



	if (found) {
	/* entry found: return owns */
		*result = (struct OPS *)table[i];
		return (true);
		}
	else {
		return (false);
		}
	}

errcode add_sel_ops(selname, count, new_ops)
char * selname;
long count;
struct OPS *new_ops;
{
long j;
long *pcount;
OWNPTR *table;
long *(*parm_vals)[MAX_FIELDS];
char *(*parm_names)[MAX_FIELDS];
long *parm_count;

	/* first select which table */
	if (selname[0] == 'r') {
		pcount = &record_num_entries;
		table = record_opsptr_arr;
		parm_count = record_field_count;
		parm_vals = record_field_vals;
		parm_names = record_field_names;
		}
	if (selname[0] == 's') {
		pcount = &struct_num_entries;
		table = struct_opsptr_arr;
		parm_count = struct_field_count;
		parm_vals = struct_field_vals;
		parm_names = struct_field_names;
		}
	if (selname[0] == 'v') {
		pcount = &variant_num_entries;
		table = variant_opsptr_arr;
		parm_count = variant_field_count;
		parm_vals = variant_field_vals;
		parm_names = variant_field_names;
		}
	if (selname[0] == 'o') {
		pcount = &oneof_num_entries;
		table = oneof_opsptr_arr;
		parm_count = oneof_field_count;
		parm_vals = oneof_field_vals;
		parm_names = oneof_field_names;
		}

		table[*pcount] = (OWNPTR) new_ops;
		(parm_count)[*pcount] = count;
		for (j = 0 ; j < count; j++) {
			(parm_vals)[*pcount][j] =
				(long*)sel_inst_fieldops[j]; 
			(parm_names)[*pcount][j] =
				(char*)sel_inst_fieldname[j]; 
			}
		(*pcount)++;
		if (*pcount == MAX_SELECTORS) {
			fprintf(stderr, 
		"add_sel_ops: too many instantiations: increase MAX_SELECTORS\n");
			exit(-10);
			}
		signal(ERR_ok);
		}


/********************************************************/
/*							*/
/*	given the ops corresponding to a selector       */
/*		and an op on that type                  */
/*							*/
/*	find name of selector                           */
/*	find the number of values for that operation    */
/*	find ops for the result of that operation       */
/*							*/
/*	these ops may be NULL_OPS, (e.g. print)		*/
/*                       bool_ops, (e.g. equal)		*/
/*                       input ops, (e.g. copy)		*/
/*	                 per field ops, (e.g. get_)	*/
/*                    or equivalent ops (e.g. o2v)	*/
/*							*/
/********************************************************/

errcode rep_find_valops(opname, opsptr, anstyp, ansnum, ansops, ansnfield)
CLUREF opname, opsptr;
CLUREF *anstyp, *ansnum, *ansops, *ansnfield;
{
static bool init = false;
static CLUREF rec, str, one, var;
errcode err;
CLUREF ops;

	if (init == false) {
		stringOPcons("record", CLU_1, CLU_6, &rec);
		stringOPcons("struct", CLU_1, CLU_6, &str);
		stringOPcons("oneof", CLU_1, CLU_5, &one);
		stringOPcons("variant", CLU_1, CLU_7, &var);
		init = true;
		}
/*	ops.num = *((long*)(opsptr.ref)); */
	ops.num = opsptr.num;
	err = find_valops(rec, opname, ops, ansnum, ansops, ansnfield);
	if (err == ERR_ok) {
		anstyp->str = rec.str;
		signal(ERR_ok);
		}
	err = find_valops(str, opname, ops, ansnum, ansops, ansnfield);
	if (err == ERR_ok) {
		anstyp->str = str.str;
		signal(ERR_ok);
		}
	err = find_valops(one, opname, ops, ansnum, ansops, ansnfield);
	if (err == ERR_ok) {
		anstyp->str = one.str;
		signal(ERR_ok);
		}
	err = find_valops(var, opname, ops, ansnum, ansops, ansnfield);
	if (err == ERR_ok) {
		anstyp->str = var.str;
		signal(ERR_ok);
		}
	signal(ERR_not_found);
	}

/********************************************************/
/*							*/
/*	given the ops corresponding to a selector       */
/*		and an op on that selector type         */
/*							*/
/*	find # of results for that operation            */
/*	find ops for the result of that operation       */
/*	find ordinal of field if relevant		*/
/*							*/
/*	these ops may be NULL_OPS, (e.g. print)		*/
/*                       bool_ops, (e.g. equal)		*/
/*                       input ops, (e.g. copy)		*/
/*	                 per field ops, (e.g. get_)	*/
/*                    or equivalent ops (e.g. o2v)	*/
/*							*/
/*	signals are ignored for the time being		*/
/*	equivalent ops are also ignored 		*/
/*		(input ops are used)                    */
/********************************************************/

extern struct OPS *int_ops;
extern struct OPS *bool_ops;
errcode find_valops(selname, opname, ops, ans1, ans2, ans3)
CLUREF selname, ops, opname;
CLUREF *ans1, *ans2, *ans3;
{
long i, j;
bool found;
long *pcount;
OWNPTR *table;
long *(*parm_vals)[MAX_FIELDS];
char *(*parm_names)[MAX_FIELDS];
long *parm_count;
char *field;
long oplen;
bool pfo = false;
bool pfo_known_ops = false;
bool ans_known = false;
char *nm = opname.str->data;
char *selnm = selname.str->data;


	/* look at operation and decide how to find valops */
	/* dispense with easy albeit unlikely operations */
	if (nm[0] == '_') {    			  /* _gcd */
		ans1->num = 1;
		ans2->num = (long)int_ops;
		ans3->num = 0;
		ans_known = true;
		}
	if ((nm[0] == 'i') ||			  /* is_ */
		(nm[0] == 'e' && nm[1] == 'q') || /* equal */
		(nm[0] == 's' && nm[1] == 'i')    /* similar, similar1 */
		) {
		ans1->num = 1;
		ans2->num = (long)bool_ops;
		if (nm[0] == 'i') {
			pfo_known_ops = true;
			}
		else {
			ans3->num = 0;
			ans_known = true;
			}
		}
	if ((nm[0] == 's' && nm[1] == '2') ||	  /* s2r */
		(nm[0] == 'o' && nm[1] == '2')    /* o2v */
		) {
		/* actually equivalent ops: should do instantiation */
		ans1->num = 1;
		ans2->num = (long)ops.num;
		ans3->num = 0;
		ans_known = true;
		}
	if ((nm[0] == 'g') ||			  /* get_ */
		(nm[0] == 'v' && nm[1] == 'a'))   /* value_ */
		{pfo = true;}
	if ((nm[0] == 'd') ||			  /* decode */
		(nm[1] == '2') ||		  /* v20, r2s */
		(nm[0] == 'c' && nm[1] == 'o')    /* copy, copy1 */
		) {
		ans1->num = 1;
		ans2->num = (long)ops.num;
		ans3->num = 0;
		ans_known = true;
		}
	if ((nm[0] == 'm') ||			  /* make_ */
		(nm[0] == 'r' && nm[1] == 'e')    /* replace_ */
		) {
		ans1->num = 1;
		ans2->num = (long)ops.num;
		pfo_known_ops = true;
		}
	if (!pfo && !pfo_known_ops && !ans_known) {
		ans1->num = 0;
		ans2->num = (long)NULL_OPS;
		if ((nm[0] == 'c') || (nm[0] == 's')) {
			pfo_known_ops = true;
			}
		else {
			ans3->num = 0;
			ans_known = true;
			}
		}
	

	/* we have a postfixable operation & need to look up valops in table */
	/* get field name (postfix) */
	field = index(nm, '_') + 1;

	/* decide what type we have and get corresponding tables */
	if (selnm[0] == 'r') {
		pcount = &record_num_entries;
		table = record_opsptr_arr;
		parm_count = record_field_count;
		parm_vals = record_field_vals;
		parm_names = record_field_names;
		}
	if (selnm[0] == 's') {
		pcount = &struct_num_entries;
		table = struct_opsptr_arr;
		parm_count = struct_field_count;
		parm_vals = struct_field_vals;
		parm_names = struct_field_names;
		}
	if (selnm[0] == 'v') {
		pcount = &variant_num_entries;
		table = variant_opsptr_arr;
		parm_count = variant_field_count;
		parm_vals = variant_field_vals;
		parm_names = variant_field_names;
		}
	if (selnm[0] == 'o') {
		pcount = &oneof_num_entries;
		table = oneof_opsptr_arr;
		parm_count = oneof_field_count;
		parm_vals = oneof_field_vals;
		parm_names = oneof_field_names;
		}

	/* get number of such instantiations and list */
	found = false;
	for (i = 0 ; i < *pcount ; i++) {
		if ((OWNPTR)ops.num == table[i]) {found = true; break;}
		}
	if (found == false) signal(ERR_not_found);
	if (ans_known) signal(ERR_ok);
	found = false;
	for (j = 0; j < parm_count[i]; j++) {
		if (!strcmp(field,parm_names[i][j])) {found = true; break;}
		}
	if (found == false) signal(ERR_not_found);
	if (pfo) {
		ans1->num = 1;
		ans2->num = parm_vals[i][j];
		}
	ans3->num = j+1;
	signal(ERR_ok);
	}

struct {
long count;
long init_flags[40];
} generic_inst_info;

struct {
long count;
} generic_own_reqs = {0};

/********************************************************/
/*							*/
/*	selectors have three types of operations	*/
/*							*/
/*	1. postfixable: e.g. get_, is_			*/
/*	2. parameterized: e.g. record$copy requires 	*/
/*				t$copy for each field 	*/
/*	3. plain: e.g. record$copy1 (not postfixable, 	*/
/*				not parameterized)	*/
/*							*/
/********************************************************/
#include "selector.h"

/********************************************************/
/*							*/
/*	Sel_Ops_Counts					*/
/*		returns the numbers of 			*/
/*			each type of operation		*/
/*		based on the name of the selector	*/
/*							*/
/********************************************************/


void sel_ops_counts(name, pfcount, paramcount, plaincount)
char * name;
long *pfcount, *paramcount, *plaincount;
{
long i;

/*	if (strcmp(name, "oneof") == 0) { */
	if (name[0] == 'o') {
		*pfcount = 3;
		*paramcount = 8;
		*plaincount = 2;
		return; }
/*	if (strcmp(name, "variant") == 0) { */
	if (name[0] == 'v') {
		*pfcount = 4;
		*paramcount = 8;
		*plaincount = 4;
		return; }
/*	if (strcmp(name, "record") == 0) { */
	if (name[0] == 'r') {
		*pfcount = 2;
		*paramcount = 8;
		*plaincount = 4;
		return; }
/*	if (strcmp(name, "struct") == 0) { */
	if (name[0] == 's') {
		*pfcount = 2;
		*paramcount = 8;
		*plaincount = 2;
		return; }
	fprintf(stderr, "sel_ops_counts: bad name %s\n", name);
	exit(-1);
	}

/********************************************************/
/*							*/
/*	Sel_Ops_Names					*/
/*		returns the list of the names for	*/
/*			each type of operation		*/
/*		based on the name of the selector	*/
/*							*/
/********************************************************/


void sel_ops_names(name, pfname, paramname, plainname)
char * name;
char ***pfname, ***paramname, ***plainname;
{
long i;

/*	if (strcmp(name, "oneof") == 0) { */
	if (name[0] == 'o') {
		*pfname = oneof_prefix_name_table;
		*paramname = oneof_param_name_table;
		*plainname = oneof_plain_name_table;
		return; }
/*	if (strcmp(name, "variant") == 0) { */
	if (name[0] == 'v') {
		*pfname = variant_prefix_name_table;
		*paramname = variant_param_name_table;
		*plainname = variant_plain_name_table;
		return; }
/*	if (strcmp(name, "record") == 0) { */
	if (name[0] == 'r') {
		*pfname = record_prefix_name_table;
		*paramname = record_param_name_table;
		*plainname = record_plain_name_table;
		return; }
/*	if (strcmp(name, "struct") == 0) { */
	if (name[0] == 's') {
		*pfname = struct_prefix_name_table;
		*paramname = struct_param_name_table;
		*plainname = struct_plain_name_table;
		return; }
	fprintf(stderr, "sel_ops_names: bad name %s\n", name);
	exit(-1);
	}

/********************************************************/
/*							*/
/*	Sel_Ops_Fcns					*/
/*		returns the list of the functions for	*/
/*			each type of operation		*/
/*		based on the name of the selector	*/
/*							*/
/********************************************************/


void sel_ops_fcns(name, pffcn, paramfcn, plainfcn)
char * name;
PROC ***pffcn, ***paramfcn, ***plainfcn;
{
long i;

/*	if (strcmp(name, "oneof") == 0) { */
	if (name[0] == 'o') {
		*pffcn = oneof_prefix_fcn_table;
		*paramfcn = oneof_param_fcn_table;
		*plainfcn = oneof_plain_fcn_table;
		return; }
/*	if (strcmp(name, "variant") == 0) { */
	if (name[0] == 'v') {
		*pffcn = variant_prefix_fcn_table;
		*paramfcn = variant_param_fcn_table;
		*plainfcn = variant_plain_fcn_table;
		return; }
/*	if (strcmp(name, "record") == 0) { */
	if (name[0] == 'r') {
		*pffcn = record_prefix_fcn_table;
		*paramfcn = record_param_fcn_table;
		*plainfcn = record_plain_fcn_table;
		return; }
/*	if (strcmp(name, "struct") == 0) { */
	if (name[0] == 's') {
		*pffcn = struct_prefix_fcn_table;
		*paramfcn = struct_param_fcn_table;
		*plainfcn = struct_plain_fcn_table;
		return; }
	fprintf(stderr, "sel_ops_fcns: bad name %s\n", name);
	exit(-1);
	}

/********************************************************/
/*							*/
/*	Sel_Ops_Restricts				*/
/*		returns the list of required functions  */
/*			one for each parameterized op	*/
/*							*/
/********************************************************/


void sel_ops_restricts(name, parm_reqs_names)
char * name;
char ***parm_reqs_names;
{
long i;

/*	if (strcmp(name, "oneof") == 0) { */
	if (name[0] == 'o') {
		*parm_reqs_names = oneof_reqs_name_table;
		return; }
/*	if (strcmp(name, "variant") == 0) { */
	if (name[0] == 'v') {
		*parm_reqs_names = variant_reqs_name_table;
		return; }
/*	if (strcmp(name, "record") == 0) { */
	if (name[0] == 'r') {
		*parm_reqs_names = record_reqs_name_table;
		return; }
/*	if (strcmp(name, "struct") == 0) { */
	if (name[0] == 's') {
		*parm_reqs_names = struct_reqs_name_table;
		return; }
	fprintf(stderr, "sel_ops_fcns: bad name %s\n", name);
	exit(-1);
	}
/*********************************************************
long sel_ops_count(name, prefix_name_table, restrict_fcn_table, field_access_table)
char *name;
char ***prefix_name_table;
char **record_plain_name_table = {"o2v", v2o"};
PROC ***restrict_fcn_table;
PROC ***field_access_table;
{
long i;

	if (strcmp(name, "oneof") == 0) {
		*prefix_name_table = oneof_prefix_name_table;	
		*restrict_fcn_table = oneof_restrict_fcn_table;	
		*field_access_table = oneof_field_access_ops;	
		return(3); }
	if (strcmp(name, "variant") == 0) {
		*prefix_name_table = variant_prefix_name_table;	
		*restrict_fcn_table = variant_restrict_fcn_table;	
		*field_access_table = variant_field_access_ops;	
		return(4); }
	if (strcmp(name, "record") == 0) {
		*prefix_name_table = record_prefix_name_table;	
		*restrict_fcn_table = record_restrict_fcn_table;	
		*field_access_table = record_field_access_ops;	
		return(2); }
	if (strcmp(name, "struct") == 0) {
		*prefix_name_table = struct_prefix_name_table;	
		*restrict_fcn_table = struct_restrict_fcn_table;	
		*field_access_table = struct_field_access_ops;	
		return(2); }
	fprintf(stderr, "sel_ops_count: bad name %s\n", name);
	exit(-1);
	}
**********************************************************/
