
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _host_name.c,v 1.3 91/06/06 13:47:55 dcurtis Exp $";
#endif
/* $Log:	_host_name.c,v $
 * Revision 1.3  91/06/06  13:47:55  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  16:58:15  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:20:40  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_host_name		*/
/*						*/

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "pclu_err.h"
#include "pclu_sys.h"

extern CLUREF empty_string;

/* per documentation of gethostname */
#define BUFSIZE 31

errcode _host_name(ans)
CLUREF *ans;
{

int uerr;
errcode err;
char buf[BUFSIZE];
CLUREF temp, len;

	uerr = gethostname(buf, BUFSIZE);
	if (uerr != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	len.num = strlen(buf);
	err = stringOPcons(buf, CLU_1, len, &temp);
	if (err != ERR_ok) resignal(err);
	ans->str = temp.str;
	signal(ERR_ok);
	}

