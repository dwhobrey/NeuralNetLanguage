/* Name: iolo.c   Type: Low level I/O based routines   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/
#include "globals.h"
#include "iolo.h"
#include "nslone.h"
#include "general.h"
#include "nslyacc.h"

extern FILE 
	*infp=NULL,*nalfp=NULL, /* input and output file pointers */
	*garfp=NULL,*monfp=NULL,*sstfp=NULL, /* substantiated file pts monitor struct info */
	*hoofp=NULL,*funfp=NULL,*tmpfp=NULL,*logfp=NULL;

extern char input_name[namesize+2]="network",
/* this stands for a NeSeL compliant C-Pre-Processor file: */
     input_ext[namesize+2]="nsp", 
     net_name[namesize+20]="N";
extern char output_name[namesize+20]="\0";
extern char *io_str=NULL; /* points to a str buff to use for io */
extern char *muddle;

extern BOOL eolon=FALSE; /* when on causes '\n' to terminate line, for yacc_parse_exp*/

char buff[buffsize+2],*buff_pt=buff,*blk_pt=buff; /* input line buffer */
char tmp_buff[argbuffsize+2],*tmp_buff_pt=tmp_buff; /* tmp buffer */

char *io_str_stack[MAX_IO_STACK]={NULL};
int io_line_stack[MAX_IO_STACK]={0};
int io_str_pt=0;

BOOL buffer_flag=FALSE; /* indicates if buffering is on */
BOOL blk_flag=FALSE; /* indicates when blanking is on */

/* Turn on chr buffering of all chrs read */
char *buffer_on()
{ tmp_buff_pt=tmp_buff; buffer_flag=TRUE;
  return(tmp_buff);
}

/* Turn off chr buffering */
char *buffer_off()
{ buffer_flag=FALSE; *tmp_buff_pt='\0';
  if(tmp_buff_pt>tmp_buff) return(tmp_buff_pt);
  return(NULL);
}

/* Blank up to cur line buff pos */
void blank_do()
{ if(blk_flag)
  while(blk_pt<buff_pt)
    {if((*blk_pt) && (*blk_pt)!='\n') *blk_pt=' ';
     ++blk_pt;}
  blk_pt=buff;
}

/* Blank on. Switch on blanking */
void blank_on()
{ blk_flag=TRUE;
}

/* Blank off. Switch off blanking */
void blank_off()
{ blk_flag=FALSE;
}

/* Blank mark. mark position in blank buff */
void blank_mark()
{ blk_pt=buff_pt; blk_flag=FALSE;
  if(blk_pt<buff) blk_pt=buff;
}

/* Out put a blanked line, compress it first though */
void blank_out()
{ BOOL lchr=TRUE; char *a_pt=buff,*b_pt=buff; blank_do();
  if(*b_pt)
    {do{
        if(*b_pt==' ' && lchr) ;
        else {*a_pt=(*b_pt); lchr=(BOOL)(*b_pt=='\n'); ++a_pt;}
        ++b_pt;
       }while(*b_pt);
     *a_pt=(*b_pt);
     if(hd_info) fputs(buff,garfp); else fputs("\n",garfp);
    }
}

/* Set input to be from string. A null string reverts input to file.
*/
void set_io_str(str)
  char *str;
{ io_str=str; buff_pt=io_str;
  if(!io_str) {buff_pt=buff; *buff_pt='\0';}
}

/* Push the current io source and state.
*/
void push_io()
{ if(io_str_pt>=MAX_IO_STACK)
    signal_error(EXIT_OSPACE,"too many io pushes!");
  io_str_stack[io_str_pt]=io_str; io_line_stack[io_str_pt++]=cur_line_num;
  io_str_stack[io_str_pt]=buff_pt;
  io_line_stack[io_str_pt++]=(int)cur_file_num;
}

void pop_io()
{ if(io_str_pt>=2)
    {buff_pt=io_str_stack[--io_str_pt];
     cur_file_num=(int)io_line_stack[io_str_pt];
     io_str=io_str_stack[--io_str_pt]; cur_line_num=io_line_stack[io_str_pt];}
  else {set_io_str(NULL); io_str_pt=0;}
}


/* Set input pos in string */
void set_io_pos(str)
  char *str;
{ if(io_str) {if(str>=io_str) buff_pt=str; }
}

/* Get the current io buffer pointer */
char *get_io_pos()
{
  return(buff_pt);
}

/* Initialise io */
void initialise_io()
{ buff_pt=buff; io_str=NULL;
  (*buff_pt)='\0';
  buffer_off(); blank_off();
}

/* Process a cpp line beginning with `#', update line_number and filename.
* cpp hash line format: # [line] integer "filename" [integer]
*/
void hash_line()
{ int temp=0,pos=(-1),oldline=0; char chr,*apt,*bpt,*wpt=buff_pt;
  buff_pt+=2; /* start of line number in a hash line */
  while(isalpha(*buff_pt)) ++buff_pt;  /* step over "line" */
  oldline=cur_line_num;
  if( sscanf(buff_pt,"%d",&temp) )
    {do{++buff_pt; chr=(*buff_pt); } while(chr!='"' && chr);
     if(chr)
       {cur_line_num=temp-1; bpt=buff_pt; ++buff_pt;
        if(*buff_pt=='.') /* step on leading '.\' */
          {*bpt=' '; ++bpt; *bpt='"'; ++buff_pt;
           if(*buff_pt=='\\' || *buff_pt=='/')
             {*bpt=' '; ++bpt; *bpt='"'; ++buff_pt;}
          }
        while(*buff_pt!='"' && *buff_pt) ++buff_pt;
        if(*buff_pt) ++buff_pt;
        apt=buff_pt;
        chr=(*apt); *apt='\0'; temp=0;
        while(temp<source_num)
          {if(strcmp(bpt,source_list[temp])==0) {pos=temp; break;}
           ++temp;
          }
        if(pos>=0)
          cur_file_num=(int)pos;
        else
          {cur_file_num=(int)source_num;
           append_list_name(&source_list,strsave(bpt),
                            &source_num,&max_sor,inc_sor); }
        *apt=chr;
        if(!hline) hline=oldline;
       }
    }
  if(!io_str)
    {if(line_info)
       {if(pretty_C)   /* make sure hash line has a "line" in it */
         {++wpt; chr= *wpt; *wpt='\0';
          if(hd_info)fputs(buff,garfp);
          *wpt=chr;
          if(chr!='l' && *(wpt+1)!='l')
            {if(hd_info)fputs("line",garfp);}
          if(hd_info)fputs(wpt,garfp);
         }
        else {if(hd_info)fputs(buff,garfp);}
       }
     else if(hd_info)fputs("\n",garfp);
     if(!hd_info) fputs("\n",garfp);
    }
}

/* Get a line of source, adjust the appropriate counters etc.
*  Returns False if at end of file.
*/
BOOL new_line()
{ if(!io_str) blank_out();
  for(;;)
    {if(io_str)
       {++buff_pt; ++cur_line_num;
        if(!(*buff_pt)) return(FALSE);}
     else
       {buff_pt=buff; cur_file_pos=fcurpos(infp);
        ++cur_line_num; ++abs_line_num;
        if(fgets(buff,buffsize,infp)==NULL) return(FALSE);}
     if((*buff_pt)=='#') hash_line(); else return(TRUE);
    }
}

/* Get a character from source file.
*  Returns next chr, or null chr if at end of file.
*/
char input_char()
{ char chr;
    do {chr=(*buff_pt);
        if(io_str) /* if inputting from a string treat \n as newline & 0=EOF*/
          {if(chr=='\n') {new_line(); break;}/*was just chr='\0';*/
           else if(!chr) break;
          }
        if(chr) {++buff_pt;  break; }
       } while(new_line());
  if(buffer_flag) {if((*tmp_buff_pt=chr)) ++tmp_buff_pt;}
  return(chr);
}

/* Unput a character back to input buffer.
*  Note cannot unput a null chr while in io_str mode.
*/
void unput_char(chr)
  char chr;
{ if(io_str)
    {if(buff_pt>io_str && chr)
       {--buff_pt; *buff_pt=chr;
        if(chr=='\n') --cur_line_num;}
     else return;
    }
  else
    {if(buff_pt>buff) --buff_pt; /* chk not unputting a null line */
     else if(chr) signal_error(EXIT_INT_ERR);
     *buff_pt=chr;}
  if(buffer_flag && tmp_buff_pt>tmp_buff && chr) --tmp_buff_pt;
}

/* Copy fil2 to fil1 */
void appendfile(fil1,fil2)
  FILE *fil1,*fil2;
{ char buffer[bigbuffsize];
  fflush(fil2); fflush(fil1);
  fmoveto(fil1,0); fmoveto(fil2,0);
  while(fgets(buffer,bigbuffsize,fil2)) fputs(buffer,fil1);
}

/* Open files:
*  infp  <- "progname.progext"  -- source to compile
*  sstfp <- "progname.nss"  -- substantiated modules
*  hoofp <- "progname.nsh/nsc"  -- C code output file for monitor & simulator
*  monfp
*  funfp <- "nslusrfn.c"  -- file for user func info for nsl2
*  tmpfp <- "progname.nsz" -- tmp file, not needed after nsl1
*/
void openfiles()
{ char filename[namesize+10],
    *apt="char *NSL%c=\"NeSeL(u)v%c.%c(DJRW)1989:UserFuncs\";\n";
  sprintf(filename,"%s.%s\0",input_name,input_ext);
/* NOTE Msdos fseek works incorrectly when filesize<512 & "r" open mode, so..*/
  if( (infp=fopen(filename,"r+"))==NULL )
    {byebye: signal_error(EXIT_BADFILE,filename);}
  if(!*output_name) strcpy(output_name,input_name);
  if(logerrs)
    {sprintf(filename,"%s.log\0",output_name);
     if( (logfp=fopen(filename,"w"))==NULL ) {logfp=stdout; goto byebye;}}
  /* we open this file just in case a header pragma is used */
  sprintf(filename,"%s.nsy\0",output_name);
  if((garfp=fopen(filename,"w+"))==NULL ) goto byebye;
  if(mx_info) fcat(filename,"nslmonfn.c\0");
  else
    {sprintf(filename,"%s.nsh\0",output_name);
     if((hoofp=fopen(filename,"w"))==NULL) goto byebye;
     sprintf(filename,"%s.nsc\0",output_name);
    }
  if( (monfp=fopen(filename,"w"))==NULL ) goto byebye;
  if(mx_info)
    {fprintf(monfp,apt,'M',version_num,version_upd); hoofp=monfp;}
  sprintf(filename,"%s.nss\0",output_name);
  if( (sstfp=fopen(filename,"w+"))==NULL ) goto byebye;
  if(func_info)
    {sprintf(filename,"nslusrfn.c\0");
     if( (funfp=fopen(filename,"w"))==NULL ) goto byebye;
     fprintf(funfp,apt,'F',version_num,version_upd);
    }
  if(monitor)
   {sprintf(filename,"%s.nsz\0",output_name);
    if( (tmpfp=fopen(filename,"w+"))==NULL ) goto byebye;
   }
}

extern void add_kludge(char *name);

/* Pick up command line args
*/
void getmainargs(argc,argv)
  int argc; char *argv[];
{ int argnum=1,chrs; char *chrpt;  line_info=TRUE; var_info=FALSE; core_def=FALSE;
  pretty_C=FALSE; monitor=FALSE; func_info=FALSE; logerrs=FALSE; fsym_info=FALSE; mx_info=FALSE;
  add_info=FALSE; time_info=FALSE; node_info=FALSE; group_info=FALSE; conx_info=FALSE; hd_info=FALSE;
  hd_info=FALSE;
  while(argnum<argc)
    {chrpt=argv[argnum];
     if(*chrpt=='-') /* read in '-chars' command line options */
       {++chrpt;
        while(*chrpt)
        switch(*chrpt++)
          {case 'a': /* add additional information to node structs */
             add_info=TRUE; time_info=TRUE; node_info=TRUE; group_info=TRUE; conx_info=TRUE;
             break;
           case 'b': /* add node information to node structs */
             add_info=TRUE; node_info=TRUE; break;
           case 'c': /* add conx size information to node structs */
             add_info=TRUE; conx_info=TRUE; break;
           case 'f': /* produce user function details for nsl2 */
             func_info=TRUE; hd_info=TRUE; break;
           case 'g': /* add group information to node structs */
             /* NOTE node_info must be set if group_info is set */
             add_info=TRUE; group_info=TRUE; node_info=TRUE; break;
           case 'h': /* extract header info from source file */
             hd_info=TRUE; break;
           case 'l': /* log errors to log file */
             logerrs=TRUE; break;
           case 'm': /* produce monitor info */
             monitor=TRUE; add_info=TRUE; conx_info=TRUE; break;
           case 'n': /* give net a name */
             if(!*chrpt) argnum++;
             if(argnum<argc) sscanf(argv[argnum],"%40s",net_name);
             break;
           case 'o':
             if(!*chrpt) argnum++;
             if(argnum<argc) sscanf(argv[argnum],"%40s",output_name);
             break;
           case 'p': /* make C output acceptable to cpp again */
             pretty_C=TRUE; line_info=TRUE; break;
           case 'q': /* leave out line number info */
             line_info=FALSE; break;
           case 'r': /* add timing information to node structs */
             add_info=TRUE; time_info=TRUE; break;
           case 's': /* produce user symbol details for monitor */
             fsym_info=TRUE; monitor=TRUE; add_info=TRUE; conx_info=TRUE; break;
		   case 'U': /* add a name to kludge list */
				add_kludge(chrpt); chrpt+=strlen(chrpt);
			 break;
           case 'v': /* add var info to .nsc file */
             monitor=TRUE; var_info=TRUE; break;
           case 'x': /* extract details for 'nslmonfn.c' */
             mx_info=TRUE; hd_info=TRUE; break;
           case 'y': /* define core lists for '*.nsc' */
             core_def=TRUE; break;
           default: ; /* ignore unknown args, might be for nsl2 */
       }  }
      else /* get input file name from command line */
        {chrs=0;
         while((*chrpt!=(char)0) && (*chrpt!='.') && (chrs<namesize) )
           {input_name[chrs]=(*chrpt);
            chrpt++; chrs++; input_name[chrs]=(char)0; }
         if(*chrpt=='.') sscanf(chrpt,".%40s",input_ext);
        }
      argnum++;
     }
  if(mx_info||func_info) {core_def=FALSE; muddle="mon";} else muddle=net_name;
}

/* Read in NeSeL source module (i.e. net or neuron decl) to mem buff.
*  terminates it with a null. User should free mem when done.
*  Hash lines complicate things a bit.
*  Sets cur_mod_size to module size in bytes.
*/
char *get_module(mod_pt,sizept)
  stack_elm *mod_pt; int *sizept;
{ char *base_pt=NULL,*ap, *line_pt=NULL; unsigned int some_bytes=0;
  int last_line; *sizept=0;
  if(!mod_pt) return(NULL);
  fmoveto(infp,mod_pt->file_pos);
  if(mod_pt->file_pos!=fcurpos(infp)) signal_error(EXIT_FILE,"source");
  cur_line_num=mod_pt->line_num;
  cur_file_num=mod_pt->file_num;
  last_line=cur_line_num+mod_pt->num_lines;
  while(cur_line_num<last_line)
    {if((*sizept+buffsize)>(int)some_bytes)
       {some_bytes=(unsigned int)(*sizept+2*buffsize+10);
        /*MSC has a problem with realloc DEBUG */
        if(!base_pt) base_pt=(char *)malloc(some_bytes);
        else base_pt=(char *)realloc(base_pt,some_bytes);
       }
     if(!base_pt)
       {signal_error(ERR_MOD_MEM,(char *)&mod_pt->name); return(NULL);}
     line_pt=base_pt+(*sizept);
     fgets(line_pt,buffsize,infp);
     if(*line_pt=='#') /* allow for hash line */
       {ap=line_pt+1; while(isalpha(*ap)) ++ap; sscanf(ap,"%d",&cur_line_num); --cur_line_num;}
     else ++cur_line_num;
     (*sizept)+=strlen(line_pt);
    }
  if(base_pt)
    {*(base_pt+(*sizept))='\0'; cur_line_num=mod_pt->line_num;}
  error_mark();
  return(base_pt);
}

/* Skip over white space.
*  Returns first non white space chr found
*/
char skip_blanks()
{ char chr;
   for(;;)
    {switch(chr=input_char())
       {case ' ': /* white space */
        case '\t': break;
        case '\n': if(eolon) return('\0'); break;
        default: return(chr);
       }
    }
}

/* Skip anything in single or double quotes.
*  Returns first no quoted chr. Newline and EOF terminate it as well.
*/
char skip_quo(flag)
  int flag;
{ char chr='\n',laschr;
  BOOL dquote=FALSE,squote=FALSE,spaces=FALSE;
  do{spaces=FALSE;
     /* note uses input_char() since '\n' will terminate a quoted string */
     switch(laschr=chr, chr=input_char())
      {case '\n': if(laschr!='\\') /*in case line cont*/
                    {dquote=FALSE; squote=FALSE;
                     if(flag) return(chr);}
                  else if(buffer_flag)
                    {if(tmp_buff_pt>tmp_buff) tmp_buff_pt-=2;}
       case ' ':
       case '\t': spaces=TRUE; break;
       case '\0': return(chr);
       case '"':
         if(!squote && laschr!='\\') dquote=(BOOL)!dquote;
         break;
       case '\'':
         if(!dquote && laschr!='\\') squote=(BOOL)!squote;
         break;
       default: ;
      }
  }while(spaces || dquote || squote);
  return(chr);
}

char skip_quoted()
{ return(skip_quo(FALSE));
}

char skip_quoln()
{ return(skip_quo(TRUE));
}

/* Skip over a matching pair.
*  Returns first chr if not lpair, or rpair if found. */
char skip_pair(lpair,rpair)
  char lpair,rpair;
{ int pair=0; char chr;
  do {
     chr=skip_quoted();
     if(chr==lpair) ++pair;
     else if(chr==rpair) --pair;
  } while(pair && chr);
  if(!chr) signal_error(ERR_BRAK,(int)rpair);
  return(chr);
}

/* Skip anything inside quote, brackets etc.
*  Returns first chr if ok, or rpair. */
char skip_bodies()
{ char chr;
  for(;;)
    switch(chr=skip_quoted()){
      case '{': unput_char(chr); return(skip_pair('{','}'));
      case '[': unput_char(chr); return(skip_pair('[',']'));
      case '(': unput_char(chr); return(skip_pair('(',')'));
/*      case '<': unput_char(chr); return(skip_pair('<','>')); */
      default: return(chr);
    }
}

char skip_over()
{ char chr;
  for(;;)
    {chr=skip_blanks();
     switch(chr)
       {case '{': unput_char(chr);
          chr=skip_pair('{','}');
          if(chr!='}') return(chr);
          break;
        case '[': unput_char(chr);
          chr=skip_pair('[',']');
          if(chr!=']') return(chr);
          break;
        case '(': unput_char(chr);
          chr=skip_pair('(',')');
          if(chr!=')') return(chr);
          break;
        default: return(chr);
       }
     }
}


/* Skip to list separator.
*  Looks for a comma or semicolon on same level, or EOF.
*/
char skip_to_sep()
{ char chr;
  do chr=skip_bodies(); while(chr!=',' && chr!=';' && chr);
  return(chr);
}

/* Skip to semicolon at same level.
*/
char skip_to_scolon()
{ char chr;
  do chr=skip_bodies(); while (chr!=';' && chr);
  return(chr);
}

/* Skip to left curly brace at same level.
*/
char skip_to_brace()
{ char chr;
  do{
    chr=skip_quoted();
    if(chr=='[' || chr=='(' /*|| chr=='<'*/)
      {unput_char(chr); chr=skip_bodies();}
    }while (chr!='{' && chr);
  return(chr);
}

/* Look for a pair of curly braces and skip.
*  Returns first or EOF.
*/
char skip_braces()
{ char chr;
  do {
    if((chr=skip_bodies())=='}') return(chr);
  } while(chr);
  return(chr);
}

/* skip over bracketed aux defs in opath */
char *skip_auxs(ostr)
  char *ostr;
{ char chr;
  push_io(); set_io_str(ostr);
  do{chr=skip_pair('(',')'); if(chr!=')') unput_char(chr);
     ostr=get_io_pos();
    }while(*ostr=='(');
  pop_io();
  return(ostr);
}

/* Skip to cast type separator, or terminator */
char skip_to_cast(tchr)
  char tchr;
{ char chr,lchr;
  do {lchr=skip_blanks(); if(lchr==tchr) return(tchr);
      unput_char(lchr); chr=skip_bodies();
      if(lchr=='(' && chr==')') chr=lchr;
     } while(chr!=',' && chr!=')' && chr!=';' && chr);
  return(chr);
}

char skip_to_rbrk()
{ char chr,lchr;
  do {lchr=skip_blanks();
      unput_char(lchr); chr=skip_bodies();
      if(lchr=='(' && chr==')') chr=lchr;
     } while(chr!=')' && chr);
  return(chr);
}

/* Get a valid identifier, stores it in cur_ident,
*  Returns null (false) if not valid.
*/
char get_ident()
{ int n=max_ident-2; char *id_pt=cur_ident, chr=skip_blanks();
  if(chr) error_mark(); /* mark this point */
  if((isalpha(chr) || chr=='_'))
    {*id_pt=chr;
     do
       {chr=input_char();
        if(n) {--n; ++id_pt; *id_pt=chr; }
       } while(isident(chr));
    }
  unput_char(chr);
  *id_pt='\0';
  return(*cur_ident);
}

char stocom()
{ char chr,lchr;
  do {lchr=skip_blanks(); if(lchr==',') return(',');
      unput_char(lchr); chr=skip_bodies();
      if(lchr=='(' && chr==')') chr=lchr;
     } while(chr!=')' && chr);
  return(chr);
}

/* Pick out a macro arg from list, chop off leading/trailing blanks.
*  Returns pt to arg in tmp buffer.
*  If islast treats remaining args as one
*/
char *get_arg(islast) BOOL islast;
{ char chr,*arg_beg,*arg_end;
  chr=skip_blanks(); unput_char(chr);
  arg_beg=buffer_on();
  do {chr=stocom();} while(islast&&chr==',');
  unput_char(chr);
  arg_end=buffer_off();
  if(arg_end) --arg_end;
  else while(arg_end>=arg_beg
        && (*arg_end==' ' || *arg_end=='\t' || *arg_end=='\n'))
    {*arg_end='\0'; --arg_end;}
  arg_end=arg_beg;
  while(*arg_end) {if(*arg_end=='\n') *arg_end=' '; ++arg_end;}
  return(arg_beg);
}

/* Replace chrs in iostr with str, adjust iopos accordingly.
*  beg_pt is start of spot to replace with str in iostr.
*  top_pt is top of iostr buffer, cur iopos is next chr to get.
*/
BOOL replace_str(beg_pt,str,top_pt)
  char *beg_pt,*str,*top_pt;
{char *at_pt; int qchrs;
 at_pt=get_io_pos(); qchrs=strlen(str)-(at_pt-beg_pt);
 if(make_hole(at_pt,qchrs,top_pt))
   {set_io_pos(at_pt+qchrs);
    at_pt=beg_pt; while(*str) {*at_pt= *str; ++str; ++at_pt;}
    return(TRUE);}
 return(FALSE);
}
