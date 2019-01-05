
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _read_link.c,v 1.2 91/06/06 13:49:49 dcurtis Exp $";
#endif
/* $Log:	_read_link.c,v $
 * Revision 1.2  91/06/06  13:49:49  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:52  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_read_link		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
extern CLUREF empty_string;

errcode _read_link(fn, buf, ans)
CLUREF fn, buf, *ans;
{
errcode err;
int count;
CLUREF newfn, name;

	err = file_name_fill(fn, empty_string, &newfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(newfn, &name);
	if (err != ERR_ok) resignal(err);
	count = readlink(name.vec->data, buf.vec->data, buf.vec->size);
	if (err == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans->num = count;
	signal(ERR_ok);
	}
