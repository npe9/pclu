
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: gc.c,v 1.2 91/06/06 13:53:23 dcurtis Exp $";
#endif
/* $Log:	gc.c,v $
 * Revision 1.2  91/06/06  13:53:23  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:16  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			gc			*/
/*						*/

#include <stdio.h>
#include "pclu_err.h"
#include "pclu_sys.h"
#undef gc

extern int heaplo;

errcode gc(ans)
CLUREF *ans;
{
int old_heaplo;

	fprintf(stderr, "gc$gc returning wrong value\n");
	old_heaplo = heaplo;
	_gcOPgc();
	ans->num = (heaplo - old_heaplo) / 4;
	signal(ERR_ok);
	}
