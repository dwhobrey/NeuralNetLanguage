/* NeSeL v2.0 April 1989 */
/* Specht Prob neural net demo */
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

#include "spec.nsh"

#define BLOB 10
#define OUTDIG 8

float spec_pats[4][3]={
  {(float)0.0,(float)0.0,(float)0.0},
  {(float)0.0,(float)1.0,(float)1.0},
  {(float)1.0,(float)0.0,(float)1.0},
  {(float)1.0,(float)1.0,(float)0.0}};

void spec_blob(NSLNSLnet *net, float val, int x,int y)
{ int x1=BLOB*x,y1=BLOB*y,w=nsl_drv(sio_ds)->scr_a;
  w_pencolor(w,wnd_black);
  w_fillstate(w,0);
  w_rectangle(w,x1,y1,x1+BLOB,y1+BLOB);
  w_pencolor(w,(int)((1.0-val)*wnd_num_colors));
  w_fillstate(w,1);
  w_rectangle(w,x1+1,y1+1,x1+BLOB-1,y1+BLOB-1);
}

/* displays output values for output, sum & pattern nodes */
void spec_state(NSLNSLnet *net,float aa, int nn,int x,int y)
{ sio_ds *np=nsl_drv(sio_ds); int w=np->scr_w;
  w_position(w,1,y); w_clear_line(w); w_printf(w,"%.5f",aa);
  spec_blob(net,aa,nn,x);
}

int spec_update(NSLNSLnet *net,va_list args)
{ sio_ds *np=nsl_drv(sio_ds); int nn,w,y=1; static char buff[20];
  sprintf(buff,"%03ld:%03d",net->time,np->xx++);
  w_title(np->scr_t,NIO_TR,NIO_DRAW,0,buff);
  w=np->scr_w;
  w_position(w,1,y); w_printf(w,"In"); ++y;
  for(nn=0;nn<np->num_in;++nn)
	spec_state(net,dsf(np->b_in+nn,spec_data_ds,output),nn,(int)0,y++);
  w_position(w,1,y); w_printf(w,"Pats"); ++y;
  for(nn=0;nn<np->num_pat;++nn)
    spec_state(net,dsf(np->b_pat+nn,spec_pat_ds,output),nn,(int)1,y++);
  w_position(w,1,y); w_printf(w,"Sums"); ++y;
  for(nn=0;nn<np->num_sum;++nn)
     spec_state(net,dsf(np->b_sum+nn,spec_sum_ds,output),nn,(int)2,y++);
  w_position(w,1,y); w_printf(w,"Out"); ++y;
  for(nn=0;nn<np->num_out;++nn)
     {spec_state(net,(float)dsx(BOOL,np->b_out+nn,spec_out_ds,output),nn,(int)3,y++);
      spec_blob(net,(float)inx(BOOL,dsp(np->b_out+nn,spec_out_ds,desired)),nn,4);
	 }
  return(0);
}

void spec_info(NSLNSLnet *net)
{ sio_ds *np=nsl_drv(sio_ds); int w=np->scr_i;
  w_clear(w);
  w_printf(w,"pattern:\n 0=000,1=011,2=101,3=110\n");
  w_printf(w,"driver:\n 0/1=learn off/on  2/3=reset off/on");
}

void spec_draw(NSLNSLnet *net,va_list args)
{ BOOL refresh=va_arg(args,BOOL);
  sio_ds *np=nsl_drv(sio_ds);
  w_clear(np->scr_a); w_clear(np->scr_w);
  spec_info(net);
  if(refresh) {w_refresh(np->scr_a,1); w_refresh(np->scr_w,1);}
}

BOOL spec_setup(NSLNSLnet *net)
{ BOOL dumy=FALSE; sio_ds *np=nsl_drv(sio_ds); int x,y,yw;
  x=np->num_in; y=np->num_out;
  if(y>x) x=y;
  if(np->num_pat>x) x=np->num_pat;
  if(np->num_sum>x) x=np->num_sum;
  yw=np->num_in+np->num_out+np->num_pat+np->num_sum+4;
  np->scr_a=w_open("state",FALSE,TRUE,FALSE,FALSE,FALSE,x*BLOB,6*BLOB,wnd_black,wnd_white);
  np->scr_w=w_open("output",FALSE,TRUE,FALSE,TRUE,FALSE,OUTDIG,yw,wnd_black,wnd_white);
  np->scr_i=w_open("info",TRUE,TRUE,FALSE,TRUE,FALSE,38,4,wnd_black,wnd_white);
  np->scr_t=np->scr_i;
  w_title(np->scr_a,NIO_BL,NIO_DRAW,FALSE,"State");
  w_title(np->scr_w,NIO_BL,NIO_DRAW,FALSE,"Output");
  w_title(np->scr_i,NIO_TL,NIO_DRAW,FALSE,"Specht XOR Info");
  spec_draw(net,&dumy);
  return(TRUE);
}

int spec_probe(NSLNSLnet *net,va_list args)
{ sio_ds *np=nsl_drv(sio_ds); int nn=va_arg(args,int);
  switch(nn)
	{case 0: case 1: case 2: case 3: /* set learning pattern */
	   dsf(np->b_user,spec_user_ds,pattern[0])=spec_pats[nn][0];
	   dsf(np->b_user,spec_user_ds,pattern[1])=spec_pats[nn][1];
	   dsx(BOOL,np->b_user,spec_user_ds,desired[0])=(BOOL)spec_pats[nn][2];
	   np->xx=0; break;
	 default:;
	}
  return(0);
}

int spec_driver(NSLNSLnet *net,va_list args)
{ sio_ds *np=nsl_drv(sio_ds); int nn=va_arg(args,int);
  switch(nn)
	{case 0: case 1: /* turn learning off/on */
        dsx(BOOL,np->b_user,spec_user_ds,learn)=(BOOL)(nn==1); break;
     case 2: case 3: /* reset off/on */
        dsx(BOOL,np->b_user,spec_user_ds,reset)=(BOOL)(nn==3); break;
	 default:;
	}
  return(0);
}

#include "spec.nsc"
#include "spec.nsd"

int NSLmain(int argc,char *argv[])
{	nsl_mon_wnds();
	nsl_install(&network,"aux_spec");
	/*nsl_driver(&network,NSLIO_SETUP,(char *)0L);*/
	nsl_monitor("printf(\"For demonstration try: call specxor;\\n\");");
	return(0);
}
