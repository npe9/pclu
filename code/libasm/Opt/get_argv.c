
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: get_argv.c,v 1.3 91/06/06 13:53:34 dcurtis Exp $";
#endif
/* $Log:	get_argv.c,v $
 * Revision 1.3  91/06/06  13:53:34  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  17:28:44  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:21:22  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			get_argv		*/
/*						*/

/* looks at global C-style argv (char **) and   */
/* creates corresponding clu sequence of strings*/
/* argv[0] (the name of the invoked progrem)	*/
/* is omitted.					*/
/* returns the clu sequence of strings		*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern CLUREF clu_empty_string;
extern char **argv;
extern long wrpipe;

static long save_argv = 0;

errcode get_argv(ans)
CLUREF *ans;
{
errcode err;
CLUREF temp;
CLUREF temp2;
CLUREF size, len;
long i;

	if (save_argv != 0) {
		ans->num = save_argv;
		signal(ERR_ok);
		}
	if (wrpipe != 0 || argv[1] == 0) {
		err = sequenceOPnew(&temp);
		if (err != ERR_ok) resignal(err);
		save_argv = temp.num;
		ans->vec = temp.vec;
		signal(ERR_ok);
		}
	/* find number of strings in argv */
	size.num = 0;
	for (i = 1;; i++) {
		if (argv[i] == 0) break;
		size.num += 1;
		}
	err = sequenceOPfill(size, CLU_0, &temp);
	if (err != ERR_ok) resignal(err);
	for(i = 1;; i++) {
		if (argv[i] == 0) break;
		len.num = strlen(argv[i]);
		err = stringOPcons(argv[i], CLU_1, len, &temp2);
		if (err != ERR_ok) resignal(err);
		temp.vec->data[i-1] = temp2.num;	
		}
	save_argv = temp.num;
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

/* set_argv modifies the static variable used by get_argv to hold */
/* the CLU sequence of strings corresponding to argv.  argv, the C */
/* global, is not affected.  new 9/10/92 dwc */

errcode set_argv(args, ans)
CLUREF args, ans;
{
	save_argv = args.num;
	signal(ERR_ok);
	}
