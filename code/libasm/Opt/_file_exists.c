
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _file_exists.c,v 1.2 91/06/06 13:44:58 dcurtis Exp $";
#endif
/* $Log:	_file_exists.c,v $
 * Revision 1.2  91/06/06  13:44:58  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:20:22  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			_file_exists		*/
/*						*/

/*        Modified by Robert G. Fermier         */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
//extern int errno;
extern CLUREF empty_string;

errcode _file_exists(fs, ftype, ans)
CLUREF fs, ftype, *ans;
{

int uerr;
errcode err;
struct stat buf;
CLUREF wd, name;

	name.str = fs.str;
	if (fs.str->size == 0 || fs.str->data[0] != '/') {
		err = working_dir(&wd);
		if (err != ERR_ok) resignal(err);
		err = stringOPconcat(wd, fs, &name);
		if (err != ERR_ok) resignal(err);
		}

	uerr = stat(name.str->data, &buf);
	if (uerr == 0) {
		if ((buf.st_mode & S_IFMT)>>12 == ftype.num) ans->tf = true;
		else ans->tf = false;
		signal(ERR_ok);
		}
	if (errno == ENOENT || errno == ENOTDIR) {
		ans->tf = false;
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}

