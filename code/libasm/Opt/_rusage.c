
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _rusage.c,v 1.2 91/06/06 13:50:13 dcurtis Exp $";
#endif
/* $Log:	_rusage.c,v $
 * Revision 1.2  91/06/06  13:50:13  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:55  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_rusage		*/
/*						*/

#include <sys/time.h>
#include <sys/resource.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>

errcode _rusage(who, buf)
CLUREF who, buf;
{
struct rusage rusage;
int err;

	if (buf.vec->size < sizeof(struct rusage)) signal(ERR_bounds);
	err = getrusage(who.num, (struct rusage *)buf.vec->data);
	if (err != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}
