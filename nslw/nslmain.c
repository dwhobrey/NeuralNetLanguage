/* testing NSLmain*/

#include <math.h>
#include "nslms.h"
#include "nslwsio.h"

int NSLmain(int argc,char *argv[])
{	char tmp_buff[256];	char *ap=tmp_buff;
	*ap='\0'; ++argv; --argc;
	while(argc--) {ap=fcat(ap,*argv); ap=fcat(ap," "); ++argv;}
	nsl_mon_wnds();
	nsl_monitor(tmp_buff);
	return(0);
}
