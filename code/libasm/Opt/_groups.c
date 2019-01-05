
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _groups.c,v 1.4 91/06/06 13:47:07 dcurtis Exp $";
#endif
/* $Log:	_groups.c,v $
 * Revision 1.4  91/06/06  13:47:07  dcurtis
 * added copyright notice
 * 
 * Revision 1.3  91/06/05  13:24:05  root
 * added elist to sequence$new2
 * 
 * Revision 1.2  91/06/03  16:56:53  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:20:36  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_groups			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
extern CLUREF empty_string;

errcode _groups(ans)
CLUREF *ans;
{
int maxcount;
CLUREF count;
CLUREF temp;
int list[16];
errcode err;
int i;

	maxcount = 16;
	count.num = getgroups(maxcount, list);
		if (count.num == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	err = sequenceOPnew2(count, &temp);
	if (err != ERR_ok) resignal(err);
	for (i = 0; i < count.num; i++) {
		temp.vec->data[i] = list[i];
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}
