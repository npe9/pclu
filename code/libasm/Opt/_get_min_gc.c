
/* Copyright Massachusetts Institute of Technology 1993 */

#include "pclu_err.h"
#include "pclu_sys.h"

#ifndef LINUX
extern int blks_min_gc;

errcode _get_min_gc(ans)
CLUREF *ans;
{
	ans->num = blks_min_gc * HBLKSIZE;
	signal(ERR_ok);
	}
#endif
