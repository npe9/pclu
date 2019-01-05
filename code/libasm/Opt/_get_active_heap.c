
/* Copyright Massachusetts Institute of Technology 1993 */

#include "pclu_err.h"
#include "pclu_sys.h"

#ifndef LINUX
errcode _get_active_heap(ans)
CLUREF *ans;
{
	ans->num = blks_active * HBLKSIZE;
	signal(ERR_ok);
	}
#else
errcode _get_active_heap(ans)
CLUREF *ans;
{
        ans->num = GC_composite_in_use + GC_atomic_in_use;
        signal(ERR_ok);
        }
#endif
