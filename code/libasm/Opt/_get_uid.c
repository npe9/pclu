
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _get_uid.c,v 1.2 91/06/06 13:46:36 dcurtis Exp $";
#endif
/* $Log:	_get_uid.c,v $
 * Revision 1.2  91/06/06  13:46:36  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:32  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_get_uid		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"
#include <sys/types.h>

errcode _get_uid(ans1, ans2)
CLUREF *ans1, *ans2;
{
uid_t uid;

	uid = getuid();
	ans1->num = uid>>16;
	ans2->num = uid & 0xffff;
	signal(ERR_ok);
	}
