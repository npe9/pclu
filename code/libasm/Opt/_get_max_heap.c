/* Copyright Massachusetts Institute of Technology 1993 */

#include "pclu_err.h"
#include "pclu_sys.h"

#ifndef LINUX
errcode _get_max_heap(ans)
CLUREF *ans;
{
	ans->num = blks_hard_limit * HBLKSIZE;
	signal(ERR_ok);
	}
#else
errcode _get_max_heap(ans)
CLUREF *ans;
{
        ans->num = GC_get_max_heap_size();
        signal(ERR_ok);
        }
#endif
