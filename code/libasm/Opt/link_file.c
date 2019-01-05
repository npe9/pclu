
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: link_file.c,v 1.2 91/06/06 13:54:48 dcurtis Exp $";
#endif
/* $Log:	link_file.c,v $
 * Revision 1.2  91/06/06  13:54:48  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:31  mtv
 * Initial revision
 * 
 */

/*						*/
/*		IMPLEMENTATION OF		*/
/*			link_file		*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

#include <errno.h>
extern CLUREF empty_string;

errcode link_file(ofn, nfn)
CLUREF ofn, nfn;
{
errcode err;
int uerr;
CLUREF wd, oname, nname, home;

	err = file_name_fill(ofn, empty_string, &oname);
	if (err != ERR_ok) resignal(err);

	err = file_nameOPunparse(oname, &oname);
	if (err != ERR_ok) resignal(err);

	err = file_name_fill(nfn, empty_string, &nname);
	if (err != ERR_ok) resignal(err);

	err = file_nameOPunparse(nname, &nname);
	if (err != ERR_ok) resignal(err);

	uerr = link(oname.ref, nname.ref);
	if (uerr == 0) {
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}

