/* NeSeL v2.0 April 1989. Expression evaluator neural net */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "memtrack.h"
/*#include <malloc.h> */
#include <stdarg.h>
#include <math.h>

#include "nsltype.h"
#include "nslstd.h"
#include "nslios.h"
#include "nslwsio.h"

#include "exp.nsh"
#include "exp.nsc"
#include "exp.nsd"

int NSLmain(int argc,char *argv[])
{ int wy,wx;
  nsl_mon_wnds();
  nsl_install(&network,exp_aux_name);
  wy=24; wx=80;
  ewnd=w_open("ExpEval",FALSE,FALSE,FALSE,TRUE,TRUE,wx,wy,wnd_black,wnd_white);
  nsl_monitor("printf(\"For demonstration try: go;\\n\");");
  return(0);
}
