
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/new_parm_util.c,v 1.6 91/08/29 16:09:32 dcurtis Exp $";
#endif 
/* $Log:	new_parm_util.c,v $
 * Revision 1.6  91/08/29  16:09:32  dcurtis
 * no change
 * 
 * Revision 1.5  91/08/29  15:14:50  dcurtis
 * fixed casting (lint)
 * 
 * Revision 1.4  91/06/06  13:28:29  root
 * added copyright notice
 * 
 * Revision 1.3  91/06/03  11:15:44  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.2  91/05/31  12:21:45  root
 * made references to include files less specific
 * 
 * Revision 1.1  91/02/04  15:49:45  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*	parameterized type utilities		*/
/*						*/

#include <stdio.h>
#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode build_parm_ops_table2();
extern errcode build_parm_table2();
extern errcode update_parm_table2();
extern errcode update_type_ops();
extern errcode update_op_ops();
extern errcode add_ops();
extern errcode update_ops();
extern errcode find_ops();

#define MAX_INSTS 500
#define MAX_PARMS 20

static long inst_info_value[MAX_PARMS];
static long inst_info_reqs[MAX_PARMS];   /* reqs for types, 0 for consts */
static long current_tdefs;
static long current_odefs;

static long fti_tdefs;
errcode find_type_instance(aops, nparm, ownreqp, result)
struct OPS *aops;
long nparm;
OWNREQ ownreqp;
struct OPS **result;
{
long ans, i, size, tdefs;
long *temp_owns;
struct OPS *temp;

/* see if the ops table already exists */
	ans = find_ops(aops, 0, nparm, result);
	if (ans == true) {
		if (current_tdefs != 0) update_type_ops(nparm, ownreqp, result);
		signal(ERR_ok);
		}

/* it doesn't exist: build it */

	/* allocate type owns */
	size = ownreqp->size;
	if (size == 0) size = UNIT;
	clu_alloc(size, &temp_owns);
	temp_owns[0] = 0;

	/* allocate and build ops table */
	build_parm_ops_table2(aops, nparm, (OWNPTR)temp_owns, &temp);

	/* put parm values into own structure */
	/*   put const value for const parameters */
	/*   build parm table per reqs for type parameters and put in owns */
	tdefs = 0;
	for (i = 0; i < nparm; i++) {
		if (inst_info_reqs[i] == 0) {
			temp_owns[i+ownreqp->own_count] = inst_info_value[i];
			continue;
			}
		build_parm_table2((struct REQS*)inst_info_reqs[i], 
			  (struct OPS*)inst_info_value[i],
			  (struct OPS**)&temp_owns[i+ownreqp->own_count],
			   &tdefs);
		}

/* save the result for the future */
	add_ops(aops, 0, nparm, temp, tdefs, 0);
	fti_tdefs = tdefs;	/* for use by find_typeop_instance */

/* hand the result back to the caller */
	*result = temp;
	signal(ERR_ok);
	}


errcode find_typeop_instance_old(aops, nparm, ntparm, ownreqp, townreqp, result)
struct OPS* aops;
long nparm, ntparm;
OWNREQ ownreqp, townreqp;
struct OPS **result;
{
long ans, i, size, tdefs, odefs;
struct OPS *temp;
long *temp_owns;

/* look up type/op instance and return it if it exists */
	ans = find_ops(aops, 0, nparm, result);
	if (ans == true) signal(ERR_ok);

/* it doesn't exist: make find_type_instance find/build type instance */
	find_type_instance(aops, ntparm, townreqp, &temp);

/* allocate op own structure & put into type ops */
	size = ownreqp->size;
	if (size == 0) size = UNIT;
	clu_alloc(size, &temp_owns);
	temp->op_owns = (OWNPTR)temp_owns;

/* build parm tables for op and stick them into the op owns */
	/*   put const value for const parameters */
	/*   build parm table per reqs for type parameters and put in owns */
	for (i = 0; i < nparm-ntparm; i++) {
		if (inst_info_reqs[ntparm+i] == 0) {
			temp_owns[i+ownreqp->own_count] = 
					inst_info_value[ntparm+i];
			continue;
			}
		build_parm_table2((struct REQS *)inst_info_reqs[ntparm+i], 
			 (struct OPS *)inst_info_value[ntparm+i],
			 (struct OPS **)&temp_owns[i+ownreqp->own_count]);
		}

/* save result for the future */
	add_ops(aops, 0, nparm, temp, 0, 0);

/* hand the result back to the caller */
	*result = temp;
	signal(ERR_ok);
	}

errcode find_typeop_instance(aops, procaddr, nparm, ntparm, ownreqp, townreqp, result)
struct OPS* aops;
errcode (*procaddr)();
long nparm, ntparm;
OWNREQ ownreqp, townreqp;
struct OPS **result;
{
long ans, i, size, tdefs, odefs;
struct OPS *type_ops, *temp;
long *temp_owns;

/* look up type/op instance and return it if it exists */
	ans = find_ops(aops, procaddr, nparm, result);
	if (ans == true) {
		if (current_tdefs != 0) update_type_ops(ntparm, townreqp, result);
		if (current_odefs != 0) update_op_ops(nparm, ntparm, ownreqp, result);
		signal(ERR_ok);}

/* it doesn't exist: make find_type_instance find/build type instance */
	find_type_instance(aops, ntparm, townreqp, &type_ops);
	tdefs = fti_tdefs;

/* build ops structure */
	clu_alloc(sizeof(struct OPS), &temp);
	temp->type_owns = type_ops->type_owns;

/* allocate op own structure & put into type ops */
	size = ownreqp->size;
	if (size == 0) size = UNIT;
	clu_alloc(size, &temp_owns);
	temp->op_owns = (OWNPTR)temp_owns;

/* build parm tables for op and stick them into the op owns */
	/*   put const value for const parameters */
	/*   build parm table per reqs for type parameters and put in owns */
	odefs = 0;
	for (i = 0; i < nparm-ntparm; i++) {
		if (inst_info_reqs[ntparm+i] == 0) {
			temp_owns[i+ownreqp->own_count] = 
					inst_info_value[ntparm+i];
			continue;
			}
		build_parm_table2((struct REQS*)inst_info_reqs[ntparm+i], 
			 (struct OPS *)inst_info_value[ntparm+i],
			 (struct OPS **)&temp_owns[i+ownreqp->own_count],
			  &odefs);
		}

/* save result for the future */
	add_ops(aops, procaddr, nparm, temp, tdefs, odefs);

/* hand the result back to the caller */
	*result = temp;
	signal(ERR_ok);
	}



errcode find_prociter_instance(procaddr, nparm, ownreqp, result)
errcode (*procaddr)();
long nparm;
OWNREQ ownreqp;
struct OPS **result;
{
long ans, i, size, odefs;
struct OPS *temp;
long *temp_owns;

/* look up type/op instance and return it if it exists */
	ans = find_ops((struct OPS*)0, procaddr, nparm, result);
	if (ans == true) {
		if (current_odefs != 0) update_op_ops(nparm, 0, ownreqp, result);
		signal(ERR_ok);}

/* build ops structure */
	clu_alloc(sizeof(struct OPS), &temp);
	temp->type_owns = 0;

/* build own structure */
	size = ownreqp->size;
	if (size == 0) size = UNIT;
	clu_alloc(size, &temp_owns);
	temp->op_owns = (OWNPTR)temp_owns;

/* build parm tables for op and stick them into the op owns */
	/*   put const value for const parameters */
	/*   build parm table per reqs for type parameters and put in owns */
	odefs = 0;
	for (i = 0; i < nparm; i++) {
		if (inst_info_reqs[i] == 0) {
			temp_owns[i+ownreqp->own_count] = inst_info_value[i];
			continue;
			}
		build_parm_table2((struct REQS*)inst_info_reqs[i], 
				(struct OPS*)inst_info_value[i],
				(struct OPS**)&temp_owns[i+ownreqp->own_count],
				 &odefs);
		}

/* save result for the future */
	add_ops((struct OPS*)0, procaddr, nparm, temp, 0, odefs);

/* hand the result back to the caller */
	*result = temp;
	signal(ERR_ok);
	}

errcode build_parm_table2(reqs, ops, table, defs)
struct REQS * reqs;
struct OPS * ops;
struct OPS ** table;
long *defs;
{
long i,j;
char *name, *name1;
struct OPS * temp;
errcode err;

/* REQS -> OPS                  \/   	3/29/90 */
	clu_alloc(sizeof(struct OPS) + 
		sizeof(struct OP_ENTRY) * (reqs->count), &temp);
/* added one to number of entries to cover print function for debugger 11/12/91*/
	temp->count = reqs->count + 1;
/* defs calculated number of fcns missing vs reqs */
	*defs += reqs->count + 1;
/* checked ops before use 11/12/91 */
	if (ops == 0) {
		temp->type_owns = 0;
		temp->op_owns = 0;
		*table = temp; 
		signal(ERR_ok);}
	temp->type_owns = ops->type_owns;
	temp->op_owns = ops->op_owns;
	for (i = 0; i < reqs->count; i++) {
		name = reqs->entry[i].name;
		for (j = 0; j < ops->count; j++) {
			name1 = ops->entry[j].name;
			if (name1 == 0 || name1[0] != name[0]) continue;
			if (!(strcmp(name1, name))) {
				*defs--;
				temp->entry[i].name = name1;
				temp->entry[i].fcn = ops->entry[j].fcn;
				break;}
			}
		}
/* add in debug_print function for debugger 11/12/91 */
	for (j = 0; j < ops->count; j++) {
		name1 = ops->entry[j].name;
		if (name1 == 0) continue;
		if (!(strcmp(name1, "debug_print"))) {
			temp->entry[i].name = name1;
			temp->entry[i].fcn = ops->entry[j].fcn;
			*defs--;
			break;}
		}
	*table = temp;
	signal(ERR_ok);
	}

/* suppose an instantiation without any where clauses has occurred */
/* suppose the same instantiation is desired but now there are where clauses */
/* let's fill them in */

errcode update_type_ops(nparm, ownreqp, table)
long nparm;
OWNREQ ownreqp;
struct OPS ** table;
{
long i, tdefs;
long *temp_owns;

	tdefs = current_tdefs;
	temp_owns = (long*)(*table)->type_owns;
	for (i = 0; i < nparm; i++) {
		if (inst_info_reqs[i] == 0) {
			continue;
			}
		update_parm_table2((struct REQS*)inst_info_reqs[i], 
			  (struct OPS*)inst_info_value[i],
			  (struct OPS**)&temp_owns[i+ownreqp->own_count],
			   &tdefs);
		}
	current_tdefs = tdefs;
	update_ops();
	}

errcode update_op_ops(nparm, ntparm, ownreqp, table)
long nparm, ntparm;
OWNREQ ownreqp;
struct OPS ** table;
{
long i, odefs;
long *temp_owns;

	odefs = current_odefs;
	temp_owns = (long*)(*table)->op_owns;
	for (i = 0; i < nparm-ntparm; i++) {
		if (inst_info_reqs[ntparm+i] == 0) {
			continue;
			}
		update_parm_table2((struct REQS*)inst_info_reqs[ntparm+i], 
			  (struct OPS*)inst_info_value[ntparm+i],
			  (struct OPS**)&temp_owns[i+ownreqp->own_count],
			   &odefs);
		}
	current_odefs = odefs;
	}

errcode update_parm_table2(reqs, ops, table, defs)
struct REQS * reqs;
struct OPS * ops;
struct OPS ** table;
long *defs;
{
struct OPS * temp;
long i,j;
char *name, *name1;

	temp = *table;
/* added one to number of entries to cover print function for debugger 11/12/91*/
	temp->count = reqs->count + 1;     /* count sb ok already */
/* checked ops before use 11/12/91 */
	if (ops == 0) {
		*table = temp; 
		signal(ERR_ok);}
	temp->type_owns = ops->type_owns;  /* type owns sb ok already */
	temp->op_owns = ops->op_owns;      /* op owns sb ok already */
	for (i = 0; i < reqs->count; i++) {
		if (temp->entry[i].fcn != (CLUPROC)0) continue; /* entry already ok */
		name = reqs->entry[i].name;
		for (j = 0; j < ops->count; j++) {
			name1 = ops->entry[j].name;
			if (name1 == 0 || name1[0] != name[0]) continue;
			if (!(strcmp(name1, name))) {
				*defs--;
				temp->entry[i].name = name1;
				temp->entry[i].fcn = ops->entry[j].fcn;
				break;}
			}
		}
/* add in debug_print function for debugger 11/12/91 */
	for (j = 0; j < ops->count; j++) {
		name1 = ops->entry[j].name;
		if (name1 == 0) continue;
		if (!(strcmp(name1, "debug_print"))) {
			*defs--;
			temp->entry[i].name = name1;
			temp->entry[i].fcn = ops->entry[j].fcn;
			break;}
		}
	*table = temp;
	signal(ERR_ok);
	}

errcode build_parm_ops_table2(base_ops, nparams, inst_owns, ans)
struct OPS *base_ops;
long nparams;		/* input to proctype$new, to be obsolete */
OWNPTR inst_owns;
struct OPS **ans;
{
long i, size;
errcode err;
CLUREF temp2, num;
struct OPS *temp;

	size = sizeof(struct OPS) + (base_ops->count - 1) * sizeof(struct OP_ENTRY);
	clu_alloc(size, &temp);
	temp->count = base_ops->count;
	temp->type_owns = inst_owns;
	for (i = 0; i < base_ops->count; i++) {
		temp->entry[i].name  = base_ops->entry[i].name;
		num.num = nparams;
		err = proctypeOPnew(num, &temp2);
		if (err != ERR_ok) resignal(err);
		temp->entry[i].fcn = temp2.proc;
		temp->entry[i].fcn->proc = base_ops->entry[i].fcn->proc;
		temp->entry[i].fcn->type_owns = inst_owns;
		temp->entry[i].fcn->op_owns = inst_owns;
		}
	*ans = temp;
	signal(ERR_ok);
	}


/* insert table describing type parameter */
/* OBSOLETE
errcode init_parm_ops_table_tentry(full_ops_table, nth, nth_ops_table)
struct OPS *full_ops_table;
long nth;
struct OPS *nth_ops_table;
{
long i;
	for (i = 0; i < full_ops_table->count; i++) {
		full_ops_table->entry[i].fcn->tables[nth] = nth_ops_table;
		}
	signal(ERR_ok);
	}
*/

/* insert table describing constant parameter */
/* OBSOLETE
errcode init_parm_ops_table_centry(full_ops_table, nth, nth_ops_val)
struct OPS *full_ops_table;
long nth;
CLUREF nth_ops_val;
{
long i;
	for (i = 0; i < full_ops_table->count; i++) {
		full_ops_table->entry[i].fcn->const_parms[nth]
				 = nth_ops_val.num;
		}
	signal(ERR_ok);
	}
*/


/* save information about a type parameter */
/*	for use in instance finding routines */

void add_parm_info_type(nth_entry, ptr_to_ops, ptr_to_reqs)
long nth_entry;
struct OPS *ptr_to_ops;
struct REQS *ptr_to_reqs;
{
	inst_info_value[nth_entry] = (long)ptr_to_ops; 
	inst_info_reqs[nth_entry] = (long)ptr_to_reqs; 
	return;
}

/* save information about a constant parameter */
/*	for use in instance finding routines */

void add_parm_info_const(nth_entry, value)
long nth_entry;
CLUREF value;
{
	inst_info_value[nth_entry] = value.num; 
	inst_info_reqs[nth_entry] = 0; 
	return;
}

/* storage for following routine */

OWNREQ  ops_arr[MAX_INSTS];   /* abstract ops */
errcode  (*ops_proc[MAX_INSTS])();   /* abstract proc */
OWNPTR	opsptr_arr[MAX_INSTS]; /* instantiated ops */

long *	parm_vals [MAX_INSTS][MAX_PARMS];
long *	parm_types [MAX_INSTS][MAX_PARMS]; /* 0 => const, !0 => type */
long     parm_types_defs [MAX_INSTS]; /* count of missing fcns on type*/
long     parm_ops_defs [MAX_INSTS]; /* count of missing fcns per op/proc*/
long num_entries = 0;

/* routine to initialize structures used by find_ops */
void find_ops_init(ans1, ans2, ans3)
OWNPTR *ans1;
OWNREQ *ans2;
void **ans3;
/*errcode (**ans3)(); */
{
long i,j;
/* removed 1/28/91 to speed up start_up dwc
	for (i = 0; i < MAX_INSTS; i++) {
		opsptr_arr[i] = 0;
		ops_arr[i] = 0;
		ops_proc[i] = 0;
		for (j = 0; j < MAX_PARMS; j++) {
			parm_vals[i][j]    = (long*) 0;
			parm_types[i][j]   = (long*) 0;
			}
		}
*/
	*ans1 = (OWNPTR)opsptr_arr;
	*ans2 = (OWNREQ)ops_arr;
	*ans3 = (void*)ops_proc;
	return;
}

/* routine to find ops given type and instance information */

long find_ops(aops, procaddr, count, ptr_to_opsptr)
struct OPS *aops;
errcode (*procaddr)();
long count;
struct OPS **ptr_to_opsptr;
{
long i, j;
bool found = false;
struct OPS *new_owns, *ops1, *ops2;


	/* if too many parms, then die */

	if (count >= MAX_PARMS) {
		fprintf(stderr, 
			"find_ops: too many parameters: increase MAX_PARMS\n");
		exit(-10);
		}

	/* first match ownreqs to ownreq_arr entries */

	for (i = 0; i < num_entries; i++) {
		if (aops == (struct OPS *)ops_arr[i] && procaddr == ops_proc[i]) {
			found = true;

			/* ownreqs matches: see if instance information lines up */

			for (j = 0 ; j < count; j++) {
				if (parm_types[i][j] == 0) {
					/* make sure instance is a constant */
					/* and check constant value equality */
					if (inst_info_reqs[j] == 0 &&
						inst_info_value[j] ==
						(long)parm_vals[i][j]) continue;
					else {found = false; break;}
					}
				else {
					/* make sure isntance is a type */
					/* and check type match via type_owns */
					if (inst_info_reqs[j] != 0) {
						ops1 = (struct OPS*)inst_info_value[j];
						ops2 = (struct OPS*)parm_vals[i][j];
						if (ops1->type_owns == ops2->type_owns) continue;
						else {found = false; break;}
						}
					else {found = false; break;}
					}
				}
			}
		if (found) break;
		}


	if (found) {
	/* entry found: return owns */
		*ptr_to_opsptr = (struct OPS *)opsptr_arr[i];
		current_tdefs = parm_types_defs[i];
		current_odefs = parm_ops_defs[i];
		return (true);
		}
	else {
		return (false);
		}
	}

errcode add_ops(aops, procaddr, count, new_ops, tdefs, odefs)
struct OPS *aops;
errcode (*procaddr)();
long count;
struct OPS *new_ops;
{
long j;
		ops_arr[num_entries] = (OWNREQ)aops;
		ops_proc[num_entries] = procaddr;
		opsptr_arr[num_entries] = (OWNPTR)new_ops;
		parm_types_defs[num_entries] = tdefs;
		parm_ops_defs[num_entries] = odefs;
		for (j = 0 ; j < count; j++) {
			parm_vals[num_entries][j] =
				(long*)inst_info_value[j]; 
			parm_types[num_entries][j] =
				(long*)inst_info_reqs[j];  /* save const/type ind */
			}
		num_entries++;
		if (num_entries == MAX_INSTS) {
			fprintf(stderr, 
		"add_ops: too many instantiations: increase MAX_INSTS\n");
			exit(-10);
			}
		signal(ERR_ok);
		}	
errcode update_ops()
{
	parm_types_defs[num_entries] = current_tdefs;
	parm_ops_defs[num_entries] = current_odefs;
	signal(ERR_ok);
	}
