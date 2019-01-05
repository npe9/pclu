
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: type.c,v 1.2 91/06/06 13:31:44 root Exp $";
#endif
/* $Log:	type.c,v $
 * Revision 1.2  91/06/06  13:31:44  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:55  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*		   type				*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode typeOPequal(t1, t2, ans)
CLUREF t1, t2, *ans;
{
	if (t1.num == t2.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode typeOPsimilar(t1, t2, ans)
CLUREF t1, t2, *ans;
{
	if (t1.num == t2.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode typeOPcopy(t, ans)
CLUREF t, *ans;
{
	ans->num = t.num;
	signal(ERR_ok);
	}

