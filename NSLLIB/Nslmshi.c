/* Name: nslmshi.c  Type: Small Model Monitor  Date: 30 March 89
*  NeSeL Neural Net Specification Language
*/
#include "nslms.h"

extern NSLNSLnet *mon_net;
extern void mon_w_puts(char *str); 
char mon_wnd_buff[256];

#define bye(a) {nsl_err=a;return(a);}

int nsl_cur_sz=0,nsl_aux_sz=0;
int uninit_struct=0,uninit_var=0;
int nsl_map_num=0,conx_num=0;

yard nodenum=0;
foot *conx_pt=NULL;

path_elm pnode; /* root of all paths */
path_elm *path_pt=NULL;
di_elm *di_pt=NULL;
node_elm *node_pt=NULL,*cx_pt=NULL;

void **free_pt=NULL;
NSLNSLstruct *stpt; /* tmp var */
NSLNSLdrv *nsl_drv_list=NULL;
NSLNSLconx *tbpt=NULL,*nsl_cx_pt=NULL;
extern NSLNSLnet **my_net_p=NULL;

char *nsl_aux_mp=NULL;
char *time_pt=NULL;
char node_buff[buffsize],conx_buff[buffsize],find_buff[buffsize];

/* Routines for pushing malloc'd net mem block addresses onto a stack 
*  for eventual freeing when net closed
*/
int NetFreeFind(void *p)
{	void **q=free_pt; int kk,num=lst_ok(free_pt);
	for(kk=0;kk<num;++kk) {if(*q==p) return(kk); ++q;}
	return(-1);
}

void NetFreePush(void *p)
{ if(!p) return;
  free_pt=(void **)nsl_lst_more(free_pt,1,TYPE_PT,sizeof(void *));
  free_pt[lst_num(free_pt)-1]=p;
}

void NetFreeFree(void ***p)
{	void **q=(*p); int num=lst_ok(q);
	while(num--) {if(*q) {lfree(*q); *q=NULL;} ++q;}
	if(*p) {lfree(*p); *p=NULL;}
}

/* returns the mem storage size of element indicated by npt */
int lsize(typ,tag)
  int typ; char *tag;
{ switch(typ)
	{case TYPE_CHAR: case TYPE_USCHAR: return(sizeof(char));
	 case TYPE_SHORT: case TYPE_USSHORT: return(sizeof(short int));
	 case TYPE_INT: case TYPE_USINT: return(sizeof(int));
	 case TYPE_LONG: case TYPE_USLONG: return(sizeof(long));
	 case TYPE_FLOAT: return(sizeof(float));
	 case TYPE_DOUBLE: return(sizeof(double));
	 case SIP_NODE:
	 case TYPE_PT: case SIP_STR: return(sizeof(char *));
	 case TYPE_UNION: case TYPE_NODE: case TYPE_STRUCT:
		stpt=find_tag(tag); if(!stpt) return(0);
		return(stpt->type_code);
	 case TYPE_VOID: case TYPE_UNKNOWN:
	 case TYPE_ARB: case TYPE_NET:
		signal_error(EXIT_INT_ERR);
	 default: if(!(typ&STAR_MASK)) signal_error(EXIT_INT_ERR);
	}
  return(sizeof(char *));
}

/* returns base adrs of elm belonging to cur struct */
char *elm_base(ppt,dpt)
  path_elm *ppt; decl_elm *dpt;
{ NSLNSLstruct *npt=ppt->net->sindx;
  return(ppt->mempt+((int)(npt[dpt->declnum].offset-npt->offset)));
}

/* returns base mem adrs of decl in cur path node with given name
*  returns NULL if err */
char *elm_adrs(ppt,name)
  path_elm *ppt; char *name;
{ decl_elm *dpt=ppt->net->decls;
  while(dpt) {if(!strcmp(dpt->name,name)) break; dpt=dpt->tail;}
  if(!dpt) return(NULL);
  return(elm_base(ppt,dpt));
}

/* returns the mem adrs of cur decl at indx */
char *indx_adrs(ppt)
  path_elm *ppt;
{ NSLNSLstruct *spt=ppt->net->sindx,*npt=spt+ppt->decl_pt->declnum;
  char *base=ppt->mempt; int typ;
  base+=(int)(npt->offset-spt->offset);
  typ=(int)npt->type_code;
  if(ppt->isua)
	{base=(char *)*((char **)base);
	 if(typ&STAR_MASK) typ-=STAR_ONE;
	}
  nsl_cur_sz=lsize(typ,npt->tag);
  if(ppt->indx>0) return(base+(nsl_cur_sz*ppt->indx));
  return(base);
}

void zap_path(ppt,parpt)
  path_elm *ppt,*parpt;
{ ppt->parent=parpt; ppt->tail=NULL; if(parpt) parpt->tail=ppt;
  ppt->net=NULL; ppt->pat=NULL; ppt->decl_pt=NULL;
  ppt->mempt=NULL;
  ppt->indx=(-1); ppt->size=0;
  ppt->uasize=0; ppt->isua=F;
}

/* see if pt at cur decl is a conx handle */
boolean is_conx(ppt)
  path_elm *ppt;
{ NSLNSLstruct *spt=ppt->net->sindx,*npt=spt+ppt->decl_pt->declnum;
  char *base=ppt->mempt; long cxhd;
  base+=(int)(npt->offset-spt->offset);
  base=(char *)*((char **)base);
  if(ppt->isua)
	{if((ppt->decl_pt->decltype&STAR_MASK)<STAR_TWO) return(F);
	 base=(char *)*((char **)base);
	}
  cxhd=(long)base;
  return((boolean)(cxhd>=0 && cxhd<nsl_num_conxs));
}
/* return aux file pos */
foot find_aux(cxhd)
  word cxhd;
{ int lo=0,hi=nsl_ax_num,test=0,oldtest,temp;
  do {oldtest=test; test=(hi+lo)/2;
	if((temp=(cxhd-nsl_ax_list[test].hd))==0)
	  return(nsl_ax_list[test].pos);
	if(temp<0) hi=test;  else lo=test;
  } while(test!=oldtest);
  return(nsl_cx_list[cxhd]);
}
/* returns NSLER_** code */
int put_conx(mpt)
  char *mpt;
{ long cxhd; int nsize; node_elm *npt; char *ypt,buff[buffsize];
  npt=(node_elm *)buff;
  mpt=(char *)*((char **)mpt);
  cxhd=(long)mpt;
  if(cxhd<0 || cxhd>=nsl_num_conxs || !nsl_cx_list) signal_error(EXIT_INT_ERR);
  /* chk if conx on aux list */
  if(nsl_ax_list) fmoveto(nsl_confp,find_aux((word)cxhd));
  else fmoveto(nsl_confp,nsl_cx_list[(int)cxhd]);
  fget_data(nsl_confp,npt,sizeof(node_elm));
  nsize=(npt->di_num)*sizeof(di_elm);
  if(nsize)
	{if(nsize<0) signal_error(EXIT_INT_ERR);
	 fget_data(nsl_confp,node_di(npt),(size_t)nsize);
	 push_stub(nsl_stub);
	 ypt=ds_meminc((char *)&(ds_value(nsl_stub)->node.base),nsize);
	 if(!ypt) bye(-NSLER_OS);
	 nsl_stub=pop_stub();
	}
  set_stub_size(nsl_stub,(int)(SIZ_SIP_NODE+nsize));
  mpt=(char *)&(ds_value(nsl_stub)->node); ypt=buff;
  nsize+=sizeof(node_elm);
  while(nsize--) *mpt++ = *ypt++;
  bye(-NSLER_OO);
}

/* returns NSLER_** code */
int next_decl(ppt)
  path_elm *ppt;
{ decl_elm *dpt=ppt->decl_pt;
	do{if(dpt)
		 {dpt=dpt->tail;
		  if(dpt)
			{nsl_stub=sip_inc(nsl_stub);
			 if(nsl_stub->type==TYPE_NODE) nsl_stub->isdef=T;
			}
		 }
		else
		 {dpt=ppt->net->decls;
		  if(dpt)
			{if(!nsl_stub->hassubpt) nsl_stub=sip_decl(nsl_stub,dpt,F);
			 if(!nsl_stub) bye(-NSLER_DC);
			 nsl_stub=ds_subpt(nsl_stub);
			 if(nsl_stub->type==TYPE_NODE) nsl_stub->isdef=T;
			}
		 }
		if(dpt)
		 {if(dpt->nodes || dpt->sst_pos)
			{if(dpt->declnum<1) dpt=NULL;
			 break;
			}
		 }
	  }while(dpt);
  ppt->decl_pt=dpt;
  if(dpt) return(0);
  bye(-NSLER_OO);
}

/* returns NSLER_AS if size not found */
mile ua_size(ppt,dpt)
  path_elm *ppt; decl_elm *dpt;
{ decl_elm *apt=ppt->net->decls; int uanum=0,*ipt;
  while(apt!=dpt) {if(apt->size==0) ++uanum; apt=apt->tail;}
  ipt=(int *)elm_adrs(ppt,"size");
  if(!ipt) bye(-NSLER_AS);
  while(uanum--) ++ipt;
  return((mile)(*ipt));
}

/* fetch next decl index, returns NSLERR_** code if err */
int next_indx(ppt)
  path_elm *ppt;
{ mile fxsize,size,indx=ppt->indx; int stype;
  if(indx<0) /* first time through, set to zero & get array size */
	{indx=0; fxsize=ppt->decl_pt->size; ppt->isua=(boolean)(fxsize==0);
	 nsl_stub->info.data.elm=0; nsl_stub->info.data.size=fxsize;
	 if(ppt->isua) size=ua_size(ppt,ppt->decl_pt); /* get UA size */
	 else size=fxsize;
	 if(size<1) bye(-NSLER_AS);
	 ppt->size=size;
	 /* determine if a) TYPE_ARB : TYPE_NODE's  ua[], fx[x] */
	 /*			  b) TYPE_xx : SIP_NODE's (pt) */
	 /*			  c) TYPE_xx : TYPE_xx's */
	 stype=(int)ppt->decl_pt->decltype;
	 if((stype&STAR_TYPE)==TYPE_NODE) /* deal with TYPE_NODE's */
		{if(ppt->isua || size>1) /* add a TYPE_ARB stub */
		  {nsl_stub->type=TYPE_ARB;
			nsl_stub=new_stub(nsl_stub,(int)0,(int)TYPE_NODE,DS_SUB,F);
			if(!nsl_stub) bye(-NSLER_OS);
			nsl_stub->info.data.elm=0; nsl_stub->info.data.size=fxsize;
			nsl_stub=sip_ary(nsl_stub,size);
			if(!nsl_stub) bye(-NSLER_OS);
		  }
		/* else it's a simple single atom node: do nothing */
		}
	 else /* deal with plain UA's, ary's & SIP_NODE conx ary's */
		{if((stype&STAR_MASK) && is_conx(ppt)) /* chk for conx pt */
		  {nsl_stub=new_stub(nsl_stub,(int)0,(int)SIP_NODE,DS_SUB,F);
			if(!nsl_stub) bye(-NSLER_OS);
		  }
		/* now create a simple ua or leave as a simple atom */
		nsl_stub->info.data.elm=0; nsl_stub->info.data.size=fxsize;
		nsl_stub=sip_ary(nsl_stub,size); /* if single atom, does nothing*/
		if(!nsl_stub) bye(-NSLER_OS);
		}
	 /* nsl_stub now at atom or first elm of ary*/
	}
  else /* simply advance nsl_stub */
	{++indx;
	 if(indx>=ppt->size) indx=(-1);
	 else
		{nsl_stub=ds_arypt(nsl_stub);
		if(!nsl_stub || nsl_stub->info.data.elm!=indx)
		  signal_error(EXIT_INT_ERR);
		}
	}
  ppt->indx=indx;
  return(indx);
}

/*------------------------------------------------------------------------*/
void set_isdefs(rpt)
  stub_elm *rpt;
{ while(rpt && !rpt->isdef) {rpt->isdef=T; rpt=ds_parent(rpt);}
}

char codetoasc(sppt)
  char **sppt;
{ char *s=(*sppt),chr;
  chr=(*s); ++s;
  if(chr=='\\')
	{chr=(*s); ++s; *sppt=s;
	 switch(chr)
		{case '0': return('\0');
		case 'a': return('\a'); case 'b': return('\b');
		case 'f': return('\f'); case 'n': return('\n');
		case 'r': return('\r'); case 't': return('\t');
		case 'v': return('\v'); /* '\xhh' not implemented */
		case '"': return('"');  case '\'': return('\'');
		case '\\': return('\\'); default: return(chr);
		}
	}
  *sppt=s;
  return(chr);
}

int sqchrtobin(s)
  char *s;
{ if(*s=='\'') ++s;
  return((int)codetoasc(&s));
}

/* convert mem string to backslash-escape code format */
void convert_str(bpt,str,len)
  char *bpt,*str; int len;
{ char lchr=' ',chr;
  *bpt='"'; ++bpt;
  if(str) while(len--)
	{chr=(*str);
	 switch(chr)
		{case '\0': chr='0'; goto pip;
		case '\a': chr='a'; goto pip; case '\b': chr='b'; goto pip;
		case '\n': chr='n'; goto pip; case '\r': chr='r'; goto pip;
		case '\t': chr='t'; goto pip; case '\f': chr='t'; goto pip;
		case '\v': chr='v'; goto pip; case '\\': chr='\\'; goto pip;
		case '"': chr='"'; pip: *bpt='\\'; ++bpt;
		default: *bpt=chr;
		}
	 lchr=chr;
	 ++str; ++bpt;
	}
  *bpt='"'; ++bpt; *bpt='\0'; ++bpt; *bpt='\0';
}
int find_gen_adrsym(adrs,list,num)
  char (*adrs)(); NSLNSLfelm *list; int num;
{ int pos=0;
  if(list)
	{while(pos<num) {if(adrs==list->fn_pt) return(pos); ++pos; ++list;}}
  return(-1);
}

/* Find symbol name with given address in unsorted sym list.
*  Returns name, or null if not found */
char *find_adrsym(adrs)
  char (*adrs)();
{ NSLNSLnet **npt; NSLNSLfelm *list; int nn,pos,num;
  if(nsl_net_list)
	{npt=nsl_net_list; nn=lst_num(nsl_net_list);
	 while(nn--)
		{if(*npt)
				{list=(*npt)->apt; num= *((*npt)->num_ufs);
			if((pos=find_gen_adrsym(adrs,list,num))>=0)
			 return((*npt)->upt[pos]);
					}
				++npt;
		}
	}
  if((pos=find_gen_adrsym(adrs,nsl_mon_list,nsl_num_mon_list))>=0)
	return(nsl_mon_names[pos]);
  return(NULL);
}
/* find symbol adrs given name */
char *find_symadr(name,np,ip)
	char *name; int *np,*ip;
{	NSLNSLnet **npt; char **list,*value; int nn,kk,pos,num;
	if(nsl_net_list)
	{	npt=nsl_net_list; kk=lst_num(nsl_net_list); nn=1;
		while(nn<=kk)
		{	if(*npt)
			{	list=(*npt)->upt; 
				if(list)
				{	num= *((*npt)->num_ufs);
					if((pos=find_name(name,list,num,sizeof(char *)))>0)
					{	value=(char *)((*npt)->apt[pos-1].fn_pt);
						 *np=nn; *ip=(pos-1);
						 return(value);
					}
				}
			}
			++npt; ++nn;
		}
	}
 if((pos=find_name(name,nsl_mon_names,nsl_num_mon_list,sizeof(char *)))>0)
	{*np=0; *ip=(pos-1);
	  value=(char *)nsl_mon_list[pos-1].fn_pt; return(value);}
	return(NULL);
}
/* initialise a simple atom: should not be a struct or array initialiser */
int do_atom(ppt)
  path_elm *ppt;
{ int stars,stype,excode=(-NSLER_OO),len; long **lpt; mile *ap;
  char *mpt=indx_adrs(ppt),buff[MAX_VAR_STR];
  stype=nsl_stub->type;
  stars=((stype&STAR_MASK)>>STAR_SHFT);
  stype&=STAR_TYPE;
  if(stars)
	 {if(stars==1 && nsl_stub->info.data.size==0) stars=0; /* trap output UAs */
	  else if(stars!=1 || stype!=TYPE_CHAR) stype=TYPE_PT;
	 }
  switch(stype)
	{case TYPE_VOID: case TYPE_UNKNOWN:
		ds_value(nsl_stub)->num=0; break;
	 case TYPE_SHORT: case TYPE_USSHORT:
		ds_value(nsl_stub)->nat=(int)(*((short int *)mpt)); break;
	 case TYPE_INT: case TYPE_USINT:
		ds_value(nsl_stub)->nat=(int)(*((int *)mpt)); break;
	 case TYPE_LONG: case TYPE_USLONG:
		ds_value(nsl_stub)->num=(long)(*((long *)mpt)); break;
	 case TYPE_FLOAT:
		ds_value(nsl_stub)->floa=(float)(*((float *)mpt)); break;
	 case TYPE_DOUBLE:
		ds_value(nsl_stub)->doub=(double)(*((double *)mpt)); break;
	 case TYPE_CHAR: case TYPE_USCHAR:
		if(stars)
			{mpt=(char *)*((char **)mpt);
			 if(mpt)
			   {ap=(mile *)mpt; ap+=LST_SYS;
			    if(lst_valid(ap)) {len=lst_num(ap)+LST_MEM;}
			    else {len=strlen(mpt)+1; convert_str(buff,mpt,len); mpt=buff;}
			    goto ptstr;
			   }
			 strcpy(buff,"0L\0");
			}
		else sprintf(buff,"%d\0",(int)*((unsigned char *)mpt));
		len=strlen(buff)+1; mpt=buff;
		ptstr: if(len&1) ++len;
		nsl_stub=ds_put_str(nsl_stub,mpt,len);
		if(!nsl_stub) bye(-NSLER_OS);
		break;
	 case SIP_NODE: /* write conx */
		excode=put_conx(mpt);
		break;
	 case TYPE_PT:
		lpt=(long **)mpt; mpt=find_adrsym(*lpt);
		if(mpt) {len=strlen(mpt)+1; goto ptstr;}
		mpt=(char *)*lpt;
		if(mpt)
		  {ap=(mile *)mpt; ap+=LST_SYS;
		   if(lst_valid(ap)) {len=lst_num(ap)*lst_siz(ap)+LST_MEM; goto ptstr;}
		  }
		ds_value(nsl_stub)->num=(long)mpt; if(mpt) ++uninit_var;
		break;
	 case TYPE_UNION: ++uninit_struct; break;
	 case TYPE_NODE: case TYPE_STRUCT: case TYPE_ARB:
	 case TYPE_NET: case SIP_STR:
	 default: signal_error(EXIT_INT_ERR);
	}
  set_isdefs(nsl_stub);
  bye(excode);
}
/*------------------------------------------------------------------------*/
/* return T if path is nonterm */
boolean check_path(ppt)
  path_elm *ppt;
{ return((boolean)((ppt->net->tag) /* struct etc have a tag */
		  && ((ppt->decl_pt->decltype==TYPE_NODE)
			  ||(ppt->decl_pt->decltype==TYPE_STRUCT)) ));
}

int do_module(refpt,fpt)
  path_elm *refpt; foot fpt;
{ path_elm tmpnode; int excode; boolean isnonterm;
  excode=claim_net((net_elm **)&refpt->net,fpt);
  if(excode<(-NSLER_OO)) bye(excode);
  zap_path((path_elm *)&tmpnode,refpt);
  refpt->decl_pt=NULL; refpt->tail=NULL;
  push_stub(nsl_stub);
  while((excode=next_decl(refpt))>=0)
	{refpt->pat=refpt->decl_pt->name;
	 push_stub(nsl_stub);
	 isnonterm=check_path(refpt);
	 refpt->indx=(-1);
	 while((excode=next_indx(refpt))>=0)
		{if(isnonterm) /*recurse and resolve subnet ref*/
		  {refpt->tail=(path_elm *)&tmpnode;
			tmpnode.mempt=indx_adrs(refpt);
			excode=do_module((path_elm *)&tmpnode,refpt->decl_pt->sst_pos);
			refpt->tail=NULL;}
		else excode=do_atom(refpt); /*reached an atom*/
		if(excode<(-NSLER_OO)) break;
		}
	 nsl_stub=pop_stub();
  if(refpt->isua&&refpt->uasize==0) nsl_stub->isdef=T;
	}
  nsl_stub=pop_stub();
  release_net((net_elm **)&refpt->net);
  bye(excode);
}
/*+++++++++++++++++++++++++++++++++*/
int scan_decl(ppt)
  path_elm *ppt;
{ decl_elm *dpt=ppt->decl_pt;
  do{if(dpt) dpt=dpt->tail;
	 else dpt=ppt->net->decls;
	 if(dpt)
		{if(dpt->nodes || dpt->sst_pos)
		  {if(dpt->declnum<1) dpt=NULL;
			break;
		  }
		}
	}while(dpt);
  ppt->decl_pt=dpt;
  if(dpt) return(0);
  bye(-NSLER_OO);
}
int scan_indx(ppt)
  path_elm *ppt;
{ mile indx=ppt->indx;
  if(indx<0)
	{indx=ppt->decl_pt->size; ppt->isua=(boolean)(indx==0);
	 if(!indx) indx=ua_size(ppt,ppt->decl_pt);
	 if(indx<1) bye(-NSLER_AS);
	 ppt->size=indx; indx=0;
	}
  else {++indx; if(indx>=ppt->size) indx=(-1);}
  ppt->indx=indx;
  return(indx);
}
/* add cur ds adrs to disk cx list */
void add_cx_map()
{ path_elm *ppt=(path_elm *)&pnode; mile *aa; node_elm *npt; word num=0;
  npt=(node_elm *)find_buff; aa=(mile *)node_di(npt);
  while(ppt)
	{*aa=(mile)ppt->decl_pt->declnum; ++aa; *aa=(mile)ppt->indx; ++aa;
	 ++num; ppt=ppt->tail;}
  enlarge_array(&nsl_cx_list,&nsl_cx_max,&nsl_cx_num,
					(int)1,(int)20,sizeof(foot));
  npt->base=nodenum;
  npt->di_num=num;
  fposeof(nsl_confp);
  nsl_cx_list[nsl_cx_num-1]=ftell(nsl_confp);
  fput_data(nsl_confp,npt,node_size(npt)); fflush(nsl_confp);
  if(nsl_map_num) {++nsl_cx_pt; nsl_aux_mp=nsl_cx_pt->dpt; --nsl_map_num;}
}

/* for descending a mem ds quickly for bld_cx_map & nsl_free */
int do_scan(refpt,fpt)
  path_elm *refpt; foot fpt;
{ path_elm tmpnode; int excode; boolean isnonterm;
  excode=claim_net((net_elm **)&refpt->net,fpt);
  if(excode<(-NSLER_OO)) bye(excode);
  zap_path((path_elm *)&tmpnode,refpt);
  refpt->decl_pt=NULL; refpt->tail=NULL;
  while((excode=scan_decl(refpt))>=0)
	{refpt->pat=refpt->decl_pt->name;
	 isnonterm=check_path(refpt);
	 refpt->indx=(-1);
	 while((excode=scan_indx(refpt))>=0)
		{if(isnonterm)
		  {refpt->tail=(path_elm *)&tmpnode;
			tmpnode.mempt=indx_adrs(refpt);
			excode=do_scan((path_elm *)&tmpnode,refpt->decl_pt->sst_pos);
			refpt->tail=NULL;}
		else
		  {if(nsl_aux_mp==indx_adrs(refpt))
			 {if((nsl_cur_sz==nsl_cx_pt->len)&&nsl_map_num) add_cx_map();}
		  }
		if(excode<(-NSLER_OO)) break;
		}
	}
  release_net((net_elm **)&refpt->net);
  bye(excode);
}
void wrt_map(cpt,num,siz) char *cpt; int num; size_t siz;
{ int sum=0;
  while(num--)
	{sum+=siz; if(sum>BUFSIZ/2) {fflush(nsl_confp); sum=0;}
	 fput_data(nsl_confp,cpt,siz); cpt+=siz;}
  fflush(nsl_confp);
}

int bld_cx_map(netpt)
  NSLNSLnet *netpt;
{ NSLNSLnode *npt; foot fpt=0,findx; int excode;
  findx=ftell(nsl_confp);
  /* make space for ax & cx list index's */
  fput_data(nsl_confp,(foot *)&fpt,sizeof(foot));
  fput_data(nsl_confp,(int *)&fpt,sizeof(int));
  fput_data(nsl_confp,(foot *)&fpt,sizeof(foot));
  fput_data(nsl_confp,(int *)&fpt,sizeof(int));
  npt=netpt->npt; nsl_num_conxs=netpt->num_conxs; nodenum=1;
  nsl_cx_pt=netpt->cpt; nsl_aux_mp=nsl_cx_pt->dpt; nsl_map_num=nsl_num_conxs;
  while(nodenum<=netpt->num_nodes)
	{fpt=netpt->sstpt[npt->fnum];
	 zap_path((path_elm *)&pnode,(path_elm *)NULL);
	 pnode.mempt=npt->dpt;
	 excode=do_scan((path_elm *)&pnode,fpt);
	 if(excode<(-NSLER_OO)||!nsl_map_num) break;
	 ++nodenum; ++npt;
	}
  flush_nets();
  if(excode<(-NSLER_OO)||nsl_map_num)
	{nul_free(nsl_cx_list); nul_free(nsl_ax_list);
	 nsl_cx_max=0; nsl_cx_num=0; nsl_ax_max=0; nsl_ax_num=0;
	 bye(-NSLER_RE);}
  fflush(nsl_confp); fposeof(nsl_confp);
  fpt=ftell(nsl_confp);
  wrt_map(nsl_cx_list,nsl_cx_num,sizeof(foot));
  fmoveto(nsl_confp,findx);
  fput_data(nsl_confp,(foot *)&fpt,sizeof(foot));
  fput_data(nsl_confp,(int *)&nsl_cx_num,sizeof(int));
  fflush(nsl_confp);
  rewind(nsl_confp);
  bye(-NSLER_OO);
}

/* save net to disk file name.nso in nsl object format
*  returns NSLER_** error codes */
int nsl_put(netpt,name)
  NSLNSLnet *netpt; char *name;
{ NSLNSLnode *npt; foot sst_pos; int excode=(-NSLER_OO); long perc;
  flush_nets(); 
  uninit_struct=0; uninit_var=0;
  nsl_cx_list=NULL; nsl_ax_list=NULL;
  nsl_cx_max=0; nsl_cx_num=0; nsl_ax_max=0; nsl_ax_num=0;
  if((excode=open_put_files(netpt,name))>=(-NSLER_OO))
	{npt=netpt->npt; nsl_num_conxs=netpt->num_conxs; nodenum=1;
	if(nsl_verbatim) mon_w_puts("Saving nodes:  0%%"); perc=netpt->num_nodes;
	 while(nodenum<=netpt->num_nodes)
		{
if(nsl_verbatim) {sprintf(mon_wnd_buff,"\b\b\b\b%3ld%%",(long)(100*nodenum/perc)); mon_w_puts(mon_wnd_buff);}
		sst_pos=netpt->sstpt[npt->fnum];
		nsl_stub=nial_new_ds(nodenum,sst_pos);
		if(!nsl_stub) {excode=(-NSLER_NL); break;}
		zap_path((path_elm *)&pnode,(path_elm *)NULL);
		pnode.mempt=npt->dpt;
		excode=do_module((path_elm *)&pnode,sst_pos);
		if(excode<(-NSLER_OO)) break;
		nial_put_ds(); ++nodenum; ++npt;
		}
	 flush_nets();
	 close_put_files((yard)netpt->num_nodes);
	}
		if(nsl_verbatim) mon_w_puts("\n");
  nul_free(nsl_cx_list); nul_free(nsl_ax_list);
  if(uninit_struct) signal_error(WRN_VAR_UNI,(mile)uninit_struct);
  if(uninit_var) signal_error(WRN_VAR_PT,(mile)uninit_var);
  if(excode>=(-NSLER_OO)) bye(NSLER_OK);
  excode=(-excode);
  bye(excode);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* compare node addresses quickly for equality */
boolean adrsequal(na,nb)
 register char *na,*nb;
{ int num=(int)node_size((node_elm *)na);
  if(num!=(int)node_size((node_elm *)nb)) return(F);
  while(num--) {if((*na++)!=(*nb++)) return(F);}
  return(T);
}

/* compare two addresses: na & nb.
*  returns <0 if na<nb, 0 if na==nb, otherwise >0 */
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
  fmoveto(nsl_confp,fpt);
  fget_data(nsl_confp,npt,sizeof(node_elm));
  num=npt->di_num;
  if(num) fget_data(nsl_confp,node_di(npt),(size_t)(num*sizeof(di_elm)));
}

mile cmpdiskadrs(na,fpt)
  node_elm *na; foot fpt;
{ getdiskadrs(fpt,find_buff);
  return(cmpadrs(na,find_buff));
}

/* Find cx in sorted list of conxs: nsl_cx_list stored on disk
*  Returns pos+1, or -(closest pos) if not found */
int find_cx(npt)
  node_elm *npt;
{ int hi=nsl_cx_num,lo=0,test=0,oldtest,temp;
  if(hi<1) return(0);
  do {oldtest=test; test=(hi+lo)/2;
	if((temp=(int)cmpdiskadrs(npt,nsl_cx_list[test]))==0) return(test+1);
	if(temp<0) hi=test;  else lo=test;
  } while(test!=oldtest);
  return(-hi);
}

void make_cx_hole(pos)
  int pos;
{ foot *fpt; int num=nsl_cx_num;
  enlarge_array(&nsl_cx_list,&nsl_cx_max,&nsl_cx_num,
				(int)1,(int)20,sizeof(foot));
  fpt=nsl_cx_list+num;
  while(num>pos) {*fpt=(*(fpt-1)); --num; --fpt;}
}

/* add cx addrs to disk cx list via sorted index */
void add_cx_list(npt)
  node_elm *npt;
{ int pos=find_cx(npt); size_t nsize;
  if(pos<1) /* not on list so add */
	{pos=(-pos); make_cx_hole(pos);
	 /* now write conx to confp */
	 fposeof(nsl_confp);
	 nsl_cx_list[pos]=ftell(nsl_confp);
	 nsize=node_size(npt);
	 fput_data(nsl_confp,npt,nsize); fflush(nsl_confp);
	}
}

void wrt_aux_lst(aux_bs)
  foot aux_bs;
{ foot aindx;
  fposeof(nsl_confp); aindx=ftell(nsl_confp);
  wrt_map(nsl_ax_list,nsl_ax_num,sizeof(aux_elm));
  fmoveto(nsl_confp,aux_bs);
  fput_data(nsl_confp,(foot *)&aindx,sizeof(foot));
  fput_data(nsl_confp,(int *)&nsl_ax_num,sizeof(int));
  fflush(nsl_confp);
}
/* check the real ds adrs from cur path
*  if real adrs differs from virtual adrs save on aux_list */
void check_realadrs()
{ path_elm *ppt=(path_elm *)&pnode; aux_elm *apt; mile *a,*b,c; boolean diff=F;
  a=(mile *)node_di(node_pt); b=(mile *)find_buff;
  while(ppt)
	{*b= *a; ++a; ++b;
	 c=ppt->indx; diff|=(boolean)(*a!=c);
	 *b=c; ++a; ++b;
	 ppt=ppt->tail;
	}
  if(diff) /* add aux adrs to disk aux list */
	{enlarge_array(&nsl_ax_list,&nsl_ax_max,&nsl_ax_num,
					(int)1,(int)20,sizeof(aux_elm));
	 fposeof(nsl_confp);
	 apt=nsl_ax_list+nsl_ax_num-1;
	 apt->hd=(word)conx_num-1;
	 apt->pos=(foot)ftell(nsl_confp);
	 fput_data(nsl_confp,node_pt,sizeof(node_elm));
	 fput_data(nsl_confp,find_buff,
		 (((node_elm *)find_buff)->di_num)*sizeof(di_elm));
	 fflush(nsl_confp);
	}
}

void fetch_nxt_cx()
{ if(conx_num<nsl_cx_num)
	{getdiskadrs(*conx_pt,cx_pt); ++conx_pt; ++conx_num;}
  else conx_pt=NULL;
}

/* quickly scan ds's & pick out all input connections */
int build_cx_list(auxp)
  foot *auxp;
{ char *apt,*bpt; foot fpt=0,findx; int excode;
  findx=ftell(nsl_confp);
  /* make space for ax & cx list index's */
  fput_data(nsl_confp,(foot *)&fpt,sizeof(foot));
  fput_data(nsl_confp,(int *)&fpt,sizeof(int));
  fflush(nsl_confp);
  *auxp=ftell(nsl_confp);
  fput_data(nsl_confp,(foot *)&fpt,sizeof(foot));
  fput_data(nsl_confp,(int *)&fpt,sizeof(int));
  nodenum=1;
  while((excode=nial_get_ds(nodenum))>=(-NSLER_OO))
	{apt=nsl_sea.ds_base; bpt=nsl_sea.ds_end;
	 while(apt<bpt)
		{if(((stub_elm *)apt)->type==SIP_NODE)
		  add_cx_list((node_elm *)&(ds_value(apt)->node));
		apt+=stub_size(apt);
		}
	 nial_free_ds(); ++nodenum;
	}
  if(excode!=(-NSLER_IX)) bye(-NSLER_NL);
  /*all these flushes are due to a weird bug writing fpt & nsl_cx_num below*/
  fflush(nsl_confp); fposeof(nsl_confp);
  fpt=ftell(nsl_confp);
  wrt_map(nsl_cx_list,nsl_cx_num,sizeof(foot));
  fmoveto(nsl_confp,findx);
  fput_data(nsl_confp,(foot *)&fpt,sizeof(foot));
  fput_data(nsl_confp,(int *)&nsl_cx_num,sizeof(int));
  fflush(nsl_confp);
  conx_pt=nsl_cx_list; conx_num=0; cx_pt=(node_elm *)conx_buff;
  fetch_nxt_cx();
  bye(-NSLER_OO);
}
/*--------------------------------------------------------------------*/
char *skip_brk(apt)
  char *apt;
{ int num=0;
  while(*apt)
	{if(*apt=='(') ++num;
	 else if(*apt==')') {--num; if(num<1) {++apt; break;}}
	 ++apt;}
  return(apt);
}
/* convert string (with escape codes) to mem string value */
/* converts inline, returns len (includes trailing null) */
int dqstr_to_mem(apt) char *apt;
{ int len=1; char *cpt=apt; ++apt;
  while(*apt && (*apt!='"')) {++len; *cpt=codetoasc(&apt); ++cpt;}
  *cpt='\0';
  return(len);
}
/* convert string to mem string & save */
char *nial_stos(apt)
  char *apt;
{ int len;
  if(!apt) ++uninit_var;
  else
	{if(*apt=='(') apt=skip_brk(apt);
	 if(*apt=='"') 
	 {	len=dqstr_to_mem(apt); 
		apt=memsave(apt,len);
		NetFreePush(apt);
		return(apt);
	 }
	}
  return(NULL);
}

double exp_value;
/* convert string encoded value to binary */
/* Possible strings: ident,&ident,&ident.ident...,dddL,"sss",'c' */
/* or (cast)... */
char *nial_stobin(apt)
  char *apt;
{ char *value=(char *)0L,chr; long num; int pos;
  exp_value=(double)0.0;
  if(!apt) ++uninit_var;
  else
	{chr=(*apt);
	 if(chr=='(') {apt=skip_brk(apt); chr=(*apt);}
	 if(chr=='&'||isalpha(chr)) /* get symbol address */
		{if(chr=='&') ++apt;
		value=find_symadr(apt,&pos,&num);
		}
	 else if(isdigit(chr)||chr=='-')
		{sscanf(apt,"%lf",&exp_value);
		num=(long)exp_value; value=(char *)num;
		}
	 else if(chr=='\'')
		{num=(long)sqchrtobin(apt); value=(char *)num;}
	 else if(chr=='"') ++uninit_var;/* type error */
	}
  return(value);
}

/* see if cur ds pos is an output var */
void check_conx()
{ decl_elm *dpt; int size,typ;
  if(conx_pt && adrsequal(node_pt,cx_pt))
	{/* add ds var to output connection table list, create copy space */
	 check_realadrs();
	 dpt=path_pt->decl_pt; typ=(int)dpt->decltype;
	 if((!dpt->size)&&(typ&STAR_MASK)) typ-=STAR_ONE;
	 size=lsize(typ,dpt->tag);
	 tbpt->len=(short int)size;
	 tbpt->cpt=malmem(char,size);
	 tbpt->dpt=indx_adrs(path_pt);
  if(!tbpt->cpt) {signal_error(ERR_OSPACE,"conx adrs",size); return;}
	 ++tbpt; fetch_nxt_cx();
	}
}

void nial_array();
void nial_godown();

#define assign(typ,val) *((typ *)indx_adrs(path_pt))=(typ)(val)

void nial_dspnode(npt)
  node_elm *npt;
{ long cxhandle; char *pt;
  cxhandle=(long)find_cx(npt);
  if(!cxhandle) signal_error(EXIT_INT_ERR);
  --cxhandle;
  pt=(char *)cxhandle;
  assign(char *,pt);
}

/* note UA's (spt->info.data.size<1) will have been trapped in nial_member */
/* check if isstr set & if a "string", func adrs, xxxL cast, 'c', 0x00 etc */
void nial_atom(spt)
  stub_elm *spt;
{ stub_elm *tpt; char *valpt=NULL; long valong=0; int valint=0;
  float valflo=(float)0.0; double valdou=(double)0.0;
  int stype=(int)spt->type,stars; boolean isdef,isstr;
  stars=(int)((spt->type&STAR_MASK)>>STAR_SHFT);
  tpt=ds_subpt(spt);
  if(tpt && tpt->type==SIP_NODE) /* trap connections */
	{if(spt->info.data.size>1) stype=TYPE_ARB; /* array of conxs */
	 else {spt=tpt; stype=SIP_NODE;} /* just one conx */
	}
  isdef=(boolean)(spt->isdef);
  if(!isdef)
	{if(stype==TYPE_NODE||stype==TYPE_STRUCT) ++uninit_struct;
	 else ++uninit_var;}
  isstr=(boolean)(isdef && (spt->conds==NISSTR));
  if(stype&STAR_MASK)
	{if((spt->info.data.size==0)&&((stype&STAR_MASK)==STAR_ONE))
		stype&=STAR_TYPE;
	 else stype=TYPE_PT;}
  switch(stype)
	{case TYPE_VOID: case TYPE_UNKNOWN: break;
	 case TYPE_SHORT: case TYPE_USSHORT:
		if(isstr)
		 {valong=(long)nial_stobin(&(ds_value(spt)->str.base));
		  valint=(int)valong;}
		else if(isdef) valint=(int)(ds_value(spt)->nat);
		assign(short int,valint);
		break;
	 case TYPE_INT: case TYPE_USINT:
		if(isstr)
		 {valong=(long)nial_stobin(&(ds_value(spt)->str.base));
		  valint=(int)valong;}
		else if(isdef) valint=(int)(ds_value(spt)->nat);
		assign(int,valint);
		break;
	 case TYPE_USLONG: case TYPE_LONG:
		if(isstr) valong=(long)nial_stobin(&(ds_value(spt)->str.base));
		else if(isdef) valong=(long)(ds_value(spt)->num);
		assign(long,valong);
		break;
	 case TYPE_PT:
		if(isstr)
		 {valpt=(char *)&(ds_value(spt)->str.base);
		  if((stars==1) && ((spt->type&STAR_TYPE)==TYPE_CHAR)&& (*valpt=='"'))
			valpt=nial_stos(valpt);
		  else if((stars==1)&&(*((mile *)valpt)==LST_CODE))
			valpt=memlist(valpt);
		  else valpt=nial_stobin(valpt);
		 }
		else if(isdef) valpt=(char *)(ds_value(spt)->num);
		assign(char *,valpt);
		break;
	 case TYPE_FLOAT:
		if(isstr)
		 {nial_stobin(&(ds_value(spt)->str.base));
		  valflo=(float)exp_value;}
		else if(isdef) valflo=(float)(ds_value(spt)->floa);
		assign(float,valflo);
		break;
	 case TYPE_DOUBLE:
		if(isstr)
		 {nial_stobin(&(ds_value(spt)->str.base));
		  valdou=(double)exp_value;}
		else if(isdef) valdou=(double)(ds_value(spt)->doub);
		assign(double,valdou);
		break;
	 case TYPE_CHAR: case TYPE_USCHAR:
		if(isstr)
		 {valong=(long)nial_stobin(&(ds_value(spt)->str.base));
		  valint=(int)valong;}
		else if(isdef) valint=(int)(ds_value(spt)->nat);
		assign(char,valint);
		break;
	 case SIP_NODE: /* display a single node */
		/* should be a pt type (stars>0) */
		nial_dspnode((node_elm *)&(ds_value(spt)->node));
		break;
	 case TYPE_NODE: case TYPE_STRUCT:
		if(spt->hassubpt) nial_godown(ds_subpt(spt));
		break;
	 case TYPE_ARB: /* could be a fixed array of conxs */
		nial_array(ds_subpt(spt)); break;
	 default: ;
	}
}

/* Unbound arrays: (size=0) recurse & do UA vars
*  Note UA needs to be initialised & may contain sub UAs
*  Fixed arrays are dealt with by nial_atom etc
*/
void nial_unbound(spt)
  stub_elm *spt;
{ stub_elm *apt; decl_elm *dpt=path_pt->decl_pt; int size=1,dtyp;
  apt=ds_subpt(spt);
  if(spt->type==TYPE_ARB) spt=apt;
  else if(apt && apt->type==SIP_NODE) spt=apt;
  else apt=spt;
  while(apt->hasarypt) {apt=ds_arypt(apt); ++size;}
  if(size==1 && !spt->isdef) /* an uninit UA */
	{*((char **)elm_base(path_pt,dpt))=NULL; return;}
  path_pt->uasize=size; path_pt->isua=T;
  dtyp=(int)dpt->decltype;
  if((dtyp&STAR_MASK)>STAR_ONE) dtyp=sizeof(char *);
  else {dtyp&=STAR_TYPE; dtyp=lsize(dtyp,dpt->tag);}
  size*=dtyp;
  apt=(stub_elm *)malmem(char,size);  
  NetFreePush(apt);
  *((char **)elm_base(path_pt,dpt))=(char *)apt;
  path_pt->indx=0;
  if(!apt) {signal_error(ERR_OSPACE,"ua-array",size); return;}
  while(spt)
	{di_pt->indx=spt->info.data.elm;
	 check_conx(); /* conx chk here DEBUG */
	 nial_atom(spt); /* recursing */
	 spt=ds_arypt(spt);
	 if(spt) ++path_pt->indx;
	}
  path_pt->indx=(-1);
  path_pt->uasize=0; path_pt->isua=F;
}

void nial_member(spt)
  stub_elm *spt;
{ if(spt->info.data.size<1) nial_unbound(spt);
  else
	{di_pt->indx=spt->info.data.elm;
	 check_conx();
	 nial_atom(spt);
	}
}

void nial_array(spt)
  stub_elm *spt;
{ if(spt->info.data.size<1) nial_unbound(spt);
  else
	{path_pt->indx=0;
	 while(spt)
		{nial_member(spt);
		spt=ds_arypt(spt);
		if(spt) ++path_pt->indx;
		}
	 path_pt->indx=(-1);
	}
}
void do_drv(mpt) NSLNSLio *mpt;
{ NSLNSLdrv *pp;
  nsl_drv_list=lmore(nsl_drv_list,1,TYPE_STRUCT,NSLNSLdrv);
  pp=nsl_drv_list+lst_num(nsl_drv_list)-1;
  pp->nd=node_pt->base;
  pp->iop=mpt;
}

void nial_substruct(spt)
  stub_elm *spt;
{ path_pt->decl_pt=path_pt->net->decls; di_pt->decl=1;
  while(spt)
	{if(spt->hasarypt) nial_array(spt);
	 else nial_member(spt);
	 spt=ds_tail(spt);
	 path_pt->decl_pt=path_pt->decl_pt->tail; ++di_pt->decl;
	}
}

void nial_godown(spt)
  stub_elm *spt;
{ char *bpt; path_elm *tmppt=path_pt,tmpnode; int excode;
  bpt=indx_adrs(path_pt);
  if(strcmp(path_pt->decl_pt->tag,"_NSLNSLtime")==0) time_pt=bpt;
  else if(strcmp(path_pt->decl_pt->tag,"_NSLNSLio")==0)  do_drv(bpt);
  path_pt=(path_elm *)&tmpnode;
  zap_path(path_pt,tmppt);
  excode=claim_net((net_elm **)&path_pt->net,tmppt->decl_pt->sst_pos);
  if(excode<(-NSLER_OO)) signal_error(EXIT_INT_ERR);
  ++node_pt->di_num; ++di_pt;
  path_pt->mempt=bpt;
  nial_substruct(spt);
  --node_pt->di_num; --di_pt;
  release_net((net_elm **)&path_pt->net);
  path_pt=tmppt; path_pt->tail=NULL;
}

extern void close_sst_file();

/* free the memory used by a net: free NSLNSLnet elm as well */
void nsl_free(netpt)
  NSLNSLnet *netpt;
{ NSLNSLnode *ndpt; NSLNSLconx *tbpt;
  close_sst_file(); flush_nets();
  if(!netpt) return;
  if(netpt->flags&NET_CDECL||netpt->flags&NET_FREE)
	  return; /* can't free a C declared variable */
  tbpt=netpt->cpt; ndpt=netpt->npt;
  if(tbpt)
	{while(tbpt->dpt) {lfree(tbpt->cpt); ++tbpt;}
	 lfree(netpt->cpt);}
  if(ndpt) /*DEBUG note, this won't have freed any UA's !!...*/
	{while(ndpt->dpt) {lfree(ndpt->dpt); ++ndpt;}
	 lfree(netpt->npt);}
  chk_free(netpt->fpt); chk_free(netpt->name); chk_free(netpt->sst_name);
  chk_free(netpt->var_name); chk_free(netpt->ins_name);
  chk_free(netpt->sstpt);
  lfree(netpt->dpt);
  NetFreeFree(&(netpt->mpt)); 
  netpt->flags&=NET_FREE; /* just in case user tries to free again */
  lfree(netpt);
}

/*-----------------------------------------------------------*/
/* fetch net to mem from object file name.nso,
*  returns NSLER_** error codes
*/
NSLNSLnet *nsl_get(name)
  char *name;
{ NSLNSLnet *netpt; NSLNSLnode *ndpt; char **fpt;
  foot aux_bs; int excode=(-NSLER_OO),num,jj,kk; long perc;
  nsl_drv_list=NULL; nsl_ndfn_list=NULL; nsl_cx_list=NULL; nsl_ax_list=NULL;
  nsl_ndfn_num=0; nsl_cx_num=0; nsl_cx_max=0; nsl_ax_num=0; nsl_ax_max=0;
  nsl_ndfn_sst_list=NULL; free_pt=NULL;
  flush_nets();
  node_pt=(node_elm *)node_buff;
  uninit_struct=0; uninit_var=0;
  if(!(netpt=malmem(NSLNSLnet,1))) {excode=(-NSLER_OS); goto byebye;}
  netpt->sst_name=NULL; netpt->var_name=NULL; netpt->ins_name=NULL;
  netpt->sstpt=NULL; netpt->flags=0; netpt->time=0; netpt->num_drv=0;
  netpt->cpt=NULL; netpt->npt=NULL; netpt->fpt=NULL;
  netpt->num_ufs=NULL; netpt->num_sts=NULL;
  netpt->num_six=NULL; netpt->num_vas=NULL; netpt->sixpt=NULL;
  netpt->ept=NULL; netpt->upt=NULL; netpt->apt=NULL;
  netpt->spt=NULL; netpt->vpt=NULL;
  netpt->mpt=NULL; netpt->args=NULL;
  netpt->name=strsave(name);
  if((excode=open_get_files(netpt))>=(-NSLER_OO))
	{
	 if(nsl_verbatim) mon_w_puts("Building conx-list...\n");
	 if((excode=build_cx_list((foot *)&aux_bs))<(-NSLER_OO)) goto byeone;
	 get_funcs();
	 tbpt=malmem(NSLNSLconx,nsl_cx_num+1); netpt->cpt=tbpt; tbpt->dpt=NULL;
	 ndpt=malmem(NSLNSLnode,nodenum); netpt->npt=ndpt; ndpt->dpt=NULL;
	 fpt=malmem(char *,nsl_ndfn_num); netpt->fpt=(void (**)())fpt;
	 if(!tbpt||!ndpt||!fpt||!netpt->name) {excode=(-NSLER_OS); goto byebye;}
	 path_pt=(path_elm *)&pnode; perc=(long)nodenum; nodenum=1;
	 zap_path(path_pt,(path_elm *)NULL);
	 if(nsl_verbatim) mon_w_puts("Loading nodes:  0%%");
	 while((excode=nial_get_ds(nodenum))>=(-NSLER_OO))
		{
		if(nsl_verbatim) 
			{sprintf(mon_wnd_buff,"\b\b\b\b%3ld%%",(long)(100*nodenum/perc)); 
			 mon_w_puts(mon_wnd_buff);
			}
		excode=claim_net((net_elm **)&path_pt->net,nsl_sea.sst_pos);
		if(excode<(-NSLER_OO)) {byedsos: nial_free_ds(); break;}
		node_pt->base=nodenum; node_pt->di_num=1; di_pt=node_di(node_pt);
		path_pt->mempt=malmem(char,path_pt->net->sindx->type_code);
		ndpt->dpt=path_pt->mempt; time_pt=NULL;
		if(!path_pt->mempt) {excode=(-NSLER_OS); goto byedsos;}
		nial_substruct(ds_subpt(nsl_stub));
		ndpt->fnum=find_name(path_pt->net->name,nsl_ndfn_list,
							 nsl_ndfn_num,sizeof(char *))-1;
		if(ndpt->fnum<0) ndpt->fnum=0;
		ndpt->tpt=(NSLNSLtime *)time_pt; ++ndpt;
		release_net((net_elm **)&path_pt->net);
		nial_free_ds(); ++nodenum;
		zap_path(path_pt,(path_elm *)NULL);
		}
	 if(excode==(-NSLER_IX)) excode=(-NSLER_OO);
	 tbpt->len=0; tbpt->cpt=NULL; tbpt->dpt=NULL;
	 ndpt->fnum=0; ndpt->dpt=NULL; ndpt->tpt=NULL;
	 netpt->num_funcs=nsl_ndfn_num;
	 netpt->num_conxs=conx_num; netpt->num_nodes=(int)nodenum-1;
	 if(nsl_verbatim) mon_w_puts("\n");
	 for(num=0;num<nsl_ndfn_num;++num)
		{*fpt=find_symadr(nsl_ndfn_list[num],&jj,&kk); ++fpt;}
	 if(nsl_ax_list)
		{if(nsl_verbatim) mon_w_puts("Saving aux-list...\n");
			wrt_aux_lst(aux_bs);
		}
	 netpt->sstpt=nsl_ndfn_sst_list; nsl_ndfn_sst_list=NULL;
	 byeone: flush_nets(); close_get_files();
	}
  byebye:
  if(uninit_struct) signal_error(WRN_STR_UNIT,(mile)uninit_struct);
  if(uninit_var) signal_error(WRN_VAR_UNIT,(mile)uninit_var);
  netpt->dpt=nsl_drv_list; netpt->mpt=free_pt; free_pt=NULL;
  if(nsl_drv_list) netpt->num_drv=lst_num(nsl_drv_list);
  nsl_drv_list=NULL;
  num=nsl_ndfn_num; while(num--) lfree(nsl_ndfn_list[num]);
  nul_free(nsl_ndfn_list); 
  nul_free(nsl_cx_list); nul_free(nsl_ax_list);
  if(excode>=(-NSLER_OO)) {nsl_err=NSLER_OK; return(netpt);}
  excode=(-excode); nsl_err=excode;
  if(netpt) nsl_free(netpt);/* try and reclaim as much mem as poss */
  return(NULL);
}
/*-------------------------------*/
int get_num(app)
  char **app;
{ char chr,*apt= *app; int num=0;
  chr=(*apt);
  while(isdigit(chr))
	{num*=10; num+=(int)(chr-'0'); ++apt; chr=(*apt);}
  *app=apt;
  return(num);
}
/* if isvar=0, for path returns node number or size of array if [*]
*  if isvar>0, returns path var adrs in appt & type code as result
*  if isvar=(-1), returns var adrs after descending from root net in path
*/
long nsl_num_adrs(netpt,isvar,path,appt,isdir)
  NSLNSLnet *netpt; long isvar; char *path,**appt; void (*isdir)();
{ path_elm panod; int excode,indx,declnum,dirx;
  long num=1,ndbs=1; foot fpt; boolean isrt;
  NSLNSLnode *npt; decl_elm *dpt; char *adrs,*apt,*bpt,*cpt,chr;
  *appt=NULL; nsl_cur_sz=0;
  if(!netpt) netpt=mon_net;
  if(sst_is_bad(netpt))
    {signal_error(WRN_DS_STUB,netpt->name); return((long)0);}
  apt=path; fpt=netpt->sst_bs;
  isrt=(boolean)(isvar==(-1));
  if(isrt) isvar=0;
  else if(isvar>0)
	{if(netpt->flags&NET_STUB) return((long)0); /* stubs don't have any DS's */
	 npt=netpt->npt+((int)(isvar-1));
	 fpt=netpt->sstpt[npt->fnum];
	 adrs=npt->dpt;
	}
  do{panod.net=NULL;
	 excode=claim_net((net_elm **)&panod.net,fpt);
	 if(excode<(-NSLER_OO)) return((long)0);
	 declnum=(-1); chr=(*apt); dpt=panod.net->decls; indx=0;
	 while(chr=='.'||chr==','||chr=='\\'||chr=='/') {++apt; chr=(*apt);}
	 dirx=(-1);
	 if(isalpha(chr))
		{bpt=apt; while(isident(*apt)) ++apt;
		cpt=apt; chr=(*apt); *apt='\0';
		if(chr==':') {++apt; declnum=get_num(&apt);}
		while(dpt)
		  {if(strcmp(dpt->name,bpt)==0)
			 {if(declnum<0) {declnum=dpt->declnum; break;}
			  if(declnum==dpt->declnum) break;}
			dpt=dpt->tail;
		  }
		*cpt=chr;
		if(!dpt)
		  {byeerr: num=0; byenum:
			release_net((net_elm **)&panod.net); return(num);}
		if(*apt=='[')
		  {++apt;
			if(*apt=='*')
			 {if(isdir) {indx=0; dirx=(-2); ++apt;}
			  else {num=(long)dpt->size; goto byenum;}
			 }
			else {indx=get_num(&apt); dirx=indx;}
			++apt;}
		}
	 else if(isdigit(chr))
		{declnum=get_num(&apt);
		while(dpt) {if(declnum==dpt->declnum) break; dpt=dpt->tail;}
		if(!dpt) goto byeerr;
		if(*apt==':') {++apt; indx=get_num(&apt); dirx=indx;}
		else dirx=(-1);
		}
	 else if(isdir&&chr=='*') {fpt=dpt->sst_pos; dirx=(-3); goto dspelms;}
	 else goto byeerr;
	 fpt=dpt->sst_pos;
	 if(isvar)
		{if(isdir&&strcmp(apt,"\\*")==0&&!fpt) goto dspelms;
		panod.decl_pt=dpt; panod.isua=(boolean)(dpt->size==0);
		panod.indx=indx; panod.mempt=adrs;
		adrs=indx_adrs((path_elm *)&panod);
		indx=0; num=(long)dpt->decltype;
		if((!dpt->size)&&(num&STAR_MASK)) num-=STAR_ONE;
		nsl_cur_sz=lsize((int)num,dpt->tag);
		}
	 else
		{num+=dpt->nodenum+dpt->nodes*indx; ndbs=num;
		indx=(int)(dpt->decltype==TYPE_NODE);
		if(indx&&isrt)
		  {indx=0; isrt=F; isvar=num; num=(long)dpt->decltype;
			if((!dpt->size)&&(num&STAR_MASK)) num-=STAR_ONE;
			nsl_cur_sz=lsize((int)num,dpt->tag);
			adrs=netpt->npt[(int)(isvar-1)].dpt;
		  }
		}
	 if(isdir&&(chr=='*'||!fpt))
		{dspelms: ++apt; panod.decl_pt=dpt; panod.mempt=adrs;
		panod.isua=(boolean)(dpt->size==0);
		if(fpt) dirx=(-3);
		isdir(&panod,ndbs,dirx);
		indx=1;}
	 release_net((net_elm **)&panod.net);
	 if(indx) break;
	}while(*apt&&fpt);
  if(isvar) *appt=adrs;
  return(num);
}

long nsl_handle(netpt,path)
  NSLNSLnet *netpt; char *path;
{ char *adrs;
  return(nsl_num_adrs(netpt,(long)0,path,(char **)&adrs,NULL));
}

void *nsl_get_user_args(NSLNSLnet *netpt)
{ if(!netpt) netpt=mon_net;
  if(!netpt) return(NULL);
  return(netpt->args);
}

void nsl_set_user_args(NSLNSLnet *netpt,void *args)
{ if(!netpt) netpt=mon_net;
  if(!netpt) return;
  netpt->args=args;
}

/*----------------------*/
/* search for net in net_list */
NSLNSLnet *nsl_get_p(name) char *name;
{ NSLNSLnet **npt=nsl_net_list; int jj;
  jj=lst_ok(nsl_net_list);
  if(name)
	{while(jj--)
		{my_net_p=npt;
		if(*npt&&strcmp((*npt)->ins_name,name)==0) return(*npt);
		++npt;
		}
	 if(strcmp(name,"network")!=0) return(NULL);
	}
  my_net_p=NULL;
  return(NULL);
}
char **new_slot(lppt) char ***lppt;
{ char **apt,**lpt=(*lppt); int nn;
  if(lpt)
	{nn=lst_num(lpt); apt=lpt;
		 while(nn--) {if(!*apt) return(apt); ++apt;}
		}
  lpt=lmore(lpt,1,TYPE_PT,char *);
  *lppt=lpt;
  apt=lst_tail(lpt,char *); *apt=NULL;
  return(apt);
}

int nsl_install(netpt,name) NSLNSLnet *netpt; char *name;
{ NSLNSLnet **npt=nsl_net_list; int jj; my_net_p=NULL;
  /* no net to install */
  if(!netpt) bye(NSLER_NU);
  if(!name) name=netpt->name;
  if(!name) bye(NSLER_NU); /* no name given */
  jj=lst_ok(nsl_net_list);
  while(jj--) {if(*npt==netpt) bye(NSLER_OK); ++npt;}
  /* name in use already */
  if(nsl_get_p(name)) bye(NSLER_NM);
  npt=(NSLNSLnet **)new_slot(&nsl_net_list);
  my_net_p=npt;
  if(netpt!=(NSLNSLnet *)1L)
	{*npt=netpt; netpt->ins_name=name;
	 if(!(netpt->flags&NET_STUB)) netpt->flags|=NET_ACTIV;}
  bye(NSLER_OK);
}
int nsl_uninstall(name) char *name;
{ NSLNSLnet *netpt;
  if(!name) bye(NSLER_NU);
  netpt=nsl_get_p(name);
  if(!netpt) bye(NSLER_NU);
  if(my_net_p) *my_net_p=NULL;
  bye(NSLER_OK);
}

/*----------------------*/
int var_adrs(netpt,nd,path,appt)
  NSLNSLnet *netpt; long nd; char *path,**appt;
{	NSLNSLnet **npt; NSLNSLvar *list; int nn,pos,num;
	if(nd==0)
	{	if(nsl_net_list)
		{	npt=nsl_net_list; nn=lst_num(nsl_net_list);
			while(nn--)
			{	if(*npt)
				{	list=(*npt)->vpt; 
					if(list)
					{	num= *((*npt)->num_vas);
						if((pos=find_name(path,&list->name,num,sizeof(NSLNSLvar)))>0)
						{	*appt=list[pos-1].adrs; nn=list[pos-1].type_code;
							byebye: nsl_cur_sz=lsize(nn,(char *)NULL); return(nn);
						}
					}
				}
				++npt;
			}
		}
		if((pos=find_name(path,&nsl_mon_vars->name,
						nsl_num_mon_vars,sizeof(NSLNSLvar)))>0)
		{	*appt=nsl_mon_vars[pos-1].adrs; nn=nsl_mon_vars[pos-1].type_code;
			 goto byebye;
		}
		*appt=NULL; nsl_cur_sz=0; return(0);
	}
	return((int)nsl_num_adrs(netpt,nd,path,appt,NULL));
}
boolean tok(typ)
  int typ;
{ return((boolean)
	(typ&&(typ!=TYPE_STRUCT)&&(typ!=TYPE_NODE)&&(typ!=TYPE_NET))
	);
}
int nsl_write(netpt,nd,path,val)
  NSLNSLnet *netpt; long nd; char *path; double val;
{ mile typ; char *adrs;
  typ=(mile)var_adrs(netpt,nd,path,(char **)&adrs);
  if(!tok(typ)) bye(-NSLER_VA);
  nsl_lst_put(adrs,val,typ);
  bye(NSLER_OK);
}
int nsl_write_p(netpt,nd,path,valp)
  NSLNSLnet *netpt; long nd; char *path,*valp;
{ int typ; char *adrs;
  typ=var_adrs(netpt,nd,path,(char **)&adrs);
  if(!typ||valp) bye(-NSLER_VA);
  while(nsl_cur_sz--) *valp++ == *adrs++;
  bye(NSLER_OK);
}
double nsl_read(netpt,nd,path)
  NSLNSLnet *netpt; long nd; char *path;
{ mile typ; char *adrs; double val=0.0;
  typ=(mile)var_adrs(netpt,nd,path,(char **)&adrs);
  if(!tok(typ)) {nsl_err=(-NSLER_VA); return(val);}
  val=nsl_lst_val((char **)&adrs,typ);
  nsl_err=NSLER_OK;
  return(val);
}
char *nsl_read_p(netpt,nd,path)
  NSLNSLnet *netpt; long nd; char *path;
{ char *adrs;
  if(var_adrs(netpt,nd,path,(char **)&adrs)) nsl_err=NSLER_OK;
  else nsl_err=(-NSLER_VA);
  return(adrs);
}
/* Adjust a network parameter.
  vpt is a buffer containing variable & value to be adjusted
  Returns a displayable result in calling buffer: "path=XX" or "path=XX==>val"
  Syntax: path | x,path | path=val | x,path=val
  where x is a node number & path is a var path or a global var name
*/
char *nsl_adjust(netpt,bpt)
  NSLNSLnet *netpt; char *bpt;
{ long vv=0; double dd,ee; boolean hassep=F;
  char chr,*apt,*path,*val=NULL; nsl_err=NSLER_OK;
  if(!bpt) return(NULL);
  apt=bpt; path=bpt;
  while(chr=(*apt++))
	{if(chr=='=') {val=apt; *(apt-1)='\0'; break;}
	 switch(chr)
		{case ',': if(!hassep) {sscanf(bpt,"%ld",&vv); path=apt; break;}
		case ':': case '/': case '\\': case '.': hassep=T; break;
		default: ;
		}
	}
  if(hassep||vv) {if(!vv) vv=(-1);}/* a node var */
  dd=nsl_read(netpt,vv,path);
  if(nsl_err!=NSLER_OK) sprintf(bpt,"symbol error!\0");
  else
	{if(val)
		{sscanf(val,"%lf",&ee);
		nsl_write(netpt,vv,path,ee);
		sprintf(val-1,"=%lg==>%lg\0",dd,ee);
		}
	 else sprintf(apt-1,"=%lg\0",dd);
	}
  return(bpt);
}
