
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: hash_obj.c,v 1.2 91/06/06 13:54:28 dcurtis Exp $";
#endif 
/* $Log:	hash_obj.c,v $
 * Revision 1.2  91/06/06  13:54:28  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:29  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			hash_obj		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode hash_obj(_obj, max, ans)
CLUREF _obj, max, *ans;
{
int temp, temp2;

	if (max.num == 0) ans->num = 0;
	else {
		temp = _obj.num / 4;
		temp2 = temp % max.num ;
		ans->num = temp2;
		}	
	signal(ERR_ok);
	}
