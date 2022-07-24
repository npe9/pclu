/* Copyright Massachusetts Institute of Technology 1993 */

#include "pclu_err.h"
#include "pclu_sys.h"

#ifndef HAVE_GET_MAX_HEAP_SIZE
errcode _get_max_heap(ans)
CLUREF *ans;
{
	ans->num = 0;
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
