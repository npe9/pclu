
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _dtable_size.c,v 1.2 91/06/06 13:44:21 dcurtis Exp $";
#endif 
/* $Log:	_dtable_size.c,v $
 * Revision 1.2  91/06/06  13:44:21  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:18  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_dtable_size 		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _dtable_size(ans)
CLUREF *ans;
{
int nfds;

	nfds = getdtablesize();
	ans->num = nfds;
	signal(ERR_ok);
	}
