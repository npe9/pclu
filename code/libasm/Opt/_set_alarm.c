
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _set_alarm.c,v 1.2 91/06/06 13:50:33 dcurtis Exp $";
#endif
/* $Log:	_set_alarm.c,v $
 * Revision 1.2  91/06/06  13:50:33  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:57  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_set_alarm		*/
/*						*/

#include <signal.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"
#include <sys/time.h>
#include <errno.h>

extern char **environ;

static bool itimer_flag = false;
extern void clu_alarm();

errcode _set_alarm(secs, ans)
CLUREF secs, *ans;
{
int uerr;
char *addr;
errcode err;
CLUREF temp;
struct sigaction vec, ovec;
struct itimerval value;
int which;

	if (secs.num > 0) {
		vec.sa_handler = alarm;
#if defined(LINUX) && !defined(OLD_LINUX)
		vec.sa_mask.__val[0] = -1;
#else
		vec.sa_mask = -1;
#endif
		vec.sa_flags = 1;
		uerr = sigaction(SIGALRM, &vec, &ovec);
		}
	which = ITIMER_REAL;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	value.it_value.tv_sec = secs.num;
	value.it_value.tv_usec = 0;
	ans->tf = itimer_flag;
	itimer_flag = false;
	uerr = setitimer(which, &value, 0);
	signal(ERR_ok);
	}

void clu_alarm(sig, code, scp)
unsigned int sig, code;
struct sigcontext *scp;
{
	itimer_flag = true;
	return;
	}
