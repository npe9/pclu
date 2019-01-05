
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _type_base.c,v 1.2 91/06/06 13:52:04 dcurtis Exp $";
#endif
/* $Log:	_type_base.c,v $
 * Revision 1.2  91/06/06  13:52:04  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:06  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION _type_base	*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _type_base(ans)
CLUREF *ans;
{
	ans->num = 020000;
	signal(ERR_ok);
	}
