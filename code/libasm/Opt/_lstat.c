
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _lstat.c,v 1.2 91/06/06 13:48:37 dcurtis Exp $";
#endif
/* $Log:	_lstat.c,v $
 * Revision 1.2  91/06/06  13:48:37  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:44  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_lstat			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
extern CLUREF empty_string;

errcode _lstat(fn, buf)
CLUREF fn, buf;
{
errcode err;
int uerr;
CLUREF wd, name, home;

	err = file_name_fill(fn, empty_string, &name);
	if (err != ERR_ok) resignal(err);

	err = file_nameOPunparse(name, &name);
	if (err != ERR_ok) resignal(err);

	if (buf.vec->size < sizeof(struct stat)) signal(ERR_bounds);

	uerr = lstat(name.str->data, (struct stat *)buf.vec->data);
	if (uerr != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

