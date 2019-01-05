
/* Copyright Massachusetts Institute of Technology 1990,1991 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_save_state		*/
/* null implementation pending gc		*/
/*						*/

#include <sys/time.h>

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _save_state(fs, mode, elist)
CLUREF fs, mode;
errlist elist;
{
	signal(ERR_ok);
	}

