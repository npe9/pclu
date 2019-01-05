
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _nice.c,v 1.2 91/06/06 13:48:59 dcurtis Exp $";
#endif
/* $Log:	_nice.c,v $
 * Revision 1.2  91/06/06  13:48:59  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:47  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_nice			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>


errcode _nice(delta)
CLUREF delta;
{
int uerr;

	uerr = nice(delta.num);
	if (uerr == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
