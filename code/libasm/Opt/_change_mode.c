
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _change_mode.c,v 1.2 91/06/06 13:43:45 dcurtis Exp $";
#endif
/* $Log:	_change_mode.c,v $
 * Revision 1.2  91/06/06  13:43:45  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:14  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_change_mode		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
//extern int errno;
extern CLUREF empty_string;

errcode _change_mode(fn, mode)
CLUREF fn, mode;
{
CLUREF newfn, name;
int err;

	err = file_name_fill(fn, empty_string, &newfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(newfn, &name);
	if (err != ERR_ok) resignal(err);
	err = chmod(name.vec->data, mode.num);
	if (err != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

