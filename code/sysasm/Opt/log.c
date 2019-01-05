
/* Copyright Massachusetts Institute of Technology 1990,1991 */

/*							*/
/*		Natural Log				*/
/*							*/


#include <math.h>

#include "pclu_err.h"
#include "pclu_sys.h"
#undef log

errcode OPlog(x, ans)
CLUREF x, *ans;
{
double arg, result;

	if (x.real <= 0 ) {
		signal(ERR_undefined);
		}
	arg = x.real;
	result = log(arg);
	ans->real = result;
	signal(ERR_ok);
	}
