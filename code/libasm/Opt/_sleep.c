
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _sleep.c,v 1.2 91/06/06 13:51:21 dcurtis Exp $";
#endif
/* $Log:	_sleep.c,v $
 * Revision 1.2  91/06/06  13:51:21  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:03  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_sleep			*/
/*						*/

#include <signal.h>
#undef signal
#include <sys/time.h>
#include "pclu_err.h"
#include "pclu_sys.h"

extern void _sleep_alarm();

errcode _sleep(millis)
CLUREF millis;
{
int uerr;
int omask, omask2;
struct sigaction vec;
int which;
struct itimerval value;

	if (millis.num == 0) signal(ERR_ok);
	vec.sa_handler = _sleep_alarm;
	/* vec.sa_mask = -1; */
	sigfillset(&vec.sa_mask);
	vec.sa_flags = true;
	uerr = sigaction(SIGALRM, &vec, 0);
	omask = sigblock(sigmask(SIGALRM));
	which = ITIMER_REAL;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	value.it_value.tv_sec = millis.num / 1000;
	value.it_value.tv_usec = millis.num % 1000;
	uerr = setitimer(which, &value, 0);
	if (uerr != 0) signal(ERR_ok);
	omask2 = sigpause(omask);
	sigsetmask(omask);
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 0;
	setitimer(which, &value, 0);
	signal(ERR_ok);
	}

void _sleep_alarm()
{
	return;
	}
