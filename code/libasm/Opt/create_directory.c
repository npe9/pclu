
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: create_directory.c,v 1.2 91/06/06 13:52:43 dcurtis Exp $";
#endif
/* $Log:	create_directory.c,v $
 * Revision 1.2  91/06/06  13:52:43  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:10  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			create_directory	*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
extern CLUREF empty_string;

errcode create_directory(fn, mode)
CLUREF fn, mode;
{
CLUREF newfn, name;
int err;

	err = file_name_fill(fn, empty_string, &newfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(newfn, &name);
	if (err != ERR_ok) resignal(err);
	err = mkdir(name.str->data, mode.num);
	if (err != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

