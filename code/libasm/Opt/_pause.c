
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _pause.c,v 1.2 91/06/06 13:49:36 dcurtis Exp $";
#endif
/* $Log:	_pause.c,v $
 * Revision 1.2  91/06/06  13:49:36  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:51  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_pause			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _pause()
{
	sigpause(0);
	signal(ERR_ok);
	}
