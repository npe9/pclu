
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _umask.c,v 1.2 91/06/06 13:52:14 dcurtis Exp $";
#endif
/* $Log:	_umask.c,v $
 * Revision 1.2  91/06/06  13:52:14  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:08  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_umask			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>

errcode _umask(mask, ans)
CLUREF mask, *ans;
{
int err;

	err = umask(mask);
	ans->num = err;
	signal(ERR_ok);
/*
	if (err == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
*/
	}
