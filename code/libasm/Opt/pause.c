
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: pause.c,v 1.2 91/06/06 13:55:25 dcurtis Exp $";
#endif
/* $Log:	pause.c,v $
 * Revision 1.2  91/06/06  13:55:25  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:36  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			pause			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

int pause()
{
	sigpause(0);
	signal(ERR_ok);
	}
