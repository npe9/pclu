
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _cvt.c,v 1.2 91/06/06 13:24:25 root Exp $";
#endif 
/* $Log:	_cvt.c,v $
 * Revision 1.2  91/06/06  13:24:25  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:32  mtv
 * Initial revision
 * 
 * Revision 1.1  91/02/04  15:46:01  mtv
 * Initial revision
 * 
 */

/*					*/
/*		_CVT			*/
/*					*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _cvt(obj, ans)
CLUREF obj, *ans;
{
	*ans = obj;
	signal(ERR_ok);
	}

OWN_req _cvt_ownreqs = {0};

int _cvt_own_init = 0;

typedef struct {
long count;
} _cvt_of_from_REQS;
typedef struct {
long count;
} _cvt_of_to_REQS;
_cvt_of_from_REQS _cvt_of_from_reqs_actual = {0};
_cvt_of_to_REQS _cvt_of_to_reqs_actual = {0};
_cvt_of_from_REQS  *_cvt_of_from_reqs = &_cvt_of_from_reqs_actual;
_cvt_of_to_REQS *_cvt_of_to_reqs = &_cvt_of_to_reqs_actual;

