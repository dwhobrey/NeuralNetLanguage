/* NeSeL v2.0 April 1989. DFace neural net */
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

#include "dface.h"
#include "dface.nsh"

/* ------------------  COMMS routines  ----------------- */
/* Normal Distribution Look-Up table parameters */
#define DIST_X 32
#define DIST_Y 100
#define DIST_H (DIST_Y/2)
#define DIST_M (DIST_H-1)
#define DIST_D (DIST_Y-1)
/* Look-up table for normal distribution for comms delay */
int dist_table[DIST_X][DIST_Y+1]={0};

/* Work out quick look up table for path length normal distribution */
void calc_dist(int len,int max)
{ float cc,mm,kk,ss,x1,x2,xx,dd,sum=(float)0.0,aa;
  int ff,bb,nn,no,iy,ix=1;
  dist_table[len][0]=(int)max;
  x1=(float)len; x2=(float)max; nn=(int)x1;
  x1-=(float)0.5; x2+=(float)0.5;
  mm=(x1+x2)/(float)2.0;
  dd=(float)0.02; ff=50;
  if(len<1) goto skip;
  ss=(float)len/com_sigma_coeff;  /* approximation for sigma */
  kk=(float)2.0*ss*ss;
  cc=ss*(float)RT2PIE;
  for(xx=x1;xx<x2;xx+=dd) sum+=Gauss(xx,kk,mm,cc);
  xx=x1;
  while(ix<DIST_H)
    {iy=ff; aa=(float)0.0;
     while(iy--) {aa+=Gauss(xx,kk,mm,cc); xx+=dd;}
     iy=(int)(aa*(float)DIST_D/sum);
     if(iy<1) iy=1;
     if(xx+dd>mm) iy=DIST_H;
     while(iy--&&ix<DIST_H) dist_table[len][ix++]=(int)nn;
     ++nn;
    } skip:
  nn=(int)dist_table[len][DIST_M]; dist_table[len][DIST_H]=(int)nn;
  ix=DIST_H; iy=DIST_H+1; no=nn; bb=nn;
  if(bb<max) {if(len%2) ++bb; else if(max%2) ++bb;}
  while(iy<DIST_Y)
    {nn=(int)dist_table[len][ix];
     if(nn!=no) {if(bb<max) bb++; no=nn;}
     dist_table[len][iy]=(int)bb;
     ++iy; --ix;
    }
  for(iy=1;iy<DIST_Y;++iy) {if(dist_table[len][iy]) --dist_table[len][iy];}
}

/* Allocate mem for comm channel: pick random vals for path len.
  sigma=SL/coeff, S=packet size=1, L=min path len, M=max len.
  range=6sigma: L..M, [L>=com_min,M<=com_max]. Formula: M=L+6*L/coeff.
*/
void com_mem(packet_in_synapse *dp)
{ pulse *apt; int L,M;
  float vary=(float)1.0+(float)6.0/com_sigma_coeff;
  /* work out max L for current com_sigma_coeff */
  L=(int)((float)com_max/vary); if(L<1) L=1;
  /* work out new path min */
  L=1+L-com_min; L=com_min+(int)rnd(L)-1;
  /* now get the new path max for this min len */
  M=(int)(L*vary); /* L + 6 sigma */
  dp->len=(int)L; dp->max=(int)M;
  if(!dist_table[L][0]) calc_dist(L,M);
  ++M; /* plus one for terminal PKT_EMPTY slot */
  if(!(apt=(pulse *)malloc(sizeof(pulse)*M)))
    {printf("no space\n"); w_exit(0);}
  dp->delay=apt;
  while(M--) (*apt++)=PKT_EMPTY;
}

/* free memory used for comms delay */
void com_free(packet_in_synapse *dp)
{ if(dp->len) {free(dp->delay); dp->delay=NULL;}
}

typedef struct 
{	int m_size;
	int m_users;
	int m_locked;
	int m_untouched;
} PktData;

PktData **PacketTable=NULL;
int PacketTableSize=20000; /* number of comm cells i.e. L * num_net_conxs */
int PacketMaxHds=0;
#define PacketIsFree(p) (!((p)->m_locked) && !((p)->m_users) && !((p)->m_untouched))
#define PacketInit(p) {(p)->m_users=0; (p)->m_locked=1; (p)->m_untouched=1;}

PktHnd PacketAlloc(int nsize)
{	PktHnd hd; PktData *p;
	
	if(!PacketTable) 
	{	PacketTable=(PktData **)malloc(sizeof(PktData *)*PacketTableSize);
		for(hd=0;hd<PacketTableSize;++hd) PacketTable[hd]=NULL;
	}

	for(hd=1;hd<=PacketMaxHds;++hd)
	{	p=PacketTable[hd];
		if(p && (p->m_size==nsize) && PacketIsFree(p)) 
		{	PacketInit(p);
			return hd;
		}
	}

	for(hd=1;hd<PacketTableSize;++hd)
	{	if(!PacketTable[hd]) 
		{	if(hd>PacketMaxHds) ++PacketMaxHds;
			p=(PktData *)malloc(sizeof(PktData)+nsize);
			PacketTable[hd]=p;
			p->m_size=nsize;
			PacketInit(p); 
			return hd;
		}
	}
	return 0;
}

void PacketFree()
{	if(PacketTable)
	{	PktHnd hd;
		for(hd=1;hd<=PacketMaxHds;hd++) 
			if(PacketTable[hd]) free(PacketTable[hd]);
		free(PacketTable);
		PacketTable=NULL; PacketMaxHds=0;
	}
}

PktHnd PacketValid(PktHnd hd)
{	if(PacketTable && (hd>0) && (hd<=PacketMaxHds) 
	   && PacketTable[hd] && !PacketIsFree(PacketTable[hd])) return hd;
	return 0;
}

void *PacketDataPt(PktHnd hd)
{	PktData *p; PktHnd old=hd;
	if(!(hd=PacketValid(hd))) return NULL;
	p=PacketTable[hd]; ++p;
	return (void *)p;
}

void PacketAccess(PktHnd hd)
{	if(!PacketValid(hd)) return;
	++(PacketTable[hd]->m_users);
	PacketTable[hd]->m_untouched=0;
}

void PacketRelease(PktHnd *phd)
{	PktHnd hd=PacketValid(*phd);
	if(hd) --(PacketTable[hd]->m_users);
	*phd=0;
}

void PacketUnlock(PktHnd *phd)
{	PktHnd hd=PacketValid(*phd);
	if(hd) PacketTable[hd]->m_locked=0;
	*phd=0;
}
/* Get input packet. This models comms delay.
*  Propagate signal down connection channel
*  All inputs are strobed every cycle.
*/
PktHnd PacketGet(NSLNSLnet *net,packet_in_synapse *dp)
{	PktHnd *fp,hd=inx(PktHnd,dp->input); 
	PacketAccess(hd);
	if(com_off||com_max<2) {PacketRelease(&dp->last); dp->last=hd;}
	else
	{	int nn;
		if(!dp->len) com_mem(dp);
		nn=dp->max; fp=dp->delay;
		while(nn--) {*fp=(*(fp+1)); ++fp;} /* last slot always empty */
		nn=(int)dist_table[dp->len][1+(int)(rndf()*(float)DIST_D)];
		fp=dp->delay+nn;
		while(*fp!=PKT_EMPTY) ++fp;
		*fp=hd;
		if(*(fp=dp->delay)!=0) 
		{	PacketRelease(&dp->last); dp->last=(*fp);}
	}
	return dp->last;
}

pulse PulsePacketGet(PktHnd hd)
{	pulse *p=(pulse *)PacketDataPt(hd);
	if(p) return *p;
	return 0;
}

pulse PeekPulse(packet_in_synapse *p)
{	return(PulsePacketGet(p->last));
}

/* ---------------   FACE GUI routines ------------------ */
/* Convert pulse val to angle */
int val2deg(pulse val)
{ long ang=360*(long)val; ang/=(1+MAX_WIDTH);
  return((int)ang);
}

/* draw a feature */
void face_draw_feature(fio_ds *fp,feat_ds *ap,feat_ds *bp)
{ int w=fp->scr_f,x,y,a; long xy; pulse val=ap->output_value;
  w_penchar(w,0);
  if(ap->oldpos.x) /* erase old line */
    {w_position(w,ap->oldpos.x,ap->oldpos.y);
     w_pencolor(w,wnd_white); w_pendown(w,1);
     w_turnto(w,ap->oldang); w_move(-w,ap->fealen);
    }
  if(ap->feapos.x) 
    {x=ap->feapos.x; y=ap->feapos.y;}
  else {x=bp->nxtpos.x; y=bp->nxtpos.y;}
  if(x)
    {a=val2deg(val);
     w_position(w,x,y);
     w_pencolor(w,wnd_black); w_pendown(w,1);
     w_turnto(w,a); w_move(-w,ap->fealen);
     ap->oldang=a; ap->oldpos.x=x; ap->oldpos.y=y;
     xy=w_position(w,(int)-1,(int)-1);
     ap->nxtpos.x=w_x(xy); ap->nxtpos.y=w_y(xy);
    }
}

/* Convert pulse val to ascii, for display purposes */
char potasc(pulse val)
{ if(val<1) return('A');
  val/=(pulse)6; return((char)('B'+val));
}

/* Convert val in [0..1] to ascii */
char intasc(float val)
{ if(val<=0.0) return('A');
  if(val>=1.0) return('Z');
  val*=(float)26.0; return((char)('A'+(int)val));
}

#define NOO_OFS 1 /* wnd y offset for state vals */
#define NOO_WID 16 /* width of state dsp column */
/* State display column offsets, for formatting purposes */
int col_ofs[]={1,10,30,50};
/* Annotations for the various moods */
char *face_var[]={
"Mellow","Happy","Angry","Sad","Squint",
"Laugh","Frown","Cry","Wink","Crying","Winking","Neither"
};

/* Display state of Expression & Emotion nodes */
void face_state(NSLNSLnet *net,void *np,int ntyp) 
{ long num,adjnum; float aa; int ww,ax,by=NOO_OFS,yd,xx,nn;
  fio_ds *fp; feat_ds *lp; expr_ds *op; Production_ds *rp;
  fp= &(dsnp(nsl_handle(NULL,"\\face_io_node"),face_io_ds)->fio);
  ww=fp->scr_s;
  switch(ntyp)
    {case ND_EXP: op=(expr_ds *)np;
       nn=op->size.input; num=op->node.name; adjnum=num-fp->b_feat; break;
     case ND_PRO: rp=(Production_ds *)np;
       nn=(int)rp->state.num_elements; num=rp->node.name; 
	   adjnum=NUM_FEAS+NUM_EXPS; 
	   break;
     case ND_FEA: lp=(feat_ds *)np; num=lp->node.name; 
	   adjnum=num-fp->b_feat; break;
	 case ND_ALT: /* TO DO */
	 default: return;
    }
  if(dsp_flag)
   {xx=(int)adjnum; if(num==fp->b_emto) ++xx;
    yd=1+w_y(w_size(ww,TRUE))-NOO_OFS; ax=col_ofs[xx/yd]; by+=xx%yd;
    w_moveto(-ww,ax,by); w_printf(-ww,"%ld",num);
    switch(ntyp)
      {case ND_EXP: 
		 for(xx=0;xx<nn;++xx)
		     w_printf(-ww,"%c",potasc(PeekPulse(&(op->input[xx].input))));
         w_printf(ww,"%c ",potasc(op->output_value));
		 break;
       case ND_PRO: 
		 for(xx=0;xx<nn;++xx)
	         w_printf(-ww,"%c",intasc(rp->state.p_elements[xx]));
		 break;
       case ND_FEA: 
		 w_printf(ww,"%c ",potasc(lp->output_value));
		 break;
	   case ND_ALT: /* TO DO */
	   default: ;
      }
   }
  ww=fp->scr_v; by=1+(int)adjnum-NUM_FEAS; 

  switch(ntyp)
    {case ND_EXP: 
		aa=((float)op->output_value)/(float)MAX_WIDTH;
		w_position(ww,1,by); w_clear_line(-ww); 
        w_printf(ww,"%s=%g",face_var[by-1],aa);
        break;
     case ND_PRO: 
	   if(num==fp->b_emto) by+=NUM_ESIM;
	   for(xx=0;xx<nn;++xx)
	   {	aa=rp->state.p_elements[xx]; 
            w_position(ww,1,by+xx); w_clear_line(-ww); 
			w_printf(ww,"%s=%g",face_var[by-1+xx],aa);
	   }
       break;
	 case ND_FEA: case ND_ALT: /* TO DO */
     default:;
    }
}

/* Display general information i.e. permissible driver patterns */
void face_info(fio_ds *fp)
{ int w=fp->scr_i;
  w_clear(w);
  w_printf(w,"driver(\"pattern\",x):\n 0=mellow 1=happy 2=angry 3=sad 4=squint");
}

/* Clean the face */
void face_clean(fio_ds *fp,va_list args)
{ BOOL refresh;
  refresh=va_arg(args,BOOL);
  w_clear(fp->scr_f); w_clear(fp->scr_v);
  face_info(fp);
  if(refresh) {w_refresh(fp->scr_f,TRUE); w_refresh(fp->scr_v,TRUE);}
}

BOOL face_setup(NSLNSLnet *net,fio_ds *fp)
{ BOOL dumy=FALSE; int w; char *ap,buff[100];
  sprintf(buff,"%s_",net->ins_name); ap=buff+strlen(buff);
  fp->b_feat=1;
  strcpy(ap,"state");
  fp->scr_s=w_open(buff,TRUE,TRUE,FALSE,TRUE,FALSE,40,8,wnd_black,wnd_white);
  strcpy(ap,"info");
  fp->scr_i=w_open(buff,TRUE,TRUE,FALSE,TRUE,FALSE,40,4,wnd_black,wnd_white);
  strcpy(ap,"values");
  fp->scr_v=w_open(buff,TRUE,TRUE,FALSE,TRUE,FALSE,16,12,wnd_black,wnd_white);
  strcpy(ap,"dface");
  fp->scr_f=w_open(buff,TRUE,TRUE,FALSE,FALSE,FALSE,100,110,wnd_black,wnd_white);
  fp->scr_t=fp->scr_s;
  w_title(fp->scr_f,NIO_TL,NIO_DRAW,FALSE,"Mr Moody");
  w_title(fp->scr_s,NIO_BL,NIO_DRAW,FALSE,"DFace Demo");
  w_title(fp->scr_i,NIO_BL,NIO_DRAW,FALSE,"DFace Info");

  /* reposition windows */
  w=fp->scr_s; if(w>=0) {w_origin(w,770,440); w_resize(w,336,140);}
  w=fp->scr_i; if(w>=0) {w_origin(w,750,290); w_resize(w,336,84);}
  w=fp->scr_v; if(w>=0) {w_origin(w,870,30); w_resize(w,144,182);}
  w=fp->scr_f; if(w>=0) {w_origin(w,640,30); w_resize(w,192,180);}

  face_clean(fp,(va_list)&dumy);
  return(TRUE);
}

/* table holding feature angles for the expressions */
pulse moods[NUM_EXPS][NUM_FEAS]={
{F_MELLOW},
{F_HAPPY},
{F_ANGRY},
{F_SAD},
{F_SQUINT}
};
/* Put expression onto face */
int face_pattern(NSLNSLnet *net,fio_ds *fp,va_list args)
{ int xx,nn; 
  nn=(int)va_arg(args,int);
  for(xx=0;xx<NUM_FEAS;++xx)
    dsx(pulse,fp->b_feat+xx,feat_ds,next)=moods[nn][xx];
  return(0);
}

/* Update face after a cycle or when initialising */
int face_update(NSLNSLnet *net,fio_ds *fp,va_list args)
{ int xx; feat_ds *ap=NULL,*bp;
  sprintf(fp->buff,"%ld",net->time);
  w_title(fp->scr_t,NIO_TR,NIO_DRAW,FALSE,fp->buff);
  for(xx=0;xx<NUM_FEAS;++xx) 
  {	bp=ap; ap=dsnp(fp->b_feat+xx,feat_ds);
	face_draw_feature(fp,ap,bp);
  }
  w_update(fp->scr_t); w_update(fp->scr_f); w_update(fp->scr_s);
  return(0);
}

/* Perform any user driver commands */
int face_user(fio_ds *fp,va_list args)
{ return(0);
}

#include "dface.nsc"
#include "dface.nsd"

int NSLmain(int argc,char *argv[])
{	nsl_mon_wnds();
	nsl_install(&network,"aux_dface");
	/*nsl_driver(&network,NSLIO_SETUP,(char *)0L);*/
	nsl_monitor("printf(\"For demonstration try: call dface;\\n\");");
	return(0);
}
