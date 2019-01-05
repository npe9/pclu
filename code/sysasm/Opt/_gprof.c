
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _gprof.c,v 1.2 91/06/06 13:25:32 root Exp $";
#endif
/* $Log:	_gprof.c,v $
 * Revision 1.2  91/06/06  13:25:32  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:36  mtv
 * Initial revision
 * 
 */


/*						*/
/*						*/
/* 	null	IMPLEMENTATION OF		*/
/*			gprof			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

errcode _gprofOPinit()
{
	signal(ERR_ok);
	}

errcode _gprofOPdump(fs)
CLUREF fs;
{
	signal(ERR_ok);
	}

errcode _gprofOPcontrol(flag)
CLUREF flag;
{
	signal(ERR_ok);
	}

