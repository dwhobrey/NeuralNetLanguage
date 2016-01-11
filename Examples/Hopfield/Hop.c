/* NeSeL v2.0 April 1989. Hopfield neural net demo */
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

#include "hop.nsh"

#define SCRV 3 /* number of state values per node */
#define SCRO 5 /* left offset for state value margin */

/* dimensions of image plane */
#define HOPX 6
#define HOPY 8
#define BLOB 20

void updnode(int w,int nn,float val)
{ int x1=BLOB*(nn%HOPX),y1=BLOB*(nn/HOPX);
  w_pencolor(w,(int)(val*wnd_num_colors));
  w_fillstate(w,1);
  w_rectangle(w,x1,y1,x1+(BLOB-2),y1+(BLOB-2));
}

#define xcor(nn) (int)(SCRO+np->numelm*((nn)%np->numlin))
#define ycor(nn) (int)(1+((int)((nn)/np->numlin)*SCRV))
/* displays various state values for node */
void dsp_state(NSLNSLnet *net,long nd,float aa,float bb,float cc)
{ char *fmtstr="%.Xf "; hop_io_ds *hp=nsl_drv(hop_io_ds); nio_ds *np=(&hp->nio); 
  if(!np->scr_s) return; /* not enough room on vdu */
  fmtstr[2]=(char)('0'+np->numelm-3);
  nd-=np->bs_hd;
  w_position(np->scr_s,xcor(nd),ycor(nd));w_printf(np->scr_s,fmtstr,aa);
  w_position(np->scr_s,xcor(nd),1+ycor(nd));w_printf(np->scr_s,fmtstr,bb);
  w_position(np->scr_s,xcor(nd),2+ycor(nd));w_printf(np->scr_s,fmtstr,cc);
}
void hop_draw(NSLNSLnet *net,va_list args)
{ int nn; BOOL refresh=va_arg(args,BOOL);
  hop_io_ds *hp=nsl_drv(hop_io_ds); nio_ds *np=(&hp->nio);
  w_clear(np->scr_d); w_clear(np->scr_l); w_clear(np->scr_p);
  if(np->scr_s)
    {w_clear(np->scr_s);
     for(nn=0;nn<np->numbar;++nn) w_printf(np->scr_s,"\n--- \n+++ \n*** ");
    }
  if(refresh)
    {w_refresh(np->scr_d,TRUE); w_refresh(np->scr_l,TRUE);
	 w_refresh(np->scr_p,TRUE); if(np->scr_s) w_refresh(np->scr_s,TRUE);}
}

BOOL hop_setup(NSLNSLnet *net,int num) 
{ BOOL dumy=FALSE; int nn,w;
  hop_io_ds *hp=nsl_drv(hop_io_ds); nio_ds *np=(&hp->nio);
  np->numbar=1+(num-1)/15; np->numlin=num/np->numbar;
  if(num%np->numbar>0) ++np->numlin;
  np->numelm=((w_x(w_size(wnd_std,TRUE))-SCRO)/np->numlin);
    /* state output levels */
  np->scr_l=w_open("levels",FALSE,TRUE,FALSE,TRUE,FALSE,num,10,wnd_black,wnd_white);
  if(num<=20)
    np->scr_s=w_open("states",TRUE,FALSE,FALSE,TRUE,FALSE,w_x(w_size(wnd_std,TRUE)),np->numbar*SCRV,
    wnd_black,wnd_white);
  else np->scr_s=0;
  /* 2D wnd for image space */
  np->scr_d=w_open("image",FALSE,TRUE,FALSE,FALSE,FALSE,HOPX*BLOB,HOPY*BLOB,wnd_black,wnd_white);
  /* original pattern */
  np->scr_p=w_open("probe",FALSE,TRUE,FALSE,FALSE,FALSE,HOPX*BLOB,HOPY*BLOB,wnd_black,wnd_white);
  np->scr_t=np->scr_l;

  /* reposition windows */
  w=np->scr_l; if(w>=0) {w_origin(w,670,310); w_resize(w,400,336);}
  w=np->scr_d; if(w>=0) {w_origin(w,884, 10); w_resize(w,204,252);}
  w=np->scr_p; if(w>=0) {w_origin(w,645, 10); w_resize(w,204,252);}

  w_title(np->scr_d,NIO_TL,NIO_DRAW,FALSE,"Image");
  w_title(np->scr_d,NIO_BC,NIO_DRAW,FALSE,"X");
  w_title(np->scr_d,NIO_LC,NIO_DRAW,FALSE,"Y");
  w_title(np->scr_p,NIO_TL,NIO_DRAW,FALSE,"Probe");
  w_title(np->scr_p,NIO_BC,NIO_DRAW,FALSE,"X");
  w_title(np->scr_p,NIO_LC,NIO_DRAW,FALSE,"Y");
  w_title(np->scr_l,NIO_TL,NIO_DRAW,FALSE,"Activity Levels");
  w_title(np->scr_l,NIO_BL,NIO_DRAW,FALSE,"HOP Demo");
  w_title(np->scr_l,NIO_LT,NIO_DRAW,FALSE,"1");
  w_title(np->scr_l,NIO_LB,NIO_DRAW,FALSE,"0");
  np->my_tracks=lmore((char *)NULL,(int)(np->fld_siz),TYPE_SHORT,int);
  for(nn=0;nn<np->fld_siz;++nn)
  	{np->my_tracks[nn]=w_track(np->scr_l,NIO_CHR,wnd_black,wnd_white,
      		(char)('A'+(nn%26)));
  	}
  hop_draw(net,(va_list)&dumy);
  return(TRUE);
}

int hop_pattern(NSLNSLnet *net,va_list args)
{ int nn; 
  hop_io_ds *hp=nsl_drv(hop_io_ds); nio_ds *np=(&hp->nio); 
  np->xx=0;
  dsi(np->d0,hop_ctrl_ds,stable)=2;
  for(nn=0;nn<np->fld_siz;++nn) dsf(np->d0,hop_ctrl_ds,output[nn])=rndf();
  return(0);
}
int hop_update(NSLNSLnet *net,va_list args)
{ int nn; static char buff[20];
  hop_io_ds *hp=nsl_drv(hop_io_ds); nio_ds *np=(&hp->nio);
  sprintf(buff,"%ld:%d",net->time,np->xx++);
  w_title(np->scr_t,NIO_TR,NIO_DRAW,FALSE,buff);
  for(nn=0;nn<np->fld_siz;++nn)
     {updnode(np->scr_p,nn,dsf(np->d0,hop_ctrl_ds,output[nn]));
      updnode(np->scr_d,nn,dsf(np->bs_hd+nn,hop_ds,output));
      w_track(np->scr_l,NIO_FLOAT,np->my_tracks[nn],
            ((float)nn/(np->fld_siz-1)),
            (float)1.0-dsf(np->bs_hd+nn,hop_ds,output));
     }
  w_update(np->scr_l);
  return(0);
}

#include "hop.nsc"
#include "hop.nsd"

int NSLmain(int argc,char *argv[])
{	nsl_mon_wnds();
	nsl_install(&network,"aux_hop");
	/*nsl_driver(&network,NSLIO_SETUP,(char *)0L);*/
	nsl_monitor("printf(\"For demonstration try: call hop;\\n\");");
	return(0);
}
