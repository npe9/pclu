
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/libasm/RCS/sleep.c,v 1.3 91/08/29 16:07:33 dcurtis Exp $";
#endif
/* $Log:	sleep.c,v $
 * Revision 1.3  91/08/29  16:07:33  dcurtis
 * fixed casting on arguments to sigvec,setitimer
 * 
 * Revision 1.2  91/06/06  13:57:00  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:41  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			sleep			*/
/*						*/

#include <signal.h>
#undef signal
#include <sys/time.h>
#include "pclu_err.h"
#include "pclu_sys.h"

extern void sleep_alarm();

unsigned int sleep(isecs)
unsigned int isecs;
{
int uerr;
int omask, omask2;
struct sigaction vec;
int which;
struct itimerval value;
CLUREF secs;

	secs.num = isecs;
	if (secs.num == 0) signal(ERR_ok);
	vec.sa_handler = sleep_alarm;
#if defined(LINUX) && !defined(OLD_LINUX)
	vec.sa_mask.__val[0] = -1;
#else
	vec.sa_mask = -1;
#endif
	vec.sa_flags = true;
	uerr = sigaction(SIGALRM, &vec, (struct sigaction*)0);
	omask = sigblock(sigmask(SIGALRM));
	which = ITIMER_REAL;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	value.it_value.tv_sec = secs.num;
	value.it_value.tv_usec = 0;
	uerr = setitimer(which, &value, (struct itimerval*)0);
	if (uerr != 0) signal(ERR_ok);
	omask2 = sigpause(omask);
	sigsetmask(omask);
	value.it_value.tv_sec = 0;
	setitimer(which, &value, (struct itimerval*)0);
	signal(ERR_ok);
	}

void sleep_alarm()
{
	return;
	}
