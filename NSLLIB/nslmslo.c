/* Name: nslmslo.c  Type: Small Model Monitor  Date: 30 March 89
*  NeSeL Neural Net Specification Language
*/

#include "nslms.h"

extern void mon_w_puts(char *str); 
char mon_wnd_buff[256];

extern char *nsl_sst_name=NULL;
extern char **nsl_ndfn_list=NULL;
extern NSLNSLnet **nsl_net_list=NULL;

extern FILE *nsl_nalfp=NULL,*nsl_sstfp=NULL,*nsl_confp=NULL,
	*nsl_dsifp=NULL,*nsl_logfp=NULL;

extern foot *nsl_cx_list=NULL,*nsl_ndfn_sst_list=NULL;
extern net_elm *nsl_net_top=NULL;

extern aux_elm *nsl_ax_list=NULL;
extern stub_elm *nsl_stub=NULL;

extern int nsl_num_conxs=0,nsl_cx_num=0,
	nsl_ax_num=0,nsl_cx_max=0,
	nsl_ax_max=0,nsl_ndfn_num=0;

foot nsl_ndfn_bs=0,nsl_ndix_bs=0;
yard nsl_ndix_num=0;

extern ds_sea_elm nsl_sea={0,0,0,0,{0},NULL,NULL};

struct nsl_error_elm {int error_num; char *error_str;}
nsl_error_list[]={
{WRN_STR_UNIT,"uninitialised structure(s) found:%d"},
{WRN_VAR_UNIT,"uninitialised variable(s) found:%d"},
{WRN_VAR_PT,"cannot save pointer(s) in this version:%d"},
{WRN_VAR_UNI,"cannot save union(s) in this version:%d"},
{WRN_NOT_IMP,"Sorry, function '%s' not implemented yet"},
{WRN_DS_STUB,"Address request on stub net '%s', select valid net first?"},
{ERR_MON_INF,"Monitor struct details not found for '%s', update?"},
{ERR_NET_CHK,"internal consistancy of net is suspect"},
{ERR_BADFILE,"unable to open input/output file:%s"},
{ERR_OSPACE,"ran out of memory space:%s(%d)"},
{ERR_FILE,"access to file went wrong"},
{ERR_VERSION,"nsl version number mismatch"},
{EXIT_INT,"exit on interrupt"},
{EXIT_INT_ERR,"internal error?!"},
{ERR_OK,"ok"},  /* same code as EXIT_OK */
{0,NULL}};

void signal_error(int error_no,...)
{ va_list args; char *ept,*spt;
  int nn=0,cur_error=1,ia; mile ma;
  va_start(args,error_no);
  do{if((cur_error=nsl_error_list[nn].error_num)==error_no) break;
	 ++nn;
	}while(cur_error);
  if(cur_error==error_no)
	{ept=nsl_error_list[nn].error_str;
	 if(ept)
		{if(*ept=='@') ++ept;
		if(cur_error>=WRN_BASE)
		  {if(nsl_verbatim) mon_w_puts("warning: ");
		   else fprintf(nsl_logfp,"warning: ");}
		if(nsl_verbatim) 
			{sprintf(mon_wnd_buff,"NM%d: ",error_no); mon_w_puts(mon_wnd_buff);}
		else fprintf(nsl_logfp,"NM%d: ",error_no);
		switch(error_no)
		  {case ERR_BADFILE: case ERR_MON_INF: case WRN_NOT_IMP: case WRN_DS_STUB:
			 spt=va_arg(args, char *);
			 if(nsl_verbatim) {sprintf(mon_wnd_buff,ept,spt);mon_w_puts(mon_wnd_buff);}
			 else fprintf(nsl_logfp,ept,spt);
			 break;
		  case ERR_OSPACE:
			 spt=va_arg(args, char *); ia=va_arg(args,int);
			 if(nsl_verbatim) {sprintf(mon_wnd_buff,ept,spt,ia);mon_w_puts(mon_wnd_buff);}
			 else fprintf(nsl_logfp,ept,spt,ia);
			 break;
			case WRN_STR_UNIT: case WRN_VAR_UNIT:
			case WRN_VAR_PT: case WRN_VAR_UNI:
			 ma=va_arg(args,mile);
			 if(nsl_verbatim) {sprintf(mon_wnd_buff,ept,ma);mon_w_puts(mon_wnd_buff);}
			 else fprintf(nsl_logfp,ept,ma);
			 break;
			default: if(nsl_verbatim) mon_w_puts(ept); else fprintf(nsl_logfp,ept);
		  }
		 if(nsl_verbatim) mon_w_puts("\n"); else fputc('\n',nsl_logfp);
		}
	}
 else
	{cur_error=EXIT_ERRERR;
		 if(nsl_verbatim) 
		 {sprintf(mon_wnd_buff,"Unknow_n error number:%d?\n",error_no);mon_w_puts(mon_wnd_buff);}
		 else fprintf(nsl_logfp,"Unknown error number:%d?\n",error_no);
	}
 if(cur_error<ERR_BASE)
	{if(cur_error>EXIT_OK)
	  {if(!nsl_verbatim)
	     {fprintf(nsl_logfp,"Can't continue. Bye!\n"); exit(cur_error);}
	  }
	}
 va_end(args);
}

/* Save string, if enough mem */
char *strsave(char *s)
{ char *p;
  if((p=malmem(char,1+strlen(s)))!=NULL) strcpy(p,s);
  return(p);
}
/* save block of mem at s & length len to a mem list */
char *memsave(s,len) char *s; int len;
{ char *p,*a,*c;
  if((p=(char *)lnew(len,TYPE_CHAR))!=NULL)
    {a=p; c=p; while(len--) {*a=(*p); ++a; ++p;} return(c);}
  return(NULL);
}
char *memlist(bp) mile *bp;
{ return((char *)lcopy(bp+LST_SYS,NULL));
}

char *fcat(char *a,char *b)
{ if(!a) signal_error(EXIT_INT_ERR);
  if(b) while(*b) *(a)++= *(b)++; *(a)='\0';
  return(a);
}

/* Find name str in sorted list of names.
*  Returns pos+1, or -(closest pos) if not found */
int find_name(name,lpt,hi,siz) char *name,*lpt; int hi,siz;
{ int lo=0,test=0,oldtest,temp;
  if(!name||hi<1) return(0);
  do {oldtest=test; test=(hi+lo)/2;
	  if((temp=strcmp(name,*((char **)(lpt+test*siz))))==0) return(test+1);
	if(temp<0) hi=test; else lo=test;
  } while(test!=oldtest);
  return(-hi);
}

NSLNSLstruct *find_gen_tag(name,ipt,spt,hi)
  char *name; int *ipt; NSLNSLstruct *spt; int hi;
{ int lo=0,test=0,pos,oldtest,temp;
  if(hi<1||!ipt||!spt||!name) return((NSLNSLstruct *)NULL);
  do{oldtest=test; test=(hi+lo)/2;
	 pos=ipt[test];
	 if((temp=strcmp(name,spt[pos].tag))==0) return(spt+pos);
	 if(temp<0) hi=test; else lo=test;
	}while(test!=oldtest);
  return((NSLNSLstruct *)NULL);
}
/* Find tag name str in nsl_XXX_structs lists of tags.
*  Returns pt, or null if not found (err if not found) */
NSLNSLstruct *find_tag(name)
  char *name;
{ NSLNSLstruct *spt; NSLNSLnet **npt; int nn;
  if(!name) return((NSLNSLstruct *)NULL);
  if(nsl_net_list)
	{npt=nsl_net_list; nn=lst_num(npt);
	 while(nn--)
		{if(*npt)
		  {if((*npt)->sixpt)
			 {if((spt=find_gen_tag(name,(*npt)->sixpt,(*npt)->spt,
							 *((*npt)->num_six)))) return(spt);
			 }
		  }
		++npt;
		}
	}
 if((spt=find_gen_tag(name,nsl_mon_stindx,nsl_mon_structs,
					 nsl_num_mon_stindx))) return(spt);
  signal_error(ERR_MON_INF,name);
  return((NSLNSLstruct *)NULL);
}

/* Enlarge an array */
void enlarge_array(basept,maxpt,numpt,need,dinc,elm_size)
  char **basept; int *maxpt,*numpt,need,dinc; size_t elm_size;
{ if((*numpt+need+2)>=(*maxpt))
	{*maxpt+=need+dinc;
	 *basept=relmem(char,elm_size*(*maxpt),*basept);
	}
  *numpt+=need;
}

char *step_on_cr(apt)
  char *apt;
{ while(*apt) {if(*apt=='\n') {*apt='\0'; break;} ++apt;}
  return(apt);
}
boolean wrong_ver(bpt,id)
  char *bpt,id;
{ char fid='\0'; int vernum=0,verext=0;
  sscanf(bpt,"%*[^(](%c)v%d.%d",&fid,&vernum,&verext);
  if(vernum<2||fid!=id) {signal_error(ERR_VERSION); return(T);}
  return(F);
}
boolean sst_is_bad(netpt) NSLNSLnet *netpt;
{ char buffer[100];
	if(!netpt) return(T);
  if(nsl_sstfp)
	{if(strcmp(nsl_sst_name,netpt->sst_name)==0)
			{rewind(nsl_sstfp); return(F);}
	 fclose(nsl_sstfp);
	}
  chk_free(nsl_sst_name); nsl_sst_name=strsave(netpt->sst_name);
  sprintf(buffer,"%s.nss\0",nsl_sst_name);
  if((nsl_sstfp=fopen(buffer,"r"))==NULL)
	{nul_free(nsl_sst_name); return(T);}
  return(F);
}
/* Files used by nsl_get_net:
*  nalfp <- "progname.nso" -- file from nsl2, net in pseudo object format
*  sstfp <- "(name from nalfp).nss" -- f f nsl1, substantiated modules
*  confp <- "outname.nsm" -- (tmp) file for connection mapping
*  Note if outname.nsm already exists it is overwritten (in case different)
*/
int open_get_files(netpt)
  NSLNSLnet *netpt;
{ int args,nss_ok=0,excode=(-NSLER_BF); yard tmp;
  char *name=netpt->name,buffer[bigbuffsize];
  if(nsl_verbatim) mon_w_puts("Opening get files...\n");
  /*----Open object file of net to read in */
  sprintf(buffer,"%s.nso\0",name);
  if((nsl_nalfp=fopen(buffer,"rb"))==NULL) goto byebye;
  fgets(buffer,bigbuffsize,nsl_nalfp); /* check version */
  if(wrong_ver(buffer,'b')) {excode=(-NSLER_VR); goto byeone;}
  /*----Open connection mapping file */
  sprintf(buffer,"%s.nsm\0",name);
  if((nsl_confp=fopen(buffer,"wb+"))==NULL) goto byeone;
  /*----Open source file of object */
  fgets(buffer,bigbuffsize,nsl_nalfp); /* get nss base file name */
  fprintf(nsl_confp,"NeSeL(c)v2.0(DJRW)1989\n%s",buffer);
  step_on_cr(buffer); netpt->sst_name=strsave(buffer);
  if(sst_is_bad(netpt)) goto byetwo;
  fgets(buffer,bigbuffsize,nsl_sstfp); /* check version */
  if(wrong_ver(buffer,'a')) goto byethree;
  /* now get node indx base & num nodes */
  fget_data(nsl_nalfp,(foot *)&nsl_ndix_bs,sizeof(foot));
  nsl_ndix_bs-=sizeof(foot);
  fget_data(nsl_nalfp,(yard *)&nsl_ndix_num,sizeof(yard));
  /* now get func list adrs & nss ok flags */
  fgets(buffer,bigbuffsize,nsl_sstfp);
  nsl_ndfn_bs=0;
  args=sscanf(buffer,"%ld,%d",&nsl_ndfn_bs,&nss_ok);
  if(args!=2 || nsl_ndfn_bs==0 || nss_ok==0) goto byethree;
  fgets(buffer,bigbuffsize,nsl_sstfp); /* now skip include files */
  sscanf(buffer,"%ld",&tmp);
  while(tmp--) fgets(buffer,bigbuffsize,nsl_sstfp);
  netpt->sst_bs=ftell(nsl_sstfp);
  return(-NSLER_OO);
  byethree: fclose(nsl_sstfp); nsl_sstfp=NULL; nul_free(nsl_sst_name);
  byetwo: fclose(nsl_confp);
  byeone: fclose(nsl_nalfp);
  byebye: signal_error(ERR_BADFILE,buffer);
  return(excode);
}

/* Files opened by nsl_put_net:
*  confp <- "progname.nsm" -- file for connection mapping
*  sstfp <- "(name in nsl_confp).nss" -- file from nsl1, substantiated modules
*  nalfp <- "outname.nso" -- out file, compiled net in pseudo object format
*  dsifp <- "outname.nsz" -- tmp file, nso index to be appended to nso
*  (progname.nsd is in memory)
*  returns NSLER_** code
*/
int open_put_files(netpt,outname)
  NSLNSLnet *netpt; char *outname;
{ int num,excode=(-NSLER_BF); yard bb=0; foot aa=0,cc,*fpt; aux_elm *apt;
  boolean isnewcon; char buffer[bigbuffsize];
  if(sst_is_bad(netpt)) goto byebye;
if(nsl_verbatim) mon_w_puts("Opening put files...\n");
  sprintf(buffer,"%s.nsz\0",outname);
  if((nsl_dsifp=fopen(buffer,"wb+"))==NULL) goto byeone;
  sprintf(buffer,"%s.nso\0",outname);
  if((nsl_nalfp=fopen(buffer,"wb"))==NULL) goto byetwo;
  fprintf(nsl_nalfp,"NeSeL(b)v2.0(DJRW)1989\n%s\n",netpt->sst_name);
  nsl_ndix_bs=ftell(nsl_nalfp);
  fput_data(nsl_nalfp,(foot *)&aa,sizeof(foot));
  fput_data(nsl_nalfp,(yard *)&bb,sizeof(yard));
  sprintf(buffer,"%s.nsm\0",netpt->name);
  nsl_confp=fopen(buffer,"rb");
  isnewcon=(boolean)(nsl_confp==NULL||(netpt->flags&NET_CXMAP));
  if(isnewcon) /* bad cx map or need to reconstruct */
	{if(nsl_confp) fclose(nsl_confp);
	 if((nsl_confp=fopen(buffer,"wb+"))==NULL) goto byethree;
	 fprintf(nsl_confp,"NeSeL(c)v2.0(DJRW)1989\n%s\n",netpt->sst_name);
if(nsl_verbatim) mon_w_puts("Building conx-list...\n");
	 excode=bld_cx_map(netpt);
	 if(excode<(-NSLER_OO)) goto byethree;
	}
  fgets(buffer,bigbuffsize,nsl_confp); /* check version */
  if(wrong_ver(buffer,'c')) {excode=(-NSLER_VR); goto byethree;}
  if(!isnewcon) /* get cx map list */
	{
if(nsl_verbatim) mon_w_puts("Loading conx-list...\n");
  fgets(buffer,bigbuffsize,nsl_confp); /* skip nss base file name */
	 fget_data(nsl_confp,(foot *)&aa,sizeof(foot));
	 fget_data(nsl_confp,(int *)&nsl_cx_num,sizeof(int));
	 fget_data(nsl_confp,(foot *)&cc,sizeof(foot));
	 fget_data(nsl_confp,(int *)&nsl_ax_num,sizeof(int));
	 nsl_cx_max=nsl_cx_num; nsl_ax_max=nsl_ax_num;
	 fmoveto(nsl_confp,aa); /* read in conx handle list file pts */
	 if(!(nsl_cx_list=malmem(foot,nsl_cx_num)))
	   {signal_error(ERR_OSPACE,"conx-list",(int)sizeof(foot)*nsl_cx_num);
	    goto byethree;}
	 fpt=nsl_cx_list; num=nsl_cx_num;
	 while(num--) {fget_data(nsl_confp,(foot *)fpt,sizeof(foot)); ++fpt;}
	 /* read in aux list as well if any */
	 if(nsl_ax_num)
		{
		if(nsl_verbatim) mon_w_puts("Loading aux-conx-list...\n");
		fmoveto(nsl_confp,cc);
		if(!(nsl_ax_list=malmem(aux_elm,nsl_ax_num)))
		  {signal_error(ERR_OSPACE,"aux-list",(int)sizeof(aux_elm)*nsl_ax_num);
		   goto byethree;}
		apt=nsl_ax_list; num=nsl_ax_num;
		while(num--)
		  {fget_data(nsl_confp,(aux_elm *)apt,sizeof(aux_elm)); ++apt;}
		}
	}
  return(-NSLER_OO);
  byethree: fclose(nsl_nalfp);
  byetwo: fclose(nsl_dsifp);
  byeone: fclose(nsl_sstfp); nsl_sstfp=NULL; nul_free(nsl_sst_name);
  byebye: signal_error(ERR_BADFILE,buffer);
  return(excode);
}

void close_sst_file()
{  if(nsl_sstfp) {fclose(nsl_sstfp); nsl_sstfp=NULL;} 
   nul_free(nsl_sst_name);
}

/* close files used by nsl_get_net */
void close_get_files()
{ if(nsl_confp) {fclose(nsl_confp); nsl_confp=NULL;}
  if(nsl_nalfp) {fclose(nsl_nalfp); nsl_nalfp=NULL;}
  close_sst_file();
}

/* close files used by nsl_put_net & append ds index to nal file */
void close_put_files(num_nodes)
  yard num_nodes;
{ foot fpt,dumy; yard num=num_nodes;
  if(nsl_dsifp&&nsl_nalfp) 
  {	rewind(nsl_dsifp); 
	fposeof(nsl_nalfp); fpt=ftell(nsl_nalfp);
	while(num--)
		{fget_data(nsl_dsifp,(foot *)&dumy,sizeof(foot));
		 fput_data(nsl_nalfp,(foot *)&dumy,sizeof(foot));}
	fmoveto(nsl_nalfp,nsl_ndix_bs);
	fput_data(nsl_nalfp,(foot *)&fpt,sizeof(foot));
	fput_data(nsl_nalfp,(yard *)&num_nodes,sizeof(yard));
  }
  if(nsl_dsifp) {fclose(nsl_dsifp); nsl_dsifp=NULL;}
  close_get_files();
}

/* free all memory used by net */
void clear_net(netpt)
  net_elm *netpt;
{ decl_elm *dpt,*ept; dpt=netpt->decls;
  while(dpt)
	{chk_free(dpt->name); chk_free(dpt->tag);
	 ept=dpt; dpt=dpt->tail; lfree(ept);}
  netpt->decls=NULL; nul_free(netpt->tag); nul_free(netpt->name);
}

/* read in struct details: returns NSLER_** code */
int get_struct(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ int args,dtyp; mile declnum=1;
  decl_elm *new_decl,*decl_tail=NULL; char *bpt,*apt;
  fgets(buffer,bigbuffsize,nsl_sstfp);
  do{/* read in struct */
	 if(*buffer=='%') break;
	 if(!(new_decl=malmem(decl_elm,1)))
		{signal_error(ERR_OSPACE,"getting structure",sizeof(decl_elm));
		 return(-NSLER_OS);}
	 bpt=buffer; while(*bpt!=':') ++bpt; *bpt='\0'; ++bpt;
	 new_decl->name=strsave(buffer);
	 args=sscanf(bpt,"%d,%d,%ld,%d",&new_decl->size,&new_decl->nodes,
				 &new_decl->sst_pos,&dtyp);
	 new_decl->decltype=(siby)dtyp;
	 if(args!=4) {signal_error(ERR_FILE); return(-NSLER_FL);}
	 new_decl->declnum=declnum; ++declnum;
	 apt=bpt; args=4; while(args && *apt) {if((*apt++)==',') --args;}
	 if(*apt!='\n')
		{bpt=apt; while(*apt!='\n') ++apt; *apt='\0';
		new_decl->tag=strsave(bpt);}
	 else new_decl->tag=NULL;
	 new_decl->nodenum=0; new_decl->tail=NULL;
	 if(decl_tail) decl_tail->tail=new_decl; else net_pt->decls=new_decl;
	 decl_tail=new_decl;
	}while(fgets(buffer,bigbuffsize,nsl_sstfp));
  return(-NSLER_OO);
}
/* read in net declaration details */
int get_decls(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ decl_elm *new_decl,*decl_tail=NULL; yard nodenum=0; int num,curdec,maxdec=0;
  char *apt,*bpt,dchr;
  do{if(*buffer!='#'||*(buffer+1)!='D') break;
	 if(!(new_decl=malmem(decl_elm,1)))
	  {signal_error(ERR_OSPACE,"getting declaration",sizeof(decl_elm));
	   return(-NSLER_OS);}
	 sscanf(buffer+2,"%d:%*c%c%d,%d,%ld",&curdec,&dchr,
			&new_decl->size,&new_decl->nodes,&new_decl->sst_pos);
	 apt=buffer+4; num=3; while(num && *apt) {if(*apt++ ==',') --num;}
	 bpt=apt; while(*apt!='(') ++apt; *apt='\0';
	 new_decl->name=strsave(bpt);
	 if(dchr=='N') new_decl->decltype=TYPE_NET;
	 else new_decl->decltype=TYPE_NODE;
	 new_decl->nodenum=nodenum; new_decl->declnum=curdec;
	 if(curdec>maxdec) maxdec=curdec;
	 if(new_decl->size==0) ++nodenum; /* should never be zero */
	 else nodenum+=new_decl->size*new_decl->nodes;
	 new_decl->tail=NULL; new_decl->tag=NULL;
	 if(decl_tail) decl_tail->tail=new_decl; else net_pt->decls=new_decl;
	 decl_tail=new_decl;
	}while(fgets(buffer,bigbuffsize,nsl_sstfp));
  net_pt->dec_num=maxdec;
  return(-NSLER_OO);
}

/* Get module info: returns ERR_** code */
int get_new_net(net_ppt,sst_pos)
  net_elm **net_ppt; foot sst_pos;
{ net_elm *net_pt; foot alpt; int args;
  char *bpt,*apt,buffer[bigbuffsize];
  if(!*net_ppt) *net_ppt=malmem(net_elm,1);
  if(!*net_ppt) {signal_error(ERR_OSPACE,"getting net",sizeof(net_elm));
     return(-NSLER_OS);}
  net_pt= *net_ppt; net_pt->refs=1;
  net_pt->decls=NULL; net_pt->root_flag=F; net_pt->name=NULL;
  net_pt->sst_pos=sst_pos; net_pt->tag=NULL;
  net_pt->subofs=0; net_pt->ners=0;
  if(sst_pos<1) {signal_error(ERR_FILE); return(-NSLER_FL);}
  fmoveto(nsl_sstfp,sst_pos); fgets(buffer,bigbuffsize,nsl_sstfp);
  args=sscanf(buffer,"%*d,%d",&net_pt->ners);
  if(args==1) /* If it's a struct def: doesn't have a ner# field */
	{bpt=buffer; args=2; while(args && *bpt) {if((*bpt++)==',') --args;}
	 if(args) {signal_error(ERR_FILE); return(-NSLER_FL);}
	 apt=bpt; while(*apt && *apt!='(') ++apt;
	 if(*apt!='(') {signal_error(ERR_FILE); return(-NSLER_FL);}
	 *apt='\0'; net_pt->name=strsave(bpt);
	 fgets(buffer,bigbuffsize,nsl_sstfp); /* skip source file & line number */
	 fgets(buffer,bigbuffsize,nsl_sstfp);
	 args=sscanf(buffer,"%ld\n",&alpt);
	 if(args==1) /* must be a node, get its struct def*/
		{/* only nodes expected, not nets */
		/* move to nodes struct def & read in */
		fmoveto(nsl_sstfp,alpt); net_pt->root_flag=T;
		fgets(buffer,bigbuffsize,nsl_sstfp);
		}
	 else /*fetch net decls*/
		{get_decls(net_pt,buffer); return(-NSLER_OO);
		/* signal_error(ERR_FILE); return(-NSLER_FL); */
		}
	}
  args=sscanf(buffer,"%d,",&net_pt->subofs);
  if(args!=1) {signal_error(ERR_FILE); return(-NSLER_FL);}
  bpt=buffer; while(*bpt && *bpt!=',') ++bpt;
  ++bpt; apt=bpt; while(*apt!='\n') ++apt; *apt='\0';
  if(*bpt)
	{net_pt->tag=strsave(bpt);
	 net_pt->sindx=find_tag(bpt);
	 if(!net_pt->sindx) return(-NSLER_MO);
	}
  fgets(buffer,bigbuffsize,nsl_sstfp); /* skip line info */
  return(get_struct(net_pt,buffer));
}

/* find a net in pool, if found make it the top net */
net_elm *find_net(sst_pos)
  foot sst_pos;
{ net_elm *npt=nsl_net_top,*lpt=NULL;
  while(npt)
	{if(npt->sst_pos==sst_pos)
		{if(lpt) {lpt->tail=npt->tail; npt->tail=nsl_net_top; nsl_net_top=npt;}
		return(npt);}
	 lpt=npt; npt=npt->tail;}
  return(NULL);
}

/* relase a net def to pool */
void release_net(netpt)
  net_elm **netpt;
{ net_elm *apt=(net_elm *)(*netpt),*npt,*lpt,*hpt; int num;
  if(apt)
	{*netpt=(net_elm *)NULL;
	 if(apt->refs<2)
		{if(apt->refs<=0) signal_error(ERR_NET_CHK);
		apt->refs=0; num=0; npt=nsl_net_top; lpt=NULL; hpt=NULL;
		while(npt) /* free the net lowest on the net stack */
		  {++num; if(npt->refs==0) {apt=npt; hpt=lpt;} lpt=npt; npt=npt->tail;}
		if(num<10) return; /* exit if < 10 nets in memory */
		if(hpt) {hpt->tail=apt->tail;}
		if(apt==nsl_net_top) nsl_net_top=apt->tail;
		clear_net(apt); lfree(apt);
		}
	 else --(apt->refs);
	}
}

int claim_net(net_ppt,sst_pos)
  net_elm **net_ppt; foot sst_pos;
{ net_elm *net_pt; int excode=(-NSLER_OO);
  if(*net_ppt) signal_error(ERR_NET_CHK);
  if((net_pt=find_net(sst_pos)))
	{*net_ppt=net_pt;
	if(net_pt->refs>=0) ++net_pt->refs;
	else {signal_error(ERR_NET_CHK); net_pt->refs=1;}
	}
  else
	{excode=get_new_net(net_ppt,sst_pos);
	 net_pt=(net_elm *)*net_ppt;
	 net_pt->tail=nsl_net_top; nsl_net_top=net_pt;
	}
  return(excode);
}

void flush_nets()
{ net_elm *npt=nsl_net_top,*apt;
  while(npt)
	{if(npt->refs!=0) printf("Net:(%s)r:%d\n",npt->name,npt->refs);
	 npt->refs=0; apt=npt; npt=npt->tail;
	 clear_net(apt); lfree(apt); }
  nsl_net_top=NULL;
}

/*---------------------------------------------------------------------*/
/* allocate a block of disfp disk space, add sys inf, make a block at eof
* update dsik indx. Note nsl_nalfp is positioned ready for caller */
void nal_alloc(nodenum,space)
  yard nodenum; foot space;
{ foot dindx,dadrs,newspace; door size; char quest='?';
  size=1+(door)((2+space+syssize)/nal_page_size);
  newspace=(foot)size*nal_page_size;
  fposeof(nsl_nalfp); dadrs=ftell(nsl_nalfp);
  while(newspace) {fput_data(nsl_nalfp,(char *)&quest,(size_t)1); --newspace;}
  /* put a nodenum & it's nal block pt to dsik index, update nsl_nalfp */
  dindx=(foot)nodenum*sizeof(foot);
  fput_data(nsl_dsifp,(foot *)&dadrs,sizeof(foot));
  /* write sys info to nodes nsl_nalfp block */
  fmoveto(nsl_nalfp,dadrs);
  fput_data(nsl_nalfp,(foot *)&space,sizeof(foot));
  fput_data(nsl_nalfp,(door *)&size,sizeof(door));
  fput_data(nsl_nalfp,(yard *)&nodenum,sizeof(yard));
}

/* pos nalfp at nodes index entry & return adrs */
foot ds_node_indx(nodenum)
  yard nodenum;
{ foot dindx=(foot)(nodenum*sizeof(foot)+nsl_ndix_bs);
  fposeof(nsl_nalfp);
  if((dindx+2)>=ftell(nsl_nalfp)||nodenum>nsl_ndix_num) return(0);
  fmoveto(nsl_nalfp,dindx);
  return(dindx);
}

/* get a nodes nal block pt from dsik index: positions nalfp at block */
foot ds_get_indx(nodenum,spacept)
  yard nodenum; foot *spacept;
{ foot dindx; yard node; door size; *spacept=0;
  if(!(dindx=ds_node_indx(nodenum))) return(0);
  fget_data(nsl_nalfp,(foot *)&dindx,sizeof(foot));
  /* read sys info from nodes nalfp block */
  if(!dindx) return(0);
  fmoveto(nsl_nalfp,dindx);
  fget_data(nsl_nalfp,(foot *)spacept,sizeof(foot));
  fget_data(nsl_nalfp,(door *)&size,sizeof(door));
  fget_data(nsl_nalfp,(yard *)&node,sizeof(yard));
  if(node!=nodenum) signal_error(EXIT_INT_ERR);
  return(dindx);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* returns the size of stub elm & its value & backtracking offset word */
size_t stub_size(spt)
  stub_elm *spt;
{ siby styp;
  if(spt->conds==NISSTR) goto do_str;
  styp=spt->type; loop:
  switch(styp)
	{case TYPE_SHORT: case TYPE_USSHORT: case TYPE_INT: case TYPE_USINT:
		return(SIZ_SIP_INT);
	 case TYPE_LONG: case TYPE_USLONG: return(SIZ_SIP_LONG);
	 case TYPE_FLOAT: return(SIZ_SIP_FLOAT);
	 case TYPE_DOUBLE: return(SIZ_SIP_DOUB);
	 case TYPE_CHAR: case TYPE_USCHAR: case SIP_STR: do_str:
		if(spt->isdef) return(SIZ_SIP_STR+(ds_value(spt)->str.len));
		return(SIZ_SIP_STR);
	 case SIP_NODE:
		return(SIZ_SIP_NODE+(ds_value(spt)->node.di_num)*sizeof(di_elm));
	 case TYPE_PT: return(SIZ_SIP_PT);
	 case TYPE_NODE: case TYPE_STRUCT: case TYPE_ARB: return(SIZ_SIP_ION);
	 case TYPE_VOID: case TYPE_UNKNOWN:
	 case TYPE_UNION: case TYPE_NET: return(SIZ_SIP);
	 default: if(!(styp&STAR_MASK)) signal_error(EXIT_INT_ERR);
		if(!spt->info.data.size && (styp&STAR_MASK)==STAR_ONE)
		 {styp&=STAR_TYPE; goto loop;}
	}
  return(SIZ_SIP_PT);
}
size_t stub_base(stype,isua)
  int stype; boolean isua;
{ loop:
  switch(stype)
	{case TYPE_SHORT: case TYPE_USSHORT: case TYPE_INT: case TYPE_USINT:
		return(SIZ_SIP_INT);
	 case TYPE_LONG: case TYPE_USLONG: return(SIZ_SIP_LONG);
	 case TYPE_FLOAT: return(SIZ_SIP_FLOAT);
	 case TYPE_DOUBLE: return(SIZ_SIP_DOUB);
	 case TYPE_CHAR: case TYPE_USCHAR: case SIP_STR: return(SIZ_SIP_STR);
	 case SIP_NODE: return(SIZ_SIP_NODE);
	 case TYPE_PT: return(SIZ_SIP_PT);
	 case TYPE_NODE: case TYPE_STRUCT: case TYPE_ARB: return(SIZ_SIP_ION);
	 case TYPE_VOID: case TYPE_UNKNOWN: case SIP_CONX:
	 case TYPE_UNION: case TYPE_NET: return(SIZ_SIP);
	 default: if(!(stype&STAR_MASK)) signal_error(EXIT_INT_ERR);
		if(isua && (stype&STAR_MASK)==STAR_ONE)
		 {stype&=STAR_TYPE; goto loop;}
	}
  return(SIZ_SIP_PT);
}

stub_elm *ds_skp_to_tail(spt)
  register stub_elm *spt;
{ if(spt->hassubpt || spt->hasarypt)
	{word num=(spt->hassubpt+spt->hasarypt);
	 do{--num;
		spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR);
		num+=(spt->hassubpt+spt->hasarypt+spt->hastail);
		}while(num);
	}
  return(ds_next(spt));
}

stub_elm *ds_skp_to_arypt(spt)
  register stub_elm *spt;
{ if(spt->hassubpt)
	{word num=1;
	 do{--num;
		spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR);
		num+=(spt->hassubpt+spt->hasarypt+spt->hastail);
		}while(num);
	}
  return(ds_next(spt));
}

/* get a stubs parent stub by moving up ds & counting pt bits */
stub_elm *ds_parent(spt)
  register stub_elm *spt;
{ if(spt->hasparent)
	{int num=0;
	  do{((char *)spt)-=(*(((word *)spt)-1)); /* move to prior stub */
		 num+=(1-spt->hassubpt - spt->hasarypt - spt->hastail);
		 if(num>0 && !spt->hasparent) signal_error(EXIT_INT_ERR);
		}while(num>0);
	  return(spt);
	}
  return(NULL);
}

/* get a stubs arypt stub by moving down ds & counting pt bits */
stub_elm *ds_arypt(spt)
  register stub_elm *spt;
{ if(spt->hasarypt)
	{if(spt->hassubpt)
		{word num=1;
		do{--num;
			spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR);
			num+=(spt->hassubpt+spt->hasarypt+spt->hastail);
		  }while(num);
		}
	 return(ds_next(spt));
	}
  return(NULL);
}

stub_elm *ds_tail(spt)
  register stub_elm *spt;
{ if(spt->hastail)
	{if(spt->hassubpt || spt->hasarypt)
		{word num=(spt->hassubpt+spt->hasarypt);
		do{--num;
			spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR);
			num+=(spt->hassubpt+spt->hasarypt+spt->hastail);
		  }while(num);
		}
	 return(ds_next(spt));
	}
  return(NULL);
}

/* adj pool's offsets if any in use */
void ds_adj_ofs(basofs,adjofs)
  word basofs; int adjofs;
{ word *opt=nsl_sea.ofs_list,num=nsl_sea.ofs_free;
  while(num--)
	{if(*opt!=1 && *opt>=basofs) (*opt)=(word)((int)*opt+adjofs);
	 ++opt;
	}
}

/* save stub pt on nsl_sea ofs list*/
void push_stub(spt)
  char *spt;
{ if(nsl_sea.ofs_free==MAX_OFS) signal_error(EXIT_INT_ERR);
  if(!spt) nsl_sea.ofs_list[nsl_sea.ofs_free++]=1;
  else
	{if(spt<nsl_sea.ds_base || spt>nsl_sea.ds_end)
		signal_error(EXIT_INT_ERR);
	 nsl_sea.ofs_list[nsl_sea.ofs_free++]=(word)(spt-nsl_sea.ds_base);
	}
}

stub_elm *pop_stub()
{ word sofs;
  if(nsl_sea.ofs_free<1) signal_error(EXIT_INT_ERR);
  sofs=nsl_sea.ofs_list[--nsl_sea.ofs_free];
  if(sofs==1) return(NULL);
  return((stub_elm *)(nsl_sea.ds_base+sofs));
}

/* move from s2 to s1 space bytes */
void memdown(s1,s2,num)
  char *s1,*s2; size_t num;
{ while(num--) *s1++ = *s2++;
}
/* move from s2 to s1 space bytes */
void memup(s1,s2,num)
  char *s1,*s2; size_t num;
{ s2+=num; s1+=num;
  while(num--) *(--s1) = *(--s2);
}

/* check ds has enough room for size more bytes
*  realloc if needed, adjust nsl_sea & user pointer upt
*/
char *ds_check(upt,size)
  char *upt; int size;
{ char *bpt=nsl_sea.ds_base; size_t ds_size=nsl_sea.ds_size;
  word uofs,dofs;
  if((nsl_sea.ds_end+size+10)>=(bpt+ds_size))
	{if(upt<bpt) signal_error(EXIT_INT_ERR);
	 uofs=(word)(upt-bpt); dofs=(word)(nsl_sea.ds_end-bpt);
	 ds_size+=(size+ds_page_size);
	 if(bpt) upt=relmem(char,sizeof(char)*(4+ds_size),bpt);
	 else upt=malmem(char,4+ds_size);
	 if(!upt) {signal_error(ERR_OSPACE,"ds realloc",(int)4+ds_size);
	   return(NULL);}
	 nsl_sea.ds_size=ds_size; nsl_sea.ds_base=upt; nsl_sea.ds_end=upt+dofs;
	 upt+=uofs;
	}
 return(upt);
}

/* create more space, for a var say, in ds cur_poll at bpt
*  returns bpt. Note may move pool via realloc, so bpt may be different... */
char *ds_meminc(bpt,adjofs)
  char *bpt; int adjofs;
{ char *tpt; size_t space;
  bpt=ds_check(bpt,adjofs);
  if(!bpt) return(NULL);
  space=(size_t)(1+nsl_sea.ds_end-bpt);  /* bytes to move */
  tpt=bpt+adjofs; /* get new base adrs of stubs to move from spt */
  memup(tpt,bpt,space); /* move from bpt to tpt space bytes */
  nsl_sea.ds_end+=adjofs;
  ds_adj_ofs((word)(bpt-nsl_sea.ds_base),adjofs);
  return(bpt);
}

/* decrease space, when freeing a str or node var say
*  bpt is base of area to free from, adjofs is number bytes to free */
void ds_memdec(bpt,adjofs)
  char *bpt; int adjofs;
{ char *tpt; size_t space;
  tpt=bpt+adjofs; /* get adrs of stubs to move from */
  space=(size_t)(1+nsl_sea.ds_end-tpt);  /* bytes to move */
  memdown(bpt,tpt,space); /* move from rpt to bpt space bytes */
  nsl_sea.ds_end-=adjofs;
  ds_adj_ofs((word)(bpt-nsl_sea.ds_base),(-adjofs));
}

void set_stub_size(cpt,size)
  char *cpt; word size;
{ cpt+=size-2;
  *((word *)cpt)=(word)size;
}

/* initialise a new stub: sets backtracking word as well */
void zap_stub(spt,size,stype,hasparent)
  stub_elm *spt; int size,stype; boolean hasparent;
{ char *cpt=(char *)spt;
  spt->hasparent=hasparent; spt->hassubpt=F; spt->hasarypt=F; spt->hastail=F;
  spt->isdef=F; spt->isvar=F; spt->conds=NISNUL;
  spt->type=(siby)stype;
  spt->info.data.size=0; spt->info.data.elm=0;
  if(size>=SIZ_SIP_ION)
	{ds_value(spt)->conx.in_num=0; ds_value(spt)->conx.out_num=0;}
  else if(size>=SIZ_SIP_STR) ds_value(spt)->str.len=0;
  if(stype==SIP_NODE)
	{ds_value(spt)->node.base=0; ds_value(spt)->node.di_num=0;}
  cpt+=size-2; *((word *)cpt)=(word)size; /* set backtracking offset */
}

/* create space for a new stub in nsl_sea,
*  ppt is parent stub, child indicates relation of new stub to parent
*  stype is child type, adjofs is extra bytes required for var str etc
*  returns base of new stub. Note realloc may have changed other fast_hands
*/
stub_elm *new_stub(ppt,adjofs,stype,child,isua)
  stub_elm *ppt; int adjofs,stype,child; boolean isua;
{ stub_elm *spt=ppt; char *bpt,*tpt; size_t space; boolean hasparent;
  if(spt) /* get new stubs eventual adrs */
	{hasparent=T; push_stub(ppt);
	 switch(child)
		{case DS_SUB: spt=ds_next(spt); break;
		case DS_ARY: spt=ds_skp_to_arypt(spt); break;
		default: /* DS_TAIL */ spt=ds_skp_to_tail(spt);
		}
	}
  else
	{spt=(stub_elm *)nsl_sea.ds_base; hasparent=F;
	 if(!spt) spt=(stub_elm *)ds_check(spt,(int)0); /* first stub */
	 if(!spt) return(NULL);
	}
  adjofs+=stub_base(stype,isua);
  spt=(stub_elm *)ds_check(spt,adjofs); /* Note this may move pool */
  if(hasparent) ppt=pop_stub();
  if(!spt) return(NULL);
  bpt=(char *)spt;
  tpt=bpt+adjofs; /* get new base adrs of stubs to move from spt */
  space=(size_t)(1+nsl_sea.ds_end-bpt);  /* bytes to move */
  memup(tpt,bpt,space); /* move from bpt to tpt space bytes */
  nsl_sea.ds_end+=adjofs;
  ds_adj_ofs((word)(bpt-nsl_sea.ds_base),adjofs);
  /* now initialise stub */
  zap_stub(spt,adjofs,stype,hasparent);
  /* set parent pointer to stub, & stubs pointers to it's childs */
  if(hasparent) switch(child)
	{case DS_SUB:
		if(ppt->hassubpt) spt->hassubpt=T; else ppt->hassubpt=T; break;
	 case DS_ARY:
		if(ppt->hasarypt) spt->hasarypt=T; else ppt->hasarypt=T; break;
	 default: /* DS_TAIL */
		if(ppt->hastail) spt->hastail=T; else ppt->hastail=T;
	}
  return(spt);
}

stub_elm *ds_put_str(spt,bpt,len)
  stub_elm *spt; char *bpt; word len;
{ char *apt; int size;
  size=(int)stub_base((int)spt->type,(boolean)(spt->info.data.size==0));
  spt->conds=NISSTR; spt->isdef=T;
  size=SIZ_SIP_STR+len-size;
  apt=(char *)&(ds_value(spt)->str);
  if(size>0)
	{push_stub(spt);
	 apt=ds_meminc(apt,size);
	 if(!apt) return(NULL);
	 spt=pop_stub();
	}
  else if(size<0) {size=(-size); ds_memdec(apt,size);}
  set_stub_size(spt,(int)(SIZ_SIP_STR+len));
  ds_value(spt)->str.len=len;
  apt+=sizeof(word);
  while(len--) *apt++ = *bpt++;
  return(spt);
}

/*--------------------------------------------------------------------------*/
void nial_free_ds()
{ if(nsl_sea.nodenum)
	{nsl_sea.nodenum=0;
	 if(nsl_sea.ds_size)
		{nul_free(nsl_sea.ds_base); nsl_sea.ds_end=NULL; nsl_sea.ds_size=0;}
	}
  nsl_sea.ofs_free=0;
}

stub_elm *nial_new_ds(nodenum,sst_pos)
  yard nodenum; foot sst_pos;
{ stub_elm *spt;
  nsl_sea.nodenum=nodenum; nsl_sea.sst_pos=sst_pos;
  nsl_sea.ds_base=NULL; nsl_sea.ds_end=NULL; nsl_sea.ds_size=0;
  spt=new_stub((stub_elm *)NULL,(int)0,(int)TYPE_NODE,DS_SUB,F);
  if(!spt) return(NULL);
  spt->info.nodenum=nodenum;
  return(spt);
}

/* fetch a nodes ds from disk via dsik_indx: returns NSLER_** code */
int nial_get_ds(nodenum)
  yard nodenum;
{ char *mpt; foot base,dsmem,space; nsl_stub=NULL;
  if(nodenum<1) return(-NSLER_IX);
  nsl_sea.sst_pos=0;
  base=ds_get_indx(nodenum,(foot *)&space);
  if(!base) return(-NSLER_IX);
  dsmem=space-sizeof(foot);
  mpt=malmem(char,(size_t)space);
  if(!mpt) {signal_error(ERR_OSPACE,"reading ds",(int)space);
     return(-NSLER_OS);}
  fget_data(nsl_nalfp,(foot *)&nsl_sea.sst_pos,sizeof(foot));
  fget_data(nsl_nalfp,mpt,(size_t)dsmem); /* read ds into memory */
  nsl_sea.ds_base=mpt;
  nsl_sea.ds_end=mpt+(size_t)dsmem;
  nsl_sea.ds_size=(size_t)space;
  nsl_sea.nodenum=nodenum;
  nsl_stub=(stub_elm *)mpt;
  return(-NSLER_OO);
}

void nial_put_ds()
{ foot space,dsmem;
  dsmem=(foot)(nsl_sea.ds_end-nsl_sea.ds_base);
  space=dsmem+sizeof(foot);
  nal_alloc(nsl_sea.nodenum,space); /*alloc a blk & position nsl_nalfp*/
  fput_data(nsl_nalfp,(foot *)&nsl_sea.sst_pos,sizeof(foot));
  fput_data(nsl_nalfp,nsl_sea.ds_base,(size_t)dsmem); /* write ds to disk */
  nial_free_ds();
}

void get_funcs()
{ int num; char buffer[bigbuffsize]; foot fpt;
  fmoveto(nsl_sstfp,nsl_ndfn_bs);
  fgets(buffer,bigbuffsize,nsl_sstfp);
  sscanf(buffer,"%d",&num);
  nsl_ndfn_list=malmem(char *,num);
  nsl_ndfn_sst_list=malmem(foot,num);
  if(!nsl_ndfn_list||!nsl_ndfn_sst_list)
    {signal_error(ERR_OSPACE,"ndfn-list",(int)sizeof(char *)*num); return;}
  nsl_ndfn_num=num; num=0;
  while(num<nsl_ndfn_num && fgets(buffer,bigbuffsize,nsl_sstfp))
	{if(!*buffer) break;
	step_on_cr(buffer);
	nsl_ndfn_list[num]=strsave(buffer);
	fgets(buffer,bigbuffsize,nsl_sstfp);
	sscanf(buffer,"%ld",&fpt);
	nsl_ndfn_sst_list[num++]=fpt;
	}
}


/*-----------------------------------------------------------------------*/
stub_elm *sip_inc(spt)
  stub_elm *spt;
{ if(!spt->hastail) signal_error(EXIT_INT_ERR);
  return(ds_tail(spt));
}

/* returns T if struct is a read only compiler struct */
boolean is_read_struct(dpt)
  decl_elm *dpt;
{ char *tag=dpt->tag;
  return((boolean)(tag && strncmp(tag,"NSL",3)==0
	&& strcmp(tag,"NSLNSLtime")!=0));
}

/* form sip list for new decl, called by get_nxt_decl
*  ppt is parent, first new decl is subpt to ppt, rest are tails
*  if crenob is set, initialises a nob node as well
*  returns ppt (realloc may have moved pool) or nob node pt if crenob
*/
stub_elm *sip_decl(ppt,decl_pt,crenob)
  stub_elm *ppt; decl_elm *decl_pt; boolean crenob;
{ stub_elm *spt; char *cpt; decl_elm *dpt=decl_pt; int stype,space;
  if(!dpt || !ppt || ppt->hassubpt) signal_error(EXIT_INT_ERR);
  /* work out total size of decl stubs */
  if(crenob)
	{ppt->type=TYPE_ARB; ppt->info.data.elm=0;
	 /* cur size=0, and ppt->info.data.size should be set already */
	 space=SIZ_SIP_ION;}
  else space=0;
  while(dpt) {space+=stub_base((int)dpt->decltype,(boolean)(dpt->size==0));
			  dpt=dpt->tail;}
  spt=ds_next(ppt);
  cpt=ds_meminc(spt,space); /* create a chunk of space for all decl stubs */
  if(!cpt) return(NULL);
  spt=(stub_elm *)cpt;
  ppt=ds_prior(spt); ppt->hassubpt=T;
  if(crenob)
	{zap_stub(spt,(int)SIZ_SIP_ION,(int)TYPE_NODE,T);
	 spt->info.data.size=ppt->info.data.size;
	 spt->hassubpt=T; ppt=spt;
	 cpt+=SIZ_SIP_ION; spt=(stub_elm *)cpt;
	}
  crenob=T;
  for(;;) /* now initialise decl stubs in ds */
	{stype=(int)decl_pt->decltype;
	 space=stub_base(stype,(boolean)(decl_pt->size==0));
	 zap_stub(spt,space,stype,T);
	 spt->info.data.size=decl_pt->size;
	 if(is_read_struct(decl_pt))
		{if(strcmp(decl_pt->name,"node")==0) spt->conds=NISNODE;
		else if(crenob && strcmp(decl_pt->name,"group")!=0)
		  {if(crenob) spt->conds=NISINFO; else spt->conds=NISNUL; crenob=F;}
		}
	 decl_pt=decl_pt->tail;
	 if(decl_pt) spt->hastail=T; else break;
	 cpt+=space; spt=(stub_elm *)cpt;
	}
  return(ppt);
}

/* return ds position of index
*  creates a new elm if not found & docre=T else returns NULL*/
stub_elm *sip_pos(apt,spt,indx,stype)
  stub_elm **apt,*spt; mile indx; int stype;
{ stub_elm *tpt; mile curelm;
  while(spt)
	{if(spt->type==TYPE_ARB) signal_error(EXIT_INT_ERR);
	 curelm=spt->info.data.elm;
	 if(curelm==indx) return(spt);
	 if(curelm<indx)
		{tpt=ds_arypt(spt);
		if(!tpt || tpt->info.data.elm>indx)
		  {push_stub(*apt);
			spt=new_stub(spt,(int)0,stype,DS_ARY,
				  (boolean)(spt->info.data.size==0));
			if(!spt) return(NULL);
			*apt=pop_stub();
			spt->info.data.elm=indx;
			spt->info.data.size=(*apt)->info.data.size;
			return(spt);
		  }
		spt=tpt;
		}
	 else break;
	}
  signal_error(EXIT_INT_ERR); return(NULL);
}

/* create an array of sips to hold a ds ary */
stub_elm *sip_ary(rpt,size)
  stub_elm *rpt; mile size;
{ stub_elm *spt=rpt; mile indx; int stype=(int)rpt->type;
  for(indx=0;indx<size;++indx)
	 {spt=sip_pos((stub_elm **)&rpt,spt,indx,stype);
	  if(!spt) return(NULL);
	  spt->isdef=T;
	 }
  return(rpt);
}

#ifdef NSLDEBUG
int ds(spt)
  stub_elm *spt;
{ long ll=(long)spt; word ww=(word)ll&0xffff;
  printf("%x:(p%ds%da%dt%d,d%dv%dc%d,T%d,S%dE%d,V%lx)\n",ww,
	spt->hasparent,spt->hassubpt,spt->hasarypt,spt->hastail,
	spt->isdef,spt->isvar,spt->conds,
	spt->type,
	spt->info.data.size,spt->info.data.elm,
	ds_value(spt)->num);
  return(1);
}

char *dl(apt,bpt,space)
  char *apt,*bpt; int space;
{ stub_elm *spt; int size; boolean hassubpt,hasarypt,hastail=T;
  while(apt<bpt && hastail)
	{spt=(stub_elm *)apt;
	 size=space; while(size>0) {printf("|"); --size;}
	 hassubpt=spt->hassubpt; hasarypt=spt->hasarypt; hastail=spt->hastail;
	 ds(spt);
	 apt+=stub_size(apt);
	 if(hassubpt) apt=dl(apt,bpt,space+1+hasarypt);
	 if(hasarypt) apt=dl(apt,bpt,space+1);
	}
  return(apt);
}
int dp()
{ if(!nsl_sea.ds_base) return(0);
  dl(nsl_sea.ds_base,nsl_sea.ds_end,0);
  printf("\n\n\n\n\n");
  return(1);
}
int da()
{ char *apt=nsl_sea.ds_base,*bpt=nsl_sea.ds_end; int num=0;
  while(apt<bpt)
	{++num;
	 ds(apt);
	 apt+=stub_size(apt);
	}
  printf("\n\n\n\n\n");
  return(num);
}
#endif
