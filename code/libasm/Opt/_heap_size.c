
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _heap_size.c,v 1.2 91/06/06 13:47:18 dcurtis Exp $";
#endif 
/* $Log:	_heap_size.c,v $
 * Revision 1.2  91/06/06  13:47:18  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:38  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_heap_size		*/
/*						*/
/*	returns the number of 32-bit units in 	*/
/*		in the heap			*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern int composite_in_use;

errcode _heap_size(ans)
CLUREF *ans;
{
/*	ans->num = (heaphi - heaplo) / 4; */
#ifdef LINUX
	ans->num = GC_composite_in_use;
#else
	ans->num = composite_in_use;
#endif
	signal(ERR_ok);
	}
