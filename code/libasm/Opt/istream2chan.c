
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: istream2chan.c,v 1.2 91/06/06 13:54:37 dcurtis Exp $";
#endif
/* $Log:	istream2chan.c,v $
 * Revision 1.2  91/06/06  13:54:37  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:30  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			istream2chan		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode istream2chan(st, ans)
CLUREF st, *ans;
{
errcode err;
CLUREF temp;

	err = istreamOPget_chan(st, &temp);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	*ans = temp;
	signal(ERR_ok);
	}
