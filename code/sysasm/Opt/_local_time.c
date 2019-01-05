
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _local_time.c,v 1.3 91/06/06 13:25:49 root Exp $";
#endif
/* $Log:	_local_time.c,v $
 * Revision 1.3  91/06/06  13:25:49  root
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  11:24:34  root
 * sparcstation compatibility: int->CLUREF
 * 	in call to date$create
 * 
 * Revision 1.1  91/02/04  15:49:36  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_local_time		*/
/*						*/

#include <time.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

errcode _local_time(left, right, ans)
CLUREF left, right, *ans;
{
struct tm *tm;
errcode err;
CLUREF temp, day, mon, year, hour, min, sec;
long clock;

	clock = (left.num << 16) + right.num;
	tm = localtime(&clock);
	day.num = tm->tm_mday;
	mon.num = tm->tm_mon + 1;
	year.num = tm->tm_year + 1900;
	hour.num = tm->tm_hour;
	min.num = tm->tm_min;
	sec.num = tm->tm_sec;
	err = dateOPcreate(day, mon, year, hour, min, sec, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	signal(ERR_ok);
	}

