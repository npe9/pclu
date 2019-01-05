
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _file_owner.c,v 1.2 91/06/06 13:45:21 dcurtis Exp $";
#endif
/* $Log:	_file_owner.c,v $
 * Revision 1.2  91/06/06  13:45:21  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:24  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_file_owner		*/
/*						*/

/*            Altered by Robert G. Fermier      */

#include <sys/types.h>
#include <sys/stat.h>

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
//extern int errno;
extern CLUREF empty_string;

errcode _file_owner(fn, ans1, ans2)
CLUREF fn, *ans1, *ans2;
{

int uerr;
errcode err;
CLUREF newfn, name;
struct stat buf;

	err = file_name_fill(fn, empty_string, &newfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(newfn, &name);
	if (err != ERR_ok) resignal(err);
	uerr = stat(name.str->data, &buf);
	if (uerr != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans1->num = buf.st_uid;
	ans2->num = buf.st_gid;
	signal(ERR_ok);
	}

