
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _get_xjname.c,v 1.3 91/06/06 13:46:57 dcurtis Exp $";
#endif 
/* $Log:	_get_xjname.c,v $
 * Revision 1.3  91/06/06  13:46:57  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  16:55:06  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:20:35  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_get_xjname		*/
/*						*/

/* returns a string that is the name of the invoked program */

#include <string.h>
#include "pclu_err.h"
#include "pclu_sys.h"

extern char **argv;

errcode _get_xjname(ans)
CLUREF *ans;
{

errcode err;
CLUREF temp, len;

	len.num = strlen(argv[0]);
	err = stringOPcons(argv[0], CLU_1, len, &temp);
	if (err != ERR_ok) resignal(err);
	ans->str = temp.str;
	signal(ERR_ok);
	}

