
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: delete_file.c,v 1.3 91/06/06 13:27:16 root Exp $";
#endif
/* $Log:	delete_file.c,v $
 * Revision 1.3  91/06/06  13:27:16  root
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  12:16:39  root
 * removed unused variable
 * 
 * Revision 1.1  91/02/04  15:49:42  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			delete_file		*/
/*						*/

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "pclu_err.h"
#include "pclu_sys.h"
extern CLUREF empty_string;


errcode delete_file(fn)
CLUREF fn;
{

int uerr;
errcode err;
CLUREF newfn, name;

	err = file_name_fill(fn, empty_string, &newfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(newfn, &name);
	if (err != ERR_ok) resignal(err);
	uerr = unlink(name.vec->data);
	if (uerr != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

