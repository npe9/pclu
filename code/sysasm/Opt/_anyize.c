
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _anyize.c,v 1.2 91/06/06 13:25:21 root Exp $";
#endif
/* $Log:	_anyize.c,v $
 * Revision 1.2  91/06/06  13:25:21  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:35  mtv
 * Initial revision
 * 
 */

/*						*/
/*		_anyize				*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern OWN_ptr _anyize_own_init;
#ifndef LINUX
extern char *heapstart, *heaplim;
#endif

errcode _anyize(x, ans)
CLUREF x;
CLUREF *ans;
{
CLUREF tag, temp;
errcode err;
OWNPTR type_owns = CUR_PROC_VAR.proc->type_owns;

	/* if already an any, do nothing */
	if (type_owns == &_anyize_own_init) {
		ans->cell = x.cell;
		signal(ERR_ok);
		}
	/* test not correct for versions of gc using the stack */
#ifdef LINUX
	if (x.ref > GC_least_plausible_heap_addr && 
				x.ref < GC_greatest_plausible_heap_addr &&
#else
	if (x.ref > heapstart && x.ref < heaplim &&
#endif
		x.cell->typ.val == CT_VAR && x.cell->tag > 100) {
		ans->num = temp.num;
		signal(ERR_ok);
		}
	/* create oneof */
	/* stick type info as tag */
	/* put object in as value */
	tag.ref = (char *)type_owns;
	err = oneofOPnew(tag, x, &temp);
	ans->num = temp.num;
	signal(ERR_ok);
	}

typedef struct {
long count;
struct REQ_ENTRY entry[7];
} _anyize_of_t_REQS;
_anyize_of_t_REQS _anyize_of_t_reqs_actual = {0};
_anyize_of_t_REQS *_anyize_of_t_reqs = &_anyize_of_t_reqs_actual;

typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
} _anyize_OPS;

OWN_ptr _anyize_own_init = {0, 0};
OWN_req _anyize_ownreqs = {2*UNIT, 1};
_anyize_OPS _anyize_ops_actual = {0,
    &_anyize_own_init,
    &_anyize_own_init};

_anyize_OPS *_anyize_ops = &_anyize_ops_actual;
_anyize_OPS *any_ops = &_anyize_ops_actual;


