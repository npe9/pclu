
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _job.c,v 1.5 91/06/06 13:48:22 dcurtis Exp $";
#endif
/* $Log:	_job.c,v $
 * Revision 1.5  91/06/06  13:48:22  dcurtis
 * added copyright notice
 * 
 * Revision 1.4  91/06/03  17:19:09  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.3  91/05/31  13:07:32  root
 * fixed aggregate initialization in ops_actual
 * 
 * Revision 1.2  91/02/14  16:41:29  dcurtis
 * added op structures
 * 
 * Revision 1.1  91/02/04  23:20:43  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_job			*/
/*						*/

#include <errno.h>

#include <stdio.h>
#include <signal.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>

extern errcode _jobOPcons0();
extern errcode _jobOPcone0();
extern errcode _jobOPOPdead_set();
extern errcode _jobOP_wordvec_create();
extern errcode _jobOPnew_environment();

extern char **envp;
extern char **argv;

typedef struct {
	CLUREF	fn;
	CLUREF	rd;
	CLUREF	wr;
	CLUREF	typ;
	CLUREF	new;
	CLUREF	perm;
	} _chan;

static int _jobOPgrpmod = 0;
static int _jobOPdead_cnt = 0;

errcode _jobOPcons(fn, jcl, pri, pro, ero, save, ans)
CLUREF fn, jcl;
_chan *pri, *pro, *ero;
CLUREF save, *ans;
{
errcode err;
CLUREF temp;

	if (pri->rd.num < 0) {
		elist[0].str = cannot_read_from_input__chan_STRING;
		signal(ERR_not_possible);
		}	
	if (pro->wr.num < 0) {
		elist[0].str = cannot_write_to_output__chan_STRING;
		signal(ERR_not_possible);
		}	
	if (ero->wr.num < 0) {
		elist[0].str = cannot_write_to_error__chan_STRING;
		signal(ERR_not_possible);
		}	
	err = _jobOPcons0(fn, jcl, pri->rd, pro->wr, ero->wr, save, &temp);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	signal(ERR_ok);
	}
	
errcode _jobOPcons1(fn, jcl, save, ans)
CLUREF fn, jcl;
CLUREF save, *ans;
{
errcode err;
CLUREF temp, r, w, e;

	r.num = 0;
	w.num = 1;
	e.num = 2;
	err = _jobOPcons0(fn, jcl, r, w, e, save, &temp);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	signal(ERR_ok);
	}

errcode _jobOPcons0(fn, jcl, pri, pro, ero, save, ans)
CLUREF fn, jcl;
int pri, pro, ero;
CLUREF save, *ans;
{
errcode err;
CLUREF temp, v, b, fs;
int uerr, nfd, i, j;
int pid, pgroup;
union wait status;
struct sigaction vec;
_chan *this_ch;
char **lenvp;
int limit;
char *str_addr;
char **s;
sigset_t sigset;

	err = _jobOPOPdead_set();
	if (err != ERR_ok) resignal(err);
	err = _job_stuff(fn, jcl, &v, &b, &fs);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	errno = 0;
	uerr = fork();
	if (uerr == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (uerr != 0) {	/* parent code */
		if (errno == 0) {
			ans->num = uerr;
			signal(ERR_ok);
			}
/* the following code does not seem to get executed:
	I'm going to keep it for a while, in case it becomes useful */
		pid = uerr;
		for (;;) {
			status.w_status = WNOHANG;
			uerr = wait(&status);
			if (uerr == pid) break;
			if (uerr == 0) break;
			if (uerr == EINTR) continue;
			if (uerr <= 0) break;
			}
		if (_jobOPgrpmod < 0) {
			uerr = getpgrp(0);
			if (uerr > 0) {
				pgroup = uerr;
				uerr = ioctl(0, TIOCSPGRP, &pgroup);
				}
			}
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	
	/* child code */
	if (_jobOPgrpmod != 0) {
		pid = getpid();
		setpgrp(pid, pid);
		if (_jobOPgrpmod < 0) {
			ioctl(0, TIOCSPGRP, pid);
			}
	}
	vec.sa_handler = 0;
	sigemptyset(&sigset);
	vec.sa_mask = sigset;
	vec.sa_flags = 0;
	sigaction(SIGFPE, &vec, 0);
	sigaction(SIGPIPE, &vec, 0);
	if (pri > 0) {
		uerr = dup2(pri, 0);
		if (uerr < 0) {
			errno = uerr;
			_exit(15);
			}
		}		
	if (pro != 1) {
		uerr = dup2(pro, 1);
		if (uerr < 0) {
			errno = uerr;
			_exit(15);
			}
		}
	if (ero != 2) {
		uerr = dup2(ero, 2);
		if (uerr < 0) {
			errno = uerr;
			_exit(15);
			}
		}
	nfd = getdtablesize();
	for (i = 3; i < nfd; i++) {
		if (save.vec->size == 0) {
			fcntl(i, F_SETFD, 1);
			continue;}
		for (j = 0; j < save.vec->size; j++) {
			this_ch = (_chan *)save.vec->data[j];
			if (this_ch->rd.num == i || this_ch->wr.num == i) {
				fcntl(i, F_SETFD, 0);
				}
			}
		}
/*
	fprintf(stderr, "BEGIN STRINGS %d\n", b.vec->size);
	fprintf(stderr, "%s\n", b.vec->data);
	fprintf(stderr, "END STRINGS\n");
*/
	limit = v.str->size/CLUREFSZ;
/*
	fprintf(stderr, "BEGIN VEC %d\n", limit);
	for (i = 0 ; i < limit; i++) {
		fprintf(stderr, "%d\n", v.vec->data[i]);
		}
	fprintf(stderr, "END VEC\n");
*/
	str_addr = b.str->data;
	for (i = 0; i < limit; i++) {
/*
		fprintf(stderr, "i %d str_addr %lX delta %lX", i, 
			str_addr, v.vec->data[i+1]);
*/
		if (v.vec->data[i+1] == 0) {v.vec->data[i] = 0; break;}
		v.vec->data[i] = (long)(str_addr + v.vec->data[i+1] - 1);
/*
		fprintf(stderr, " result %lX\n", v.vec->data[i]);
*/
		}
/*
	fprintf(stderr, " result %lX\n", v.vec->data[i]);
	fprintf(stderr, "BEGIN VEC %d\n", limit);
	for (i = 0 ; i < limit; i++) {
		fprintf(stderr, "%d\n", v.vec->data[i]);
		}
	fprintf(stderr, "END VEC\n");
*/
	lenvp = envp;
/*
	fprintf(stderr, "BEGIN ENV\n");
	for (s = lenvp, i = 0 ; i < 10; s++, i++) {
		fprintf(stderr, "%s\n", *s);
		}
	fprintf(stderr, "END ENV\n");
*/
	uerr = execve(fs.vec->data, v.vec->data, lenvp);
	if (uerr == -1 && errno == ENOEXEC) {
		for (i = 0 ; ; i++) {
			argv[i] = (char*)b.vec->data[i+1];
			if (argv[i] == 0) break;
			}
		argv[0] = "/bin/sh";
		uerr = execve("/bin/sh", argv, lenvp);
		}
	errno = uerr;
	_exit(15);
	}


errcode _jobOPcone(fn, jcl, envs, pri, pro, ero, save, ans)
CLUREF fn, jcl, envs;
_chan *pri, *pro, *ero;
CLUREF save, *ans;
{
errcode err;
CLUREF temp;

	if (pri->rd.num < 0) {
		elist[0].str = cannot_read_from_input__chan_STRING;
		signal(ERR_not_possible);
		}	
	if (pro->wr.num < 0) {
		elist[0].str = cannot_write_to_output__chan_STRING;
		signal(ERR_not_possible);
		}	
	if (ero->wr.num < 0) {
		elist[0].str = cannot_write_to_error__chan_STRING;
		signal(ERR_not_possible);
		}	
	err = _jobOPcone0(fn, jcl, envs, pri->rd, pro->wr, ero->wr, save, &temp);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	signal(ERR_ok);
	}

errcode _jobOPcone1(fn, jcl, envs, save, ans)
CLUREF fn, jcl, envs;
CLUREF save, *ans;
{
errcode err;
CLUREF temp, r, w, e;

	r.num = 0;
	w.num = 1;
	e.num = 2;
	err = _jobOPcone0(fn, jcl, envs, r, w, e, save, &temp);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	ans->num = temp.num;
	signal(ERR_ok);
	}

errcode _jobOPcone0(fn, jcl, envs, pri, pro, ero, save, ans)
CLUREF fn, jcl;
int pri, pro, ero;
CLUREF save, *ans;
{
errcode err;
CLUREF temp, v, b, fs;
int uerr;
int pid, pgroup, nfd, i, j;
CLUREF envwv, envstrs;
union wait status;
struct sigaction vec;
_chan *this_ch;
sigset_t sigset;

	err = _jobOPOPdead_set();
	if (err != ERR_ok) resignal(err);
	err = _jobOP_wordvec_create(envs, &envwv);
	if (err != ERR_ok) resignal(err);
	err = _jobOPnew_environment(envs, &envstrs);
	if (err != ERR_ok) resignal(err);
	err = _job_stuff(fn, jcl, &v, &b, &fs);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	errno = 0;
	uerr = fork();
	if (uerr == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (uerr != 0) {	/* parent code */
		if (errno == 0) {
			ans->num = uerr;
			signal(ERR_ok);
			}
		pid = uerr;
		for (;;) {
			status.w_status = WNOHANG;
			uerr = wait(&status);
			if (uerr == pid) break;
			if (uerr == 0) break;
			if (uerr == EINTR) continue;
			if (uerr <= 0) break;
			}
		if (_jobOPgrpmod < 0) {
			uerr = getpgrp(0);
			if (uerr > 0) {
				pgroup = uerr;
				uerr = ioctl(0, TIOCSPGRP, &pgroup);
				}
			}
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	
	/* child code */
	if (_jobOPgrpmod != 0) {
		pid = getpid();
		setpgrp(pid, pid);
		if (_jobOPgrpmod < 0) {
			ioctl(0, TIOCSPGRP, pid);
			}
		}
	vec.sa_handler = 0;
	sigemptyset(&sigset);
	vec.sa_mask = sigset;
	vec.sa_flags = 0;
	sigaction(SIGFPE, &vec, 0);
	sigaction(SIGPIPE, &vec, 0);
	if (pri > 0) {
		uerr = dup2(pri, 0);
		if (uerr < 0) {
			errno = uerr;
			_exit(15);
			}
		}
	if (pro != 1) {
		uerr = dup2(pro, 1);
		if (uerr < 0) {
			errno = uerr;
			_exit(15);
			}
		}
	if (ero != 2) {
		uerr = dup2(ero, 2);
		if (uerr < 0) {
			errno = uerr;
			_exit(15);
			}
		}
	nfd = getdtablesize();
	for (i = 3; i < nfd; i++) {
		if (save.vec->size == 0) {
			fcntl(i, F_SETFD, 1);
			continue;}
		for (j = 0; j < save.vec->size; j++) {
			this_ch = (_chan *)save.vec->data[j];
			if (this_ch->rd.num == i || 
					this_ch->wr.num == i) {
				fcntl(i, F_SETFD, 0);
				}
			}
		}
	for (i = 0; i < v.vec->size; i++) {
		temp.num = b.vec->data[i];
		temp.vec->data[0] = temp.vec->data[0] + (int)b.vec->data;
		}
	uerr = execve(fs.vec->data, b.vec->data, envwv.vec->data);
	if (uerr == -1 && errno == ENOEXEC) {
		for (i = 0 ; ; i++) {
			argv[i] = (char *)b.vec->data[i+1];
			if (argv[i] == 0) break;
			}
		argv[0] = "/bin/sh";
		uerr = execve("/bin/sh", argv, envwv.vec->data[0]);
		}
	errno = uerr;
	_exit(15);
	}

void _jobOPOPdeath()
{
	_jobOPdead_cnt += 1;
	return;
	}

errcode _jobOPOPdead_set()
{
struct sigaction vec;
sigset_t sigset, temp;

	vec.sa_handler = _jobOPOPdeath;
	vec.sa_flags = SA_STACK;
	/* vec.sa_mask = 0xff7bf0e0; */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGABRT);
	sigaddset(&sigset, SIGIOT);
	sigaddset(&sigset, SIGBUS);
	sigaddset(&sigset, SIGPIPE);
	sigaddset(&sigset, SIGALRM);
	sigaddset(&sigset, SIGSTKFLT);
	sigaddset(&sigset, SIGCHLD);
	sigaddset(&sigset, SIGCONT);
	sigaddset(&sigset, SIGSTOP);
	sigaddset(&sigset, SIGTTIN);
	sigaddset(&sigset, SIGTTOU);
	sigaddset(&sigset, SIGURG);
	sigaddset(&sigset, SIGXCPU);
	sigaddset(&sigset, SIGXFSZ);
	sigaddset(&sigset, SIGVTALRM);
	sigaddset(&sigset, SIGPROF);
	sigaddset(&sigset, SIGWINCH);
	vec.sa_mask = sigset;
	sigaction(SIGCHLD, &vec, 0);
	signal(ERR_ok);
	}

errcode _jobOPexec(fn, jcl, save, ans)
CLUREF fn, jcl;
CLUREF save, *ans;
{
errcode err;
CLUREF temp, v, b, fs, sz;
int uerr, ind;
int pid, nfd, i, j;
_chan *this_ch;

	err = _job_stuff(fn, jcl, &v, &b, &fs);
	if (err != ERR_ok) resignal(err);
	nfd = getdtablesize();
	for (i = 3; i < nfd; i++) {
		if (save.vec->size == 0) {
			fcntl(i, F_SETFD, 1);
			continue;}
		for (j = 0; j < save.vec->size; j++) {
			this_ch = (_chan *)save.vec->data[j];
			if (this_ch->rd.num == i || this_ch->wr.num == i) {
				fcntl(i, F_SETFD, 0);
				}
			}
		}
	/* create a char** array from v and b */
	sz.num = v.vec->data[0];
	sequenceOPnew2(sz, &temp);
	for (i = 0; i < sz.num; i++) {
		ind = v.vec->data[i+1] - 1;
		temp.vec->data[i] = (int)&b.str->data[ind];
		}
	temp.vec->data[i] = 0;
	uerr = execve(&(fs.str->data[0]), &(temp.vec->data[0]), envp);
	if (uerr ==  -1 && errno == ENOEXEC) {
		for (i = 0 ; ; i++) {
			argv[i] = (char *)b.vec->data[i+1];
			if (argv[i] == 0) break;
			}
		argv[0] = "/bin/sh";
		uerr = execve("/bin/sh", argv, envp);
		}
	errno = uerr;
	_exit(15);
	}

errcode _jobOPexece(fn, jcl, envs, save, ans)
CLUREF fn, jcl;
CLUREF save, *ans;
{
errcode err;
CLUREF temp, v, b, fs;
int uerr;
int pid, nfd, i, j;
CLUREF envwv, envstrs;
_chan *this_ch;

	err = _jobOP_wordvec_create(envs, &envwv);
	if (err != ERR_ok) resignal(err);
	err = _jobOPnew_environment(envs, &envstrs);
	if (err != ERR_ok) resignal(err);
	err = _job_stuff(fn, jcl, &v, &b, &fs);
	if (err != ERR_ok) resignal(err);
	nfd = getdtablesize();
	for (i = 3; i < nfd; i++) {
		if (save.vec->size == 0) {
			fcntl(i, F_SETFD, 1);
			continue;}
		for (j = 0; j < save.vec->size; j++) {
			this_ch = (_chan *)save.vec->data[j];
			if (this_ch->rd.num == i || this_ch->wr.num == i) {
				fcntl(i, F_SETFD, 0);
				}
			}
		}
	for (i = 0; i < v.vec->size; i++) {
		temp.num = b.vec->data[i];
		temp.vec->data[0] = temp.vec->data[0] + (int)b.vec->data;
		}
	uerr = execve(fs.vec->data, b.vec->data, envwv.vec->data);
	if (uerr ==  -1 && errno == ENOEXEC) {
		for (i = 0 ; ; i++) {
			argv[i] = (char *)b.vec->data[i+1];
			if (argv[i] == 0) break;
			}
		argv[0] = "/bin/sh";
		uerr = execve("/bin/sh", argv, envwv.vec->data);
		}
	errno = uerr;
	_exit(15);
	}

errcode _jobOPfork(ans1, ans2)
CLUREF *ans1, *ans2;
{
errcode err;
int uerr;

	err = _jobOPOPdead_set();
	if (err != ERR_ok) resignal(err);
	uerr = fork();
	if (uerr == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (uerr == 0) {
		ans1->num = getpid();
		ans2->tf = true;
		}
	ans1->num = uerr;
	ans2->tf = false;
	signal(ERR_ok);
	}
	
errcode _jobOPrun(j, ans)
CLUREF j, *ans;
{
int uerr;
union wait status;

	for (;;) {
		status.w_status = 0;
		uerr = wait(&status);
		if (uerr == -1 && errno == EINTR) continue;
		if (uerr == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (uerr != j.num) continue;

/* 		if (status.w_T.w_Termsig + status.w_T.w_Coredump == 0177) { */
/* 12/14/93: dcurtis: removed above line in favor of following line
*/
		if (!WIFSTOPPED(status)) {
			ans->num = WSTOPSIG(status);
			signal(ERR_ok);
			}
		uerr = kill(j.num, SIGCONT);
		if (uerr == 0) continue;
		if (uerr == EINTR) continue;
		break;
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
		

errcode _jobOPsig(j, signum)
CLUREF j, signum;
{
int uerr;

	uerr = kill(j.num, signum.num);
	if (uerr == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}


errcode _jobOPsig_group(j, signum)
CLUREF j, signum;
{
int uerr, pg;

	pg = getpgrp(j.num);
	if (pg == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	uerr = killpg(pg, signum.num);
	if (uerr == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}

errcode _jobOPdead(ans)
CLUREF *ans;
{
	ans->num = _jobOPdead_cnt;
	signal(ERR_ok);
	}

errcode _jobOPwait(j, ans)
CLUREF j, *ans;
{
union wait status;
int uerr;

	status.w_status = 0;
	for (;;) {
		uerr = wait(&status);
		if (uerr == -1 && errno == EINTR) continue;
		if (uerr == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (uerr != j.num) continue;
		ans->num = 0;
		if (WIFEXITED(status)) ans->num = WEXITSTATUS(status);
		if (WIFSIGNALED(status)) ans->num = WTERMSIG(status);
		signal(ERR_ok);
		}
	}

errcode _jobOPwait0(j, ans)
CLUREF j, *ans;
{
union wait status;
int uerr;

	status.w_status = 0;
	for (;;) {
		uerr = wait3(&status, WNOHANG, 0);
		if (uerr == -1 && errno == EINTR) continue;
		if (uerr == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (uerr == 0) {
			ans->num = -1;
			signal(ERR_ok);
			}
		if (uerr != j.num) continue;
		ans->num = 0;
		if (WIFEXITED(status)) ans->num = WEXITSTATUS(status);
		if (WIFSIGNALED(status)) ans->num = WTERMSIG(status);
		signal(ERR_ok);
		}
	}

errcode _jobOPany_wait(ans1, ans2)
CLUREF *ans1, *ans2;
{
union wait status;
int uerr;

	status.w_status = WUNTRACED;
	for (;;) {
		uerr = wait(&status);
		if (uerr == -1 && errno == EINTR) continue;
		if (uerr == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		ans1->num = uerr;
		ans2->num = 0;
		if (WIFEXITED(status)) ans2->num = WEXITSTATUS(status);
		if (WIFSIGNALED(status)) ans2->num = WTERMSIG(status);
		signal(ERR_ok);
		}
	}


errcode _jobOPany_wait0(ans1, ans2)
CLUREF *ans1, *ans2;
{
union wait status;
int uerr;

	status.w_status = WNOHANG;
	for (;;) {
		uerr = wait(&status);
		if (uerr == -1 && errno == EINTR) continue;
		if (uerr == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		ans1->num = uerr;
		ans2->num = 0;
		if (WIFEXITED(status)) ans2->num = WEXITSTATUS(status);
		if (WIFSIGNALED(status)) ans2->num = WTERMSIG(status);
		signal(ERR_ok);
		}
	}


errcode _jobOPself(ans)
CLUREF *ans;
{
int self;

	self = getpid();
	ans->num = self;
	signal(ERR_ok);
	}


errcode _jobOPget_pid(j, ans)
CLUREF j, *ans;
{
	ans->num = j.num;
	signal(ERR_ok);
	}

errcode _jobOPget_group(j, ans)
CLUREF j, *ans;
{
int uerr;

	uerr = getpgrp(j);
	if (uerr >= 0) {
		ans->num = uerr;
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}

errcode _jobOPset_group(j, g, ans)
CLUREF j, g, *ans;
{
int uerr;

	uerr = setpgrp(j, g);
	if (uerr == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}


errcode _jobOPcontrol_tty(j)
CLUREF j;
{
int pgrp;
int uerr;

	pgrp = getpgrp(j);
	if (pgrp >= 0) {
		uerr = ioctl(0, TIOCSPGRP, &pgrp);
		if (uerr == 0) signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}

errcode _jobOPget_group_mode(ans)
CLUREF *ans;
{
	ans->num = _jobOPgrpmod;
	signal(ERR_ok);
	}


errcode _jobOPset_group_mode(mode)
CLUREF mode;
{
	_jobOPgrpmod = mode.num;
	signal(ERR_ok);
	}


errcode _jobOPequal(j1, j2, ans)
CLUREF j1, j2, *ans;
{
	if (j1.num == j2.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _jobOPsimilar(j1, j2, ans)
CLUREF j1, j2, *ans;
{
	if (j1.num == j2.num) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _jobOPcopy(j, ans)
CLUREF j, *ans;
{
	ans->ref = j.ref;
	signal(ERR_ok);
	}

errcode _jobOP_wordvec_create(envs, ans)
CLUREF envs, *ans;
{
errcode err;
CLUREF temp, sz;

	sz.num = envs.vec->size + 1;
	err = _wordvecOPcreate(sz, &temp);
	if (err != ERR_ok) resignal(err);
	ans->ref = temp.ref;
	signal(ERR_ok);
	}

errcode _jobOPnew_environment(envs, ans)
CLUREF envs, *ans;
{	
	ans->ref = envs.ref;
	signal(ERR_ok);
	}		
typedef struct {
int count;
OWNPTR	type_owns;
OWNPTR  op_owns;
struct OP_ENTRY entry[24];
} _job_OPS;

OWN_ptr _job_own_init = {1, 0};

CLU_proc _job_oe_cons = {{0,0,0,0}, _jobOPcons, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_cons1 = {{0,0,0,0}, _jobOPcons1, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_cone = {{0,0,0,0}, _jobOPcone, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_cone1 = {{0,0,0,0}, _jobOPcone1, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_exec = {{0,0,0,0}, _jobOPexec, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_exece = {{0,0,0,0}, _jobOPexece, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_fork = {{0,0,0,0}, _jobOPfork, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_run = {{0,0,0,0}, _jobOPrun, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_sig = {{0,0,0,0}, _jobOPsig, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_sig_group = {{0,0,0,0}, _jobOPsig_group, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_wait = {{0,0,0,0}, _jobOPwait, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_wait0 = {{0,0,0,0}, _jobOPwait0, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_any_wait = {{0,0,0,0}, _jobOPany_wait, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_any_wait0 = {{0,0,0,0}, _jobOPany_wait0, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_equal = {{0,0,0,0}, _jobOPequal, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_similar = {{0,0,0,0}, _jobOPsimilar, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_copy = {{0,0,0,0}, _jobOPcopy, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_self = {{0,0,0,0}, _jobOPself , &_job_own_init,&_job_own_init};
CLU_proc _job_oe_get_pid = {{0,0,0,0}, _jobOPget_pid, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_get_group = {{0,0,0,0}, _jobOPget_group, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_control_tty = {{0,0,0,0}, _jobOPcontrol_tty, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_get_group_mode = {{0,0,0,0}, _jobOPget_group_mode, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_set_group_mode = {{0,0,0,0}, _jobOPset_group_mode, &_job_own_init,&_job_own_init};
CLU_proc _job_oe_set_group = {{0,0,0,0}, _jobOPset_group, &_job_own_init,&_job_own_init};
_job_OPS _job_ops_actual = {24,
&_job_own_init,
&_job_own_init, {
{&_job_oe_equal, "equal"},
{&_job_oe_similar, "similar"},
{&_job_oe_copy, "copy"},
{&_job_oe_self, "self"},
{&_job_oe_get_pid, "get_pid"},
{&_job_oe_get_group, "get_group"},
{&_job_oe_set_group, "set_group"},
{&_job_oe_cons, "cons"},
{&_job_oe_cons1, "cons1"},
{&_job_oe_cone, "cone"},
{&_job_oe_cone1, "cone1"},
{&_job_oe_exec, "exec"},
{&_job_oe_exece, "exece"},
{&_job_oe_fork, "fork"},
{&_job_oe_run, "run"},
{&_job_oe_sig, "sig"},
{&_job_oe_sig_group, "sig_group"},
{&_job_oe_wait, "wait"},
{&_job_oe_wait0, "wait0"},
{&_job_oe_any_wait, "any_wait"},
{&_job_oe_any_wait0, "any_wait0"},
{&_job_oe_control_tty, "control_tty"},
{&_job_oe_get_group_mode, "get_group_mode"},
{&_job_oe_set_group_mode, "set_group_mode"}}
};

_job_OPS *_job_ops = &_job_ops_actual;
