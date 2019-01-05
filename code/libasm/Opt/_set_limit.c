
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _set_limit.c,v 1.2 91/06/06 13:50:43 dcurtis Exp $";
#endif
/* $Log:	_set_limit.c,v $
 * Revision 1.2  91/06/06  13:50:43  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:59  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_set_limit		*/
/*						*/

#include <sys/time.h>
#include <sys/resource.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>

errcode _set_limit(resource, soft)
CLUREF resource, soft;	
{
struct rlimit rlimit;
int err;

	err = getrlimit(resource.num, &rlimit);
	if (err != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	rlimit.rlim_cur = soft.num;
	err = setrlimit(resource.num, &rlimit);
	if (err != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}
