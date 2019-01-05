
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _exit.c,v 1.2 91/06/06 13:44:33 dcurtis Exp $";
#endif
/* $Log:	_exit.c,v $
 * Revision 1.2  91/06/06  13:44:33  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:19  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_exit			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#undef _exit

errcode _OPexit(status)
CLUREF status;
{
	_cleanup_();
	_chanOP_save_tty();
	_exit(status.num);
	}
