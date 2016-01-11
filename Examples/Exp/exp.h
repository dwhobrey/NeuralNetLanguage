/* NeSeL v2.0 April 1989 */
/* exp.h: Expression evaluator neural net */
pragma header 0
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>
#include <math.h>

#include "nsltype.h"
#include "nslstd.h"
#include "nslios.h"
#include "nslwsio.h"
pragma header -1

#undef NORED /* undef this to model 3-levels of redundancies with comms */

#ifdef NORED /* no reduncancies */
/* Main Parameters for no redundancies & minimal delay */
#define NN_LINES 1 /* number of real noise lines per connection */
#define STAGE_MIN 1 /* min number of elements in a stage */
#define NUM_RECO_STAGE 1 /* num of 1st layer reco atoms sampling noise lines */
#define NUM_OR_STAGE 1 /* num of 1st layer or atoms ...*/
#define NUM_COMP_STAGE 1 /* num of atoms in compensation layer */
#define DEF_NUMOK (float)0.55 /* % coherent inputs to fire dendrite */
#define COM_OFF 1 /* set to 1 to turn comms off */
#define COM_MIN 0 /* minimum comm path length */
#define COM_MAX 0 /* maximum comm length */
#define DLY_CYCLES 3 /* num of cycles per noisy reco/or */
#define DLY_COMM 1 /* delay to allow for comms */
#define WAIT_UNIT 10 /* base num of cycles to wait */
/* bus channel flushing times:*/
#define FLUSH_RD 0 /* upon read request */
#define FLUSH_WT 1 /* upon write */

char *exp_aux_name="aux_exp1";

#else /* default */

/* Main parameters for 3 levels of redundancies & comms max = 10 cycles */
#define NN_LINES 3 /* number of real noise lines per connection */
#define STAGE_MIN 3 /* min number of elements in a stage */
#define NUM_RECO_STAGE 3 /* num of 1st layer reco atoms sampling noise lines */
#define NUM_OR_STAGE 3 /* num of 1st layer or atoms ...*/
#define NUM_COMP_STAGE 3 /* num of atoms in compensation layer */
#define DEF_NUMOK (float)0.55 /* % coherent inputs to fire dendrite */
#define COM_OFF 0 /* set to 1 to turn comms off */
#define COM_MIN 1 /* minimum comm path length */
#define COM_MAX 10 /* maximum comm length */
#define WAIT_UNIT COM_MAX /* base num of cycles to wait */
#define DLY_CYCLES 3 /* num of cycles per noisy reco/or */
#define DLY_COMM (COM_MAX/2) /* delay to allow for comms */
/* bus channel flushing times:*/
#define FLUSH_RD WAIT_UNIT /* upon read request */
#define FLUSH_WT WAIT_UNIT /* upon write */

char *exp_aux_name="aux_exp3";

#endif

/* Common parameter constants */
#define MAX_VAL_STK 10
#define MAX_OPR_STK 8
#define MAX_SYM_STK 20

#define NUM_SYM_LINES 3 /* num of conx lines needed to code a symbol */
#define NUM_LINE_STATES 4
#define MAX_WIDTH 12
#define MAX_NNS NN_LINES /* max number of noise lines per virtual line */
#define STATE_RANGE (MAX_WIDTH/NUM_LINE_STATES)
#define STATE_SIGMA ((float)STATE_RANGE/(float)2.0)
#define STATE_DEV 1 /* int(STATE_DEV) */
#define STATE_BASE 2  /* STATE_DEV+1 */
#define STATE_IDLE 0  /* means no message */
#define STATE_TINY (STATE_BASE)
#define STATE_TINYB (STATE_TINY-STATE_DEV)
#define STATE_LOW (STATE_TINY+STATE_RANGE)
#define STATE_LOWB (STATE_LOW-STATE_DEV)
#define STATE_MED (STATE_LOW+STATE_RANGE)
#define STATE_MEDB (STATE_MED-STATE_DEV)
#define STATE_HI (STATE_MED+STATE_RANGE)
#define STATE_HIB (STATE_HI-STATE_DEV)
#define STATE_MAX (STATE_HI+STATE_RANGE-STATE_DEV-1)

#define DEND_OK (STATE_IDLE+1) /* lowest pot, including idle, for cohort */
#define DEND_COHORT (STATE_TINYB+1) /* lowest valid pot for cohort */
#define DEF_POT STATE_IDLE /* idling potential */
#define DEF_IP STATE_IDLE /* initial input pot */
#define DEF_REF 0 /* initial refractory period */
#define DEF_REFMAX 1 /* number of cycles to stay dormant */
#define DEF_ROHMAX WAIT_UNIT /* refract on HI dormant period */
#define DEF_OUTREFMAX 1 /* refractory period after output */
#define DEF_OUTHOLMAX 1 /* number of times to send pulse */
#define DEF_HOLD 0 /* initial hold period */
#define DEF_HOLMAX (4*WAIT_UNIT) /* number of cycles to hold input */
#define DEF_INHMAX WAIT_UNIT
#define DEF_TAP 0 /* initial input pulse */
#define DEF_SPARK 0 /* initial spark period */
#define DEF_SPAMAX (4*WAIT_UNIT) /* num of cycles to hold coherent input */
#define DEF_BUFMAX (4*WAIT_UNIT) /* number of cycles to hold a bus value */
#define DEF_THRES STATE_HIB /* firing threshold */
#define DEF_RANGE (STATE_RANGE-1) /* firing range */
#define DEF_LOCAL FALSE /* conxs are non-local by default */
#define DEF_INHIBIT FALSE /* synapses are excitatory */
#define DEF_INVERSE FALSE /* do not invert potential firing range */
#define DEF_REFRACT TRUE /* refract on valid HI inputs */

/* Symbol signal values */
#define SYM_NULL 0  /*000 non symbol */
#define SYM_END 1   /*001*/
#define SYM_LEFT 2  /*002*/
#define SYM_RIGHT 3 /*003*/
#define SYM_ADD 4   /*010*/
#define SYM_SUB 5   /*011*/
#define SYM_MULT 6  /*012*/
#define SYM_DIV 7   /*013*/
#define SYM_SEP 8   /*020*/  /*Note, values >=020 are treated as `other' */
#define SYM_0 9     /*021*/  /*in the do_symbol case switch */
#define SYM_1 10    /*022*/
#define SYM_2 11    /*023*/
#define SYM_3 12    /*030*/
#define SYM_4 13    /*031*/
#define SYM_5 14    /*032*/
#define SYM_6 15    /*033*/
#define SYM_7 16    /*100*/
#define SYM_8 17    /*101*/
#define SYM_9 18    /*102*/

/* ASCII symbols recognised in initial expression */
char *exp_syms="()+-*/$0123456789";

/* Symbol line codes */
#define END_SYM    STATE_TINYB,STATE_TINYB,STATE_LOWB
#define LEFT_SYM   STATE_TINYB,STATE_TINYB,STATE_MEDB
#define RIGHT_SYM  STATE_TINYB,STATE_TINYB,STATE_HIB
#define ADD_SYM  STATE_TINYB,STATE_LOWB,STATE_TINYB
#define SUB_SYM  STATE_TINYB,STATE_LOWB,STATE_LOWB
#define MUL_SYM  STATE_TINYB,STATE_LOWB,STATE_MEDB
#define DIV_SYM  STATE_TINYB,STATE_LOWB,STATE_HIB
#define SEP_SYM  STATE_TINYB,STATE_MEDB,STATE_TINYB
#define SEP_SYM_M  STATE_TINY,STATE_MED,STATE_TINY
#define END_SYM_M  STATE_TINY,STATE_TINY,STATE_LOW

#define rep(a,b) lrand(a,a,b,b)  /* a list of <b>'s repeated <a> times */
#define rcon rep(NUM_SYM_LINES,NN_LINES) /* a list of NN_LINES */
#define rout rep(NN_LINES,0) /* a not very random list of 0's */
#define rin 0..(NN_LINES-1)
#define dup(a) lmap(1,lrand,rcon,rcon,[a],[a])
/* Note: Noisy or's & reco's have to have a fixed conx pattern in this model
*  when NUM_??_STAGE < 6 so that cohort will work correctly: */
#define noisyor 0..(NUM_OR_STAGE-1)
#define noisyre 0..(NUM_RECO_STAGE-1)
/* When NUM_??_STAGE > 6 they can have a random conx:
#define noisyor lrand(STAGE_MIN,NUM_OR_STAGE,0,NUM_OR_STAGE-1)
#define noisyre lrand(STAGE_MIN,NUM_RECO_STAGE,0,NUM_RECO_STAGE-1)
*/

/* Because this is a pseudo multivalued logic,
*  `excluded-middle' doesn't apply, so...
*    (not STATE_HI) == (STATE_TINY||STATE_LOW||STATE_MED).
*  Therefore, this sets an input to recognize (not STATE_HI): */
#define inverse ../isinverse=TRUE
#define inhibit ../isinhibit=TRUE,../threshold=STATE_HIB,\
../range=(STATE_MAX-STATE_HIB)
#define local islocal=TRUE /* this indicates a local comms connection */
/* some nodes need to repond for any size signal e.g. delay lines */
#define sensi_noroh {threshold=STATE_TINYB,range=(STATE_MAX-STATE_TINYB),\
isrefract=FALSE}
#define or_noroh {threshold=STATE_HIB,range=(STATE_MAX-STATE_HIB),\
isrefract=FALSE}

#define NT_OR 1
#define NT_RECO 2
#define NT_LATCH 3

int ewnd=0; /* Output wnd handle */
int dsp_flag=0,num_nd=0;

int com_off=COM_OFF,com_min=COM_MIN,com_max=COM_MAX;
int refmax=DEF_REFMAX,rohmax=DEF_ROHMAX,
      holmax=DEF_HOLMAX,inhmax=DEF_INHMAX,
      outrefmax=DEF_OUTREFMAX,outholmax=DEF_OUTHOLMAX,
      spamax=DEF_SPAMAX,bufmax=DEF_BUFMAX,
      flushrd=FLUSH_RD,flushwt=FLUSH_WT;
/* a noise level of 1.0 implies pulse level may deviate by up to STATE_DEV */
float noise_level=(float)0.35; /* prob of mean pulse deviation */
float num_ok=DEF_NUMOK;
double sigma_coeff=(double)12.0;

typedef int symbol;
typedef int pulse;

node
{ int potential,pothold,hold;
  BOOL islocal;
  int len,max;
  pulse tap,*delay,*input;
} pulse_in_synapse={DEF_IP,DEF_IP,DEF_HOLD,DEF_LOCAL,0,0,DEF_TAP,0L};

node
{ int potential,refract,hold;
  pulse output;
} pulse_out_synapse={DEF_POT,DEF_REF,DEF_HOLD,STATE_IDLE};

node
{ int threshold,range,potential,refract,spark;
  BOOL isinverse,isinhibit,isrefract;
  pulse_in_synapse input[];
} dendrite={DEF_THRES,DEF_RANGE,DEF_IP,DEF_REF,DEF_SPARK,
            DEF_INVERSE,DEF_INHIBIT,DEF_REFRACT};

node
{ int bufimer;
  pulse latch[NUM_SYM_LINES];
  dendrite input[NUM_SYM_LINES];
} dend_tree={0,latch={0},input={sensi_noroh}};

node
{ pulse output,*queue; int delay;
  pulse_in_synapse input;
} delay_ds={STATE_IDLE,(int *)0L,DLY_CYCLES};

node
{ BOOL saturate;
  pulse_out_synapse output;
  dendrite input[];
} or_ds={FALSE,input={or_noroh}};

node
{ pulse_out_synapse output;
  dendrite input[];
} reco_ds={input={{isrefract=FALSE}}};

node
{ BOOL pending;
  pulse_out_synapse pushout,popout,output[NUM_SYM_LINES];
  dendrite enable[],push,pop;
  dend_tree input[];
} stack_top_ds={FALSE,output/potential=(int [])[END_SYM_M]};

node /* all these vars are for one stack cell! */
{ int pause,bufimer1,bufimer2;
  pulse latch[NUM_SYM_LINES],latch1[NUM_SYM_LINES],latch2[NUM_SYM_LINES];
  pulse_out_synapse pushout,popout,done,output[NUM_SYM_LINES];
  dendrite push,pop,head[NUM_SYM_LINES],tail[NUM_SYM_LINES];
} stack_ds={0,0,0,latch={0},latch1={0},latch2={0},
    output/potential=(int [])[END_SYM_M],
    head={sensi_noroh},tail={sensi_noroh}};

node
{ pulse_out_synapse output[NUM_SYM_LINES];
} stack_null_ds={output/potential=(int [])[END_SYM_M]};

node
{ int number,units; int bufimer;
  pulse_out_synapse done;
  dendrite clear,trigger,input[NUM_SYM_LINES];
} comp_num_ds={0,0,0,input={sensi_noroh}};

node
{ int pause;
  int *number;
  pulse_out_synapse strobe,done,output[NUM_SYM_LINES];
  dendrite clear,trigger;
  symbol *s,stk[20];
} comp_str_ds={0,s=(int *)0L,stk={0}};

node
{ int number,*num_one,*num_two;
  pulse_out_synapse done;
  dendrite input[];
} operate_ds={0};

node
{ BOOL last;
  pulse_out_synapse strobe,done,output[NUM_SYM_LINES];
  dendrite clear,trigger;
  char *s,stk[40];
} setup_ds={FALSE,s=(char *)0L,stk={'\0'}};

node
{ BOOL last;
  int bufimer;
  pulse_out_synapse done,strobe;
  dendrite clear,trigger,input[NUM_SYM_LINES];
  char *s,stk[40];
} result_ds={FALSE,0,done/output=STATE_HI,s=(char *)0L,stk={'\0'},
   input={sensi_noroh}};

node
{ pulse_out_synapse output;
  dendrite input,clear;
} latch_ds;

node
{ int output,*input;
  pulse_out_synapse done;
  dendrite trigger;
} lat_num_ds={0};

node
{ pulse_out_synapse output[NUM_SYM_LINES];
} sep_ds={output/potential=(int [])[SEP_SYM_M]};

node
{ long sym,val,opr;
} nio_ds={0,0,0};

node
{ nio_ds nio;
  NSLNSLio args;
} expio_ds;

pulse p_end_sym[]={END_SYM_M};
