
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: connected_dir.c,v 1.5 91/07/08 10:38:01 root Exp $";
#endif 
/* $Log:	connected_dir.c,v $
 * Revision 1.5  91/07/08  10:38:01  root
 * avoided using len+1 throughout (fixed last fix)
 * 
 * Revision 1.4  91/07/08  10:00:15  root
 * recovered +1 lost during insertion of sun fixes
 * 
 * Revision 1.3  91/06/06  13:27:06  root
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  11:51:30  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  15:49:41  mtv
 * Initial revision
 * 
 */

/*							*/
/*		GET CURRENT (-or- CONNECTED) DIRECTORY	*/
/*							*/

/* TODO: signals could more nearly emulate original	*/


#include <sys/param.h>

#include "pclu_err.h"
#include "pclu_sys.h"

#ifdef LINUX
#include <unistd.h>
#else
extern char *getwd();
#endif
extern char *strcat();

errcode connected_dir(ans)
CLUREF *ans;
{
static char name[MAXPATHLEN];
char *result;
CLUREF temp;
errcode err;
CLUREF lenp1;

#ifdef LINUX
	result = getcwd(name, MAXPATHLEN);
#else
	result = getwd(name);
#endif
	lenp1.num = strlen(name) + 1;
	err = stringOPcons(name, CLU_1, lenp1, &temp);
	if (result == NULL) {
		elist[0].str = cannot_get_connected_directory_STRING;
		elist[1] = temp;    /* new behavior: probably unnecessary */
		signal(ERR_failure);
		}
	if (err != ERR_ok) resignal(err);
	temp.str->data[lenp1.num - 1] = '/';
	ans->str = temp.str;
	signal(ERR_ok);
	}
