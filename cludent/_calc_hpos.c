#include "pclu_err.h"
#include "pclu_sys.h"

errcode _calc_hpos(s, max, ret, elist)
CLUREF s, max, *ret;
errlist elist;
{
#ifdef sparc
  register char *n1, n2;
#else
  register signed char *n1, n2;
#endif
  register int n3, rr;

  n3 = max.num - 1;

  if (n3 <= 0) {
    ret->num = 0;
    signal (ERR_ok);
  }
  rr = 0;
  if (n3 > s.str->size) n3 = s.str->size;
#ifdef sparc
  n1 = (char *) &s.str->data[0];
#else
  n1 = (signed char *) &s.str->data[0];
#endif
  while (1) {
    n2 = *(n1++);
    if ((n2 >= ' ') && (n2 < '\177')) {
      rr++;
      if (--n3 > 0) continue;
      ret->num = rr;
      signal (ERR_ok);
    }
    rr += 2;
    if (n2 == '\t') {
      rr += 6;
      rr -= rr & 7;    /* What's the right way to clear low order bits in C? */
    }
    if (--n3 > 0) continue;
    ret->num = rr;
    signal (ERR_ok);
  }
}

/*

_calc_hpos = qproc (s: string, max: int) returns (int)
	ifleq	subl	1,rr,n3
	then	return(0)
	end
	clrl	rr
	movl	s,n1
	movzwl	(n1),n2
	ifgtr	cmpl	n3,n2
	then	movl	n2,n3
	end
	addl	4,n1
	qloop
		movb	(n1)+,n2
		iflss	cmpb	n2,' '
			blss	next
			cmpb	n2,'\177'
		then	incl	rr
			sobgtr	n3,this
			return(rr)
		end
		addl	2,rr
		ifeql	cmpb	'\t',n2
		then	addl	6,rr
			bicb	7,rr
		end
		sobgtr	n3,this
	end
	return(rr)
	end _calc_hpos
 */
