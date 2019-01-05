
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/now.c,v 1.4 91/08/29 16:39:06 dcurtis Exp $";
#endif 
/* $Log:	now.c,v $
 * Revision 1.4  91/08/29  16:39:06  dcurtis
 * removed call to perror (inappropriate)
 * 
 * Revision 1.3  91/06/06  13:29:15  root
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  11:20:20  root
 * sparcstation compatibility: int->CLUREF
 * 	in call to _local_time
 * 
 * Revision 1.1  91/02/04  15:49:47  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			now			*/
/*						*/

#include <sys/time.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

errcode now(ans)
CLUREF *ans;
{
CLUREF temp;
errcode err;
int unix_err;
struct timeval tp;
struct timezone tzp;
CLUREF left, right;

	unix_err = gettimeofday(&tp, &tzp);
	left.num = tp.tv_sec >> 16;
	right.num = tp.tv_sec & 0Xffff;
	err = _local_time(left, right, &temp);
	if (err != ERR_ok) resignal(err);	
	ans->num = temp.num;
	signal(ERR_ok);
	}

