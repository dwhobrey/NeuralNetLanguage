/* Name: nsltox.c	Type: nsl object to std c or txt	Date: 30 March 89
* NeSeL Neural Net Specification Language Compiler
*/
#include "nsltox.h"

boolean add_time=F;
int conx_num,var_count,cur_count,drv_num=0,
  uninit_struct=0,uninit_var=0,uninit_ua=0;
net_elm *net_top=NULL;
long drv_list[MAX_DRVS];
mile *idx_pt,idx[MAX_LEVELS];
char *version="NeSeL(c)v%c.%c(DJRW)1989\n%s\n",version_num='2',version_upd='1';
char *base_list[]={"void",
	"char","double","float","int","long",
	"void","","struct","union","short",
	"unsigned char","unsigned int","unsigned long","unsigned short"};
struct error_elm {int error_num; char *error_str;};
struct error_elm error_list[]={
{WRN_UA_UNIT,"uninitialised unbound array(s) found:%d"},
{WRN_STR_UNIT,"uninitialised structure(s) found:%d"},
{WRN_VAR_UNIT,"uninitialised variable(s) found:%d"},
{WRN_DRV_FUL,"max size of NSLNSLdrv list exceeded:%d"},
{ERR_NET_CHK,"internal consistancy of net is suspect"},
{EXIT_INT,"exit on interrupt"},
{EXIT_BADFILE,"unable to open input/output file:%s"},
{EXIT_OSPACE,"ran out of memory space:%s"},
{EXIT_EOF,"EOF encountered prematurely"},
{EXIT_FILE,"access to file went wrong:%s"},
{EXIT_DSP_FUL,"data structure queue overflow"},
{EXIT_INT_ERR,"internal error?!"},
{EXIT_VERSION,"nsl2 version number mismatch"},
{EXIT_BADNSS,"nss file was bad:%s"},
{ERR_OK,"no error... "},	/* same code as EXIT_OK */
{0,NULL}};

char node_buff[buffsize],conx_buff[buffsize],find_buff[buffsize],
  atom_buff[buffsize],name_buff[buffsize];
char xlate_name[namesize+2]="network";
char output_name[namesize+2]="\0";
char base_name[namesize+2]="\0";
char *net_default="N";
char *aux_name,*aux_default="N";

/* Main parser syntax error handling routine */
void raw_error(args)
	va_list args;
{ int error_no,nn=0,cur_error=1; mile ma; boolean skip=F;
	char *ept,*spt;
	error_no=va_arg(args, int);
	++num_errors;
	do{if((cur_error=error_list[nn].error_num)==error_no) break;
	 ++nn;
	}while(cur_error);
	if(T || (cur_error!=error_no) || (cur_error<ERR_BASE))
	{if(cur_error==error_no)
		{if(cur_error>=warn_limit) return;
		ept=error_list[nn].error_str;
		if(ept)
			{if(*ept=='@') ++ept;
			if(cur_error>=WRN_BASE) fprintf(logfp,"warning: ");
			fprintf(logfp,"NX%d: ",error_no);
			switch(error_no)
			 {case EXIT_OSPACE: case EXIT_BADFILE: case EXIT_BADNSS: case EXIT_FILE:
				spt=va_arg(args, char *);
				fprintf(logfp,ept,spt);
				break;
				case WRN_STR_UNIT: case WRN_VAR_UNIT: case WRN_UA_UNIT:
				ma=va_arg(args,mile);
				fprintf(logfp,ept,ma);
				break;
				default: fprintf(logfp,ept);
			 } fputc('\n',logfp);
			}
		}
	 else
		{cur_error=EXIT_ERRERR;
		fprintf(logfp,"Unknown error number:%d?\n",error_no);
		}
	}
	if(cur_error<ERR_BASE)
	{if(cur_error>EXIT_OK) fprintf(logfp,"Can't continue. Bye!\n");
	 exit(cur_error);}
	if(cur_error<WRN_BASE) raise_error=cur_error;
}

/* just display error */
void signal_error(va_alist)
	va_dcl
{ va_list args; va_start(args); raw_error(args); va_end(args);
}

/*-----------------------------------------------------------------*/
/* routines to alloc mem */
char *mem_mal(nn)
  size_t nn;
{ char *pp;
  if(pp=malloc(nn)) {return(pp);}
  signal_error(EXIT_OSPACE,"malloc");
  return(NULL); /* never actually gets here */
}
char *mem_rel(rr,nn,kk)
  char *rr; size_t nn,kk;
{ char *pp,*ss,*tt;
/* Note: some versions of realloc corrupt block if realloc fails, so...*/
  pp=mem_mal(nn);
  if(rr)
	{ss=pp; tt=rr; if(kk>nn) kk=nn; while(kk--) *ss++ =(*tt++);
	 fremem(rr);}
  return(pp);
}

/* Save string, if enough mem */
char *strsave(s)
	char *s;
{ char *p;
	if( (p=malmem(char,1+strlen(s))) != NULL) strcpy(p,s);
	return(p);
}

char *fcat(a,b)
	register char *a,*b;
{ if(!a) signal_error(EXIT_INT_ERR);
	if(b) while(*b) *(a)++= *(b)++; *(a)='\0';
	return(a);
}

/* Find name str in sorted list of names.
*	Returns pos+1, or -(closest pos) if not found */
int find_name(name,list,hi)
	char *name, **list; int hi;
{ int lo=0,test=0,oldtest,temp;
	if(hi<1) return(0); /* user could have garbage in list even if no elms */
	do {oldtest=test; test=(hi+lo)/2;
	if((temp=strcmp(name,list[test]))==0) return(test+1);
	if(temp<0) hi=test;	else lo=test;
	} while(test!=oldtest);
	return(-hi);
}

/* Enlarge an array */
void enlarge_array(basept,maxpt,numpt,need,dinc,elm_size)
	char **basept; int *maxpt,*numpt,need,dinc; size_t elm_size;
{ size_t old_size;
  if((*numpt+need+2)>=(*maxpt))
	{old_size=elm_size*(*maxpt);
	 *maxpt+=need+dinc;
	 *basept=relmem(char,elm_size*(*maxpt),*basept,old_size);
	}
	*numpt+=need;
}

/*-----------------------------------------------------------------------*/
/* Open files:
*	nalfp <- "progname.nso" -- file from nsl2, net in pseudo object format
*	sstfp <- "(name from nalfp).nss" -- f f nsl1, substantiated modules
*	txtfp <- "outname.nsd" -- output file, net ds in exe C format
*	confp <- "outname.nsm" -- (tmp) file for connection mapping
*	symfp <- "outname.nst" -- textual output file, more readable format
*	tmpfp <- "progname.nsy" -- tmp file for building a nodes ds
*	tabfp <- "progname.nsz" -- tmp file for final txt conx list
*/
void openfiles()
{ int vernum=0,verext=0,nss_ok=0,args;
	char filename[namesize+10],buffer[bigbuffsize];
	sprintf(filename,"%s.nso\0",xlate_name);
	if( (nalfp=fopen(filename,"rb"))==NULL )
	{byebye: signal_error(EXIT_BADFILE,filename); }
	fgets(buffer,bigbuffsize,nalfp); /* check version */
	sscanf(buffer,"%*[^(](b)v%d.%d",&vernum,&verext);
	if(vernum<2) signal_error(EXIT_VERSION);
	fgets(buffer,bigbuffsize,nalfp); /* get nss base file name */
	sscanf(buffer,"%s\n",base_name);
	/* now get node indx base & num nodes */
	fget_data(nalfp,(foot *)&index_base,sizeof(foot));
	index_base-=sizeof(foot);
	fget_data(nalfp,(yard *)&index_num,sizeof(yard));
	sprintf(filename,"%s.nss\0",base_name);
	if( (sstfp=fopen(filename,"r"))==NULL ) goto byebye;
	fgets(buffer,bigbuffsize,sstfp); /* check version */
	vernum=0; verext=0;
	sscanf(buffer,"%*[^(](a)v%d.%d",&vernum,&verext);
	if(vernum<2) signal_error(EXIT_VERSION);
	fgets(buffer,bigbuffsize,sstfp); /* now get func list adrs */
	func_bs=0;
	args=sscanf(buffer,"%ld,%d",&func_bs,&nss_ok);
	if(args!=2 || func_bs==0) signal_error(EXIT_VERSION);
	if(!nss_ok) signal_error(EXIT_BADNSS,filename);
	fgets(buffer,bigbuffsize,sstfp); /* now skip include files */
	sscanf(buffer,"%ld",&sst_bs);
	while(sst_bs--) fgets(buffer,bigbuffsize,sstfp);
	sst_bs=ftell(sstfp);
	if(opt_txt)
	{sprintf(filename,"%s.nst\0",output_name);
	 if( (symfp=fopen(filename,"w+"))==NULL ) goto byebye;
	}
	if(opt_exe||opt_stub)
	{sprintf(filename,"%s.nsd\0",output_name);
	 if( (txtfp=fopen(filename,"w+"))==NULL ) goto byebye;}
	if(opt_exe)
	{sprintf(filename,"%s.nsm\0",output_name);
	 if( (confp=fopen(filename,"wb+"))==NULL ) goto byebye;
	 sprintf(filename,"%s.nsy\0",xlate_name);
	 if( (tmpfp=fopen(filename,"w+"))==NULL ) goto byebye;
	 sprintf(filename,"%s.nsz\0",xlate_name);
	 if( (tabfp=fopen(filename,"w+"))==NULL ) goto byebye;
	}
}

/* Pick up command line args */
void getmainargs(argc,argv)
	int argc; char *argv[];
{ char *chrpt; int argnum=1,chrs; boolean opt_aux=F;
	warn_limit=WRN_MAX; opt_txt=F; opt_exe=F; opt_name=F; opt_idx=F; opt_stub=T;
	net_name=net_default; aux_name=aux_default;
	while(argnum<argc)
	{chrpt=argv[argnum];
	 if(*chrpt=='-' || *chrpt=='/') /* read in '-chars' command line options */
		{++chrpt;
		while(*chrpt)
		switch(*chrpt++)
			{case 'o':
			 argnum++;
			 if(argnum<argc) sscanf(argv[argnum],argstr1,output_name);
			 break;
			case 'i': /* include index numbers in text output */
			opt_idx=T; break;
			case 't': /* produce textual output */
			opt_txt=T; opt_stub=F; break;
			case 'd': /* produce exe c ds output */
			 opt_exe=T; opt_stub=F; break;
			case 'n': /* give net a name */
			 argnum++;
			 if(argnum<argc) {net_name=argv[argnum]; opt_name=T;}
			 break;
			case 'N': /* set aux name */
			 argnum++; opt_aux=T;
			 if(argnum<argc)
				{if(*argv[argnum]=='0') aux_name=NULL;
				else aux_name=argv[argnum];
				}
			 break;
			case 'w': /* switch off warnings */
			 warn_limit=WRN_BASE;
			 break;
			case 'W': /* set warning level */
			 warn_limit=WRN_MAX;
			 break;
			default:
			 fprintf(stdout,"Unrecognised argument:%s\n",argv[argnum]);
		}	}
		else /* get input file name from command line */
		{chrs=0;
		 while((*chrpt!=(char)0) && (*chrpt!='.') && (chrs<40) )
			{xlate_name[chrs]=(*chrpt);
			chrpt++; chrs++; xlate_name[chrs]='\0'; }
		}
		argnum++;
	 }
	if(!*output_name) strcpy(output_name,xlate_name);
	if(!opt_aux&&opt_name) aux_name=net_name;
}
/*--------------------------------------------------------------------*/
void get_funcs()
{ int num; char buffer[bigbuffsize]; foot fpt;
	fmoveto(sstfp,func_bs);
	fgets(buffer,bigbuffsize,sstfp);
	sscanf(buffer,"%d",&num);
	func_list=malmem(char *,num);
	func_sst_list=malmem(foot,num);
	func_num=num; num=0;
	while(num<func_num && fgets(buffer,bigbuffsize,sstfp))
	{if(!*buffer) break;
	*(buffer+strlen(buffer)-1)='\0'; /* step on \n */
	func_list[num]=strsave(buffer);
	fgets(buffer,bigbuffsize,sstfp);
	sscanf(buffer,"%ld",&fpt);
	func_sst_list[num++]=fpt;
	}
}

/* free all memory used by net */
void clear_net(netpt)
	net_elm *netpt;
{ decl_elm *dpt,*ept; dpt=netpt->decls;
	while(dpt)
	{chk_free(dpt->name); chk_free(dpt->tag);
	 ept=dpt; dpt=dpt->tail; fremem(ept);}
	netpt->decls=NULL; nul_free(netpt->tag); nul_free(netpt->name);
}

/* read in struct details */
void get_struct(net_pt,buffer)
	net_elm *net_pt; char *buffer;
{ int args,dtyp; mile declnum=1;
	decl_elm *new_decl,*decl_tail=NULL; char *bpt,*apt;
	fgets(buffer,bigbuffsize,sstfp);
	do{/* read in struct */
	 if(*buffer=='%') break;
	 new_decl=malmem(decl_elm,1);
	 bpt=buffer; while(*bpt!=':') ++bpt; *bpt='\0'; ++bpt;
	 new_decl->name=strsave(buffer);
	 args=sscanf(bpt,"%d,%d,%ld,%d",&new_decl->size,&new_decl->nodes,
				 &new_decl->sst_pos,&dtyp);
/*	 new_decl->decltype=(siby)( ((dtyp&0x30)<<1)|(dtyp&0x0f) ); */
	 new_decl->decltype=(siby)dtyp;
	 if(args!=4) signal_error(EXIT_FILE,"struct");
	 new_decl->declnum=declnum; ++declnum;
	 apt=bpt; args=4; while(args && *apt) {if((*apt++)==',') --args;}
	 if(*apt!='\n')
		{bpt=apt; while(*apt!='\n') ++apt; *apt='\0';
		new_decl->tag=strsave(bpt);}
	 else new_decl->tag=NULL;
	 new_decl->nodenum=0; new_decl->tail=NULL;
	 if(decl_tail) decl_tail->tail=new_decl; else net_pt->decls=new_decl;
	 decl_tail=new_decl;
	}while(fgets(buffer,bigbuffsize,sstfp));
}

void get_decls(net_pt,buffer)
	net_elm *net_pt; char *buffer;
{ decl_elm *new_decl,*decl_tail=NULL; yard nodenum=0; int num;
	char *apt,*bpt,dchr;
	do{/* read in decls and auxs defs */
	 if(*buffer!='#') break;
	 new_decl=malmem(decl_elm,1);
	 sscanf(buffer+2,"%d:%*c%c%d,%d,%ld",&new_decl->declnum,&dchr,
			&new_decl->size,&new_decl->nodes,&new_decl->sst_pos);
	 apt=buffer+4; num=3; while(num && *apt) {if(*apt++ ==',') --num;}
	 bpt=apt; while(*apt!='(') ++apt; *apt='\0';
	 new_decl->name=strsave(bpt);
	 new_decl->tag=NULL;
	 if(dchr=='N') new_decl->decltype=TYPE_NET;
	 else new_decl->decltype=TYPE_NODE;
	 if(*(buffer+1)=='X' || *(buffer+1)=='?') /*DEBUG should abort on '?'*/
		{new_decl->nodenum=0; new_decl->declnum=(-new_decl->declnum);}
	 else
		{new_decl->nodenum=nodenum;
		if(new_decl->size==0) ++nodenum; /* should never be zero */
		else nodenum+=new_decl->size*new_decl->nodes;}
	 new_decl->tail=NULL;
	 if(decl_tail) decl_tail->tail=new_decl; else net_pt->decls=new_decl;
	 decl_tail=new_decl;
	}while(fgets(buffer,bigbuffsize,sstfp));
}

/* Get module info */
void get_new_net(net_ppt,sst_pos)
	net_elm **net_ppt; foot sst_pos;
{ net_elm *net_pt; foot alpt; int args;
	char *bpt,*apt,buffer[bigbuffsize];
	if(!*net_ppt) *net_ppt=malmem(net_elm,1);
	net_pt= *net_ppt; net_pt->refs=1;
	net_pt->decls=NULL; net_pt->root_flag=F; net_pt->name=NULL;
	net_pt->sst_pos=sst_pos; net_pt->tag=NULL;
	net_pt->subofs=0; net_pt->ners=0;
	if(sst_pos<1) signal_error(EXIT_FILE,"net1");
	fmoveto(sstfp,sst_pos); fgets(buffer,bigbuffsize,sstfp);
	args=sscanf(buffer,"%*d,%d",&net_pt->ners);
	if(args==1) /* If it's a struct def: doesn't have a ner# field */
	{bpt=buffer; args=2; while(args && *bpt) {if((*bpt++)==',') --args;}
	 if(args) signal_error(EXIT_FILE,"net2");
	 apt=bpt; while(*apt && *apt!='(') ++apt;
	 if(*apt!='(') signal_error(EXIT_FILE,"net3");
	 *apt='\0'; net_pt->name=strsave(bpt);
	 fgets(buffer,bigbuffsize,sstfp); /* skip source file & line number */
	 fgets(buffer,bigbuffsize,sstfp);
	 args=sscanf(buffer,"%ld\n",&alpt);
	 if(args==1) /* must be a node */
		{/* move to nodes struct def & read in */
		fmoveto(sstfp,alpt); net_pt->root_flag=T;
		fgets(buffer,bigbuffsize,sstfp);
		}
	 else /* fetch net decls */
		{get_decls(net_pt,buffer);
		return;
		}
	}
	args=sscanf(buffer,"%d,",&net_pt->subofs);
	if(args!=1) signal_error(EXIT_FILE,"net4");
	bpt=buffer; while(*bpt && *bpt!=',') ++bpt;
	++bpt; apt=bpt; while(*apt!='\n') ++apt; *apt='\0';
	if(*bpt) net_pt->tag=strsave(bpt);	/* THIS = NULL IF A MODULE */
	fgets(buffer,bigbuffsize,sstfp); /* skip line info */
	get_struct(net_pt,buffer);
}

/* find a net in pool, if found make it the top net */
net_elm *find_net(sst_pos)
	foot sst_pos;
{ net_elm *npt=net_top,*lpt=NULL;
	while(npt)
	{if(npt->sst_pos==sst_pos)
		{if(lpt) {lpt->tail=npt->tail; npt->tail=net_top; net_top=npt;}
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
		apt->refs=0; num=0; npt=net_top; lpt=NULL; hpt=NULL;
		while(npt) /* free the net lowest on the net stack */
			{++num; if(npt->refs==0) {apt=npt; hpt=lpt;} lpt=npt; npt=npt->tail;}
		if(num<10) return; /* exit if < 10 nets in memory */
		if(hpt) {hpt->tail=apt->tail;}
		if(apt==net_top) net_top=apt->tail;
		clear_net(apt); fremem(apt);
		}
	 else --(apt->refs);
	}
}

void claim_net(net_ppt,sst_pos)
	net_elm **net_ppt; foot sst_pos;
{ net_elm *net_pt;
	if(*net_ppt) signal_error(ERR_NET_CHK);
	if((net_pt=find_net(sst_pos)))
	{*net_ppt=net_pt;
	if(net_pt->refs>=0) ++net_pt->refs;
	else {signal_error(ERR_NET_CHK); net_pt->refs=1;}
	}
	else
	{get_new_net(net_ppt,sst_pos);
	 net_pt=(net_elm *)*net_ppt;
	 net_pt->tail=net_top; net_top=net_pt;
	}
}

void flush_nets()
{ net_elm *npt=net_top,*apt;
	while(npt)
	{if(npt->refs!=0) printf("Net:(%s)r:%d\n",npt->name,npt->refs);
	 npt->refs=0; apt=npt; npt=npt->tail;
	 clear_net(apt); fremem(apt); }
}
/*-------------------------------------------------------------------------*/

void nal_initialise()
{ds_pool.nodenum=0; ds_pool.ds_base=NULL; ds_pool.ds_end=NULL; ds_pool.size=0;
}

/* pos nalfp at nodes index entry & return adrs */
foot ds_node_indx(nodenum)
	yard nodenum;
{ foot dindx=(foot)(nodenum*sizeof(foot)+index_base);
	fposeof(nalfp);
	if((dindx+2)>=ftell(nalfp)||nodenum>index_num) return(0);
	fmoveto(nalfp,dindx);
	return(dindx);
}

/* get a nodes nal block pt from dsik index: positions nalfp at block */
foot ds_get_indx(nodenum,spacept)
	yard nodenum; foot *spacept;
{ foot dindx; yard node; door size; *spacept=0;
	if(!(dindx=ds_node_indx(nodenum))) return(0);
	fget_data(nalfp,(foot *)&dindx,sizeof(foot));
	/* read sys info from nodes nalfp block */
	if(!dindx) return(0);
	fmoveto(nalfp,dindx);
	fget_data(nalfp,(foot *)spacept,sizeof(foot));
	fget_data(nalfp,(door *)&size,sizeof(door));
	fget_data(nalfp,(yard *)&node,sizeof(yard));
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


/*--------------------------------------------------------------------------*/
/* fetch the (partial) node ds from disk via dsik_indx
*	returns T if ok (ds in dpt) */
boolean ds_get_node(dpt,nodenum)
	ds_sea_elm *dpt; yard nodenum;
{ char *mpt; foot base,dsmem,space;
	dpt->sst_pos=0;
	base=ds_get_indx(nodenum,(foot *)&space);
	if(!base) return(F);
	dpt->sst_pos=0;
	dsmem=space-sizeof(foot);
	if(dpt->size<dsmem)
	  {if(dpt->ds_base) fremem(dpt->ds_base);
		mpt=malmem(char,(size_t)space); dpt->size=dsmem;
	  }
	else mpt=dpt->ds_base;
	fget_data(nalfp,(foot *)&dpt->sst_pos,sizeof(foot));
	fget_data(nalfp,mpt,(size_t)dsmem); /* read ds into memory */
	dpt->ds_base=mpt;
	dpt->ds_end=mpt+(size_t)dsmem;
	dpt->nodenum=nodenum;
	return(T);
}

/*------------------------------------------------------------------------*/
void sym_dspnode(npt)
	node_elm *npt;
{ di_elm *dpt=node_di(npt); int num=npt->di_num;
	fprintf(symfp,"$%d",npt->base);
	while(num)
	{if(!dpt->decl) break;
	 fprintf(symfp,":%d:%d",dpt->decl,dpt->indx);
	 ++dpt; --num;}
}
/* get value from list and return as double */
double get_lst_val(mlpt,lstype)
  void **mlpt; mile lstype;
{ char *apt=(char *)*mlpt; double val;
  switch(lstype&LST_TYPE)
    {case TYPE_PT: break;
       val=(double)((long)(*((char **)apt)));
       apt+=sizeof(char *); break;
     case TYPE_FLOAT:
       val=(double)(*((float *)apt)); apt+=sizeof(float); break;
     case TYPE_DOUBLE:
       val=(*((double *)apt)); apt+=sizeof(double); break;
     case TYPE_LONG: case TYPE_USLONG:
       val=(double)(*((long *)apt)); apt+=sizeof(long); break;
     case TYPE_INT: case TYPE_USINT:
       val=(double)(*((int *)apt)); apt+=sizeof(int); break;
     case TYPE_CHAR: case TYPE_USCHAR:
       val=(double)(*apt); apt+=sizeof(char); break;
     default:
       val=(double)(*((mile *)apt)); apt+=sizeof(mile);
    }
  *mlpt=(void *)apt;
  return(val);
}

void dsp_list(fp,ap) FILE *fp; void *ap;
{ short lstyp=lst_typ(ap),xx=0,num=lst_num(ap);
  fprintf(fp,"{");
  while(num--)
   {fprintf(fp,"%g",get_lst_val(&ap,lstyp));
    if(num) fprintf(fp,",");
   }
 fprintf(fp,"}");
}

void sym_array();
void sym_substruct();

void sym_member(spt)
	stub_elm *spt;
{ stub_elm *tpt; char *valpt; int stars,stype=(int)spt->type;
  boolean isdef,isua;
	stars=(int)((spt->type&STAR_MASK)>>STAR_SHFT);
	isua=(boolean)(spt->info.data.size==0);
	tpt=ds_subpt(spt);
	if(tpt && tpt->type==SIP_NODE)
	{if(spt->info.data.size==1) {spt=tpt; stype=SIP_NODE;}
	 else stype=TYPE_ARB;}
	isdef=(boolean)(spt->isdef);
	if(stype&STAR_MASK)
	{if(isua&&(stype&STAR_MASK)==STAR_ONE) stype&=STAR_TYPE;
	 else {stype=TYPE_PT; fputs("&",symfp);}}
	else if(spt->conds==NISSTR) stype=SIP_STR;
	switch(stype)
	{case TYPE_VOID:
	 case TYPE_UNKNOWN: fputs("?Undef",symfp); break;
	 case TYPE_SHORT: case TYPE_USSHORT: case TYPE_INT: case TYPE_USINT:
		if(isdef) fprintf(symfp,"%d",ds_value(spt)->nat);
		else fputs("?",symfp);
		break;
	 case TYPE_PT:
		if(isdef&&spt->conds==NISSTR)
		 {valpt=(char *)&(ds_value(spt)->str.base);
		  if((stars==1)&&((spt->type&STAR_TYPE)==TYPE_CHAR)&& (*valpt=='"'))
		    goto is_sip_str;
		  else if((stars==1)&&(*((mile *)valpt)==LST_CODE))
			   dsp_list(symfp,valpt+LST_MEM);
		  else goto is_sip_str;
		  break;
		 }
	 case TYPE_USLONG: case TYPE_LONG:
		if(isdef) fprintf(symfp,"%ld",ds_value(spt)->num);
		else fputs("?",symfp);
		break;
	 case TYPE_FLOAT:
		if(isdef) fprintf(symfp,"%g",ds_value(spt)->floa);
		else fputs("?",symfp);
		break;
	 case TYPE_DOUBLE:
		if(isdef) fprintf(symfp,"%g",ds_value(spt)->doub);
		else fputs("?",symfp);
		break;
	 case TYPE_CHAR: case TYPE_USCHAR:
	 case SIP_STR: is_sip_str:
		if(isdef) fputs((char *)&(ds_value(spt)->str.base),symfp);
		else fputs("?",symfp);
		break;
	 case SIP_NODE: sym_dspnode((node_elm *)&(ds_value(spt)->node)); break;
	 case TYPE_NODE:
	 case TYPE_STRUCT:
		if(spt->hassubpt) sym_substruct(ds_subpt(spt));
		else fputs("{?}",symfp);
		break;
	 case TYPE_ARB:
		sym_array(ds_subpt(spt)); break;
	 case TYPE_UNION:	/* ???? what ???? */
		fputs("?Unitype",symfp); break;
	 case TYPE_NET:
		fputs("?Nettype",symfp); break;
	 default: fprintf(symfp,"?UT%d",spt->type);
	}
}

void sym_array(spt)
	stub_elm *spt;
{ int nn=0;
	fputs("{",symfp);
	while(spt)
	{if(opt_idx) fprintf(symfp,"%d:",spt->info.data.elm);
	 sym_member(spt);
	 spt=ds_arypt(spt);
	 if(spt) {fputs(",",symfp); ++nn; if(nn>8) {nn=0; fputs("\n",symfp);}}
	}
	fputs("}\n",symfp);
}

void sym_substruct(spt)
	stub_elm *spt;
{ int nn=0;
	fputs("{",symfp);
	while(spt)
	{if(spt->hasarypt) sym_array(spt);
	 else sym_member(spt);
	 spt=ds_tail(spt);
	 if(spt) {fputs(",",symfp); ++nn; if(nn>8) {nn=0; fputs("\n",symfp);}}
	}
	fputs("}\n",symfp);
}

/*-----------------*/
boolean nial_get_ds(nodenum)
	yard nodenum;
{if(!nodenum||!ds_get_node(&ds_pool,nodenum))
	  {ds_stub=NULL; return(F);}
	ds_stub=(stub_elm *)ds_pool.ds_base; /* root should be set up by now */
	return(T);
}

void nial_put_ds(nodenum)
	yard nodenum;
{ ds_stub=NULL;
  if(!nodenum) return;
  if(ds_pool.nodenum!=nodenum) signal_error(EXIT_INT_ERR);
  ds_pool.nodenum=0;
}

/* find a nodes function number */
int nial_func(nodenum)
	yard nodenum;
{ foot sst_pos,space; int num=0,dumy,args;
	char *bpt,*apt,buffer[bigbuffsize];
	sst_pos=ds_get_indx(nodenum,(foot *)&space);
	if(!sst_pos) return(0);
	fget_data(nalfp,(foot *)&sst_pos,sizeof(foot));
	if(sst_pos<1) signal_error(EXIT_FILE,"func1");
	fmoveto(sstfp,sst_pos); fgets(buffer,bigbuffsize,sstfp);
	args=sscanf(buffer,"%*d,%d",&dumy);
	if(args==1) /* If it's a struct def: doesn't have a ner# field */
	{bpt=buffer; args=2; while(args && *bpt) {if((*bpt++)==',') --args;}
	 if(args) signal_error(EXIT_FILE,"func2");
	 apt=bpt; while(*apt && *apt!='(') ++apt;
	 if(*apt!='(') signal_error(EXIT_FILE,"func3");
	 *apt='\0';
	 num=find_name(bpt,func_list,func_num);
	}
	else signal_error(EXIT_FILE,"func4");
	return(num);
}
/*********************************************************************/

/* Base type string. takes magnitude of type */
char *base_str(base_type)
	int base_type;
{ base_type&=STAR_TYPE;
	if(base_type>MAX_TYPES) base_type=6;
	return(base_list[base_type]);
}

void zap_path(ppt,parpt)
	path_elm *ppt,*parpt;
{ ppt->parent=parpt; ppt->tail=NULL; if(parpt) parpt->tail=ppt;
	ppt->net=NULL; ppt->pat=NULL; ppt->decl_pt=NULL; ppt->uasize=0;
}

/* compare node addresses quickly for equality */
boolean adrsequal(na,nb)
 register char *na,*nb;
{ int num=node_size((node_elm *)na);
	if(num!=(int)node_size((node_elm *)nb)) return(F);
	while(num--) {if((*na++)!=(*nb++)) return(F);}
	return(T);
}

/* compare two addresses: na & nb.
*	returns <0 if na<nb, 0 if na==nb, otherwise >0 */
mile cmpadrs(na,nb)
	node_elm *na,*nb;
{ mile *a,*b,didif; yard basdif=na->base-nb->base; int lena,lenb;
	if(basdif!=0) {if(basdif<0) return(-1); return(1);}
	a=(mile *)node_di(na); b=(mile *)node_di(nb);
	lena=na->di_num; lenb=nb->di_num;
	while(lena && lenb)
	{didif=(*a)-(*b); if(didif!=0) return(didif);
	 ++a; ++b;
	 didif=(*a)-(*b); if(didif!=0) return(didif);
	 ++a; ++b;
	 --lena; --lenb;
	}
	return((mile)(lena-lenb));
}

void getdiskadrs(fpt,npt)
	foot fpt; node_elm *npt;
{ int num;
	fmoveto(confp,fpt);
	fget_data(confp,npt,sizeof(node_elm));
	num=npt->di_num;
	if(num) fget_data(confp,node_di(npt),(size_t)(num*sizeof(di_elm)));
}

mile cmpdiskadrs(na,fpt)
	node_elm *na; foot fpt;
{ getdiskadrs(fpt,find_buff);
	return(cmpadrs(na,find_buff));
}

/* Find cx in sorted list of conxs: cx_list stored on disk
*	Returns pos+1, or -(closest pos) if not found */
int find_cx(npt)
	node_elm *npt;
{ int hi=cx_num,lo=0,test=0,oldtest,temp;
	if(hi<1) return(0);
	do {oldtest=test; test=(hi+lo)/2;
	if((temp=(int)cmpdiskadrs(npt,cx_list[test]))==0) return(test+1);
	if(temp<0) hi=test;	else lo=test;
	} while(test!=oldtest);
	return(-hi);
}

void make_cx_hole(pos)
	int pos;
{ foot *fpt; int num=cx_num;
	enlarge_array(&cx_list,&cx_max,&cx_num,(int)1,(int)20,sizeof(foot));
	fpt=cx_list+num;
	while(num>pos) {*fpt=(*(fpt-1)); --num; --fpt;}
}

/* add cx addrs to disk cx list via sorted index */
void add_cx_list(npt)
	node_elm *npt;
{ int pos=find_cx(npt); size_t nsize;
	if(pos<1) /* not on list so add */
	{pos=(-pos); make_cx_hole(pos);
	 /* now write conx to confp */
	 fposeof(confp);
	 cx_list[pos]=ftell(confp);
	 nsize=node_size(npt);
	 fput_data(confp,npt,nsize); fflush(confp);
	}
}

void fetch_nxt_cx()
{ if(conx_num<cx_num)
	{getdiskadrs(*conx_pt,cx_pt); ++conx_pt; ++conx_num;}
	else conx_pt=NULL;
}

/* quickly scan ds's & pick out all input connections */
void build_cx_list()
{ char *apt,*bpt; foot fpt=0,findx,*cpt; yard num=1,sum=0;
	fprintf(confp,version,version_num,version_upd,base_name);
	findx=ftell(confp);
	fput_data(confp,(foot *)&fpt,sizeof(foot)); /* cx index */
	fput_data(confp,(int *)&fpt,sizeof(int));
	fput_data(confp,(foot *)&fpt,sizeof(foot)); /* aux index */
	fput_data(confp,(int *)&fpt,sizeof(int));
	while(nial_get_ds(num))
	{apt=ds_pool.ds_base; bpt=ds_pool.ds_end;
	 while(apt<bpt)
		{if(((stub_elm *)apt)->type==SIP_NODE)
			add_cx_list((node_elm *)&(ds_value(apt)->node));
		apt+=stub_size(apt);
		}
	 nial_put_ds(num++);
	}
	fpt=ftell(confp); cpt=cx_list; num=cx_num;
	while(num--)
	{sum+=sizeof(foot); if(sum>(BUFSIZ/2)) {fflush(confp); sum=0;}
	 fput_data(confp,(foot *)cpt,sizeof(foot)); ++cpt;}
	fmoveto(confp,findx);
	fput_data(confp,(foot *)&fpt,sizeof(foot));
	fput_data(confp,(int *)&cx_num,sizeof(int));
	fflush(confp);
	conx_pt=cx_list; conx_num=0; cx_pt=(node_elm *)conx_buff;
	fetch_nxt_cx();
}
/*--------------------------------------------------------------------*/
void uba_stack()
{ enlarge_array(&uba_stk,&uba_max,&uba_num,(int)1,(int)20,sizeof(foot));
	fflush(tmpfp);
	uba_stk[uba_num-1]=ftell(tmpfp);
}

/* write atom type to atom_buff, ignores sign of decltype	*/
char *atom_type(stars)
	int stars;
{ char *apt,*tag=path_pt->decl_pt->tag; boolean notblk;
	apt=base_str(path_pt->decl_pt->decltype);
	notblk=(boolean)(*apt!='\0');
	apt=fcat(atom_buff,apt);
	if(tag) {if(notblk) {*apt=' '; ++apt;} apt=fcat(apt,tag);}
	if(stars) {*apt=' '; ++apt; while(stars--) {*apt='*';++apt;} *apt='\0';}
	return(atom_buff);
}

char *atom_realtype()
{ int stype=path_pt->decl_pt->decltype,stars;
	stars=(stype&STAR_MASK)>>STAR_SHFT;
	if(stype==TYPE_PT) ++stars;
	if(stars>0 && path_pt->decl_pt->size==0 && stype!=TYPE_NODE) --stars;
	return(atom_type(stars));
}

/* write an atoms cast string */
void atom_cast(stars)
	int stars;
{ if(path_pt->decl_pt->decltype==TYPE_NODE
	 && path_pt->decl_pt->size==0) ++stars;
	fprintf(tmpfp,"(%s)",atom_type(stars));
}

/* see if cur ds pos is an output var */
void check_conx()
{ path_elm *ppt=path_pt; di_elm *dip=di_pt; mile *ixp=idx_pt; char *apt;
	if(conx_pt && adrsequal(node_pt,cx_pt))
	{/* add ds var to tabfp list */
	 /* DEBUG what about pts to pts?? e.g. char **fred; ?? */
	 ++var_count; apt=atom_realtype();
	 fprintf(tabfp,"{(short int)sizeof(%s),(char *)&%s%ld_%d,(char *)",
			 apt,net_name,node_pt->base,var_count);
	 fprintf(tmpfp,"\n$$\n"); uba_stack(); /* suspend cur var output */
	 /* now write copy var to tmpfp */
	 apt=atom_realtype();
	 fprintf(tmpfp,"%s %s%ld_%d;\n%%%%\n",
			 apt,net_name,node_pt->base,var_count);
	 while(ppt->parent) /* find parent, or parental UA if any */
		{if(ppt->decl_pt->size==0) break;
		ppt=ppt->parent; --dip; --ixp;
		}
	 if(ppt->decl_pt->size==0)
		{if(ppt->uasize==1)
			fprintf(tabfp,"&%s%ld_%d",net_name,node_pt->base,cur_count);
		else /* DEBUG, added `&' below for outputs from a UA array */
			fprintf(tabfp,"&%s%ld_%d[%d]",
					 net_name,node_pt->base,cur_count,*ixp);
		ppt=ppt->tail; ++dip; ++ixp;
		}
	 else fprintf(tabfp,"&%s%ld",net_name,node_pt->base);
	 while(ppt) /* now write ds adrs of conx */
		{fprintf(tabfp,".%s",ppt->decl_pt->name);
		if(ppt->decl_pt->size!=1) fprintf(tabfp,"[%d]",*ixp);
		ppt=ppt->tail; ++dip; ++ixp;
		}
	 fprintf(tabfp,"},\n");
	 fetch_nxt_cx();
	}
}

void nial_dspnode(npt)
	node_elm *npt;
{ yard cxhandle;
	atom_cast((int)1);
	cxhandle=(yard)find_cx(npt);
	if(!cxhandle) signal_error(EXIT_INT_ERR);
	fprintf(tmpfp,"%ldL",cxhandle-1);
}

void nial_array();
void nial_godown();

/* note UA's (spt->info.data.size<1) will have been trapped in nial_member */
void nial_atom(spt)
	stub_elm *spt;
{ stub_elm *tpt; char *ept="0",*valpt;
	int stype=(int)spt->type,stars; boolean isdef,ispt=F,undefed=F,isua;
	stars=(int)((spt->type&STAR_MASK)>>STAR_SHFT);
	isua=(boolean)(spt->info.data.size==0);
	tpt=ds_subpt(spt);
	if(tpt && tpt->type==SIP_NODE) /* trap connections */
	{if(spt->info.data.size>1) stype=TYPE_ARB; /* array of conxs */
	 else {spt=tpt; stype=SIP_NODE;} /* just one conx */
	}
	isdef=(boolean)(spt->isdef);
	if(stype&STAR_MASK)
	{if(isua &&(stype&STAR_MASK)==STAR_ONE) {stype&=STAR_TYPE; stars=0;}
	 else stype=TYPE_PT;}
	else if(spt->conds==NISSTR) stype=SIP_STR;
	switch(stype)
	{case TYPE_VOID: case TYPE_UNKNOWN: undefed=T; break;
	 case TYPE_SHORT: case TYPE_USSHORT: case TYPE_INT: case TYPE_USINT:
		if(isdef) fprintf(tmpfp,"%d",ds_value(spt)->nat);
		else {ept="0"; undefed=T;}
		break;
	 case TYPE_PT:
		if(isdef&&spt->conds==NISSTR)
		 {valpt=(char *)&(ds_value(spt)->str.base);
		  if((stars==1)&&((spt->type&STAR_TYPE)==TYPE_CHAR)&& (*valpt=='"'))
		    goto is_sip_str;
		  else if((stars==1)&&(*((mile *)valpt)==LST_CODE))
			   dsp_list(tmpfp,valpt+LST_MEM);
		  else goto is_sip_str;
		  break;
		 }
	 case TYPE_USLONG: case TYPE_LONG:
		if(isdef) fprintf(tmpfp,"%ld",ds_value(spt)->num);
		else {ept="0L"; ispt=T; undefed=T;}
		break;
	 case TYPE_FLOAT:
		if(isdef) fprintf(tmpfp,flo_str,ds_value(spt)->floa);
		else {ept="0.0"; undefed=T;}
		break;
	 case TYPE_DOUBLE:
		if(isdef) fprintf(tmpfp,"%g",ds_value(spt)->doub);
		else {ept="0.0"; undefed=T;}
		break;
	 case TYPE_CHAR: case TYPE_USCHAR: case SIP_STR: is_sip_str:
		if(isdef) fputs((char *)&(ds_value(spt)->str.base),tmpfp);
		else undefed=T;
		break;
	 case SIP_NODE: /* display a single node */
		nial_dspnode((node_elm *)&(ds_value(spt)->node));
		break;
	 case TYPE_NODE: case TYPE_STRUCT:
		if(spt->hassubpt) nial_godown(ds_subpt(spt));
		else {atom_cast(stars);fprintf(tmpfp,"0L"); ++uninit_struct;}
		break;
	 case TYPE_ARB: /* could be a fixed array of conxs */
		nial_array(ds_subpt(spt)); break;
	 default: undefed=T;
	}
	if(undefed)
	 {atom_cast(stars); fprintf(tmpfp,ept); ++uninit_var;}
}

/* Unbound arrays: (size=0) recurse & do UA vars
*	Note UA needs to be initialised & may contain sub UAs
*	Fixed arrays are dealt with by nial_atom etc
*/
void nial_unbound(spt)
	stub_elm *spt;
{ stub_elm *apt,*bpt=spt; int size=1,tmpcount=cur_count,stars=0,nn=0;
	++var_count; cur_count=var_count;
	apt=ds_subpt(spt);
	if(spt->type==TYPE_ARB)
	{spt=apt; stars=(int)((spt->type&STAR_MASK)>>STAR_SHFT);}
	else
	{stars=(int)((spt->type&STAR_MASK)>>STAR_SHFT);
	 if(apt && apt->type==SIP_NODE) spt=apt; else apt=spt;
	}
	while(apt->hasarypt) {apt=ds_arypt(apt); ++size;}
	atom_cast((int)(stars)); /*DEBUGFEB91 was 1+stars */
	if((size==1)&&!bpt->hassubpt&&!bpt->isdef)
	{fprintf(tmpfp,"0L"); ++uninit_ua; goto byebye;}
	path_pt->uasize=size;
	if(size<2) fprintf(tmpfp,"&");
	fprintf(tmpfp,"%s%ld_%d\n$$\n",
	 net_name,node_pt->base,cur_count); /* suspend cur ds */
	uba_stack(); /* record start of this UA */
	if(stars>0) --stars;
	fprintf(tmpfp,"%s %s%ld_%d",atom_type((int)stars),
			net_name,node_pt->base,cur_count);
	if(size<2) fprintf(tmpfp,"=\n"); else fprintf(tmpfp,"[%d]={\n",size);
	while(spt)
	{di_pt->indx=spt->info.data.elm;
	 check_conx(); /* conx chk here DEBUG */
	 nial_atom(spt); /* recursing */
	 spt=ds_arypt(spt);
	 if(spt)
		{fputs(",",tmpfp); ++(*idx_pt); ++nn;
		if(nn>8) {fputs("\n",tmpfp); nn=0;}}
	}
	if(size>1) {fprintf(tmpfp,"}"); *idx_pt=0;}
	fprintf(tmpfp,";\n%%%%\n");
	byebye: cur_count=tmpcount; path_pt->uasize=0;
}

void nial_member(spt)
	stub_elm *spt;
{ if(spt->info.data.size<1) /* trap UAs */
	nial_unbound(spt);
	else
	{di_pt->indx=spt->info.data.elm;
	 check_conx(); /* conx chk here DEBUG */
	 nial_atom(spt);}
}

void nial_array(spt)
	stub_elm *spt;
{ int nn=0; if(spt->info.data.size<1) /* trap UAs */
	{nial_unbound(spt); return;}
	fputs("{",tmpfp);
	while(spt)
	{nial_member(spt);
	 spt=ds_arypt(spt);
	 if(spt)
		{fputs(",",tmpfp); ++(*idx_pt); ++nn;
		if(nn>8) {fputs("\n",tmpfp); nn=0;}}
	}
	fputs("}\n",tmpfp);
	*idx_pt=0;
}

void nial_substruct(spt)
	stub_elm *spt;
{ int nn=0; path_pt->decl_pt=path_pt->net->decls; di_pt->decl=1; *idx_pt=0;
	fputs("{",tmpfp);
	while(spt)
	{if(spt->hasarypt) nial_array(spt);
	 else nial_member(spt);
	 if(path_pt->decl_pt->tag && strcmp(path_pt->decl_pt->tag,"_NSLNSLio")==0)
	 {	if(drv_num>=MAX_DRVS) signal_error(WRN_DRV_FUL,(mile)drv_num); 
		else drv_list[drv_num++]=node_pt->base;
	 }
	 spt=ds_tail(spt);
	 path_pt->decl_pt=path_pt->decl_pt->tail; ++di_pt->decl;
	 if(spt)
		{fputs(",",tmpfp); ++nn;
		if(nn>8) {fputs("\n",tmpfp); nn=0;}}
	}
	fputs("}\n",tmpfp);
}

void nial_godown(spt)
	stub_elm *spt;
{ path_elm *tmppt=path_pt,node;
	path_pt=(path_elm *)&node;
	zap_path(path_pt,tmppt);
	claim_net((net_elm **)&path_pt->net,tmppt->decl_pt->sst_pos);
	++node_pt->di_num; ++di_pt; ++idx_pt;
	if(strcmp(tmppt->decl_pt->tag,"_NSLNSLtime")==0) add_time=T;
	if( (strcmp(tmppt->decl_pt->tag,"_NSLNSLid")==0)
	 && (strcmp(tmppt->decl_pt->name,"node")==0))
	strcpy(name_buff,(char *)&ds_value(ds_tail(spt))->str.base);
	nial_substruct(spt);
	--node_pt->di_num; --di_pt; --idx_pt;
	release_net((net_elm **)&path_pt->net);
	path_pt=tmppt; path_pt->tail=NULL;
}

/* write out data structure to tmpfp */
void xlate_node(nodenum)
	yard nodenum;
{ int sus_num; char buff[bigbuffsize];
	uba_num=0; var_count=0; cur_count=0;
	node_pt->base=nodenum; node_pt->di_num=1; di_pt=node_di(node_pt); idx_pt=idx;
	rewind(tmpfp);
	uba_stack();
	fprintf(tmpfp,"%s %s%ld=\n",
			path_pt->net->tag,net_name,node_pt->base);
	nial_substruct(ds_subpt(ds_stub));
	fprintf(tmpfp,";\n%%%%\n"); fflush(tmpfp);
	while(uba_num--) /* copy tmpfp node structs to txtfp */
	{fmoveto(tmpfp,uba_stk[uba_num]); sus_num=0;
	 do{if(fgets(buff,bigbuffsize,tmpfp))
			{if(*buff=='$') ++sus_num;
			if(*buff=='%') --sus_num;
			else if(sus_num==0 &&*buff!='\n') fputs(buff,txtfp);
			}
		else break;
		}while(sus_num>=0);
	}
}

/* write out data structure */
void nial_data(spt)
	stub_elm *spt;
{ net_elm *npt; decl_elm *dpt; char *apt; foot fpt; mile decl,indx;
	fprintf(symfp,"#%d ",spt->info.nodenum);
	if(*name_buff)
	{npt=NULL; fpt=sst_bs; apt=name_buff+1; /* skip quote */
	 while(*apt)
		{claim_net((net_elm **)&npt,fpt);
		decl=0; indx=0;
		while(isdigit(*apt)) {decl=(10*decl)+(*apt-'0'); ++apt;}
		if(*apt==':')
			{fprintf(symfp,"\\"); ++apt;
			while(isdigit(*apt)) {indx=(10*indx)+(*apt-'0'); ++apt;}}
		else /* aux path */
			{fprintf(symfp,";"); decl=(-decl);}
		dpt=npt->decls;
		while(dpt && dpt->declnum!=decl) dpt=dpt->tail;
		if(!dpt) {signal_error(EXIT_INT_ERR);}
		if(decl<0) decl=(-decl);
		fprintf(symfp,"%s:%d",dpt->name,decl);
		if(dpt->size!=1||indx>0) fprintf(symfp,"[%d]",indx);
		if(*apt==',') fpt=dpt->sst_pos;
		if(*apt) ++apt;
		release_net((net_elm **)&npt);
		}
	}
	fprintf(symfp,"\n");
	sym_substruct(ds_subpt(spt));
}

/*-----------------------------------------------------------*/
void translate()
{ yard nodenum; int num,jj; path_elm node; char *npt;
	cx_list=NULL; cx_max=0; cx_num=0;
	uba_stk=NULL; uba_max=0; node_pt=(node_elm *)node_buff;
	if(opt_stub) goto skip;
	if(opt_exe) build_cx_list();
	path_pt=(path_elm *)&node; nodenum=1;
	while(nial_get_ds(nodenum))
	{*name_buff='\0';
	 if(opt_exe)
		{zap_path(path_pt,(path_elm *)NULL);
		claim_net((net_elm **)&path_pt->net,ds_pool.sst_pos);
		xlate_node(nodenum);
		release_net((net_elm **)&path_pt->net);
		}
	 if(opt_txt) nial_data(ds_stub);
	 nial_put_ds(nodenum++);
	}
	flush_nets();
	if(opt_exe)
	{chk_free(uba_stk); skip:
	 get_funcs();
	 fprintf(txtfp,"void (*nsl_%s_funcs[])()={\n",net_name);
	 jj=0;
	 for(num=0;num<func_num;++num)
		{fprintf(txtfp,"%s,",func_list[num]);
		 ++jj; if(jj>5) {fputc('\n',txtfp); jj=0;}
		}
	 fprintf(txtfp,"(void (*)())0L};\n");
	 fprintf(txtfp,"long nsl_%s_sst[]={\n",net_name);
	 jj=0;
	 for(num=0;num<func_num;++num)
		{fprintf(txtfp,"%ldL,",func_sst_list[num]);
		 ++jj; if(jj>5) {fputc('\n',txtfp); jj=0;}
		}
	 fprintf(txtfp,"0L};\n");
	 nodenum=1;
	 fprintf(txtfp,"NSLNSLnode nsl_%s_nodes[]={\n",net_name);
	 if(opt_exe){
	 jj=0;
	 while(num=nial_func(nodenum))
		{fprintf(txtfp,"{%d,(char *)&%s%ld,(NSLNSLtime *)",
				num-1,net_name,nodenum);
		if(add_time) fprintf(txtfp,"&%s%ld.time},",net_name,nodenum);
		else fprintf(txtfp,"0L},");
		++nodenum; ++jj; if(add_time||jj>3) {fprintf(txtfp,"\n"); jj=0;}
		} }
	 fprintf(txtfp,"{0,(char *)0L,(NSLNSLtime *)0L}};\n");
	 num=func_num;
	 while(num--) fremem(func_list[num]);
	 fremem(func_list); fremem(func_sst_list);
	 chk_free(cx_list); nul_free(ds_pool.ds_base);
	}
	if(opt_exe||opt_stub)
	{fprintf(txtfp,"NSLNSLconx nsl_%s_conxs[]={\n",net_name);
	 if(opt_exe)
		{rewind(tabfp);
		while(fgets(node_buff,buffsize,tabfp)) fputs(node_buff,txtfp);
		}
	 fprintf(txtfp,"{0,(char *)0L,(char *)0L}};\n");
	 fprintf(txtfp,"NSLNSLdrv nsl_%s_drvs[]={\n",net_name);
	 if(opt_exe){
	 jj=0;
	 while(jj<drv_num)
		{fprintf(txtfp,"{%ld,&N%ld.args},\n",drv_list[jj],drv_list[jj]); ++jj;}
		}
	 fprintf(txtfp,"{0L,(NSLNSLio *)0L}};\n");
	 if(opt_name) npt=net_name; else npt="network"; num=NET_FLAGS;
	 if(opt_stub) num+=NET_STUB;
	 fprintf(txtfp,"NSLNSLnet %s={%d,%d,%d,%ld,0,\n",
			 npt,num,func_num,cx_num,(nodenum-1));
	 fprintf(txtfp,
		"	nsl_%s_funcs,%ld,nsl_%s_sst,nsl_%s_conxs,nsl_%s_nodes,\n",
		net_name,sst_bs,net_name,net_name,net_name);
	 fprintf(txtfp,"	\"%s\",\"%s\",\"%s\",(char *)0L,\n",
		output_name,base_name,npt);
	 if(aux_name)
		{fprintf(txtfp,
			"	&nsl_num_%s_list,&nsl_num_%s_structs,&nsl_num_%s_stindx,\n",
			aux_name,aux_name,aux_name);
		fprintf(txtfp,"	&nsl_num_%s_vars,nsl_%s_stindx,\n",aux_name,aux_name);
		fprintf(txtfp,"	nsl_%s_evaluate,nsl_%s_names,nsl_%s_list,\n",
			aux_name,aux_name,aux_name);
		fprintf(txtfp,"	nsl_%s_structs,nsl_%s_vars,\n",aux_name,aux_name);
		}
	 else
		{fprintf(txtfp,"(int *)0L,(int *)0L,(int *)0L,(int *)0L,(int *)0L,\n");
		fprintf(txtfp,"(void (*)())0L,(char **)0L,(NSLNSLfelm *)0L,\n");
		fprintf(txtfp,"(NSLNSLstruct *)0L,(NSLNSLvar *)0L,");
		}
	 if(drv_num) fprintf(txtfp,"nsl_%s_drvs,%d",net_name,drv_num);
	 else fprintf(txtfp,"(NSLNSLdrv *)0L,0");
	 fprintf(txtfp,",(void **)0L,(void *)0L};\n");
	 if(uninit_ua) signal_error(WRN_UA_UNIT,(mile)uninit_ua);
	 if(uninit_struct) signal_error(WRN_STR_UNIT,(mile)uninit_struct);
	 if(uninit_var) signal_error(WRN_VAR_UNIT,(mile)uninit_var);
	}
}

void keyint(int sig)
{ fprintf(logfp,"\nTranslation aborted.\n");
	exit(sig);
}

/* The main one */
int main(argc,argv)
	int argc; char *argv[];
{ logfp=stdout; raise_error=ERR_OK;
	signal(SIGINT,keyint);
	getmainargs(argc,argv);
	openfiles();
	nal_initialise();
	translate();
	return(0);
}

/* File relations:
	base.n	:	cpp base.n >base.cpp
				---> base.cpp
	base.cpp :	nsl1 base.cpp
				---> base.nss
	base.nss :	nsl2 base
				---> base.nso base.nsc
				nsl2 base -o outname
				---> outname.nso outname.nsc
	outname.nso
			:	nsltox -n netname -o dataname outname
				--->	dataname.nsd dataname.nsc dataname.nsm
	(netname dataname)
			:	nslms (netname dataname) -o objname
				--->	objname.nso
	objname.nso
			:	nsltox -n netname objname
				---> objname.nsd objname.nsc objname.nsm
*/
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
int dd(dpt)
  ds_sea_elm *dpt;
{ if(!dpt) return(0);
  printf("<%ld>\n",dpt->nodenum);
  dl(dpt->ds_base,dpt->ds_end,0);
  return(1);
}
int dp()
{ dd(&ds_pool);
  return(1);
}
int da(dpt)
  ds_sea_elm *dpt;
{ char *apt=dpt->ds_base,*bpt=dpt->ds_end; int num=0;
  printf("<%ld>\n",dpt->nodenum);
  while(apt<bpt)
	{++num;
	 ds(apt);
	 apt+=stub_size(apt);
	}
  return(num);
}
long cs()
{ char *apt=ds_pool.ds_base,*bpt=ds_pool.ds_end; long sum=0;
  while(apt<bpt) sum+=(long)(*apt++);
  return(sum);
}
#endif
