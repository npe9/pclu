
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: delete_directory.c,v 1.2 91/06/06 13:52:54 dcurtis Exp $";
#endif 
/* $Log:	delete_directory.c,v $
 * Revision 1.2  91/06/06  13:52:54  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:12  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			delete_directory	*/
/*						*/

/*         Modified by Robert G. Fermier        */

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
extern CLUREF empty_string;

errcode delete_directory(fn)
CLUREF fn;
{
errcode err;
CLUREF newfn, name;

	err = file_name_fill(fn, empty_string, &newfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(newfn, &name);
	if (err != ERR_ok) resignal(err);
	err = rmdir(name.str->data);
	if (err != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

