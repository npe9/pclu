
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _file_access.c,v 1.2 91/06/06 13:44:45 dcurtis Exp $";
#endif
/* $Log:	_file_access.c,v $
 * Revision 1.2  91/06/06  13:44:45  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:20  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_file_access		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
//extern int errno;
extern CLUREF empty_string;

errcode _file_access(fs, mode, ans)
CLUREF fs, mode, *ans;
{
errcode err;
int uerr;
CLUREF wd, name;

	name.str = fs.str;
	if (fs.str->size == 0 || fs.str->data[0] != '/') {
		err = working_dir(&wd);
		if (err != ERR_ok) resignal(err);
		err = stringOPconcat(wd, fs, &name);
		if (err != ERR_ok) resignal(err);
		}

	uerr = access(name.str->data, mode.num);
	if (uerr == 0)
		ans->tf = true;
	else
		ans->tf = false;
	signal(ERR_ok);
	}

