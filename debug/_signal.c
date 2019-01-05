
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _signal.c,v 1.4 91/07/09 15:23:22 root Exp $";
#endif lint
/* $Log:	_signal.c,v $
 * Revision 1.4  91/07/09  15:23:22  root
 * removed unused variables _signalOPbuf, _signalOPibuf
 * 
 * Revision 1.3  91/06/06  13:51:10  dcurtis
 * added copyright notice
 * 
 * Revision 1.2  91/06/03  16:59:43  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.1  91/02/04  23:21:01  mtv
 * Initial revision
 * 
 */

/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*			_signal			*/
/*						*/

#include <signal.h>
#undef signal

#include "pclu_err.h"
#include "pclu_sys.h"

#include <sys/types.h>
#include <sys/dir.h>

extern CLUREF clu_empty_string;
extern char **environ;
extern int wrpipe;
extern int errno;
#ifdef LINUX
#define gcflag GC_in_progress
#else
extern int gcflag;
#endif

typedef void (VPROC)();

extern void _signalOPhand();
static int _signalOPflags;
static int _signalOPholds;
static int _signalOPcnts[32];
static VPROC *_signalOPohands[32];
static int _signalOPomasks[32];
static int _signalOPostks[32];
static int _signalOPmsgs = 0;   /* CLUREF/CLUSEQUENCE in disguise */
void sigfixmask(sigset_t *setp, int m);
int sigetmask(sigset_t *setp);

errcode _signalOPset(sig, hold)
CLUREF sig, hold;
{
int omask;
struct sigaction vec, ovec;

	if (sig.num < 0 || sig.num > 32) signal(ERR_bad_code);
	if (~hold.tf) {
		vec.sa_handler = _signalOPhand;
		sigfixmask(&vec.sa_mask, 0xf8e70020);
		/* vec.sa_mask = 0xf8e70020; */
		vec.sa_flags = 0;
		/* vec.sa_flags = SA_STACK; */
		sigaction(sig.num, &vec, &ovec);
		if (~(_signalOPflags & (1 << (sig.num - 1)))) {
			_signalOPflags |= 1 << (sig.num - 1); 
			_signalOPohands[sig.num - 1] = ovec.sa_handler;
			_signalOPomasks[sig.num - 1] = sigetmask(&ovec.sa_mask);
			/* _signalOPomasks[sig.num - 1] = ovec.sa_mask; */
			_signalOPostks[sig.num - 1] = ovec.sa_flags;
			}
		if (_signalOPholds & (1 << (sig.num - 1))) {
			_signalOPholds &= ~(1 << (sig.num - 1));
			omask = sigblock(0);
			omask &= ~(1 << (sig.num - 1));
			sigsetmask(omask);
			}
		}
	else {
		_signalOPholds |= (1 << (sig.num - 1));
		sigblock( 1 << (sig.num - 1) );
		}
	signal(ERR_ok);
			
}

void _signalOPhand(sig, code, scp)
int sig, code;
struct sigcontext *scp;
{
CLUREF msgs, str;

	/* printf("signal caught: %d %X %d\n", sig, scp->sc_pc, gcflag); */
	_signalOPcnts[sig-1] += 1;
	/* if (gcflag) return; */
	if (_signalOPmsgs != 0) {
		msgs.num = _signalOPmsgs;
		str.num = msgs.vec->data[sig - 1];
		if (str.num != 0) {
			write(1, str.vec->data, str.vec->size);
			write(1, "\n", 1);
			}
		}
	if ((sig == 10) || (sig == 11)) {
		debugOPwhere_cmd(clu_empty_string);
		/* the following loses (illegal instr...)
		debugOPrestart_cmd(clu_empty_string);
		*/
		_exit(-1);
		/* scp->sc_pc = scp->sc_pc + 4; */
	        /* printf("incremented pc: %X\n", scp->sc_pc); */
		}
	/* scp->sc_pc = scp->sc_pc + 4; */
	/* printf("signal returning: %d %X\n", code, scp->sc_pc); */
	return;
	}

errcode _signalOPunset(sig)
CLUREF sig;
{
int omask;
errcode err;
struct sigaction vec;

	if (sig.num < 0 || sig.num > 32) signal(ERR_bad_code);
	if (_signalOPflags && (1 << (sig.num - 1))) {
		_signalOPflags &= ~(1 << (sig.num - 1)); 
		vec.sa_handler = _signalOPohands[sig.num - 1];
		sigfixmask(&vec.sa_mask, _signalOPomasks[sig.num - 1]);
		/* vec.sa_mask = _signalOPomasks[sig.num - 1]; */
		vec.sa_flags = _signalOPostks[sig.num - 1];
		sigaction(sig.num, &vec, 0);
		}
	if (_signalOPholds && (1 << (sig.num - 1))) {
		_signalOPholds &= ~(1 << (sig.num - 1));
		omask = sigblock(0);
		omask &= ~(1 << (sig.num - 1));
		sigsetmask(omask);
		}
	signal(ERR_ok);
}


errcode _signalOPget(sig, ans)
CLUREF sig, *ans;
{
	if (sig.num < 0 || sig.num > 32) signal(ERR_bad_code);
	ans->num = _signalOPcnts[sig.num - 1];
	_signalOPcnts[sig.num - 1] = 0;
	signal(ERR_ok);
}

errcode _signalOPset_message(sig, msg)
CLUREF sig, msg;
{
errcode err;
CLUREF msgs, str;

	if (sig.num < 0 || sig.num > 32) signal(ERR_bad_code);
	if (_signalOPmsgs == 0) { 
		err = sequenceOPfill(CLU_32, CLU_0, &_signalOPmsgs);
		if (err != ERR_ok) resignal(err);
		}
	msgs.num = _signalOPmsgs;
	msgs.vec->data[sig.num - 1] = msg.num;
	signal(ERR_ok);
	}

errcode _signalOPget_message(sig, ans)
CLUREF sig, *ans;
{
CLUREF msgs, str;

	if (sig.num < 0 || sig.num > 32) signal(ERR_bad_code);
	msgs.num = _signalOPmsgs;
	if (_signalOPmsgs == 0 || msgs.vec->data[sig.num - 1] == 0) {
		ans->vec = clu_empty_string.vec;
		signal(ERR_ok);
		}
	ans->num = msgs.vec->data[sig.num - 1];
	signal(ERR_ok);
	}

errcode _signalOPprint()
{
int i;
CLUREF msgs;

	if (_signalOPmsgs == 0) {
		write(1, "table empty\n", 1);
		signal(ERR_ok);
		}
	msgs.num = _signalOPmsgs;
	for(i = 0; i < 32; i++) {
		if (msgs.vec->data[i] == 0) {
			write(1, "slot empty\n", 11);
			}
		else {
			write(1, "slot full\n", 10);
			}
		}
	signal(ERR_ok);
	}

void sigfixmask(sigset_t *setp, int m)
{
int i;
	sigemptyset(setp);
        for (i = 0; i < 32; i++) {
                int ord = 1 << i;
                if (m & ord) sigaddset(setp, i+1);
                }
}

int sigetmask(sigset_t *setp)
{
int result = 0;
int i;

        for (i = 1; i <= 32; i++) {
                if (sigismember(setp, i)) result |= 1 << (i-1);
                }       
}

