
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _environs.c,v 1.5 91/07/09 13:08:53 root Exp $";
#endif
/* $Log:	_environs.c,v $
 * Revision 1.5  91/07/09  13:08:53  root
 * changed args to stringOPcons to CLUREFs (sun compatibility)
 * 
 * Revision 1.4  91/06/06  13:24:51  root
 * added copyright notice
 * 
 * Revision 1.3  91/05/31  11:51:16  root
 * fixed checkin comment
 * 
 * Revision 1.2  91/05/31  11:50:38  root
 * removed unused variable declaration
 * 
 * Revision 1.1  91/02/04  15:49:33  mtv
 * Initial revision
 * 
 */


/*							*/
/*							*/
/*		IMPLEMENTATION OF			*/
/*			_environ			*/
/*	an iterator that yields the environment strings */
/*							*/

#include "pclu_err.h"
#include "pclu_sys.h"

extern char **envp;

errcode _environs(iterbod, locals, iecode)
char * locals;
errcode (*iterbod)();
errcode *iecode;
{
errcode err;
CLUREF iv_1, len;
int index;

	if (envp[0] == NULL) signal(ERR_ok);
	index = 0;
	for (index = 0; envp[index] != NULL; index++) {
		len.num = strlen(envp[index]);
		err = stringOPcons(envp[index], CLU_1, len, &iv_1);
		if (err != ERR_ok) resignal(err);
		err = iterbod(iv_1, locals, iecode);
		if (err == ERR_ok) continue;
		/* 9/9/93 dwc: just propagate break, do not convert to ok */
		if (err == ERR_break) signal(ERR_break);
		if (err == ERR_iterbodyexit) signal(err);
		if (err == ERR_iterbodyreturn) signal(err);
		if (err == ERR_iterbodysignal) signal(err);
		if (err == ERR_iteriterbodyexit) signal(err);
		if (err == ERR_iteriterbodyreturn) signal(err);
		if (err == ERR_iteriterbodysignal) signal(err);
		}
	signal(ERR_ok);
	}


