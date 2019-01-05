
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _get_pname.c,v 1.4 91/06/06 13:46:01 dcurtis Exp $";
#endif
/* $Log:	_get_pname.c,v $
 * Revision 1.4  91/06/06  13:46:01  dcurtis
 * added copyright notice
 * 
 * Revision 1.3  91/06/03  16:53:11  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.2  91/02/11  11:45:54  dcurtis
 * changed bogus code to sane code
 * 
 * Revision 1.1  91/02/04  23:20:28  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_get_pname		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern char **argv;
#include <errno.h>

errcode _get_pname(ans)
CLUREF *ans;
{
errcode err;
CLUREF temp;
char *buf, *end;
CLUREF len;

	len.num = strlen(argv[0]);
	err = stringOPcons(argv[0], CLU_1, len, &temp);
	if (err == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}
