/** NeSeL v1.0 April 1989 **/
/* BAM Stats gathering program */
#include <math.h>
#include <stdio.h>

#define buffsize 250
#define NUM_SAMPLES 200
#define SAMS (NUM_SAMPLES+2)
#define BARS 20
#define CYCS 150
#define MAX_PL 100
#define MAX_PT 10

int hamlo,hamhi,cychi,cyclo;

FILE *infp,*myfp;
int sam=0,aalowcyc=0,aahicyc=0,bblowcyc=0,bbhicyc=0,max_path=0,max_pro=3;
double avpl,plstd,hoa[SAMS],hpb[SAMS],hoc[SAMS];
double meanwith[BARS+2],meanhd[BARS+2];
int noa[SAMS],npb[SAMS],noc[SAMS],cycwith[SAMS],cycnone[SAMS],
  idx[SAMS],pro[MAX_PT]= {0};
int distwith[CYCS]={0},distnone[CYCS]={0};
long paths[MAX_PL] = {0};
char buff[buffsize];

void upd_paths()
{ long xfer; max_path=0;
  while(fgets(buff,buffsize,infp))
    {sscanf(buff,"%ld",&xfer);
     paths[max_path]=xfer;
     ++max_path;
    }
  while(max_path&&!paths[max_path-1]) --max_path;
}

/* put new data in arrays using ham dist pb as key */
void upd_arrays(oa,pb,oc,ka,kb,kc,ca,cb)
  double oa,pb,oc; int ka,kb,kc,ca,cb;
{ int nn,kk; double *p=hpb;
  for(nn=0;nn<sam;++nn) {if(*p>pb) break; ++p;}
  kk=sam;
  while(kk>nn)
    {hoa[kk]=hoa[kk-1]; hpb[kk]=hpb[kk-1]; hoc[kk]=hoc[kk-1];
     noa[kk]=noa[kk-1]; npb[kk]=npb[kk-1]; noc[kk]=noc[kk-1];
     idx[kk]=idx[kk-1];
     cycwith[kk]=cycwith[kk-1]; cycnone[kk]=cycnone[kk-1];
     --kk;
    }
  hoa[nn]=oa; hpb[nn]=pb; hoc[nn]=oc;
  noa[nn]=ka; npb[nn]=kb; noc[nn]=kc;
  cycwith[nn]=ca; cycnone[nn]=cb;
  ++sam;
  idx[nn]=sam;
}
double vtof(v) int v;
{ double d=(double)v*(double)100.0/(double)sam;
  return(d);
}

int vtop(v) int v;
{ return((int)vtof(v));
}


void main(argc,argv) int argc; char *argv[];
{ int gon=0,nn,ca,cb,ka,kb,kc,kd,ke,fixwith=0,fixnone=0,
    cycflu=100,cyclim=100,sucwith=0,sucnone=0,succomp=0,plmax=0;
  long xf,xfermax; char tchr,fchr; int net_dim,b_l,d_s;
  double da,db,dc,dd,val,plsig,b_s,
    sigmawith=(double)0.0,sigmanone=(double)0.0,
    sumwith=(double)0.0,sumnone=(double)0.0;
  char *stub,name[50];
  stub=argv[1]; sprintf(name,"%s.dat",stub);
  if(!(infp=fopen(name,"r"))) return;
  if(argv[2]) gon=1;
  fgets(buff,buffsize,infp);
  sscanf(buff,"%d,%d",&cycflu,&cyclim);
  /* get data */
  while(fgets(buff,buffsize,infp))
    {if(*buff=='%') break;
     /*statnum,cycwith,hamout,hamprob,outnum,probnum,cycnone,hamnone,nonenum*/
     sscanf(buff,"%d,%d,%lf,%lf,%d,%d,%d,%lf,%d",
           &ke,&ca,&da,&db,&ka,&kb,&cb,&dc,&kc);
     if(ca<cyclim) ++pro[1+ka]; else ++pro[0];
     if(ka>max_pro) max_pro=ka;
     upd_arrays(da,db,dc,ka,kb,kc,ca,cb);
     if(ca<aalowcyc||aalowcyc==0) aalowcyc=ca;
     if(cb<bblowcyc||bblowcyc==0) bblowcyc=cb;
     if(ca>aahicyc) aahicyc=ca;
     if(cb>bbhicyc) bbhicyc=cb;
    }
  upd_paths();
  fclose(infp);
  for(nn=0;nn<sam;++nn)
    {if(cycwith[nn]<cyclim)
       {sumwith+=(double)cycwith[nn]; ++fixwith;
        if(noa[nn]==npb[nn]) {++sucwith; ++succomp;}
        else if(noa[nn]==noc[nn]) ++succomp;
       }
     if(cycnone[nn]<cyclim)
       {sumnone+=(double)cycnone[nn]; ++fixnone;
        if(noc[nn]==npb[nn]) ++sucnone;
       }
     ++distwith[cycwith[nn]-aalowcyc];
     ++distnone[cycnone[nn]-bblowcyc];
    }
  hamlo=(int)hpb[0]; hamhi=1+(int)hpb[sam-1];
  cyclo=aalowcyc; cychi=aahicyc;
  if(cyclo>bblowcyc) cyclo=bblowcyc;
  if(cychi<bbhicyc) cychi=bbhicyc;
  ++cychi; --cyclo;
  /* workout average cycles to fixpoint +- std */
  sumwith/=(double)fixwith; sumnone/=(double)fixnone;
  for(nn=0;nn<sam;++nn)
    {val=(double)cycwith[nn];
      if(val<(double)cyclim)
       {val-=sumwith; val*=val; sigmawith+=val;}
     val=(double)cycnone[nn];
      if(val<(double)cyclim)
       {val-=sumnone; val*=val; sigmanone+=val;}
    }
  sigmawith/=(double)((double)fixwith-(double)1.0);
  sigmawith=sqrt(sigmawith);
  sigmanone/=(double)((double)fixnone-(double)1.0);
  sigmanone=sqrt(sigmanone);
  db=(double)0.0; dd=(double)0.0;
  for(nn=0;nn<max_path;++nn) {if(paths[nn]) break;}
  cb=nn;
  if(nn>0)
  {while(nn<max_path)
     {da=(double)paths[nn]; dd+=da; ++nn;
      da*=(double)nn;
      db+=da;
     }
   dc=db/dd; /* average path len */
   db=(double)0.0;
   for(nn=cb;nn<max_path;++nn)
     {da=(double)(1+nn);
      da-=dc; da*=da; da*=(double)paths[nn];
      db+=da;
     }
   db/=(double)(dd-1); da=sqrt(db); db/=dd;
   plsig=sqrt(db); avpl=dc; plstd=da;
   xfermax=0; nn=cb;
   while(nn<max_path)
     {xf=(long)paths[nn]; ++nn;
      if(xf>xfermax) xfermax=xf;
     }
   plmax=nn;
  }
  else {plsig=(double)0.0; avpl=(double)0.0; plstd=(double)0.0; xfermax=0;}
  if(!(myfp=fopen("data.sum","a"))) return;
  sscanf(stub,"%c%d%*c%d%c%d",&tchr,&net_dim,&d_s,&fchr,&b_l);
  if(fchr=='f'||fchr=='F') {b_s=(double)d_s+(double)b_l/(double)10.0; b_l=0;}
  else b_s=(double)d_s;
  fprintf(myfp,"%d & %.2f,%.2f,%.2f & %.2f,%.2f &",net_dim,
    vtof(succomp),vtof(sucwith),vtof(sucnone),vtof(fixwith),vtof(fixnone));
  for(nn=0;nn<=max_pro;++nn) fprintf(myfp,"%d,",vtop(pro[nn]));
  fprintf(myfp,"%d &",vtop(pro[1+max_pro]));
  fprintf(myfp," %.2f$\\pm$%.2f &",sumwith,sigmawith);
  fprintf(myfp," %.2f$\\pm$%.2f &",sumnone,sigmanone);
  fprintf(myfp," %.2f$\\pm$%.2f & ",avpl,plstd);
  if(b_l<1) b_l=0;
  if(tchr=='b'||tchr=='B'||tchr=='a'||tchr=='A') tchr='N'; else tchr='Y';
  fprintf(myfp," %d,%g,%c,%d,%d\\\\\n",b_l,b_s,tchr,cycflu,cyclim);
  fclose(myfp);
  if(!gon) return;
  sprintf(name,"%s.h",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Num samples: %d\n",sam);
  fprintf(myfp,"#Num success, with comms: %d\n",sucwith);
  fprintf(myfp,"#Num success, no comms: %d\n",sucnone);
  fprintf(myfp,"#Num comparative success: %d\n",succomp);
  fprintf(myfp,"#Num fixed point, with comms: %d\n",fixwith);
  fprintf(myfp,"#Num fixed point, no comms: %d\n",fixnone);
  fprintf(myfp,"#Av cyc, with comms: %g +-%g\n",sumwith,sigmawith);
  fprintf(myfp,"#Av cyc, no comms: %g +-%g\n",sumnone,sigmanone);
  fprintf(myfp,"#Num flush cycs:%d, cycle cut-off:%d\n",cycflu,cyclim);
  fprintf(myfp,"#Av path len: %g +-%g, sigma=%g\n ",avpl,plstd,plsig);
  fclose(myfp);
  sprintf(name,"%s.a",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Cycles to convergence vs ham dist, with comms\n");
  fprintf(myfp,"#\"With comms\"\n");
  for(nn=0;nn<sam;++nn)
    {if(cycwith[nn]<cyclim)
       fprintf(myfp,"%g %d  #%d\n",hpb[nn],cycwith[nn],idx[nn]);
    }
  fclose(myfp);
  sprintf(name,"%s.b",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Cycles to convergence vs ham dist, no comms\n");
  fprintf(myfp,"#\"No comms\"\n");
  for(nn=0;nn<sam;++nn)
    {if(cycnone[nn]<cyclim)
       fprintf(myfp,"%g %d\n",hpb[nn],cycnone[nn]);
    }
  fclose(myfp);
  sprintf(name,"%s.c",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Success on closest prototype vs ham dist, with comms\n");
  fprintf(myfp,"#\"With comms\"\n");
  da=(double)0.001+hpb[sam-1]-hpb[0]; kb=0; kc=0; kd=0; ke=0;
  for(nn=0;nn<sam;++nn)
    {ca=0;
     ka=(int)((double)(BARS)*(hpb[nn]-hpb[0])/da);
     if(ka!=kb)
       {if(kd!=0) meanwith[ke]=(double)kc/(double)kd;
        else meanwith[ke]=(double)0.0;
        meanhd[ke]=hpb[nn];
        kb=ka; kc=0; kd=0; ++ke;}
     ++kd;
     if(cycwith[nn]<cyclim)
       {if(noa[nn]==npb[nn]) {ca=noc[nn]+1; ++kc;}
       }
     fprintf(myfp,"%g %d\n",hpb[nn],ca);
    }
  if(kd!=0) meanwith[ke]=(double)kc/(double)kd;
  else meanwith[ke]=(double)0.0;
  meanhd[ke]=hpb[nn-1];
  fclose(myfp);
  sprintf(name,"%s.i",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Success density distribution, with comms\n");
  fprintf(myfp,"#\"Density dist, with comms\"\n");
  for(nn=0;nn<=ke;++nn)
    fprintf(myfp,"%g %g\n",meanhd[nn],meanwith[nn]);
  fclose(myfp);
  sprintf(name,"%s.d",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Success on closest prototype vs ham dist, no comms\n");
  fprintf(myfp,"#\"No comms\"\n");
  da=(double)0.001+hpb[sam-1]-hpb[0]; kb=0; kc=0; kd=0; ke=0;
  for(nn=0;nn<sam;++nn)
     {ca=0;
      ka=(int)((double)(BARS)*(hpb[nn]-hpb[0])/da);
      if(ka!=kb)
        {if(kd!=0) meanwith[ke]=(double)kc/(double)kd;
         else meanwith[ke]=(double)0.0;
         meanhd[ke]=hpb[nn];
         kb=ka; kc=0; kd=0; ++ke;}
      ++kd;
      if(cycnone[nn]<cyclim)
        {if(noc[nn]==npb[nn]) {ca=noc[nn]+1; ++kc;}}
      fprintf(myfp,"%g %d\n",hpb[nn],ca);
     }
  if(kd!=0) meanwith[ke]=(double)kc/(double)kd;
  else meanwith[ke]=(double)0.0;
  meanhd[ke]=hpb[nn-1];
  fclose(myfp);
  sprintf(name,"%s.j",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Success density distribution, no comms\n");
  fprintf(myfp,"#\"Density dist, no comms\"\n");
  dc=hpb[0]+da/((double)ka*(double)2.0);
  for(nn=0;nn<=ke;++nn)
    fprintf(myfp,"%g %g\n",meanhd[nn],meanwith[nn]);
  fclose(myfp);
  sprintf(name,"%s.e",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Cycle distribution, with comms\n");
  fprintf(myfp,"#\"With comms\"\n");
  da=(double)sam;
  for(nn=0;nn<=(aahicyc-aalowcyc);++nn)
    {val=(double)distwith[nn];
     if(val==(double)0.0&&nn>1)
       {if(!distwith[nn+1]&&!distwith[nn-1]) continue;}
     fprintf(myfp,"%d %g\n",nn+aalowcyc,val/da);
    }
  fclose(myfp);
  sprintf(name,"%s.f",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Cycle distribution, no comms\n");
  fprintf(myfp,"#\"No comms\"\n");
  for(nn=0;nn<=(bbhicyc-bblowcyc);++nn)
    {val=(double)distnone[nn];
     if(val==(double)0.0&&nn>1)
       {if(!distnone[nn+1]&&!distnone[nn-1]) continue;}
     fprintf(myfp,"%d %g\n",nn+bblowcyc,val/da);
    }
  fclose(myfp);
  sprintf(name,"%s.g",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"#%s\n",stub);
  fprintf(myfp,"#Xfer table: path_len num_xfers\n");
  fprintf(myfp,"#\"Xfers\"\n");
  nn=cb;
  while(nn<max_path)
     {xf=(long)paths[nn]; ++nn;
      fprintf(myfp,"%d %ld\n",nn,xf);
     }
   fclose(myfp);
  sprintf(name,"%s.gab",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,
    "title \"BAM:%s, Cycles To Convergence vs Hamming Distance\"\n",stub);
  fprintf(myfp,"nobars\n");
  fprintf(myfp,"xlab \"Probe Hamming Distance From Prototype\"\n");
  fprintf(myfp,"ylab \"Cycles To Convergence\"\n");
  fprintf(myfp,"set samples 1000\n");
  fprintf(myfp,"plot [%d:%d] \"%s.a\", ply1(x), \"%s.b\", bfit(x)\n",
    hamlo,hamhi,stub,stub);
  fclose(myfp);
  sprintf(name,"%s.gcd",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,
    "title \"BAM:%s, Success On Closest Prototype vs Hamming Distance\"\n",
    stub);
  fprintf(myfp,"nobars\n");
  fprintf(myfp,"xlab \"Probe Hamming Distance From Prototype\"\n");
  fprintf(myfp,"ylab \"Prototype Number, 0=Failed\"\n");
  fprintf(myfp,"plot [%d:%d][-1:5] \"%s.c\", \"%s.d\"\n",hamlo,hamhi,stub,stub);
  fclose(myfp);
  sprintf(name,"%s.gef",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,
    "title \"BAM:%s, Percent Runs vs Cycles To Convergence Distribution\"\n",
    stub);
  fprintf(myfp,"nobars\n");
  fprintf(myfp,"xlab \"Convergence Cycle Time\"\n");
  fprintf(myfp,"ylab \"Percentage Of Runs\"\n");
  fprintf(myfp,"set samples 500\n");
  fprintf(myfp,"plot [%d:%d] \"%s.e\", join(x), \"%s.f\", join(x)\n",
     cyclo,cychi,stub,stub);
  fclose(myfp);
  sprintf(name,"%s.gij",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,
    "title \"BAM:%s, Success On Closest Prototype vs Hamming Distance\"\n",
    stub);
  fprintf(myfp,"nobars\n");
  fprintf(myfp,"xlab \"Probe Hamming Distance From Prototype\"\n");
  fprintf(myfp,"ylab \"Success Density, 0=Failed\"\n");
  fprintf(myfp,"plot [%d:%d][0.0:1.1] \"%s.i\", cur(x), \"%s.j\", cur(x)\n",
    hamlo,hamhi,stub,stub);
  fclose(myfp);
  sprintf(name,"%s.ggg",stub);
  if(!(myfp=fopen(name,"w"))) return;
  fprintf(myfp,"title \"BAM:%s, Number Of Transfers vs Path Length\"\n",
    stub);
  fprintf(myfp,"nobars\n");
  fprintf(myfp,"xlab \"Path Length / Net Nodes\"\n");
  fprintf(myfp,"ylab \"Number Of Transfers\"\n");
  fprintf(myfp,"set samples 500\n");
  fprintf(myfp,"sigma = %g\n",plstd);
  fprintf(myfp,"mm = %g\n",avpl);
  fprintf(myfp,"konst = %ld.0\n",xfermax);
  fprintf(myfp,"gauss(x) = konst * exp(-0.5*(((x-mm)/sigma)**2))\n");

  fprintf(myfp,"plot [0:%d] \"%s.g\", gauss(x)\n",plmax,stub);
  fclose(myfp);
}
