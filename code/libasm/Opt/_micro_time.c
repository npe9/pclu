
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _micro_time.c,v 1.2 91/06/06 13:48:47 dcurtis Exp $";
#endif
/* $Log:	_micro_time.c,v $
 * Revision 1.2  91/06/06  13:48:47  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:46  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_micro_time		*/
/*						*/
/*	returns microseconds since 1970 GMT	*/
/*		low 32 bits only		*/
/*						*/

#include <sys/time.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _micro_time(ans)
CLUREF *ans;
{
struct timeval tp;
double temp;
unsigned long temp2;

	gettimeofday(&tp, 0);
	temp = ((double)tp.tv_sec * 1000000) + tp.tv_usec;
	temp2 = temp;
	ans->num = temp2;
	signal(ERR_ok);
	}
