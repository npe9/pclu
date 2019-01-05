
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _get_pid.c,v 1.2 91/06/06 13:25:21 root Exp $";
#endif
/* $Log:	_get_pid.c,v $
 * Revision 1.2  91/06/06  13:25:21  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:35  mtv
 * Initial revision
 * 
 */

/*						*/
/*		Get (Unix) Process Id		*/
/*						*/

#include <sys/types.h>
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _get_pid(ans)
CLUREF *ans;
{
pid_t temp;
	temp = getpid();
	ans->num = 0L + temp;
	signal(ERR_ok);
	}
