
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _page_size.c,v 1.2 91/06/06 13:49:25 dcurtis Exp $";
#endif
/* $Log:	_page_size.c,v $
 * Revision 1.2  91/06/06  13:49:25  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:50  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_page_size 		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _page_size(ans)
CLUREF *ans;
{
int temp;

	temp = getpagesize();
	ans->num = temp;
	signal(ERR_ok);
	}
