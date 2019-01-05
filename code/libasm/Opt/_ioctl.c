
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _ioctl.c,v 1.2 91/06/06 13:48:07 dcurtis Exp $";
#endif
/* $Log:	_ioctl.c,v $
 * Revision 1.2  91/06/06  13:48:07  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:42  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_ioctl			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>

errcode _ioctl(fdesc, reqh, reql, argp, disp)
CLUREF fdesc, reqh, reql, argp, disp;
{
int req;
int err;

	req = reqh.num<<16 + reql.num;
	err = ioctl(fdesc, req, argp.vec->data[disp.num]);
	if (err != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

