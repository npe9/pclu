
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: file_date.c,v 1.2 91/06/06 13:53:04 dcurtis Exp $";
#endif
/* $Log:	file_date.c,v $
 * Revision 1.2  91/06/06  13:53:04  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:13  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			file_date		*/
/*						*/

#include <sys/types.h>
#include <sys/stat.h>

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
extern CLUREF empty_string;

errcode file_date(fn, read, ans)
CLUREF fn, read, *ans;
{

int uerr;
errcode err;
CLUREF newfn, name, temp;
struct stat buf;
time_t time;
CLUREF arg1, arg2;

	err = file_name_fill(fn, empty_string, &newfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(newfn, &name);
	if (err != ERR_ok) resignal(err);
	uerr = stat(name.str->data, &buf);
	if (uerr != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (read.tf == true) time = buf.st_atime;
	else time = buf.st_mtime;
	arg1.num = (time >> 16) & 0xffff;
	arg2.num = time & 0xffff;
	err = _local_time(arg1, arg2, &temp);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	signal(ERR_ok);
	}

