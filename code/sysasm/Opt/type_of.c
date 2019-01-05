
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: type_of.c,v 1.2 91/06/06 13:31:55 root Exp $";
#endif
/* $Log:	type_of.c,v $
 * Revision 1.2  91/06/06  13:31:55  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:55  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			type_of			*/
/*						*/
/*		returns the type field of an any*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode type_of(a, ans)
CLUREF a, *ans;
{
	ans->num = a.cell->tag;
	signal(ERR_ok);
	}

