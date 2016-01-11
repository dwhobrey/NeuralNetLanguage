#include <stdio.h>
#include <math.h>

/* Action Potential Generation, based on R.J.MacGregor's model []
*  b=Sensitivity to potassium conductance
*  c=Threshold sensitivity
*  ek=Resting potassium conductance
*  th0=Initial threshold
*  tth=Time constant for accommodation
*  tmem=Membrane time constant
*  dgk=Decay constant for potassium action
*  dcth=Decay constant for threshold
*  ---- These are workspace variables:
*  gtot=Total conductance into cell
*  dce=Cell membrane decay constant
*  ---- These are the input variables:
*  scn=Step current input
*  ---- These are the main state variables:
*  gk=Potassium conductance
*  e=Cell membrane potential
*  th=Cell threshold
*  s=Spiking variable, indicates if cell has fired
*  p=Cell potential
*/
float step=(float)1.0, ek=(float)-10.0;
float c=(float)0.5,tth=(float)30.0,b=(float)5.0,tgk=(float)3.0;
float th0=(float)10.0,tmem=(float)5.0;
float e=(float)0.0, th, s=(float)0.0, gk=(float)0.0;
float dcth, dgk;
float scn=(float)20.0,p;

void ptnrn10()
{ float dce,gtot;
  gk=gk*dgk+b*s*((float)1.0-dgk);
    gtot=(float)1.0+gk; dce=(float)exp(-gtot*step/tmem);
  e=e*dce+(scn+gk*ek)*((float)1.0-dce)/gtot;
  th=th0+(th-th0)*dcth+c*e*((float)1.0-dcth);
  s=(float)0.0; if(e>=th) s=(float)1.0;
  p=e+s*((float)50.0-e);
}

void main()
{ th=th0;
  for(;;){
    dcth=(float)exp(-step/tth); dgk=(float)exp(-step/tgk);
    ptnrn10();
    printf("step=%g,ek=%g,dcth=%g,dgk=%g,th0=%g\n",step,ek,dcth,dgk,th0);
    printf("tgk=%g,tmem=%g,tth=%g,b=%g,c=%g\n",tgk,tmem,tth,b,c);
    printf("scn=%g,s=%g,p=%g,e=%g,gk=%g,th=%g\n",scn,s,p,e,gk,th);
  }
}