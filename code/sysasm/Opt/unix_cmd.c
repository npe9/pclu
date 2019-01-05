/*						*/
/*						*/
/*		IMPLEMENTATION OF		*/
/*	   	   unix_cmd			*/
/*						*/

#include <sys/wait.h>
#undef signal
#include "pclu_err.h"
#include "pclu_sys.h"

#ifndef LINUX
extern char *system();
#endif

errcode unix_cmd(input)
CLUREF input;
{
/* 2/26/01 dwc: unix prefers that system return int... */
int result;
CLUREF temp_str, n, num, size;
errcode err;

	result = system((char*)input.vec->data); 
/*	printf("err = %X, %X %X %X\n", err, WTERMSIG(err), 
		WEXITSTATUS(err), WSTOPSIG(err)); */
	if (result == 0) signal(ERR_ok);
	err = stringOPcons("call to system failed: ", CLU_1, CLU_23, 
			   &temp_str);
	n.num = (long)result;
	err = intOPunparse(n, &num);
	err = stringOPconcat(temp_str, num, &temp_str);
	elist[0] = temp_str;
	signal(ERR_failure);
	}
