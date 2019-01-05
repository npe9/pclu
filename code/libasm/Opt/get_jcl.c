
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: get_jcl.c,v 1.3 91/06/06 13:54:00 dcurtis Exp $";
#endif
/* $Log:	get_jcl.c,v $
 * Revision 1.3  91/06/06  13:54:00  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  17:24:06  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:21:24  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			get_jcl			*/
/*						*/

/* return string containing command line arguments except for the first */
/* (the first is the name of the program) */

#include "pclu_err.h"
#include "pclu_sys.h"

extern CLUREF clu_empty_string;
extern int argc;
extern char **argv;

errcode get_jcl(ans)
CLUREF *ans;
{
int uerr;
CLUREF size;
int i, j, k;
errcode err;
static bool done = 0;
static CLUREF jcl;
CLUREF temp;

	if (done == true) {
		ans->str = jcl.str;	
		signal(ERR_ok);
		}
	done = true;
	if (argc <= 1) {
		ans->str = clu_empty_string.str;
		jcl.str = clu_empty_string.str;
		signal(ERR_ok);
		}
	size.num = 0;
	for (i = 1; i < argc; i++) {
		size.num += strlen(argv[i]) + 1;
		}
	err = stringOPcons0(size, &temp);
	if (err != ERR_ok) resignal(err);
	i = 0;
	for (j = 1; j < argc; j++) {
		for (k = 0; k <= strlen(argv[j]); k++) {
			temp.str->data[i] = argv[j][k];
			if (temp.str->data[i] == 0) 
				temp.str->data[i] = ' ';
			i++;
			}
		}
	jcl.str = temp.str;
	ans->str = temp.str;
	signal(ERR_ok);
	}
