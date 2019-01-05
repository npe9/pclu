
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _change_signal.c,v 1.3 91/06/06 13:44:09 dcurtis Exp $";
/* $Log:	_change_signal.c,v $
 * Revision 1.3  91/06/06  13:44:09  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  17:07:47  root
 * sparcstation compatibility: decompose a union variable
 * 
 * Revision 1.1  91/02/04  23:20:17  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_change_signal		*/
/*						*/

#include <signal.h>
#undef signal

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>

extern CLUREF empty_string;

errcode _change_signal(sig, label, ans)
CLUREF sig, label, *ans;
{
int err;
struct sigaction vec, ovec;

	vec.sa_handler = (void (*)())label.ref;
#if defined(LINUX) && !defined(OLD_LINUX)
	vec.sa_mask.__val[0] = 0xff7bf0e0;
#else
	vec.sa_mask = 0xff7bf0e0;
#endif
	vec.sa_flags = 1;

	err = sigaction(sig.num, &vec, &ovec);
	if (err != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans->num = (int)ovec.sa_handler;
	signal(ERR_ok);
	}

#endif
