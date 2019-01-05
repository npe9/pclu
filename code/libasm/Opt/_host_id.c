
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _host_id.c,v 1.2 91/06/06 13:47:44 dcurtis Exp $";
#endif
/* $Log:	_host_id.c,v $
 * Revision 1.2  91/06/06  13:47:44  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:39  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_host_id		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"
#include <sys/types.h>

errcode _host_id(ans1, ans2)
CLUREF *ans1, *ans2;
{
long hostid;

	hostid = gethostid();
	ans1->num = hostid>>16;
	ans2->num = hostid & 0xffff;
	signal(ERR_ok);
	}
