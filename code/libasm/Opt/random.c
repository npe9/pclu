
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: random.c,v 1.3 91/07/18 14:36:18 root Exp $";
#endif
/* $Log:	random.c,v $
 * Revision 1.3  91/07/18  14:36:18  root
 * fixed constants to match original
 * 
 * Revision 1.2  91/06/06  13:56:23  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  23:21:39  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			random			*/
/*						*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern CLUREF clu_empty_string;
extern char **environ;

static CLUREF random_hi;
static CLUREF random_lo;
static int random_init = 0;

#define hi random_hi
#define lo random_lo
#define init random_init

errcode randomOPreset()
{
	hi.num = 0Xbfc896df;
	lo.num = 0X6ab8724a;
	init = true;
	signal(ERR_ok);
	}

errcode randomOPseed(x)
CLUREF x;
{
	if (init == false) {
		hi.num = 0Xbfc896df;
		lo.num = 0X6ab8724a;
		init = true;
		}
	hi.num = hi.num ^ x.num;
	signal(ERR_ok);
	}

errcode randomOPnext(x, ans)
CLUREF x, *ans;
{
CLUREF newhi, newlo;
CLUREF temphi, templo; /* aka n1 n0 */
CLUREF save;		/* aka n2 */
int flag;

	if (init == false) {
		hi.num = 0Xbfc896df;
		lo.num = 0X6ab8724a;
		init = true;
		}
	templo.num = lo.num;
	temphi.num = hi.num;
	lo.num = temphi.num;
	save.num = temphi.num;
	flag = temphi.num & 1;
/*	temphi.num = temphi.num >> 1; not necessary */
	templo.num = templo.num >> 1;
	templo.num = (templo.num & 0x7fffffff) | (flag << 31);
	save.num = save.num ^ templo.num;
	hi.num = save.num;
	save.num = save.num >> 2;
	ans->num = (save.num & 0X7fffffff) % x.num; /* force result non-negative */
	signal(ERR_ok);
	}
