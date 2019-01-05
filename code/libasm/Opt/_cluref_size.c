
/* Copyright Massachusetts Institute of Technology 1993 */

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _cluref_size(ans)
CLUREF *ans;
{
	ans->num = sizeof(CLUREF);
	signal(ERR_ok);
	}
