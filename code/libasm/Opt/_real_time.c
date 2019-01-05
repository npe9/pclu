
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _real_time.c,v 1.2 91/06/06 13:50:03 dcurtis Exp $";
#endif
/* $Log:	_real_time.c,v $
 * Revision 1.2  91/06/06  13:50:03  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:54  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_real_time		*/
/*						*/
/*	returns milliseconds since 1970	GMT	*/
/*		low 32 bits only		*/
/*						*/

#include <sys/time.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _real_time(ans)
CLUREF *ans;
{
struct timeval tp;
double temp;
unsigned long temp2;

	gettimeofday(&tp, 0);
/*
	temp = ((double) tp.tv_sec * 1000)  + (tp.tv_usec /1000);
	temp2 = temp;
*/
/* let it overflow: we're only interested in the lsb's */
	temp2 = (tp.tv_sec * 1000) + (tp.tv_usec/1000);
	ans->num = temp2;
	signal(ERR_ok);
	}
