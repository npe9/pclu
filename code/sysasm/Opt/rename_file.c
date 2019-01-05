
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: rename_file.c,v 1.3 91/06/06 13:30:36 root Exp $";
#endif
/* $Log:	rename_file.c,v $
 * Revision 1.3  91/06/06  13:30:36  root
 * added copyright notice
 * 
 * Revision 1.2  91/05/31  12:36:49  root
 * removed unused variable
 * 
 * Revision 1.1  91/02/04  15:49:52  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			rename_file		*/
/*						*/

#include <sys/types.h>
#include <sys/stat.h>

#include "pclu_err.h"
#include "pclu_sys.h"

extern CLUREF empty_string;

#include <errno.h>

errcode rename_file(ofn, nfn)
CLUREF ofn, nfn;
{

int uerr;
errcode err;
CLUREF nnewfn, nname;
CLUREF onewfn, oname;

	err = file_name_fill(ofn, empty_string, &onewfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(onewfn, &oname);
	if (err != ERR_ok) resignal(err);

	err = file_name_fill(nfn, empty_string, &nnewfn);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPunparse(nnewfn, &nname);
	if (err != ERR_ok) resignal(err);

	uerr = rename(oname.str->data, nname.str->data);
	if (uerr != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

