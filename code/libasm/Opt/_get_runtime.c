
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _get_runtime.c,v 1.3 91/07/09 15:21:59 root Exp $";
#endif
/* $Log:	_get_runtime.c,v $
 * Revision 1.3  91/07/09  15:21:59  root
 * removed unused variables (save1, save2, save3)
 * 
 * Revision 1.2  91/06/06  13:46:24  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:31  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_get_runtime		*/
/*						*/

#include <sys/time.h>
#include <sys/resource.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _get_runtime(ans1, ans2, ans3)
CLUREF *ans1, *ans2, *ans3;		/* sec, msec, usec */
{
struct rusage rusage;
unsigned long micros, secs;

	getrusage(0, &rusage);
	micros = (rusage.ru_utime.tv_usec + rusage.ru_stime.tv_usec);
	ans3->num = micros % 1000;
	secs = micros / 1000000;
	ans2->num = (micros / 1000) - (secs*1000);
	ans1->num = rusage.ru_utime.tv_sec + rusage.ru_stime.tv_sec + secs;
	signal(ERR_ok);
	}
