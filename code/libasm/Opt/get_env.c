
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: get_env.c,v 1.3 91/06/06 13:53:45 dcurtis Exp $";
#endif
/* $Log:	get_env.c,v $
 * Revision 1.3  91/06/06  13:53:45  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  16:51:52  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:21:23  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			get_env			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern CLUREF clu_empty_string;
extern char **environ;
extern int wrpipe;

static int argv = 0;

errcode get_env(ans)
CLUREF *ans;
{
errcode err;
CLUREF temp;
CLUREF temp2;
CLUREF size, len;
int i;

	if (argv != 0) {
		ans->num = argv;
		signal(ERR_ok);
		}
	if (wrpipe != 0 || environ[0] == 0) {
		err = sequenceOPnew(&temp);
		if (err != ERR_ok) resignal(err);
		argv = temp.num;
		ans->vec = temp.vec;
		signal(ERR_ok);
		}
	/* find number of strings in environ */
	size.num = 0;
	for (i = 0;; i++) {
		if (environ[i] == 0) break;
		size.num += 1;
		}
	err = sequenceOPfill(size, CLU_0, &temp);
	if (err != ERR_ok) resignal(err);
	for(i = 0;; i++) {
		if (environ[i] == 0) break;
		len.num = strlen(environ[i]);
		err = stringOPcons(environ[i], CLU_0, len, &temp2);
		if (err != ERR_ok) resignal(err);
		temp.vec->data[i] = temp2.num;	
		}
	argv = temp.num;
	ans->vec = temp.vec;
	signal(ERR_ok);
	}
