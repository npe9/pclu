
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _get_gid.c,v 1.2 91/06/06 13:45:49 dcurtis Exp $";
#endif
/* $Log:	_get_gid.c,v $
 * Revision 1.2  91/06/06  13:45:49  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:27  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_get_gid		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"
#include <sys/types.h>

errcode _get_gid(ans)
CLUREF *ans;
{
gid_t gid;

	gid = getgid();
	ans->num = gid;
	signal(ERR_ok);
	}
