
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _stop.c,v 1.2 91/06/06 13:51:51 dcurtis Exp $";
#endif
/* $Log:	_stop.c,v $
 * Revision 1.2  91/06/06  13:51:51  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:05  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_stop			*/
/*						*/

#include <signal.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _stop()
{
int pid;
int err;

	pid = getpid();
	err = kill(pid, SIGSTOP);
	signal(ERR_ok);
	}
