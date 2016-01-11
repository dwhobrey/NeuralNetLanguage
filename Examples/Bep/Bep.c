/* NeSeL v2.0 April 1989. Bep neural net demo */
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
#include "bep.nsh"

#define BLOB 20
#define WTDIG 8
int blobx=BLOB,bloby=BLOB;

float bep_pats[4][3]={
  {(float)0.0,(float)0.0,(float)0.0},
  {(float)0.0,(float)1.0,(float)1.0},
  {(float)1.0,(float)0.0,(float)1.0},
  {(float)1.0,(float)1.0,(float)0.0}};

void bep_blob(NSLNSLnet *net,float val,int x,int y)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio); 
  int x1=blobx*x,y1=bloby*y,w=np->scr_a;
  w_pencolor(w,wnd_black);
  w_fillstate(w,0);
  w_rectangle(-w,x1,y1,x1+blobx,y1+bloby);
  w_pencolor(w,(int)((1.0-val)*wnd_num_colors));
  w_fillstate(w,1);
  w_rectangle(-w,x1+1,y1+1,x1+blobx-1,y1+bloby-1);
}

/* displays output & weight values for output or hidden node */
void bep_state(NSLNSLnet *net,long nd,float aa,float bb,
			   bep_synapse *wp, int num, BOOL ishid)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio); 
  char *lay,*level,*fmtstr="%.Xf "; int y,kk,qq,rr; int w=np->scr_w;
  qq=w_x(w_size(w,TRUE))-8; qq/=(2+num);
  fmtstr[2]=(char)('0'+qq-3);
  if(ishid)
    {qq=(int)(nd-np->b_hid); kk=0; y=1+qq;
	 while(qq>=np->layers[kk]) {qq-=np->layers[kk]; ++kk;}
	 lay="hid"; level="0"; *level=(char)('0'+kk);
	 rr=1+kk;
	}
  else
	{qq=(int)(nd-np->b_out);
	 y=1+np->num_hid+qq;
	 lay="out"; level=" \0";
	 rr=1+np->num_layers;
	}
  w_position(w,1,y); w_clear_line(-w);
  w_printf(-w,"%s%s:%d:",lay,level,qq);
  w_printf(-w,fmtstr,aa); w_printf(-w,fmtstr,bb); 
  bep_blob(net,aa,qq,rr);
  while(num--) {w_printf(-w,fmtstr,wp->weight); ++wp;}
}
void bep_graph(NSLNSLnet *net)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio); 
  int w=np->scr_g;
  float top,bot,cc,gain,yy,xx,xinc=(float)(1.0/100.0);
  w_clear(w);
  gain=dsf(np->b_ctrl,bep_ctrl_ds,sg);
  top=dsf(np->b_ctrl,bep_ctrl_ds,sa);
  bot=dsf(np->b_ctrl,bep_ctrl_ds,sb);
  cc=dsf(np->b_ctrl,bep_ctrl_ds,sc);
  for(xx=(float)0.0;xx<(float)1.0;xx+=xinc)
    {yy=generic_sigmoid(xx,top,bot,gain,cc);
     if(yy>=(float)0.0&&yy<=(float)1.0)
       w_setpixel(w,(int)(100.0*xx),(int)(100.0*(1.0-yy)));
    }
}

void bep_info(NSLNSLnet *net)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio);   
  int w=np->scr_i;
  w_clear(w);
  w_printf(w,"pattern:\n 0=000,1=011,2=101,3=110\n");
  w_printf(w,"driver:\n 0/1=learn off/on  2/3=update off/on\n");
  w_printf(w," 4/5=reset off/on  6=graph");
}
void bep_draw(NSLNSLnet *net,va_list args)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio);   
  BOOL refresh=va_arg(args,BOOL);
  w_clear(np->scr_a); w_clear(np->scr_w);
  bep_info(net); bep_graph(net);
  if(refresh) {w_refresh(np->scr_a,TRUE); w_refresh(np->scr_w,TRUE);}
}

BOOL bep_setup(NSLNSLnet *net)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio);   
  BOOL dumy=FALSE; int x,y,ya,yw,kk,w;
  x=np->num_in; y=np->num_out;
  if(y>x) x=y;
  kk=np->num_layers; ya=kk+3; yw=y+np->num_hid;
  while(kk--) {if(np->layers[kk]>x) x=np->layers[kk];}
  np->scr_a=w_open("active",FALSE,TRUE,TRUE,FALSE,FALSE,x*BLOB,ya*BLOB,wnd_black,wnd_white);
  np->scr_w=w_open("weights",FALSE,TRUE,FALSE,TRUE,FALSE,8+(2+x)*WTDIG,yw,wnd_black,wnd_white);
  np->scr_i=w_open("info",TRUE,TRUE,FALSE,TRUE,FALSE,38,5,wnd_black,wnd_white);
  np->scr_g=w_open("graph",TRUE,TRUE,FALSE,FALSE,FALSE,100,100,wnd_black,wnd_white);
  np->scr_t=np->scr_w;

  blobx=w_x(w_size(np->scr_a,FALSE))/x;
  bloby=w_y(w_size(np->scr_a,FALSE))/ya;

  /* reposition windows */
  w=np->scr_a; if(w>=0) {w_origin(w,760,10); w_resize(w,320,384);}
  w=np->scr_w; if(w>=0) {w_origin(w,760,440); w_resize(w,336,70);}
  w=np->scr_i; if(w>=0) {w_origin(w,760,550); w_resize(w,320,98);}
  w=np->scr_g; if(w>=0) {w_origin(w,600,10); w_resize(w,128,140);}

  w_title(np->scr_a,NIO_TL,NIO_DRAW,FALSE,"Activities");
  w_title(np->scr_a,NIO_BL,NIO_DRAW,FALSE,"Bep Demo");
  w_title(np->scr_w,NIO_BL,NIO_DRAW,FALSE,"Output/Bias/Weights");
  w_title(np->scr_g,NIO_TL,NIO_DRAW,FALSE,"Transfer fn");
  w_title(np->scr_i,NIO_TL,NIO_DRAW,FALSE,"Bep XOR Info");
  bep_draw(net,(va_list)&dumy);
  return(TRUE);
}

int bep_pattern(NSLNSLnet *net,va_list args)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio);   
  int nn=va_arg(args,int);
  switch(nn)
	{case 0: case 1: case 2: case 3: /* set learning pattern */
	   dsf(np->b_user,user_ds,pattern[0])=bep_pats[nn][0];
	   dsf(np->b_user,user_ds,pattern[1])=bep_pats[nn][1];
	   dsf(np->b_user,user_ds,desired[0])=bep_pats[nn][2];
	   np->xx=0; break;
	 default:;
	}
  return(0);
}
int bep_user(NSLNSLnet *net,va_list args)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio);   
  int nn=va_arg(args,int);
  switch(nn)
	{case 0: case 1: /* turn learning off/on */
        dsx(BOOL,np->b_user,user_ds,learn)=(BOOL)(nn==1); break;
  case 2: case 3: /* turn update off/on */
        dsx(BOOL,np->b_user,user_ds,update)=(BOOL)(nn==3); break;
  case 4: case 5: /* reset off/on */
        dsx(BOOL,np->b_user,user_ds,reset)=(BOOL)(nn==5); break;
	 case 6: bep_graph(net); break;
	 default:;
	}
  return(0);
}

int bep_update(NSLNSLnet *net,va_list args)
{ bep_io_ds *bp=nsl_drv(bep_io_ds); bio_ds *np=(&bp->bio);   
  int nn; static char buff[20];
  sprintf(buff,"%03ld:%03d",net->time,np->xx++);
  w_title(np->scr_t,NIO_TR,NIO_DRAW,FALSE,buff);
  for(nn=0;nn<np->num_in;++nn)
    bep_blob(net,dsf(np->b_in+nn,bep_in_ds,output),nn,0);
  for(nn=0;nn<np->num_out;++nn)
    {bep_blob(net,dsf(np->b_out+nn,bep_out_ds,output),
	    nn,1+np->num_layers);
     bep_blob(net,inf(dsp(np->b_out+nn,bep_out_ds,desired)),
	   nn,2+np->num_layers);
	}
  w_update(np->scr_a); w_update(np->scr_w);
  return(0);
}

#include "bep.nsc"
#include "bep.nsd"

int NSLmain(int argc,char *argv[])
{	nsl_mon_wnds();
	nsl_install(&network,"aux_bep");
	/*nsl_driver(&network,NSLIO_SETUP,(char *)0L);*/
	nsl_monitor("printf(\"For demonstration try: call bepxor;\\n\");");
	return(0);
}
