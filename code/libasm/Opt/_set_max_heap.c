
/* Copyright Massachusetts Institute of Technology 1993 */

#include "pclu_err.h"
#include "pclu_sys.h"

#ifndef LINUX
extern int maxheapsize;
extern int blks_min_gc;

errcode _set_max_heap(n)
CLUREF n;
{
	if ((n.num < (blks_active * HBLKSIZE)) ||
	    (n.num < (blks_min_gc * HBLKSIZE))
	    )
	  signal(ERR_illegal);
	if (n.num > maxheapsize) {
	  elist[0].num = maxheapsize;
	  signal(ERR_illegal_size);
	}
	blks_hard_limit = divHBLKSZ(n.num);
	signal(ERR_ok);
	}
#else
errcode _set_max_heap(n)
CLUREF n;
{
	GC_set_max_heap_size(n.num);
	signal(ERR_ok);
}
#endif
