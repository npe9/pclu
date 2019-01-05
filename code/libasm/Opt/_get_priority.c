
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _get_priority.c,v 1.2 91/06/06 13:46:12 dcurtis Exp $";
#endif
/* $Log:	_get_priority.c,v $
 * Revision 1.2  91/06/06  13:46:12  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:30  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_get_priority			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>

errcode _get_priority(which, who, ans)
CLUREF which, who, *ans;
{
int err;

	errno = 0;
	err = getpriority(which.num, who.num);
	if (errno == 0) {
		ans->num = err;
		signal(ERR_ok);}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
