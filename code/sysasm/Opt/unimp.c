
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: unimp.c,v 1.3 91/07/09 11:29:04 root Exp $";
#endif
/* $Log:	unimp.c,v $
 * Revision 1.3  91/07/09  11:29:04  root
 * int -> CLUREF for sun compatibility
 * 
 * Revision 1.2  91/06/06  13:32:06  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:56  mtv
 * Initial revision
 * 
 */

/*						*/
/*	SOME UNIMPLEMENTED STUBS		*/
/*						*/

#include <stdio.h>
#include "pclu_err.h"
#include "pclu_sys.h"

long heaplo = 0xdead;
long heaphi = 0xffff;
long freelm = 0x0042;

errcode _gcOPgc()
{
	dmd_gc();
	signal(ERR_ok);
	}

/*
errcode _gcOPcontrol(add, mask)
CLUREF add, mask;
{
errcode err;

	  err = gcOPcontrol(add, mask);
	  resignal(err);
	  }
*/

#ifndef LINUX
extern int gccount;
#endif
errcode _gcOPcount(ans)
CLUREF *ans;
{
	ans->num = gccount;
	signal(ERR_ok);
	}

errcode _rtn_infoOPis_proc(s, ans)
CLUREF s, *ans;
{
	ans->tf = true;
	signal(ERR_ok);
	}

errcode _rtn_infoOPis_iter(s, ans)
CLUREF s, *ans;
{
	ans->tf = true;
	signal(ERR_ok);
	}
errcode _objOPis_uninit(x, ans)
CLUREF x, *ans;
{
	fprintf(stderr, "_obj$is_uninit not implemented\n");
	signal(ERR_failure);
	}
errcode atan4900(x, ans)
CLUREF x, *ans;
{
	fprintf(stderr, "atan4900 not implemented\n");
	signal(ERR_failure);
	}
errcode _objOPis_routine(x, ans)
CLUREF x, *ans;
{
	fprintf(stderr, "_obj$is_routine not implemented\n");
	signal(ERR_failure);
	}
errcode structOPcreate(x, ans)
CLUREF x, *ans;
{
	fprintf(stderr, "struct$create not implemented\n");
	signal(ERR_failure);
	}
errcode _printn()
{
	signal(ERR_ok);
	}

