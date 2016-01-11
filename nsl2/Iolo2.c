/* Name: iolo2.c   Type: Low level I/O based routines   Date: 30 March 89
*  NeSeL Neural Net Specification Language Preprocessor
*/
/* these routines deal with input from strings only */

#include "globals2.h"
#include "iolo2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"

#define argstr1 "%40s" /* this 40 is the value of namesize */
#define argstr2 ".%40s"

#define max_io_stack 40

char
  *buff_pt, /* cur io pos */
  *io_str, /* base of cur io str (unput lower limit) */
  *io_str_stack[max_io_stack], /* for pushing & poping io pos */
  cur_ident[max_ident], /* current ident str */
  input_name[namesize+2]="network",
  output_name[namesize+20]="\0";

int io_str_pt=0;

/* Initialise io */
void io_initialise() {io_str=NULL; buff_pt=NULL;}

/* Set input to be from string */
void set_io_str(str)
  char *str;
{ buff_pt=str; io_str=str;}

/* Set input pos in string */
void set_io_pos(str)
  char *str;
{ if(io_str) {if(str>=io_str) buff_pt=str;}
}

/* Get the current io buffer pointer */
char *get_io_pos() {return(buff_pt);}

void push_io()
{ if(io_str_pt>=max_io_stack)
    signal_error(EXIT_OSPACE,"too many io pushes");
  io_str_stack[io_str_pt++]=io_str; io_str_stack[io_str_pt++]=buff_pt;
}

void pop_io()
{ if(io_str_pt>=2)
    {buff_pt=io_str_stack[--io_str_pt]; io_str=io_str_stack[--io_str_pt];}
  else {set_io_str((char *)NULL); io_str_pt=0;}
}

/* Get a character from source str.
*  Returns next chr, or null chr if at end of str.
*/
char input_char()
{ char chr;
  if(!buff_pt) signal_error(EXIT_INT_ERR,156);
  if((chr=(*buff_pt))) ++buff_pt;
  return(chr);
}

/* Unput a character back to input buffer.
*  Note cannot unput a null chr.
*/
void unput_char(chr)
  char chr;
{ if(buff_pt>io_str && chr) {--buff_pt; *buff_pt=chr;}
}

/* Skip over white space.
*  Returns first non white space chr found
*/
char skip_blanks()
{ char chr;
   for(;;)
     switch(chr=input_char()){
       case ' ': /* white space */
       case '\t':
       case '\n': break;
       default: return(chr);
     }
}

/* Skip anything in single or double quotes.
*  Returns first no quoted chr. Newline and EOF terminate it as well.
*/
char skip_quoted()
{ char chr='\n',laschr;
  boolean dquote=F,squote=F,spaces=F;
  do
    {spaces=F;
     /* note uses input_char() since '\n' will terminate a quoted string */
     switch(laschr=chr, chr=input_char()){
       case '\n': dquote=F; squote=F;
       case ' ':
       case '\t': spaces=T; break;
       case '\0': return(chr);
       case '"':
         if(!squote && laschr!='\\') dquote= (boolean)!dquote;
         break;
       case '\'':
         if(!dquote && laschr!='\\') squote= (boolean)!squote;
         break;
       default: ;
    }
  } while(spaces || dquote || squote);
  return(chr);
}

/* Skip over a matching pair.
*  Returns first chr if not lpair, or rpair if found.
*/
char skip_pair(lpair,rpair)
  char lpair,rpair;
{ int pair=0; char chr;
  do {
     chr=skip_quoted();
     if(chr==lpair) ++pair;
     else if(chr==rpair) --pair;
  } while(pair && chr);
  return(chr);
}


/* Skip anything inside quote, brackets etc.
*  Returns first chr if ok, or rpair.
*/
char skip_bodies()
{ char chr;
  for(;;)
    switch(chr=skip_quoted()){
      case '{': unput_char(chr);
        return(skip_pair('{','}'));
      case '[': unput_char(chr);
        return(skip_pair('[',']'));
      case '(': unput_char(chr);
        return(skip_pair('(',')'));
      case '<': unput_char(chr);
        return(skip_pair('<','>'));
      default: return(chr);
    }
}

/* Skip to colon at same level*/
char skip_to_colon()
{ char chr;
  do chr=skip_quoted(); while (chr!=':' && chr);
  return(chr);
}

/* when reading in decl arg vars, skip to next */
char skip_to_dec()
{ char chr,lchr;
  do {lchr=skip_blanks();
      unput_char(lchr); chr=skip_bodies();
      if(lchr=='(' && chr==')') chr=lchr;
     } while(chr!=',' && chr!=')' && chr);
  return(chr);
}

/* when reading in a var list, skip to end of element initialiser */
char skip_to_var()
{ char chr,lchr;
  do {lchr=skip_blanks(); unput_char(lchr); chr=skip_bodies();
      if(lchr=='{' && chr=='}') chr=lchr;
     } while(chr!=',' && chr!='}' && chr);
  return(chr);
}

/* Get a valid identifier, stores it in cur_ident,
*  Returns null (false) if not valid.
*/
char get_ident()
{ int n=max_ident-2; char *id_pt=cur_ident, chr=skip_blanks();
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

/* read in a ascii long value, convert to binary */
long get_num(strpt,term)
  char **strpt,term;
{ long val=0; char *apt= *strpt;
  if(apt)
    {while(*apt>='0') {val=val*10+(*apt)-'0'; ++apt;}
     while(*apt && *apt!=term) ++apt;
     *strpt=apt;
    }
  return(val);
}

/* find end of next id, may be wild */
char *get_wild_id(str,iswild)
  char *str; boolean *iswild;
{ *iswild=F;
  for(;;)
    {switch(*str)
       {case '\0': case '|': case '\\': case '/': case '.':
        case '[': case '(': case '{':  case '<':
        return(str);
        case '?': case '*': *iswild=T;
        default: ++str;
    }  }
}

/* Open files:
*  sstfp <- "progname.nss"  -- substantiated modules
*  nalfp <- "progname.nso"  -- compiled net in pseudo object format
*  dsifp <- "progname.nsz"  -- tmp file, index into nalfp
*/
void openfiles()
{ char filename[namesize+10];
  if(!*output_name) strcpy(output_name,input_name);
  if(opt_logerr)
    {sprintf(filename,"%s.log\0",output_name);
     if( (logfp=fopen(filename,"a"))==NULL ) {logfp=stdout; goto byebye;}}
  sprintf(filename,"%s.nss\0",input_name);
  if( (sstfp=fopen(filename,"r"))==NULL )
    {byebye: signal_error(EXIT_BADFILE,filename); }
  sprintf(filename,"%s.nso\0",output_name);
  if( (nalfp=fopen(filename,"wb+"))==NULL ) goto byebye;
  sprintf(filename,"%s.nsz\0",output_name);
  if( (dsifp=fopen(filename,"wb+"))==NULL ) goto byebye;
}

/* Pick up command line args */
void getmainargs(argc,argv)
  int argc; char *argv[];
{ char *chrpt; int argnum=1,chrs;
  opt_logerr=F; err_filter=T; mod_ison=F; bookeep=T;
  warn_limit=WRN_MAX;
  while(argnum<argc)
    {chrpt=argv[argnum];
     if(*chrpt=='-' || *chrpt=='/') /* read in '-chars' command line options */
       {++chrpt;
        while(*chrpt)
        switch(*chrpt++)
          {case 'e': /* switch on full error reporting, do not filter */
             err_filter=F; break;
           case 'j': /* switch on flasher */
             mod_ison=T; break;
           case 'k':  /* turn off book keeping & other slow things */
             bookeep=F;
             break;
           case 'l': /* log errs to log file */
             opt_logerr=T;
             break;
           case 'o':
             if(!*chrpt) argnum++;
             if(argnum<argc) sscanf(argv[argnum],argstr1,output_name);
             break;
           case 'u': /* set number of ds buffers */
              ds_sea_max=0;
              while(*chrpt)
                {ds_sea_max=(short)(10*ds_sea_max+((*chrpt)&0xf)); ++chrpt;}
              if(ds_sea_max<2) ds_sea_max=2;
              break;
           case 'z': /* set number of string buffers */
              str_cir_max=0;
              while(*chrpt)
                {str_cir_max=(mile)10*str_cir_max+((*chrpt)&0xf); ++chrpt;}
              if(str_cir_max<2) str_cir_max=2;
              break;
           case 'w': /* switch off warnings */
             warn_limit=WRN_BASE;
             break;
           case 'W': /* set warning level */
             if(*chrpt) switch(*chrpt++)
               {case '0': warn_limit=WRN_TWO; break;
                case '1': warn_limit=WRN_THREE; break;
                case '2': warn_limit=WRN_FOUR; break;
                default: warn_limit=WRN_MAX;
               }
             break;
           default: ; /* ignore unknown args, might be from nsl1 */
       }  }
      else /* get input file name from command line */
        {chrs=0;
         while((*chrpt!=(char)0) && (*chrpt!='.') && (chrs<40) )
           {input_name[chrs]=(*chrpt);
            chrpt++; chrs++; input_name[chrs]=(char)'\0'; }
        }
      argnum++;
     }
}
