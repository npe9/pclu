
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _environ.c,v 1.4 91/07/09 13:04:35 root Exp $";
#endif
/* $Log:	_environ.c,v $
 * Revision 1.4  91/07/09  13:04:35  root
 * fixed length arg to stringOPcons to be a CLUREF (sun compatibility)
 * 
 * Revision 1.3  91/07/09  11:17:09  root
 * 1 -> CLU_1 for sun compatibility
 * 
 * Revision 1.2  91/06/06  13:24:40  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:32  mtv
 * Initial revision
 * 
 */

/*						*/
/*		_ENVIRON			*/
/*	fetch value for environment variable	*/

#include <sys/param.h>
#include "pclu_err.h"
#include "pclu_sys.h"

extern char *getenv();

errcode _OPenviron(name, ans)
CLUREF name, *ans;
{
char *value;
CLUREF temp, len;
errcode err;
	value = getenv(name.str->data);
	if (value == NULL) signal(ERR_not_found);
	len.num = strlen(value);
	err = stringOPcons(value, CLU_1, len, &temp);
	if (err != ERR_ok) resignal(err);
	ans->str = temp.str;
	signal(ERR_ok);
	}
