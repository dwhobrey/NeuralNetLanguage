/* Name: nsltwo2.c   Type: main C file    Date: 30 March 89
*  NeSeL Neural Net Specification Language Preprocessor
*/
#include "globals2.h"
#include "iolo2.h"
#include "iohi2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "nslyacc2.h"
#include "pathhi2.h"

/* Note -- format of ver str is critical */
char *version="NeSeL(b)v%c.%c(DJRW)1989\n%s\n",version_num='2',version_upd='1';

extern void thirdpass();

void keyint(int sig)
{ fprintf(logfp,"\nCompilation aborted.\n");
  exit(sig);
}

/* The main one */
int main(argc,argv)
  int argc; char *argv[];
{ logfp=stdout; err_file_num=(-1); err_line_num=0; num_errors=0;
  source_list=NULL;
  sour_num=0; /* incase we get an error even before doing anything */
  raise_error=ERR_OK; flags.wild=F;
  io_initialise();
  signal(SIGINT,keyint); /*signal(SIGTSTP,keyint);*/
  getmainargs(argc,argv);
  openfiles();
  nal_initialise();
  thirdpass();
  return(num_errors>0);
}
