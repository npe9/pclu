
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _event.c,v 1.2 91/06/06 13:25:03 root Exp $";
#endif
/* $Log:	_event.c,v $
 * Revision 1.2  91/06/06  13:25:03  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:34  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_event			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

int defer;

errcode _eventOPdefer()
{
	defer += 1;
	signal(ERR_ok);
	}


errcode _eventOPundefer()
{
	defer -= 1;
	signal(ERR_ok);
	}



