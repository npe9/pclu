
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: file_exists.c,v 1.2 91/06/06 13:53:13 dcurtis Exp $";
#endif
/* $Log:	file_exists.c,v $
 * Revision 1.2  91/06/06  13:53:13  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:14  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			file_exists		*/
/*						*/

/*          Altered by Robert G. Fermier        */

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
extern CLUREF empty_string;

errcode file_exists(fn, ans)
CLUREF fn, *ans;
{
errcode err;
int uerr;
CLUREF wd, name, home;

	err = file_name_fill(fn, empty_string, &name);
	if (err != ERR_ok) resignal(err);

	err = file_nameOPunparse(name, &name);
	if (err != ERR_ok) resignal(err);

	/* For now, emulate Berkeley behavior: if name is empty
	   access is ok: 3/24/94 dcurtis */
	if (strlen(name.str->data) == 0) {
		ans->tf = true;
		signal(ERR_ok);
		}

	uerr = access(name.str->data, 0);
	if (uerr == 0) {
		ans->tf = true;
		signal(ERR_ok);
		}
	if (errno == ENOENT || errno == ENOTDIR) {
		ans->tf = false;
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}

