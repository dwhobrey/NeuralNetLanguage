/* NeSeL v2.0 April 1989 */
/* WTA neural net demo */
#include <math.h>
#include <stdio.h>
#include <varargs.h>
#include "wta.nsh"
#include "nslstd.h"
#include "nslios.h"
#include "nslwsio.h"

#define SCRV 3 /* number of state values per node */
#define SCRO 5 /* left offset for state value margin */

#define xcor(nn) (int)(SCRO+np->numelm*((nn)%np->numlin))
#define ycor(nn) (int)(1+((int)((nn)/np->numlin)*SCRV))
/* displays various state values for node */
void dsp_state(nd,aa,bb,cc) long nd; float aa,bb,cc;
{ nio_ds *np=nsl_drv(nio_ds); char *fmtstr="%.Xf ";
  fmtstr[2]=(char)('0'+np->numelm-3);
  nd-=np->bs_hd;
  wposition(np->scr_s,xcor(nd),ycor(nd));wprintf(np->scr_s,fmtstr,aa);
  wposition(np->scr_s,xcor(nd),1+ycor(nd));wprintf(np->scr_s,fmtstr,bb);
  wposition(np->scr_s,xcor(nd),2+ycor(nd));wprintf(np->scr_s,fmtstr,cc);
}
void wta_draw(args) va_list args;
{ int nn; char chr; BOOL refresh=va_arg(args,BOOL);
  nio_ds *np=nsl_drv(nio_ds);
  wclear(np->scr_d); wclear(np->scr_l); wclear(np->scr_s);
  for(nn=0;nn<np->numbar;++nn) wprintf(np->scr_s,"\n--- \n+++ \n*** ");
  if(refresh)
    {wrefresh(np->scr_d,TRUE); wrefresh(np->scr_l,TRUE); wrefresh(np->scr_s,TRUE);}
}
BOOL wta_setup(num) int num;
{ BOOL dumy=FALSE; int nn;
  nio_ds *np=nsl_drv(nio_ds);
  if(num>60) {w_printf(wnd_std,"too many nodes to display\n"); return(FALSE);}
  np->numbar=1+(num-1)/15; np->numlin=num/np->numbar;
  if(num%np->numbar>0) ++np->numlin;
  np->numelm=((w_x(w_size(wnd_std,TRUE))-SCRO)/np->numlin);
  /* 2D wnd for domain space */
  np->scr_d=w_open("domain",TRUE,TRUE,TRUE,TRUE,FALSE,10,10,wnd_black,wnd_white);
  /* state output levels */
  np->scr_l=w_open("levels",FALSE,TRUE,FALSE,TRUE,FALSE,num,10,wnd_black,wnd_white);
  np->scr_s=w_open("states",TRUE,FALSE,FALSE,TRUE,FALSE,w_x(wsize(wnd_std,TRUE)),np->numbar*SCRV,
    wnd_black,wnd_white);
  np->scr_t=np->scr_l;
  w_title(np->scr_d,NIO_TL,NIO_DRAW,FALSE,"Domain Space");
  w_title(np->scr_d,NIO_BC,NIO_DRAW,FALSE,"X");
  w_title(np->scr_d,NIO_LC,NIO_DRAW,FALSE,"Y");
  w_title(np->scr_d,NIO_LT,NIO_DRAW,FALSE,"1");
  w_title(np->scr_d,NIO_LB,NIO_DRAW,FALSE,"0");
  w_title(np->scr_d,NIO_BL,NIO_DRAW,FALSE,"0");
  w_title(np->scr_d,NIO_BR,NIO_DRAW,FALSE,"1");
  w_title(np->scr_l,NIO_TL,NIO_DRAW,FALSE,"Activity Levels");
  w_title(np->scr_l,NIO_BL,NIO_DRAW,FALSE,"WTA Demo");
  w_title(np->scr_l,NIO_LT,NIO_DRAW,FALSE,"1");
  w_title(np->scr_l,NIO_LB,NIO_DRAW,FALSE,"0");
  np->my_tracks=lmore((char *)NULL,(int)(np->fld_siz),TYPE_SHORT,int);
  for(nn=0;nn<np->fld_siz;++nn)
  	{np->my_tracks[nn]=w_track(np->scr_l,NIO_CHR,wnd_black,wnd_white,
      		(char)('A'+(nn%26)));
  	}
  np->trk_probe=w_track(np->scr_d,NIO_CHR,wnd_black,wnd_white,'*');
  wta_draw(&dumy);
  return(TRUE);
}

int wta_probe(args) va_list args;
{ nio_ds *np=nsl_drv(nio_ds); np->xx=0;
  dsf(np->d0,data_ds,output)=rndf(); dsf(np->d0+1,data_ds,output)=rndf();
  return(0);
}
int wta_update(args) va_list args;
{ int nn; nio_ds *np=nsl_drv(nio_ds); static char buff[20];
  sprintf(buff,"%ld:%d",nsl_network->time,np->xx++);
  w_title(np->scr_t,NIO_TR,NIO_DRAW,FALSE,buff);
  w_track(np->scr_d,NIO_FLOAT,np->trk_probe,
  			dsf(np->d0,data_ds,output),
  			(float)1.0-dsf(np->d0+1,data_ds,output));
  for(nn=0;nn<np->fld_siz;++nn)
     {w_track(np->scr_l,NIO_FLOAT,np->my_tracks[nn],
	 			((float)nn/(np->fld_siz-1)),
      			(float)1.0-dsf(np->bs_hd+nn,wta_ds,output));
     }
  return(0);
}

#include "wta.nsc"
#include "wta.nsd"

void main()
{ nsl_install(&network,"aux_wta");
  nsl_mon_wnds();
  nsl_driver(&network,NSLIO_SETUP,(char *)0L);
  nsl_monitor("printf(\"For demonstration try: call wta;\\n\");");
}
