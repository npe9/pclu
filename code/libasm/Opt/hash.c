
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: hash.c,v 1.3 91/07/18 14:36:41 root Exp $";
#endif
/* $Log:	hash.c,v $
 * Revision 1.3  91/07/18  14:36:41  root
 * added temp to force shifting of data as an integer, not a character
 * 	per warning from a compiler (ultrix 4.2)
 * 
 * Revision 1.2  91/06/06  13:54:10  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:26  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			hash			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>

errcode hash(s, i, ans)
CLUREF s, i, *ans;
{
int j, sum, temp;

	if (s.str->size == 0) {
		ans->num = 0;
		signal(ERR_ok);
		}

/*	this algorithm is a little strange: 
	each character is added in twice
	and a byte past the end of the string may be added in as well */

	sum = 0;
	for (j = 0; j < s.str->size; j++, j++) {
		temp = s.str->data[j];
		sum += temp << 16;
		sum += s.str->data[j+1];
		}
	if (i.num == 0) ans->num = 0;
	else {
		if (sum < 0) sum = -sum;
		ans->num = sum % i.num;
		}

	signal(ERR_ok);
	}

