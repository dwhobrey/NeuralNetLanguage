/* NeSeL v2.0 April 1989 */
/* Boltzmann Machine neural net Demo */
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

#include "bolt.nsh"


#define BLOB 20
#define OUTDIG 8

BOOL bolt_pats[4][3]={
  {0,0,0},
  {0,1,1},
  {1,0,1},
  {1,1,0}};

void bolt_blob(NSLNSLnet *net,float val,int x,int y)
{ int x1=BLOB*x,y1=BLOB*y,w=nsl_drv(bolt_io_ds)->bmio.scr_a;
  w_pencolor(w,wnd_black);
  w_fillstate(w,0);
  w_rectangle(w,x1,y1,x1+BLOB,y1+BLOB);
  w_pencolor(w,(int)((1.0-val)*wnd_num_colors));
  w_fillstate(w,1);
  w_rectangle(w,x1+1,y1+1,x1+BLOB-1,y1+BLOB-1);
}

/* displays output values for nodes */
void bolt_state(NSLNSLnet *net,int bb,int nn,int x,int y)
{ bmio_ds *np=(&nsl_drv(bolt_io_ds)->bmio); 
  int w=np->scr_w,xx; float aa;
  w_position(w,1,y); w_clear_line(-w);
  aa=dsf(np->b_node+bb,bolt_ds,output);
  w_printf(-w,"%.5f ",aa);
  w_printf(-w,"%.5f ",dsf(np->b_node+bb,bolt_ds,bias));
  for(xx=0;xx<(np->num_vis+np->num_hid-1);++xx)
    w_printf(-w,"%.5f ",dsf(np->b_node+bb,bolt_ds,input[xx].weight));
  bolt_blob(net,aa,nn,x);
}

int bolt_update(NSLNSLnet *net,va_list args)
{ bmio_ds *np=(&nsl_drv(bolt_io_ds)->bmio); 
  int bb,nn,w,y=1; static char buff[20];
  sprintf(buff,"%03ld:%03d",net->time,np->xx++);
  w_title(np->scr_t,NIO_TR,NIO_DRAW,FALSE,buff);
  w=np->scr_w;
  w_position(w,1,y); w_printf(-w,"Vis"); ++y; bb=np->num_vis;
  for(nn=0;nn<bb;++nn) bolt_state(net,nn,nn,(int)0,y++);
  w_position(w,1,y); w_printf(-w,"Hid"); ++y;
  for(nn=0;nn<np->num_hid;++nn) bolt_state(net,nn+bb,nn,(int)1,y++);
  w_update(np->scr_a); w_update(np->scr_w);
  return(0);
}

void bolt_info(NSLNSLnet *net)
{ bmio_ds *np=(&nsl_drv(bolt_io_ds)->bmio); 
  int w=np->scr_i;
  w_clear(w);
  w_printf(w,"pattern:\n 0=000,1=011,2=101,3=110\n");
  w_printf(w,"driver:\n 0/1=reset off/on  2/3=clear off/on\n");
  w_printf(w," 4/5=clamp off/on  6/7=anneal off/on\n");
  w_printf(w," 8/9=learn off/on");
}

void bolt_draw(NSLNSLnet *net,va_list args)
{ BOOL refresh=va_arg(args,BOOL);
  bmio_ds *np=(&nsl_drv(bolt_io_ds)->bmio); 
  w_clear(np->scr_a); w_clear(np->scr_w);
  bolt_info(net);
  if(refresh) {w_refresh(np->scr_a,TRUE); w_refresh(np->scr_w,TRUE);}
}

BOOL bolt_setup(NSLNSLnet *net)
{ bmio_ds *np=(&nsl_drv(bolt_io_ds)->bmio); 
  int x,y,yw; BOOL dumy=FALSE; 
  x=np->num_vis; y=np->num_hid; yw=x+y+2;
  if(y>x) x=y;
  np->scr_a=w_open("state",FALSE,TRUE,FALSE,FALSE,FALSE,x*BLOB,3*BLOB,wnd_black,wnd_white);
  np->scr_w=w_open("output",FALSE,TRUE,FALSE,TRUE,FALSE,(yw-1)*OUTDIG,yw,wnd_black,wnd_white);
  np->scr_i=w_open("info",TRUE,TRUE,FALSE,TRUE,FALSE,38,6,wnd_black,wnd_white);
  np->scr_t=np->scr_w;
  w_title(np->scr_a,NIO_BL,NIO_DRAW,FALSE,"State");
  w_title(np->scr_w,NIO_BL,NIO_DRAW,FALSE,"Output");
  w_title(np->scr_i,NIO_TL,NIO_DRAW,FALSE,"Boltzmann XOR Info");
  bolt_draw(net,(va_list)&dumy);
  return(TRUE);
}

int bolt_pattern(NSLNSLnet *net,va_list args)
{ bmio_ds *np=(&nsl_drv(bolt_io_ds)->bmio); 
  int nn=va_arg(args,int);
  switch(nn)
	{case 0: case 1: case 2: case 3: /* set learning pattern */
	   dsx(BOOL,np->b_user,bolt_user_ds,output[0])=bolt_pats[nn][0];
	   dsx(BOOL,np->b_user,bolt_user_ds,output[1])=bolt_pats[nn][1];
	   dsx(BOOL,np->b_user,bolt_user_ds,output[2])=bolt_pats[nn][2];
	   np->xx=0; break;
	 default:;
	}
  return(0);
}

int bolt_user(NSLNSLnet *net,va_list args)
{ bmio_ds *np=(&nsl_drv(bolt_io_ds)->bmio); 
  int nn=va_arg(args,int);
  switch(nn)
	{case 0: case 1: /* turn reset off/on */
	    dsx(BOOL,np->b_user,bolt_user_ds,reset)=(BOOL)(nn==1); break;
     case 2: case 3: /* clear off/on */
        dsx(BOOL,np->b_user,bolt_user_ds,clear)=(BOOL)(nn==3); break;
	 case 4: case 5: /* clamp off/on */
        dsx(BOOL,np->b_user,bolt_user_ds,fixed)=(BOOL)(nn==5); break;
	 case 6: case 7: /* anneal off/on */
        dsx(BOOL,np->b_user,bolt_user_ds,anneal)=(BOOL)(nn==7); break;
	 case 8: case 9: /* learn off/on */
        dsx(BOOL,np->b_user,bolt_user_ds,learn)=(BOOL)(nn==9); break;
	 default:;
	}
  return(0);
}

#include "bolt.nsc"
#include "bolt.nsd"

int NSLmain(int argc,char *argv[])
{	nsl_mon_wnds();
	nsl_install(&network,"aux_bolt");
	/*nsl_driver(&network,NSLIO_SETUP,(char *)0L);*/
	nsl_monitor("printf(\"For demonstration try: call boltxor;\\n\");");
	return(0);
}
