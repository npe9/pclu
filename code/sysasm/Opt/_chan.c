
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/_chan.c,v 1.11 91/09/27 10:18:56 dcurtis Exp $";
#endif
/* $Log:	_chan.c,v $
 * Revision 1.11  91/09/27  10:18:56  dcurtis
 * fixed literal output to ttys in put functions
 * 	per sony news fixes from Howard Wilkinson
 * 
 * Revision 1.10  91/07/18  14:38:26  root
 * eliminate warnings for addresses used in sigvec calls
 * 
 * Revision 1.9  91/06/06  13:24:06  root
 * added copyright notice
 * 
 * Revision 1.8  91/06/03  16:20:07  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.7  91/05/31  11:47:40  root
 * fixed aggregate initialization in ops_actual
 * removed some unused variable declarations
 * fixed _chan$similar == => =
 * 
 * Revision 1.6  91/02/18  13:37:02  dcurtis
 * removed definition of elist as a local variablein _chan$$set_tty
 * 
 * Revision 1.5  91/02/14  16:10:40  dcurtis
 * fixed get to return max, max when last char in buf is terminator,
 * rather than max, 0
 * 
 * Revision 1.4  91/02/11  14:09:52  dcurtis
 * added signal(ERR_ok) to socket
 * /
 * 
 * Revision 1.3  91/02/11  11:32:06  dcurtis
 * _chanOPsendto: changed send to sendto & initialized size & asize
 * _chanOPputb: >= to > in bounds test on low: now single byte strings 
 * 		can be sent
 * 
 * Revision 1.2  91/02/08  15:20:19  dcurtis
 * fixed warnings related to signal handler signatures
 * 
 * Revision 1.1  91/02/04  15:49:30  mtv
 * Initial revision
 * 
 * Revision 1.1  91/02/04  15:45:49  mtv
 * Initial revision
 * 
 */

#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

#include "pclu_err.h"       
#include "pclu_sys.h"       

#include <errno.h>
#include <sys/types.h>
#include <sys/file.h>
/* #include <sys/ioctl.h> */
#ifdef LINUX
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#else
/* #include <sgtty.h> */
#endif
#include <sys/stat.h>

extern char *index();
extern int _chanOPtstop();
extern int _chanOPtdie();
extern errcode _chanOPOPset_tty();
extern errcode _chanOPOP_put_doit();
extern errcode _chanOPOPreadit();

typedef struct {
	CLUREF	fn;
	CLUREF	rd;
	CLUREF	wr;
	CLUREF	typ;
	CLUREF	new;
	CLUREF	perm;
	} _chan;

typedef char  * CLUREFP;

#define tty 0
#define bty 1
#define nul 2
#define oth 3

#define SIGNAL_MASK 0xff7bf0e0

static long _chan_defmode;
long wrpipe = 0; 
static long rdpipe = 0; 
static _chan * _chan_pri = NULL;
static _chan * _chan_pro = NULL;
static _chan * _chan_ero = NULL;
static _chan * _chan_nul = NULL;
static struct termios isbuf;
static struct termios sbuf;
static long ilbuf;
static long lbuf;

int speeds[16] = {50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800,
			9600, 19200, 38400};

errcode _chanOPopen(fn, mode, fmode, uchan)
CLUREF fn;
CLUREF mode;
CLUREF fmode;
CLUREF *uchan;
{
/*
static bool illegal_access_mode_EXISTS = false;
static CLUSTRING illegal_access_mode_STRING;
*/
long flags = -1;
CLUREF fullname;
_chan *newch;
CLUREF fname;
int acc;
int fd;
errcode err;
int current_mode;
struct termios obuf;
CLUREF temp_str;

	if (!strcmp(mode.str->data, "read")) flags = 0;
	if (!strcmp(mode.str->data, "write"))
			flags = O_WRONLY + O_TRUNC + O_CREAT;
	if (!strcmp(mode.str->data, "append"))
			flags = O_WRONLY + O_APPEND + O_CREAT;
	if (!strcmp(mode.str->data, "random"))
			flags = O_RDWR + O_TRUNC + O_CREAT;
	if (!strcmp(mode.str->data, "modify"))
			flags = O_RDWR + O_CREAT;
	if (flags == -1) {
		elist[0].str = illegal_access_mode_STRING;
		signal(ERR_not_possible);}
	clu_alloc(sizeof(_chan), &newch);
	err = stringOPcons("", CLU_1, CLU_0, &temp_str);
	if (err != ERR_ok) goto def_err_hdlr;
	err = file_name_fill(fn, temp_str, &fullname);
	if (err != ERR_ok) goto def_err_hdlr;
	newch->fn = fullname;
	err = file_nameOPunparse(newch->fn, &fname);	
	/* assume fname is zero terminated */
	newch->new.tf = false;
	if (flags & O_WRONLY || flags & O_RDWR) {
		newch->new.tf = true;
		if (flags & O_TRUNC || flags & O_CREAT) {
			acc = access(fname.str->data, F_OK);
			if (acc == 0) newch->new.tf = false;
			}
		}
	current_mode = 0666;
	if (_chan_defmode != 0) current_mode = _chan_defmode;
	if (fmode.num != 0) current_mode = fmode.num;
	fd = open(fname.str->data, flags, current_mode);
	if (fd < 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	newch->rd.num = -1;
	newch->wr.num = -1;
	if (flags & O_RDWR || (!flags & 1)) {
		newch->rd.num = fd;}
	if (flags & O_RDWR || flags & O_WRONLY) {
		newch->wr.num = fd;}
	newch->typ.num = oth;
	err = tcgetattr(fd, &obuf);
	if (err == 0) {
		newch->typ.num = tty;	
		if (newch->rd.num == newch->wr.num) {
			obuf.c_iflag |= ICRNL;
			obuf.c_oflag |= OPOST + ONLCR;
			obuf.c_lflag &= ~(ICANON + ECHO);
			obuf.c_cc[VMIN] = 1;
			obuf.c_cc[VTIME] = 0;
			err = tcsetattr(fd, TCSANOW, &obuf);
			if (err != 0) {
				close(fd);
				elist[0] = _unix_erstr(errno);
				signal(ERR_not_possible);
				}
			}
		}
	err = _fixup_file_name(newch->fn, CLU_0, &fullname);
	if (err != ERR_ok) goto def_err_hdlr;
	newch->fn = fullname;
	uchan->ref = (char *)newch;
	signal(ERR_ok);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}
 
errcode _chanOP_open(fn, flags, fmode, uchan)
CLUREF fn;
CLUREF flags;
CLUREF fmode;
CLUREF *uchan;
{
CLUREF fullname;
_chan *newch;
CLUREF fname;
int acc;
int fd;
errcode err;
int current_mode;
struct termios obuf;
CLUREF temp_str;

	clu_alloc(sizeof(_chan), &newch);
	err = stringOPcons("", CLU_1, CLU_0, &temp_str);
	if (err != ERR_ok) goto def_err_hdlr;
	err = file_name_fill(fn, temp_str, &fullname);
	if (err != ERR_ok) goto def_err_hdlr;
	newch->fn = fullname;
	err = file_nameOPunparse(newch->fn, &fname);	
	/* assume fname is zero terminated */
	newch->new.tf = false;
	if (flags.num & O_WRONLY || flags.num & O_RDWR) {
		newch->new.tf = true;
		if (flags.num & O_TRUNC || flags.num & O_CREAT) {
			acc = access(fname.str->data, F_OK);
			if (acc == 0) newch->new.tf = false;
			}
		}
	current_mode = 0666;
	if (_chan_defmode != 0) current_mode = _chan_defmode;
	if (fmode.num != 0) current_mode = fmode.num;
	fd = open(fname.str->data, flags.num, current_mode);
	if (fd < 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	newch->rd.num = -1;
	newch->wr.num = -1;
	if (flags.num & O_RDWR || (!flags.num & 1)) {
		newch->rd.num = fd;}
	if (flags.num & O_RDWR || flags.num & O_WRONLY) {
		newch->wr.num = fd;}
	newch->typ.num = oth;
	err = tcgetattr(fd, &obuf);
	if (err == 0) {
		newch->typ.num = tty;	
		if (newch->rd.num == newch->wr.num) {
			obuf.c_iflag |= ICRNL;
			obuf.c_oflag |= OPOST + ONLCR;
			obuf.c_lflag &= ~(ICANON + ECHO);
			obuf.c_cc[VMIN] = 1;
			obuf.c_cc[VTIME] = 0;
			err = tcsetattr(fd, TCSANOW, &obuf);
			if (err != 0) {
				close(fd);
				elist[0] = _unix_erstr(errno);
				signal(ERR_not_possible);
				}
			}
		}
	err = _fixup_file_name(newch->fn, CLU_0, &fullname);
	if (err != ERR_ok) goto def_err_hdlr;
	newch->fn = fullname;
	uchan->ref = (char *)newch;
	signal(ERR_ok);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}

errcode _chanOPpipe(ans1, ans2)
CLUREF *ans1, *ans2;
{
_chan *temp_chan1, *temp_chan2;
int fildes[2];
CLUREF temp_str, fname;
int result;
errcode err;

	err = stringOPcons("/dev/pipe", CLU_1, CLU_9, &temp_str);
	if (err != 0) goto def_err_hdlr;
	err = file_nameOPparse(temp_str, &fname);
	if (err != 0) goto def_err_hdlr;
	clu_alloc(sizeof(_chan), &temp_chan1);
	temp_chan1->fn.str = fname.str;
	temp_chan1->wr.num = -1;
	temp_chan1->typ.num = oth;
	clu_alloc(sizeof(_chan), &temp_chan2);
	temp_chan2->fn.str = fname.str;
	temp_chan2->rd.num = -1;
	temp_chan2->typ.num = oth;

	result = pipe(fildes);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	temp_chan1->rd.num = fildes[0];
	temp_chan2->wr.num = fildes[1];
	ans1->ref = (char *) temp_chan1;
	ans2->ref = (char *) temp_chan2;
	signal(ERR_ok);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}
 
errcode _chanOPsocket(family, socktype, protocol, ans)
CLUREF family, socktype, protocol;
CLUREF *ans;
{
_chan *temp_chan;
CLUREF temp_str, fname;
int result;
errcode err;

	err = stringOPcons("/dev/socket", CLU_1, CLU_11, &temp_str);
	if (err != 0) goto def_err_hdlr;
	err = file_nameOPparse(temp_str, &fname);
	if (err != 0) goto def_err_hdlr;
	clu_alloc(sizeof(_chan), &temp_chan);
	temp_chan->fn.str = fname.str;
	temp_chan->typ.num = oth;

	result = socket(family.num, socktype.num, protocol.num);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	temp_chan->rd.num = result;
	temp_chan->wr.num = result;
	ans->ref = (char *) temp_chan;
	signal(ERR_ok);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}

errcode _chanOPsocketpair(domain, socktype, protocol, ans1, ans2)
CLUREF domain, socktype, protocol;
CLUREF *ans1, *ans2;
{
_chan *temp_chan1, *temp_chan2;
int fildes[2];
CLUREF temp_str, fname;
int result;
errcode err;

	err = stringOPcons("/dev/socket", CLU_1, CLU_11, &temp_str);
	if (err != 0) goto def_err_hdlr;
	err = file_nameOPparse(temp_str, &fname);
	if (err != 0) goto def_err_hdlr;
	clu_alloc(sizeof(_chan), &temp_chan1);
	temp_chan1->fn.str = fname.str;
	temp_chan1->typ.num = oth;
	clu_alloc(sizeof(_chan), &temp_chan2);
	temp_chan2->fn.str = fname.str;
	temp_chan2->typ.num = oth;

	result = socketpair(fildes);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	temp_chan1->rd.num = fildes[0];
	temp_chan1->wr.num = fildes[0];
	temp_chan2->rd.num = fildes[1];
	temp_chan2->wr.num = fildes[1];
	ans1->ref = (char *) temp_chan1;
	ans2->ref = (char *) temp_chan2;
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}
 
errcode _chanOPprimary_input(newchan)
CLUREF *newchan;
{
_chan * temp_chan;
errcode err;
CLUREF chanref;

	if (_chan_pri != NULL) {
		newchan->ref = (char *)_chan_pri;
		signal(ERR_ok);
		}
	clu_alloc(sizeof(_chan), &temp_chan);
	_chan_pri = temp_chan;
	_chan_pri->wr.num = -1;
	_chan_pri->rd.num = 0;
	chanref.ref = (char*)_chan_pri;
	err =  _chanOPOPset_tty(chanref, &chanref);
	if (err != ERR_ok) goto def_err_hdlr;
	newchan->ref = (char *)_chan_pri;
	signal(ERR_ok);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}
 
errcode _chanOPprimary_output(newchan)
CLUREF *newchan;
{
_chan * temp_chan;
errcode err;
CLUREF chanref;
	if (_chan_pro != NULL) {
		newchan->ref = (char *)_chan_pro;
		signal(ERR_ok);
		}
	clu_alloc(sizeof(_chan), &temp_chan);
	_chan_pro = temp_chan;
	_chan_pro->rd.num = -1;
	_chan_pro->wr.num = 1;
	chanref.ref = (char*)_chan_pro;
	err = _chanOPOPset_tty(chanref, &chanref);
	if (err != ERR_ok) goto def_err_hdlr;
	newchan->ref = (char *)_chan_pro;
	signal(ERR_ok);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}
 
errcode _chanOPerror_output(newchan)
CLUREF *newchan;
{
_chan * temp_chan;
errcode err;
CLUREF fname;
CLUREF temp_str;
	if (_chan_ero != NULL) {
		newchan->ref = (char *)_chan_ero;
		signal(ERR_ok);
		}
	err = stringOPcons("/dev/e", CLU_1, CLU_6, &temp_str);
	if (err != 0) goto def_err_hdlr;
	err = file_nameOPparse(temp_str, &fname);
	if (err != 0) goto def_err_hdlr;
	clu_alloc(sizeof(_chan), &temp_chan);
	_chan_ero = temp_chan;
	_chan_ero->fn.str = fname.str;
	_chan_ero->rd.num = -1;
	_chan_ero->wr.num = 2;
	_chan_ero->typ.num = oth;
	_chan_ero->perm.tf = true;
	newchan->ref = (char *)_chan_ero;
	signal(ERR_ok);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}

 
errcode _chanOPnul(newchan)
CLUREF *newchan;
{
_chan * temp_chan;
errcode err;
CLUREF fname;
CLUREF temp_str;
int result;
	if (_chan_nul != NULL) {
		newchan->ref = (char *)_chan_nul;
		signal(ERR_ok);
		}
	result = open("/dev/null", O_RDWR, 0);
	if (result < 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_failure);}
	err = stringOPcons("/dev/null", CLU_1, CLU_9, &temp_str); /* MTV */
	if (err != 0) goto def_err_hdlr;
	err = file_nameOPparse(temp_str, &fname);
	if (err != 0) goto def_err_hdlr;
	clu_alloc(sizeof(_chan), &temp_chan);
	_chan_nul = temp_chan;
	_chan_nul->fn.str = fname.str;
	_chan_nul->rd.num = -1;
	_chan_nul->wr.num = result;
	_chan_nul->typ.num = nul;
	_chan_nul->perm.tf = true;
	newchan->ref = (char *)_chan_nul;
	signal(ERR_ok);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}

errcode _chanOPcontrol(chref, cmd, arg, ans)
CLUREF chref;
CLUREF cmd, arg, *ans;
{
int fd, result;
_chan *ch = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	if (cmd.num == 0) {
		elist[0].str = cannot_duplicate__chan_STRING;
		signal(ERR_not_possible);
		}
	result = fcntl(fd, cmd.num, arg.num);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans->num = result;
	signal(ERR_ok);
	}

errcode _chanOPlock(chref, op)
CLUREF chref;
CLUREF op;
{
int fd, result;
_chan *ch = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	result = flock(fd, op.num);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

errcode _chanOPsync(chref)
CLUREF chref;
{
int fd, result;
_chan *ch = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		signal(ERR_ok);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	result = fsync(fd);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}

errcode _chanOP_save_tty(opt)
bool opt;
{
int err;
int zcnt = 0;
	/* 
	   added the following cbreak so that ctrl-c would not hang the shell
	   when running pclu from a makefile and using tcsh or sh.
	   sh still needs a reset.
	   8/5/92 dcurtis
	*/
	if (_chan_pri != NULL && _chan_pri->typ.num == tty
			&& wrpipe <= 0) {
		err = tcgetattr(0, &isbuf);
		if (opt) sbuf.c_lflag |= (ICANON + ECHO);
		err = tcsetattr(0, TCSANOW, &sbuf);
		printf("\n");
		/* err = ioctl(0, TIOCFLUSH, zcnt); */
		}
	signal(ERR_ok);
	}

errcode _chanOP_restore_tty()
{
	if (_chan_pri != NULL && _chan_pri->typ.num == tty
			&& wrpipe <= 0) {
		tcsetattr(0, TCSANOW, &isbuf);
		}
	signal(ERR_ok);
	}

errcode _chanOPOPset_tty(chref, newch)
CLUREF chref;
CLUREF *newch;
{
int result;
errcode err;
CLUREF fname;
struct stat d0;
struct stat d1;
struct termios tbuf;
struct sigaction vec, ovec, oovec;
int s;
CLUREF temp;
_chan *ch = (_chan *)chref.ref;

	err = stringOPcons("/dev/p", CLU_1, CLU_6, &temp);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPparse(temp, &fname);
	if (err != ERR_ok) goto def_err_hdlr;
	ch->fn.str = fname.str;
	ch->perm.tf = true;
	if (ch->rd.num != -1) s = ch->rd.num;
	else s = ch->wr.num;
	result = tcgetattr(s, &sbuf);
	if (result != 0) {
		ch->typ.num = oth;
		signal(ERR_ok);
		}
	result = fstat(1, &d1);
	result = fstat(0, &d0);
	if (d1.st_dev != d0.st_dev ||
		d1.st_ino != d0.st_ino) {
		ch->typ.num = bty;
		newch->ref = (char *)ch;
		signal(ERR_ok);
		}
	/* file descriptors 0 and 1 represent the same dev & inode */
	/*	i.e. this is stdin/stdout */
	ch->rd.num = 0;
	ch->wr.num = 1;
	_chan_pri = ch;
	_chan_pro = ch;
	bcopy(&sbuf, &tbuf, (sizeof (struct termios)));
	tbuf.c_iflag |= ICRNL;
	tbuf.c_oflag |= OPOST + ONLCR;
	tbuf.c_lflag &= ~(ICANON + ECHO);
	tbuf.c_cc[VMIN] = 1;
	tbuf.c_cc[VTIME] = 0;
	/* printf("old %x new %x\n", sbuf.sg_flags, tbuf.sg_flags); */
	result = tcsetattr(0, TCSANOW, &tbuf);
	if (result != 0) goto unix_err_hdlr;
	if (wrpipe == 0) { /* no debugger */
		vec.sa_handler = (void (*)())_chanOPtdie;
		sigfixmask(&vec.sa_mask, SIGNAL_MASK);
		/* vec.sa_mask = SIGNAL_MASK; */
		vec.sa_flags = 0;	/* 02/03/98 */
		result = sigaction(SIGINT, &vec, &ovec);
		if (result != 0) goto unix_err_hdlr;
		result = sigaction(SIGQUIT, &vec, &ovec);
		if (result != 0) goto unix_err_hdlr;
		if (ovec.sa_handler != NULL) {
			result = sigaction(SIGQUIT, &ovec, &oovec);
			if (result != 0) goto unix_err_hdlr;
			}
		result = sigaction(SIGILL, &vec, &ovec);
		if (result != 0) goto unix_err_hdlr;
		result = sigaction(SIGBUS, &vec, &ovec);
		if (result != 0) goto unix_err_hdlr;
		result = sigaction(SIGSEGV, &vec, &ovec);
		if (result != 0) goto unix_err_hdlr;
		vec.sa_handler = (void (*)())_chanOPtstop;
		result = sigaction(SIGTSTP, &vec, &ovec);
		if (result != 0) goto unix_err_hdlr;
		}
	newch->ref = (char *)ch;
	signal(ERR_ok);
unix_err_hdlr:
	elist[0] = _unix_erstr(errno);
	signal(ERR_failure);
def_err_hdlr:
	elist[0] = _pclu_erstr(err);
	signal(ERR_failure);
	}

int _chanOPtstop()
{
struct sigaction temp;
int pid;
int result, mask;

	temp.sa_handler = NULL;
	sigemptyset(&temp.sa_mask);
	/* temp.sa_mask = 0; */
	temp.sa_flags = 0;
	_chanOP_save_tty(true);
	sigaction(SIGTSTP, &temp, 0);
	pid = getpid();
	result = kill(pid, SIGTSTP);
	mask = sigsetmask(0);
	sigsetmask(mask);
	_chanOP_restore_tty();
	temp.sa_handler = (void (*)())_chanOPtstop;
	sigfixmask(&temp.sa_mask, SIGNAL_MASK);
	/* temp.sa_mask = SIGNAL_MASK; */
	temp.sa_flags = 0;	/* 02/03/98 */
	sigaction(SIGTSTP, &temp, 0);
	return(0);
	}

int _chanOPtdie(sig)
int sig;
{
struct sigaction temp;
int pid;
int result;

	temp.sa_handler = NULL;
	sigemptyset(&temp.sa_mask);
	/* temp.sa_mask = 0; */
	temp.sa_flags = 0;
	sigaction(sig, &temp, 0);
	pid = getpid();
	result = kill(pid, sig);
	_chanOP_save_tty(true);
	return(0);
	}

errcode _chanOPputc(chref, c, image)
CLUREF chref;
CLUREF c;
CLUREF image;
{
int lit;
int result;
_chan *ch = (_chan *)chref.ref;

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (image.tf == true && ch->typ.num == tty) 
		lit = true;
		else lit = false;
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIS, LLITOUT);
#endif
	for (;;) {
		result = write(ch->wr.num, &c.ch, 1);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (result != 1) {
			elist[0].str = unknown_error_STRING;
			signal(ERR_not_possible);
			}
		break;
		}
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIC, LLITOUT);
#endif
	signal(ERR_ok);
	}


errcode _chanOPputi(chref, i, image)
CLUREF chref;
CLUREF i;
CLUREF image;
{
int lit;
int result;
_chan *ch = (_chan *)chref.ref;

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (image.tf == true && ch->typ.num == tty) 
		lit = true;
		else lit = false;
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIS, LLITOUT);
#endif
	for (;;) {
		result = write(ch->wr.num, &i.num, 1);
		if (result == -1 && errno == EINTR) continue;
		if (result < 0) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (result != 1) {
			elist[0].str = unknown_error_STRING;
			signal(ERR_not_possible);
			}
		break;
		}
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIC, LLITOUT);
#endif
	signal(ERR_ok);
	}


errcode _chanOPputs(chref, s, image)
CLUREF chref;
CLUREF s;
CLUREF image;
{
int lit;
int result;
_chan *ch = (_chan *)chref.ref;

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (image.tf == true && ch->typ.num == tty) 
		lit = true;
		else lit = false;
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIS, LLITOUT);
#endif
		for (;;) {
		result = write(ch->wr.num, s.str->data,
					s.str->size);
		if (result == -1 && errno == EINTR) continue;
		if (result < 0) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (result != s.str->size) {
			elist[0].str = unknown_error_STRING;
			signal(ERR_not_possible);
			}
		break;
		}
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIC, LLITOUT);
#endif
	signal(ERR_ok);
	}

errcode _chanOPputb(chref, bv, low, high, image)
CLUREF chref;
CLUREF bv, low, high;
CLUREF image;
{
int lit;
int result;
int top;
int size;
_chan *ch = (_chan *)chref.ref;

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (low.num < 1 || low.num > bv.str->size) signal(ERR_bounds);
	top = bv.str->size;
	if (high.num < top) top = high.num;
	if (image.tf == true && ch->typ.num == tty) 
		lit = true;
		else lit = false;
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIS, LLITOUT);
#endif
	size = top - low.num + 1;
	if (size <= 0) signal(ERR_ok);
	for (;;) {
		result = write(ch->wr.num, &bv.str->data[low.num-1], size);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (result != size) {
			elist[0].str = unknown_error_STRING;
			signal(ERR_not_possible);
			}
		break;
		}
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIC, LLITOUT);
#endif
	signal(ERR_ok);
	}


errcode _chanOPputw(chref, wv, low, high, image)
CLUREF chref;
CLUREF wv, low, high;
CLUREF image;
{
int lit;
int result;
int top;
int size;
_chan *ch = (_chan *)chref.ref;

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (low.num < 1 || low.num >= wv.vec->size) signal(ERR_bounds);
	top = wv.vec->size;
	if (high.num < top) top = high.num;
	if (image.tf == true && ch->typ.num == tty) 
		lit = true;
		else lit = false;
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIS, LLITOUT);
#endif
	size = top - low.num + 1;
	if (size <= 0) signal(ERR_ok);
	for (;;) {
		result = write(ch->wr.num, &wv.vec->data[low.num-1], size);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (result != size) {
			elist[0].str = unknown_error_STRING;
			signal(ERR_not_possible);
			}
		break;
		}
#ifndef LINUX
	if (lit) result = ioctl(ch->wr.num, TIOCLBIC, LLITOUT);
#endif
	signal(ERR_ok);
	}

/* callers assume that initial condition is low = 1, high = 0 */

errcode _chanOPput(chref, bv, low, high, s, image, ans1, ans2)
CLUREF chref;
CLUREF bv, low, high;
CLUREF s, image;
CLUREF *ans1, *ans2;
{
int lit;
int bv_space_left, string_chars_left;
int string_chars_to_newline;
int bv_offset, string_offset;
char * newline_addr;
bool force;
int bv_low, bv_high;
int copy_count;
errcode err;
_chan *ch = (_chan *)chref.ref;

/* initial conditions and bounds checking */

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (low.num < 1) signal(ERR_bounds);
	if (bv.str->size == 0) signal(ERR_bounds);
	if (low.num > bv.str->size) signal(ERR_bounds);
	if (high.num < low.num - 1) signal(ERR_bounds);

	if (image.tf == true && ch->typ.num == tty) 
		lit = true;
		else lit = false;
	if (s.str->size <= 0) {
		ans1->num = low.num;
		ans2->num = high.num;
		signal(ERR_ok);
		}

/* initialize invariants */

	bv_space_left = bv.str->size - high.num;	/* ??? +1 */
	if (bv_space_left < 0) signal(ERR_bounds);
	string_chars_left = s.str->size;
	bv_low = low.num - 1;
	bv_high = high.num;
	bv_offset = bv_high;
	string_offset = 0;

/* loop copying chars from s to bv */
/*    force writes for newlines in s, or when bv gets full */

	while (string_chars_left > 0) {

/* look for newline char */
		newline_addr = index(&s.str->data[string_offset], '\n');
		if (newline_addr == NULL) {
			force = false; 
			string_chars_to_newline = 0;
			}
		else {
			force = true; 
			string_chars_to_newline = 
				newline_addr - &(s.str->data[string_offset]);
			string_chars_to_newline += 1;
			}

/* decide how many chars to copy */
		copy_count = bv_space_left;
		if (string_chars_left < copy_count) copy_count = string_chars_left;
		if (string_chars_to_newline != 0 && string_chars_to_newline < copy_count) 
			copy_count = string_chars_to_newline;

/* copy the chars */
		bcopy(&s.str->data[string_offset], &bv.str->data[bv_offset],
				copy_count);

/* write out bv if necessary */
		if (force || copy_count == bv_space_left) {
			err = _chanOPOP_put_doit(lit, ch->wr.num, bv, bv_low,
					bv_high - bv_low + copy_count);
			if (err != ERR_ok) signal(err); /* propagate */
			bv_offset = 0;
			bv_high = 0;
			bv_low = 0;
			}
		else {
			bv_offset += copy_count;
			bv_high += copy_count;
			if (bv_offset > bv.str->size) {
				bv_offset = 0; bv_low = 0; bv_high = 0;
				}
			}

/* update loop invariants */
		string_chars_left -= copy_count;
		string_offset += copy_count;
		bv_space_left = bv.str->size - bv_high;
		}
	ans1->num = bv_low + 1;
	ans2->num = bv_high;
	signal(ERR_ok);
	}

errcode _chanOPOP_put_doit(lit, fd, bv, low, size)
int lit, fd, low, size;
CLUREF bv;
{
int result;

#ifndef LINUX
	if (lit) result = ioctl(fd, TIOCLBIS, LLITOUT);
#endif
	for (;;) {
		result = write(fd, &bv.str->data[low], size);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		if (result != size) {
			elist[0].str = unknown_error_STRING;
			signal(ERR_not_possible);
			}
		break;
		}
#ifndef LINUX
	if (lit) result = ioctl(fd, TIOCLBIC, LLITOUT);
#endif
	signal(ERR_ok);
	}


errcode _chanOPreset(chref)
CLUREF chref;
{
int s;
int result;
_chan *ch = (_chan *)chref.ref;

	if (ch->typ.num > 1) {
		s = -1;
		if (ch->rd.num >= 0) s = ch->rd.num;
		if (ch->wr.num >= 0) s = ch->wr.num;
		if (s != -1) {
			result = lseek(s, 0, 0);
			if (result == -1) {
				elist[0] = _unix_erstr(errno);
				signal(ERR_not_possible);
				}
			}
		}
	signal(ERR_ok);
	}
		

errcode _chanOPclose(chref)
CLUREF chref;
{
int s;
int result;
_chan *ch = (_chan *)chref.ref;

	if (ch->perm.tf == true) signal(ERR_permanent);
	s = -1;
	if (ch->rd.num >= 0) s = ch->rd.num;
	if (ch->wr.num >= 0) s = ch->wr.num;
	if (s != -1) {
		result = close(s);
		if (result != 0) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		ch->rd.num = -1;
		ch->wr.num = -1;
		}
	signal(ERR_ok);
}

errcode _chanOPabort(chref)
CLUREF chref;
{
int s;
errcode err;
int result;
CLUREF fn;
_chan *ch  = (_chan *)chref.ref;

	if (ch->perm.tf == true) signal(ERR_permanent);
	if (ch->rd.num != -1) s = ch->rd.num;
	else s = ch->wr.num;
	result = close(s);
	if (result != 0) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ch->rd.num = -1;
	ch->wr.num = -1;
	if (ch->new.num == 1) {
		err = file_nameOPunparse(ch->fn, &fn);
		if (err != ERR_ok) resignal(err);
		unlink(fn.str->data);
		}
	signal(ERR_ok);
}

errcode _chanOPis_tty(chref, ans)
CLUREF chref;
CLUREF *ans;
{
_chan *ch  = (_chan *)chref.ref;

	if (ch->typ.num == 0) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _chanOPpending(chref, image, ans)
CLUREF chref;
CLUREF image, *ans;
{
int result;
int s;
int obuf;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	s = ch->rd.num;
	result = ioctl(s, FIONREAD, &obuf);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (obuf > 0) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}

errcode _chanOPpend_count(chref, ans)
CLUREF chref;
CLUREF *ans;
{
int result;
int s;
int obuf;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	s = ch->rd.num;
	result = ioctl(s, FIONREAD, &obuf);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans->num = result;
	signal(ERR_ok);
	}


errcode _chanOPbuf_count(chref, ans)
CLUREF chref;
CLUREF *ans;
{
int result;
int s;
int obuf;
_chan *ch  = (_chan *)chref.ref;

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	s = ch->wr.num;
	result = ioctl(s, TIOCOUTQ, &obuf);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans->num = result;
	signal(ERR_ok);
	}

errcode _chanOPgetc(chref, image, ans)
CLUREF chref, image;
CLUREF *ans;
{
int result;
char temp;
int echo_count;
char echo_buf[10];
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	while (1) {
		result = read(ch->rd.num, &temp, 1);
		if (result == -1 && errno == EINTR) continue;
		if (result >= 0) break;
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (result == 0) signal(ERR_end_of_file);

	/* temp &= 0xff; */
	/* check for no echo */
	if (ch->typ.num != 0 || image.tf == true) {
		/* printf("no echo %d %d %x\n", ch->typ.num, image.tf, temp); */
		ans->ch = temp;
		signal(ERR_ok);
		}

	/* echo */
	/* printf("echoing %x\n", temp);  */
	echo_buf[0] = temp;
	echo_count = 1;
	if (temp >= '\177') { /* meta character */
		if (temp == '\177') {
			echo_count++;
			echo_buf[0] = '^';
			echo_buf[1] = '?';
			}
		else {
			echo_count++;
			echo_buf[1] = echo_buf[0] - 0100;
			echo_buf[0] = '&';
			if (echo_buf[1] < '\140') {
				echo_buf[0] = '!';
				}
			else {
				echo_buf[1] -= 0100;
				if (echo_buf[1] == '\177') {
					echo_buf[1] = '?';
					echo_buf[0] = '!';
					}
				}
			}
		}

	else { /* not a meta character */
		if (temp < ' ') { /* control character */
			if (temp != '\n' && temp != '\t') {
				echo_count++;
				echo_buf[1] = echo_buf[0];
				echo_buf[0] = '^';
				}
			}
		}


	result = write(ch->wr.num, echo_buf, echo_count);
	ans->ch = temp;
	signal(ERR_ok);
	}
	

errcode _chanOPgeti(chref, image, ans)
CLUREF chref, image;
CLUREF *ans;
{
int result;
int temp;
int echo_count;
char echo_buf[10];
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	while (1) {
		result = read(ch->rd.num, &temp, 1);
		if (result == -1 && errno == EINTR) continue;
		if (result >= 0) break;
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (result == 0) signal(ERR_end_of_file);

	temp &= 0xff;
	/* check for no echo */
	if (ch->typ.num != 0 || image.tf == true) {
		ans->num = temp;
		signal(ERR_ok);
		}

	/* echo */
	echo_buf[0] = temp;
	echo_count = 1;
	if (temp >= '\177') { /* meta character */
		if (temp == '\177') {
			echo_count++;
			echo_buf[0] = '^';
			echo_buf[1] = '?';
			}
		else {
			echo_count++;
			echo_buf[1] = echo_buf[0] - 0100;
			echo_buf[1] = '&';
			if (echo_buf[1] < '\140') {
				echo_buf[0] = '!';
				}
			else {
				echo_buf[1] -= 0100;
				if (echo_buf[1] == '\177') {
					echo_buf[1] = '?';
					echo_buf[0] = '!';
					}
				}
			}
		}

	else { /* not a meta character */
		if (temp < ' ') { /* control character */
			if (temp != '\n' && temp != '\t') {
				echo_count++;
				echo_buf[1] = echo_buf[0];
				echo_buf[0] = '^';
				}
			}
		}


	result = write(ch->wr.num, echo_buf, echo_count);
	signal(ERR_ok);
	}
	



errcode _chanOPgetb(chref, bv, ans)
CLUREF chref;
CLUREF bv;
CLUREF *ans;
{
int result;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (bv.str->size == 0) signal(ERR_bounds);
	while (1) {
		result = read(ch->rd.num, bv.str->data, bv.str->size);
		if (result == -1 && errno == EINTR) continue;
		if (result >= 0) break;
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
		if (result == 0) signal(ERR_end_of_file);
		ans->num = result;
		signal(ERR_ok);
	}

errcode _chanOPgetw(chref, bv, ans)
CLUREF chref;
CLUREF bv;
CLUREF *ans;
{
int result;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (bv.str->size == 0) signal(ERR_bounds);
	while (1) {
		result = read(ch->rd.num, bv.str->data, bv.str->size);
		if (result == -1 && errno == EINTR) continue;
		if (result >= 0) break;
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
		if (result == 0) signal(ERR_end_of_file);
		ans->num = result;
		signal(ERR_ok);
	}

errcode _chanOPgetwv(chref, bv, strt, n, ans)
CLUREF chref;
CLUREF bv, strt, n;
CLUREF *ans;
{
int result, count;
_chan *ch  = (_chan *)chref.ref;

	if (strt.num < 1) signal(ERR_bounds);
	count = bv.str->size-strt.num+1;
	if (n.num < count) count = n.num;
	if (count < 0) signal(ERR_bounds);
	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (bv.str->size == 0) signal(ERR_bounds);
	while (1) {
		result = read(ch->rd.num, &bv.str->data[strt.num-1], count);
		if (result == -1 && errno == EINTR) continue;
		if (result >= 0) break;
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (result == 0) signal(ERR_end_of_file);
	ans->num = result;
	signal(ERR_ok);
	}

errcode _chanOPgetbv(chref, bv, strt, n, ans)
CLUREF chref;
CLUREF bv, strt, n;
CLUREF *ans;
{
int count;
int result;
errcode err;
_chan *ch  = (_chan *)chref.ref;

	if (strt.num < 1) signal(ERR_bounds);
	/* inlined int$min 8/28/90 dcurtis */
	count = bv.str->size-strt.num+1;
	if (n.num < count) count = n.num;
	if (count < 0) signal(ERR_bounds);
	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	while (1) {
		result = read(ch->rd.num, &bv.str->data[strt.num-1], count);
		if (result == -1 && errno == EINTR) continue;
		if (result >= 0) break;
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	if (result == 0) signal(ERR_end_of_file);
	ans->num = result;
	signal(ERR_ok);
	}

errcode _chanOPget(chref, bv, low, high, term, image, ans, newlow, newhigh)
CLUREF chref;
CLUREF bv, low, high, term, image;
CLUREF *ans, *newlow, *newhigh;
{
errcode err;
bool done;
CLUREF temp, newchars, beg, len;
int count;
int newcount;
int strt;
int i, j, offset, found;
char target;
_chan *ch  = (_chan *)chref.ref;

/* check arguments */
/*	printf("low = %d, high = %d\n", low.num, high.num); */
	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	if (low.num < 1) signal(ERR_bounds);
	if (bv.str->size == 0) signal(ERR_bounds);
	if (low.num > (bv.str->size + 1)) signal(ERR_bounds);
	if (high.num > bv.str->size) signal(ERR_bounds);
	count = high.num;
	if (count < 0) signal(ERR_bounds);
	if (term.str->size <= 0) signal(ERR_bounds);    /* new behavior */
	if (term.str->size > bv.str->size) signal(ERR_bounds); /* new behavior */

/*** CONVERT LOW AND HIGH FROM CLU INDEXES (STARTING AT 1) TO C ****/
/*** INDEXES (STARTING AT 0)                                    ****/
	low.num = low.num - 1;
	high.num = high.num - 1; 

/* guarantee that there is some data to search through */
/**** MODIFICATION:    V--------------------V 2/13/91 */
	if (count == 0 || high.num < low.num) {

/**** MODIFICATION: ---V   From strt = 1;  *****/
		strt = 0;  
		err = _chanOPOPreadit(ch->rd.num, bv, strt, &count);
		if (err == ERR_end_of_file) signal(err);
		if (err != ERR_ok) signal(err); /* propagate */
		low.num = 0;		/* 5/23/90 */
		high.num = count - 1;   /* 5/23/90 */ /* unnecessary */
		}

	temp = clu_empty_string;

	if (term.str->size == 1) {
/* search for single character terminator string in buffer (bv) */
	target = term.str->data[0];
	while (1) {
		done = false;

/**** MODIFICATION: ------------V  Deleted + 1          ******/
		for (i = low.num; i < count; i++) {
			if (bv.str->data[i] != target) continue;
			done = true; break;
			}

/***** MODIFICATION: Added +1 ------------------V          V-- Deleted +1 */
		beg.num = low.num + 1;
		len.num = i - low.num;
		err = stringOPsubstr(bv, beg, len, &newchars);
		if (err != ERR_ok) resignal(err);
		err = stringOPconcat(temp, newchars, &temp);
		if (err != ERR_ok) resignal(err);
		if (done) {
			ans->str = temp.str;

/**** MODIFICATION: Changed +1 to +2 --nixed--V            *****/
			newlow->num = i + 1;
			newhigh->num = count;

/**** MODIFICATION: If it ends up with the terminator at the end of the  *****/
/****               buffer, then read in a new buffer full before *****/
/****               returning.  Also return cleanly if EOF. *****/

/* the following case appears impossible */

			if (newlow->num > newhigh->num) {
				strt = 0;  
				err = _chanOPOPreadit(ch->rd.num, bv, strt, 
 							&count);
				if ((err != ERR_ok)&&(err != ERR_end_of_file))
					signal(err); /* propagate */
				if (err == ERR_end_of_file) signal(err);
				newlow->num = 1;
				}
/* the following case is new 5/23/90 */
/* removed 2/13/91 */
/*
			if (newlow->num == newhigh->num) {
				newhigh->num = 0;
				}
*/
			if (newlow->num > bv.str->size) newlow->num = count;
/*			printf("%s\n", temp.str->data); */
/*			printf("newlow = %d, newhigh = %d\n", newlow->num, newhigh->num); */
			signal(ERR_ok);
			}
		else {

/**** MODIFICATION: Added --V     strt = 0;    *****/
			strt = 0;
			err = _chanOPOPreadit(ch->rd.num, bv, strt, &count);

/**** MODIFICATION: Added --V     low.num = 0;    *****/
			low.num = 0;

			if (err == ERR_end_of_file) {
				ans->str = temp.str;
				newlow->num = 1;
				newhigh->num = 0;
				signal(ERR_ok);
				}
			if (err != ERR_ok) signal(err); /* propagate */
			}
		}
		}

	else {


/***** MODIFICATION: THE REMAINDER OF GET IS ENTIRELY REWRITTEN ****/	
/* search for multi character terminator string in bv */
/* substantially reworked 8/12/92 (dwc)  (used by clucc):
	redefine multi-character terminator string: any char in it will terminate the read
	handle eof properly
	leave terminator in buffer
*/
		while (1) {
			found = false;
			for (i = low.num; i < count; i++) {
				for (j = 0; j < term.str->size; j++) {
					if (bv.str->data[i] == term.str->data[j]) {
						found = true; break;}
					}
				if (found == true) {	/*** THEN DONE ***/
					beg.num = low.num + 1;
					len.num = i - low.num;
					err = stringOPsubstr (bv, beg, len, &newchars);
					if (err != ERR_ok) resignal (err);
					err = stringOPconcat (temp, newchars, &temp);
					if (err != ERR_ok) resignal (err);
					ans->str = temp.str;
					newlow->num = i + 1;
					newhigh->num = count;

/**** MODIFICATION: If it ends up with the terminator at the end of the  *****/
/****               buffer, then read in a new buffer full before *****/
/****               returning.  Also return cleanly if EOF. *****/

					if (newlow->num > newhigh->num) {
						strt = 0;  
						err = _chanOPOPreadit(ch->rd.num, bv, strt, 
 									&count);
						if ((err != ERR_ok)&&(err != ERR_end_of_file))
							signal(err); /* propagate */
						if (err == ERR_end_of_file) signal(err);
						newlow->num = 1;
					/** End If **/}

					if (newlow->num > bv.str->size) newlow->num = count;
					signal (ERR_ok);
				/** End If done **/}
			/** End For i **/}
	
			beg.num = low.num + 1;
			len.num = i - low.num;
			err = stringOPsubstr (bv, beg, len, &newchars);
			if (err != ERR_ok) resignal (err);
			err = stringOPconcat (temp, newchars, &temp);
			if (err != ERR_ok) resignal (err);
			strt = 0;
			err = _chanOPOPreadit(ch->rd.num, bv, strt, &count);
			low.num = 0;
			if (err == ERR_end_of_file) {
				ans->str = temp.str;
				newlow->num = 1;
				newhigh->num = 0;
				signal(ERR_ok);
				}
			if (err != ERR_ok) signal (err); /* propagate */
		/** End while **/}
	/** end else **/}
/** end get **/}

/* OBSOLETE ...
					{if ((i+j+1) >= count) {

/ ** PULL PARTIAL TERMINATOR TO START OF BUFFER ** /
						beg.num = low.num + 1;
						len.num = i - j;
						err = stringOPsubstr(bv, beg, len, 
						                    &newchars);
						if (err != ERR_ok) resignal (err);
						err = stringOPconcat(temp, newchars, &temp);
						if (err != ERR_ok) resignal(err);
						for (offset = 0;  (offset <= j); offset++) {
							bv.str->data[offset] = 
                                   bv.str->data[offset + i];
							}
						strt = j + 1;
						err = _chanOPOPreadit(ch->rd.num, bv, strt, 
						                    &count);
						if (err == ERR_end_of_file) signal (err);
						if (err != ERR_ok) signal (err); / * propagate * /
						i = 0;
						low.num = 0;
					/ ** End If ** /}
*/

errcode _chanOPOPreadit(s, bv, start, count)
int s;
CLUREF bv;
int start;
int *count;
{
int result;

/**** MODIFICATION: Changed "+ 1" to "+ start" --------------------V ***/
	result = read(s, &bv.str->data[start], bv.str->size - start);

	if (result == 0) signal(ERR_end_of_file);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	*count = result + start;
	signal(ERR_ok);
	}



errcode _chanOPrecv(chref, bv, flags, ans)
CLUREF chref;
CLUREF bv, flags;
CLUREF *ans;
{
int result;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	while (1) {
		result = recv(ch->rd.num, bv.str->data, bv.str->size, flags.num);
		if (result == -1 && errno == EINTR) continue;
		if (result >= 0) break;
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans->num = result;
	signal(ERR_ok);
	}


errcode _chanOPrecvfrom(chref, bv, flags, addr, ans1, ans2)
CLUREF chref;
CLUREF bv, flags, addr;
CLUREF *ans1, *ans2;
{
int result, fromlen;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	fromlen = addr.str->size;
	while (1) {
		result = recvfrom(ch->rd.num, bv.str->data, bv.str->size, 
			flags.num, addr.str->data, &fromlen);
		if (result == -1 && errno == EINTR) continue;
		if (result >= 0) break;
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans1->num = result;
	ans2->num = fromlen;
	signal(ERR_ok);
	}

errcode _chanOPsend(chref, buf, len, flags)
CLUREF chref;
CLUREF buf, len, flags;
{
int result;
int size;
_chan *ch  = (_chan *)chref.ref;

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	size = buf.vec->size;
	if (len.num <= size) size = len.num;
	else signal(ERR_bounds);
	for (;;) {
		result = send(ch->wr.num, buf.vec->data, size, flags.num);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		break;
		}
	signal(ERR_ok);
	}

errcode _chanOPsendto(chref, buf, len, flags, addr, alen)
CLUREF chref;
CLUREF buf, len, flags, addr, alen;
{
int result;
int size, asize;
_chan *ch  = (_chan *)chref.ref;

	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	size = buf.vec->size;
	asize = addr.vec->size;
	if (len.num <= size) size = len.num;
	else signal(ERR_bounds);
	if (alen.num <= asize) asize = alen.num;
	else signal(ERR_bounds);
	for (;;) {
		result = sendto(ch->wr.num, buf.vec->data, size, flags.num,
				addr.vec->data, asize);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		break;
		}
	signal(ERR_ok);
	}

errcode _chanOPread_wait(chref, millis, ans)
CLUREF chref;
CLUREF millis, *ans;
{
int result;
struct timeval timeout;
fd_set readfds, exceptfds;
_chan *ch  = (_chan *)chref.ref;

	FD_ZERO(&readfds);
	FD_ZERO(&exceptfds);
	if (ch->rd.num < 0) {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	FD_SET(ch->rd.num, &readfds);
	FD_SET(ch->rd.num, &exceptfds);
	timeout.tv_sec = millis.num / 1000;
	timeout.tv_usec = (millis.num - (timeout.tv_sec * 1000)) * 1000;
	for (;;) {
		result = select(ch->rd.num + 1, &readfds, 0, &exceptfds, &timeout);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		break;
		}
	if (result == 0) ans->num = 0;
	else ans->num = 1;
	signal(ERR_ok);
   	}


errcode _chanOPwrite_wait(chref, millis, ans)
CLUREF chref;
CLUREF millis, *ans;
{
int result;
struct timeval timeout;
fd_set writefds, exceptfds;
_chan *ch  = (_chan *)chref.ref;

	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	if (ch->wr.num < 0) {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	FD_SET(ch->wr.num, &writefds);
	FD_SET(ch->wr.num, &exceptfds);
	timeout.tv_sec = millis.num / 1000;
	timeout.tv_usec = (millis.num - (timeout.tv_sec * 1000)) * 1000;
	for (;;) {
		result = select(ch->rd.num + 1, 0, &writefds, &exceptfds, &timeout);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		break;
		}
	if (result == 0) ans->num = 0;
	else ans->num = 1;
	signal(ERR_ok);
   	}


errcode _chanOPwait(reads, writes, millis, ans)
CLUREF reads, writes;
CLUREF millis, *ans;
{
int result, i;
struct timeval timeout;
fd_set readfds, writefds, exceptfds;
_chan **rds = (_chan **)reads.vec->data;
_chan **wrs = (_chan **)writes.vec->data;
int ith;
int max = 0;

	FD_ZERO(&readfds); FD_ZERO(&writefds); FD_ZERO(&exceptfds);
	for (i = 0 ; i < reads.vec->size; i++) {
		ith = rds[i]->rd.num;
		if (ith < 0) {
			elist[0].str = cannot_read_from_this__chan_STRING;
			signal(ERR_not_possible);
			}
		FD_SET(ith, &readfds);
		FD_SET(ith, &exceptfds);
		if (ith > max) max = ith + 1;
		}

	for (i = 0 ; i < writes.vec->size; i++) {
		ith = wrs[i]->wr.num;
		if (ith < 0) {
			elist[0].str = cannot_write_to_this__chan_STRING;
			signal(ERR_not_possible);
			}
		FD_SET(ith, &writefds);
		FD_SET(ith, &exceptfds);
		if (ith > max) max = ith + 1;
		}

	/* 2/26/01 dwc: folded functionality into above loops */
	/* exceptfds.fds_bits[0] = readfds.fds_bits[0] | writefds.fds_bits[0]; */

	timeout.tv_sec = millis.num / 1000;
	timeout.tv_usec = (millis.num - (timeout.tv_sec * 1000)) * 1000;
	for (;;) {
		result = select(max, &readfds, 
				&writefds, &exceptfds, &timeout);
		if (result == -1 && errno == EINTR) {
			ans->num = 0;
			signal(ERR_ok);
			}
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		break;
		}
	ans->num = result;
	signal(ERR_ok);
   	}

errcode _chanOPlisten(chref, backlog)
CLUREF chref;
CLUREF backlog;
{
int fd, uerr;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	uerr = listen(fd, backlog.num);
	if (uerr == 0) 	signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}	

errcode _chanOPaccept(chref, addr, ans1, ans2)
CLUREF chref;
CLUREF addr, *ans1, *ans2;
{
errcode err;
int fd, result, len, s, z;
_chan *newch;
CLUREF temp_str, fn;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	len = addr.vec->size;
	for (;;) {
		result = accept(fd, addr.vec->data, &len);
		if (result == -1 && errno == EINTR) continue;
		if (result == -1) {
			elist[0] = _unix_erstr(errno);
			signal(ERR_not_possible);
			}
		break;
		}
	s = result;
	z = len;
	err = stringOPcons("/dev/socket", CLU_1, CLU_11, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPparse(temp_str, &fn);
	if (err != ERR_ok) resignal(err);
	clu_alloc(sizeof(_chan), &newch);
	newch->fn = fn;
	newch->rd.num = s;
	newch->wr.num = s;
	newch->typ.num = oth;
	newch->new.tf = true;
	newch->perm.tf = false;
	ans1->ref = (char *)newch;
	ans2->num = z;
	signal(ERR_ok);
	}

errcode _chanOPbind(chref, name, len)
CLUREF chref;
CLUREF name, len;
{
int fd, uerr;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	if (len.num > name.vec->size) signal(ERR_bounds);
	uerr = bind(fd, name.vec->data, len.num);
	if (uerr == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
	

errcode _chanOPconnect(chref, name, len)
CLUREF chref;
CLUREF name, len;
{
int fd, uerr;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	if (len.num > name.vec->size) signal(ERR_bounds);
	uerr = connect(fd, name.vec->data, len.num);
	if (uerr == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
	

errcode _chanOPshutdown(chref, how)
CLUREF chref;
CLUREF how;
{
int fd, uerr;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	uerr = shutdown(fd, how.num);
	if (uerr == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}

errcode _chanOPpeername(chref, name, ans)
CLUREF chref;
CLUREF name, *ans;
{
int fd, uerr, size;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	size = name.vec->size;
	uerr = getpeername(fd, name.vec->data, &size);
	if (uerr == 0) {
		ans->num = size;
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
	

errcode _chanOPsockname(chref, name, ans)
CLUREF chref;
CLUREF name, *ans;
{
int fd, uerr, size;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	size = name.vec->size;
	uerr = getsockname(fd, name.vec->data, &size);
	if (uerr == 0) {
		ans->num = size;
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
	

errcode _chanOPgetsockopt(chref, level, option, ans)
CLUREF chref;
CLUREF level, option, *ans;
{
int fd, uerr;
int optlen, optval;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	optlen = 4;
	uerr = getsockopt(fd, level.num, option.num, &optval, &optlen);
	if (uerr == 0) {
		ans->num = optval;
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
	

errcode _chanOPsetsockopt(chref, level, option, value)
CLUREF chref;
CLUREF level, option, value;
{
int fd, uerr;
int optlen;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	optlen = 4;
	uerr = setsockopt(fd, level.num, option.num, &value, optlen);
	if (uerr == 0) {
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}
	
	
	

errcode _chanOPget_name(chref, ans)
CLUREF chref;
CLUREF *ans;
{
_chan *ch  = (_chan *)chref.ref;

	ans->vec = ch->fn.vec;
	signal(ERR_ok);
	}

errcode _chanOPget_date(chref, ans)
CLUREF chref;
CLUREF *ans;
{
int uerr, fd;
struct stat stat;
CLUREF time;
errcode err;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	uerr = fstat(fd, &stat);
	if (uerr == 0) {

		err = _local_time((stat.st_mtime >> 16) & 0xffff,
			stat.st_mtime & 0xffff, &time);
		if (err != ERR_ok) resignal(err);
		ans->num = time.num;

		signal(ERR_ok);
		}

	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}	





errcode _chanOPget_speeds(chref, ans1, ans2)
CLUREF chref;
CLUREF *ans1, *ans2;
{
int result, fd;
int insp, outsp;
struct termios buf;
_chan *ch  = (_chan *)chref.ref;

	if (ch->typ.num != 0) {
		elist[0].str = not_a_terminal__chan_STRING;
		signal(ERR_not_possible);
		}
	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = speeds_unknown_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	result = tcgetattr(fd, &buf);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	insp = cfgetispeed(&buf);
	outsp = cfgetospeed(&buf);
	/* printf("%d %d\n", insp, outsp);
	printf("%d %d\n", buf.c_ispeed, buf.c_ospeed); */
	ans1->num = insp;
	ans2->num = outsp;
	signal(ERR_ok);
	}	






errcode _chanOPget_block_size(chref, ans)
CLUREF chref;
CLUREF *ans;
{
int uerr, fd;
struct stat stat;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	uerr = fstat(fd, &stat);
	if (uerr == 0) {
		ans->num = stat.st_blksize;
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}	


errcode _chanOPstat(chref, buf)
CLUREF chref;
CLUREF buf;
{
int result, fd;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	if (buf.vec->size < sizeof(struct stat)) signal(ERR_bounds);
	result = fstat(fd, (struct stat *)buf.vec->data);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}	

errcode _chanOPget_access(chref, ans)
CLUREF chref;
CLUREF *ans;
{
int result, fd;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	result = lseek(fd, 0, 1);
	if (result == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	ans->num = result;
	signal(ERR_ok);
	}	




errcode _chanOPset_access(chref, pos)
CLUREF chref;
CLUREF pos;
{
int uerr, fd;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	uerr = lseek(fd, pos.num, 0);
	if (uerr == -1) {
		elist[0] = _unix_erstr(errno);
		signal(ERR_not_possible);
		}
	signal(ERR_ok);
	}	



errcode _chanOPget_length(chref, ans)
CLUREF chref;
CLUREF *ans;
{
int uerr, fd;
struct stat stat;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	uerr = fstat(fd, &stat);
	if (uerr == 0) {
		ans->num = stat.st_size;
		signal(ERR_ok);
		}
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}	


errcode _chanOPset_length(chref, len)
CLUREF chref;
CLUREF len;
{
int uerr, fd;
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num < 0 && ch->wr.num < 0) {
		elist[0].str = _chan_is_closed_STRING;
		signal(ERR_not_possible);
		}
	fd = ch->rd.num;
	if (fd < 0) fd = ch->wr.num;
	uerr = ftruncate(fd, len.num);
	if (uerr == 0) signal(ERR_ok);
	elist[0] = _unix_erstr(errno);
	signal(ERR_not_possible);
	}	



errcode _chanOPequal(ch1, ch2, ans)
CLUREF ch1, ch2, *ans;
{
	if (ch1.ref == ch2.ref) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}


errcode _chanOPsimilar(ch1, ch2, ans)
CLUREF ch1, ch2, *ans;
{
	if (ch1.ref == ch2.ref) ans->tf = true;
	else ans->tf = false;
	signal(ERR_ok);
	}




errcode _chanOPcopy(ch, ans)
CLUREF ch, *ans;
{
	ans->ref = ch.ref;
	signal(ERR_ok);
	}

errcode _chanOPget_read_channel(chref, img, ans)
CLUREF chref;
CLUREF img, *ans;
{
_chan *ch  = (_chan *)chref.ref;

	if (ch->rd.num >= 0) {
		ans->num = ch->rd.num;
		signal(ERR_ok);
		}
	else {
		elist[0].str = cannot_read_from_this__chan_STRING;
		signal(ERR_not_possible);
		}
	}

errcode _chanOPget_write_channel(chref, img, ans)
CLUREF chref;
CLUREF img, *ans;
{
_chan *ch  = (_chan *)chref.ref;

	if (ch->wr.num >= 0) {
		ans->num = ch->wr.num;
		signal(ERR_ok);
		}
	else {
		elist[0].str = cannot_write_to_this__chan_STRING;
		signal(ERR_not_possible);
		}
	}


	
errcode _chanOPget_def_mode(ans)
CLUREF *ans;
{
	if (_chan_defmode == 0) ans->num = 0666;
	else ans->num = _chan_defmode;
	signal(ERR_ok);
	}



errcode _chanOPset_def_mode(mode)
CLUREF mode;
{
	_chan_defmode = mode.num;
	signal(ERR_ok);
	}

errcode _chanOPclose_all()
{
int count, i;

	count = getdtablesize();
	for (i = 3 ; i < count; i++) {
		if (i == rdpipe || i == wrpipe) continue;
		close(i);
		}
	signal(ERR_ok);
	}

errcode _chanOPdebug_print(chref, pst)
CLUREF chref, pst;
{
_chan *ch  = (_chan *)chref.ref;
CLUREF temp_str, comma, nm, ans;
errcode err;

	stringOPcons("[", CLU_1, CLU_1, &temp_str);
	err = pstreamOPstart(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;
	stringOPcons(",", CLU_1, CLU_1, &comma);
	if (err != ERR_ok) resignal(err);

	stringOPcons("fn: ", CLU_1, CLU_4, &nm);
	err = pstreamOPtext(pst, nm, &ans);
	if (err != ERR_ok) resignal(err);
	err = file_nameOPprint(ch->fn, pst);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPpause(pst, comma, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;

	stringOPcons("rd: ", CLU_1, CLU_4, &nm);
	err = pstreamOPtext(pst, nm, &ans);
	if (err != ERR_ok) resignal(err);
	err = intOPprint(ch->rd, pst);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPpause(pst, comma, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;

	stringOPcons("wr: ", CLU_1, CLU_4, &nm);
	err = pstreamOPtext(pst, nm, &ans);
	if (err != ERR_ok) resignal(err);
	err = intOPprint(ch->wr, pst);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPpause(pst, comma, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;

	stringOPcons("typ: ", CLU_1, CLU_5, &nm);
	err = pstreamOPtext(pst, nm, &ans);
	if (err != ERR_ok) resignal(err);
	err = intOPprint(ch->typ, pst);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPpause(pst, comma, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;

	stringOPcons("new: ", CLU_1, CLU_5, &nm);
	err = pstreamOPtext(pst, nm, &ans);
	if (err != ERR_ok) resignal(err);
	err = boolOPprint(ch->new, pst);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPpause(pst, comma, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;

	stringOPcons("perm: ", CLU_1, CLU_6, &nm);
	err = pstreamOPtext(pst, nm, &ans);
	if (err != ERR_ok) resignal(err);
	err = boolOPprint(ch->perm, pst);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPpause(pst, comma, &ans);
	if (err != ERR_ok) resignal(err);
	if (ans.tf == false) goto done;

done:
	err = stringOPcons("}", CLU_1, CLU_1, &temp_str);
	if (err != ERR_ok) resignal(err);
	err = pstreamOPstop(pst, temp_str, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

typedef struct {
long count;
    OWNPTR type_owns;
    OWNPTR op_owns;
struct OP_ENTRY entry[4];
} _chan_OPS;

OWN_ptr _chan_own_init = {1, 0};

CLU_proc _chan_oe_copy = {{0,0,0,0}, _chanOPcopy, &_chan_own_init, &_chan_own_init};
CLU_proc _chan_oe_equal = {{0,0,0,0}, _chanOPequal, &_chan_own_init, &_chan_own_init};
CLU_proc _chan_oe_similar = {{0,0,0,0}, _chanOPsimilar, &_chan_own_init, &_chan_own_init};
CLU_proc _chan_oe_debug_print = {{0,0,0,0}, _chanOPdebug_print, &_chan_own_init, &_chan_own_init};

_chan_OPS _chan_ops_actual = {4,
    &_chan_own_init,
    &_chan_own_init, {
{&_chan_oe_copy, "copy"},
{&_chan_oe_equal, "equal"},
{&_chan_oe_similar, "similar"},
{&_chan_oe_debug_print, "debug_print"}}
};

_chan_OPS *_chan_ops = &_chan_ops_actual;
