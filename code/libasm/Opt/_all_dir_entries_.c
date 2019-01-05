
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _all_dir_entries_.c,v 1.4 91/06/06 13:42:42 dcurtis Exp $";
#endif
/* $Log:	_all_dir_entries_.c,v $
 * Revision 1.4  91/06/06  13:42:42  dcurtis
 * added copyright notice
 * 
 * Revision 1.3  91/06/06  13:42:20  dcurtis
 * *** empty log message ***
 * 
 * Revision 1.2  91/06/03  17:25:55  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:20:10  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_all_dir_entries_	*/
/*						*/

#include <sys/types.h>
#include <sys/dir.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"


extern CLUREF clu_empty_string;
extern char **environ;
extern int wrpipe;
#include <errno.h>

static int argv = 0;

errcode _all_dir_entries_(dir, for_body, locals, iecode)
CLUREF dir;
errcode (*for_body)();
char *locals;
errcode *iecode;
{
int uerr;
DIR *dirp;
errcode err;
CLUREF name, name_len, lenp1;
int i;
struct direct *dp;

	dirp = opendir(dir.str->data);
	if (dirp == (DIR *)NULL) signal(ERR_ok);
	for (;;) {
		errno = 0;
		dp = readdir(dirp);
		if (dp == (struct direct *)NULL) break;
		if (dp ->d_reclen == 1 && !strcmp(dp->d_name, ".")) continue;
		if (dp ->d_reclen == 2 && !strcmp(dp->d_name, "..")) continue;
		name_len.num = dp->d_reclen;
		lenp1.num = name_len.num + 1;
		err = _bytevecOPcreate(lenp1, &name);
		if (err != ERR_ok) resignal(err);
		for (i = 0 ; i < name_len.num ; i++) {
			name.str->data[i] = dp->d_name[i];
			}
		name.str->data[i] = '\000';
		err = for_body(CLU_1, name, locals, iecode);
		if (err == ERR_ok) continue;
		if (err == ERR_break) break;
		if (err == ERR_forbodyexit) signal(err);
		if (err == ERR_forbodyreturn) signal(err);
		if (err == ERR_forbodysignal) signal(err);
		if (err == ERR_iterforbodyexit) signal(err);
		if (err == ERR_iterforbodyreturn) signal(err);
		if (err == ERR_iterforbodysignal) signal(err);
		}
	close(dirp);
	signal(ERR_ok);
	}
