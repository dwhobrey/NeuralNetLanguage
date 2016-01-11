#include <stdio.h>
#include <math.h>
#include <varargs.h>
#include "nsltype.h"
#include "nslstd.h"
#include "nslios.h"

#define NUM_SYM_LINES 3
#define NUM_LINE_STATES 4
#define MAX_WIDTH 12
#define STATE_RANGE (MAX_WIDTH/NUM_LINE_STATES)
#define STATE_SIGMA ((float)STATE_RANGE/(float)2.0)
#define STATE_DEV 1 /* int(STATE_DEV) */
#define STATE_BASE 2  /* STATE_DEV+1 */
#define STATE_IDLE 0  /* means no message */

#define NUM_INPUTS 10

float sg=(float)12.0,sc=(float)-0.60;
int num_inp=NUM_INPUTS;

typedef int pulse;

typedef struct
{ float err,state;
  pulse output;
  pulse input[NUM_INPUTS];
  pulse mem[NUM_INPUTS];
  BOOL not[NUM_INPUTS];
} reco_ds;

float ssg=(float)12.0,ssc=(float)-1.1;
float elim=(float)1.0;

void recognizer(rp) reco_ds *rp;
{ float dd,mm; int xx,nn=num_inp; pulse val;
  rp->err=(float)0.0; rp->state=(float)0.0;
  for(xx=0;xx<nn;++xx)
     {val=rp->input[xx];
      if(!val) {rp->output=STATE_IDLE; return;}
      mm=(float)rp->mem[xx];
      dd=((float)val)-mm;
      if(rp->not[xx])
        {if(dd<(float)0.0) dd=(-dd); dd=mm-dd; dd/=(mm-STATE_SIGMA);}
      else dd/=STATE_SIGMA;
      dd*=dd;
      dd=generic_sigmoid(dd,(float)1.0,(float)0.0,ssg,ssc);
      rp->err+=dd;
     }
  rp->err/=(float)elim;
  rp->state=(float)1.0-generic_sigmoid(rp->err,(float)1.0,(float)0.0,sg,sc);
  rp->output=(pulse)1+(pulse)((float)(MAX_WIDTH-1)*rp->state);
}

reco_ds re={(float)0.0,(float)0.0,0,{11,11,1,1,1,1,1,1,1,1},
  {11,11,11,11,11,11,11,11,11,11},{0,1,1,1,1,1,1,1,1,1}};
void main()
{ char *ap,chr,buff[100]; int xx,a[NUM_INPUTS];
  num_inp=8;
  do{printf("Enter (e/f/s) or inputs, mems, nots: i/m/n,a,b,c..>");
     gets(buff); ap=buff; chr=(*ap); ++ap; xx=0;
     while(*ap&&*ap==',')
       {sscanf(ap,",%d",&a[xx]); ++xx; ++ap; while(*ap&&*ap!=',') ++ap;}
     printf("Got:%c",chr);
     if(chr=='e') break;
     if(chr=='f') sscanf(buff,"%c,%f,%f",&chr,&sg,&sc);
     else if(chr=='s') sscanf(buff,"%c,%f,%f",&chr,&ssg,&ssc);
     else
       {if(chr=='n') num_inp=xx;
        while(xx--)
          {if(chr=='m') re.mem[xx]=a[xx];
           else if(chr=='n') re.not[xx]=a[xx];
           else re.input[xx]=a[xx];
          }
       }
     printf("\n");
     recognizer(&re);
     printf("err=%f,state=%f,out=%d\n",re.err,re.state,re.output);
     printf("sg=%f,sc=%f\n",sg,sc);
    }while(1);
}