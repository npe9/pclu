
/* Copyright Massachusetts Institute of Technology 1990,1991 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_bin_id			*/
/*						*/

#include <sys/time.h>

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _bin_id(ans, elist)
CLUREF *ans;
errlist elist;
{
struct timeval t;
struct timezone tz;
int uerr;

	uerr = gettimeofday(&t, &tz);
	ans->num = t.tv_sec;
	signal(ERR_ok);
	}

