/* NeSeL parameters for 2x32 field bam net */
#define NUM_PAS 4 /* number of partitions */
#define NUM_NOS 8 /* number of nodes per partition */
#define NUM_FLD (NUM_PAS*NUM_NOS) /* field size */
#define NET_DIM 9 /* approx 1+sqrt(2*NUM_FLD) */
/* NOTE these patterns must be same */
#define PATS "(float)[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
                      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
                      1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,\
                      0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1],\
              (float)[0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,\
                      1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,\
                      1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,\
                      1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0]"
int patsa[][NUM_FLD]=
  {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
   },
   {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0
   }};
int patsb[][NUM_FLD]=
  {{1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
    0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1
   },
   {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
    1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0
   }};

int num_pats=2;
int base_len=0,
    base_age=6, /* num of cycles ouputs must remain stable to settle */
    base_flush=NET_DIM, /* num cycles between pats to flush comms */
    base_stoch=0, /* when nonzero enables stochastic convergence */
    base_nocomms=0; /* when nonzero disables comms */
long base_cycles=100; /* num cycles before a break is forced */
double base_sigma=(double)12, /* comm path length parameters */
    base_rate=(double)1.0; /* for stochastic model, rate of updating */
    /* 0.0=never updated, 0.5=50% of time, 1.0=always updated, etc */
