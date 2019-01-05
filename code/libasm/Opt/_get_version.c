
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _get_version.c,v 1.2 91/06/06 13:46:47 dcurtis Exp $";
#endif 
/* $Log:	_get_version.c,v $
 * Revision 1.2  91/06/06  13:46:47  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:34  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_get_version		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern int version;

errcode _get_version(ans)
CLUREF *ans;
{
	ans->num = version;
	signal(ERR_ok);
	}
