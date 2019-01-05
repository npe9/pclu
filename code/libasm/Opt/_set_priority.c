
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _set_priority.c,v 1.2 91/06/06 13:50:53 dcurtis Exp $";
#endif
/* $Log:	_set_priority.c,v $
 * Revision 1.2  91/06/06  13:50:53  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:00  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_set_priority			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>

errcode _set_priority(which, who, prio)
CLUREF which, who, prio;
{
int err;

	err = setpriority(which.num, who.num, prio.num);
	if (err == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
