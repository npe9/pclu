
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: hash_int.c,v 1.2 91/06/06 13:54:18 dcurtis Exp $";
#endif 
/* $Log:	hash_int.c,v $
 * Revision 1.2  91/06/06  13:54:18  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:28  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			hash_int		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode hash_int(i, j, ans)
CLUREF i, j, *ans;
{
	if (j.num == 0) {
		ans->num = 0;
		signal(ERR_ok);
		}
	ans->num = i.num % (unsigned) (j.num);
        if (ans->num < 0) ans->num += j.num;          /* Added by MTV */
	signal(ERR_ok);
	}
