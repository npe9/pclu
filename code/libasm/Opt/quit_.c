
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: quit_.c,v 1.3 91/06/06 13:55:42 dcurtis Exp $";
#endif
/* $Log:	quit_.c,v $
 * Revision 1.3  91/06/06  13:55:42  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  17:23:19  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:21:37  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			quit_			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode quit_()
{
	_cleanup_();
	_chanOP_save_tty();
	_exit(CLU_0);
	}
