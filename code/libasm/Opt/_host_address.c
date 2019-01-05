
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: _host_address.c,v 1.3 91/07/18 14:37:55 root Exp $";
#endif 
/* $Log:	_host_address.c,v $
 * Revision 1.3  91/07/18  14:37:55  root
 * added cast for NULL to eliminate warning
 * 
 * Revision 1.2  91/06/06  13:47:29  dcurtis
 * added copyright notice
 * 
 * Revision 1.1  91/02/14  16:45:43  dcurtis
 * Initial revision
 * 
 */

/* Copyright Massachusetts Institute of Technology 1985, 1989 */

#include "pclu_err.h"
#include "pclu_sys.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

errcode _host_address(host, ans1, ans2)
CLUREF host, *ans1, *ans2;
{
struct hostent *hp;
int l,r;
int *np;

	hp = gethostbyname(host.str->data);
	if (hp == (struct hostent*)NULL) {
		signal(ERR_not_found);
		}
	if (hp->h_addrtype != AF_INET) {
		signal(ERR_bad_address);
		}
/*	printf("%X\n", *hp->h_addr); */
	np = (int*) hp->h_addr;
	l = (*np >> 16) & 0xffff;	
	r = *np & 0xffff;	
	ans1->num = l;
	ans2->num = r;
	signal(ERR_ok);
	}
