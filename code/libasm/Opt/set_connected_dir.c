
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: set_connected_dir.c,v 1.3 91/06/06 13:56:48 dcurtis Exp $";
#endif
/* $Log:	set_connected_dir.c,v $
 * Revision 1.3  91/06/06  13:56:48  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  16:49:07  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:21:40  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			set_connected_dir	*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
extern CLUREF empty_string;

errcode set_connected_dir(fs)
CLUREF fs;
{
errcode err;
int uerr;
CLUREF wd, name, home;

	if (fs.vec->size == 0) {
		err = stringOPcons("HOME", CLU_1, CLU_4, &home);
		if (err != ERR_ok) resignal(err);
		err = _environ(home, &name);
		if (err != ERR_ok) resignal(err);
		}
	else {
		err = file_nameOPcreate(fs, empty_string, empty_string, empty_string,
					&name);
		if (err == ERR_bad_format) signal(err);
		if (err != ERR_ok) resignal(err);
		err = file_name_fill(name, empty_string, &name);
		if (err != ERR_ok) resignal(err);
		err = file_nameOPget_dir(name, &name);
		if (err != ERR_ok) resignal(err);
		}
	uerr = chdir(name.str->data);
	if (uerr != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

