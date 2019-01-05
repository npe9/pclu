
/* Copyright Massachusetts Institute of Technology 1990,1991 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_restore_state		*/
/* null implementation pending gc		*/
/*						*/

#include <sys/time.h>

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _restore_state(fs, ans, elist)
CLUREF fs, *ans;
errlist elist;
{
	ans->tf = false;
	signal(ERR_ok);
	}

