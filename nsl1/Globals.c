/* Name: globals.c   Type: general c routines   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/

#include <varargs.h>
#include "globals.h"
#include "iolo.h"
#include "nslone.h"
#include "general.h"

int firsterr=1;

struct error_elm {int error_num; char *error_str;};
struct error_elm error_list[]={
{WRN_PAR_MUL ,"multiply defined labels in module '%s'"},
{WRN_PAR_UDS ,"undefined labels/modules in module '%s'"},
{WRN_HASH, "'#include' file inserted at a weird place? expect problems..."},
{WRN_SEL_AND,"selector(s) and address op '&' used in var exp"},
{WRN_OBJ_SIZ,"object '%s' size cannot be less than 1, taking 1 ok?"},
{WRN_OBJ_ARG,"no name for object argument initialiser '%s', ignoring ok?"},
{WRN_VAR_NAME,"name expected in variable declaration(%s)"},
{WRN_VAR_SYN,"var declaration syntax suspect(%s)"},
{WRN_NUL_STRUC,"{}, null struct initialiser found"},
{WRN_TYP_MUL ,"typedef label multiply defined:%s"},
{WRN_NOD_STAR,"pointers to node type structs not allowed(%s)"},
{WRN_RESYN,"unable to recover from previous error..."},
{WRN_TYP_UNK,"undefined type '%s', treating as int ok?"},
{WRN_MAC_UNX,"didn't expand macro '%s', treating as an ident ok?"},
{ERR_PAR ,"yacc couldn't parse it correctly"},
{ERR_ARG ,"too many args to function"},
{ERR_MEM ,"not enough memory to install function"},
{ERR_FILE ,"access to %s file went wrong"},
{ERR_MESS,"syntax error: %s"},
{ERR_SEMIC ,"';' expected"},
{ERR_MACRO ,"unable to macro expand, buffer size too small"},
{ERR_TAG ,"struct defined in node def, only tags permitted"},
{ERR_BRAK,"EOF encountered looking for closing bracket '%c'"},
{ERR_VAR_FIX ,"bad declaration array constant expression(%s), taking 1 ok?"},
{ERR_VAR_BIT ,"bad bit-field expression(%s), taking 1 ok?"},
{ERR_VAR_RSB,"']' expected in array declaration(%s)"},
{ERR_BUF_OVR ,"node '%s' buffer overflow"},
{ERR_COM_DIS ,"node %s's internal info has disappeared!?"},
{ERR_NER_SYN ,"syntax error while parsing node def: '}' or ';' expected"},
{ERR_NER_ID, "node name expected in node def"},
{ERR_TYP_SYN ,"'}' expected in typedef"},
{ERR_TYP_UDF ,"undefined type '%s'"},
{ERR_TYP_ID, "name expected in type declaration"},
{ERR_RED_WORD ,"attempt to redefine reserved word or prior identifier '%s'"},
{ERR_ARY_AND,"address operator '&' not allowed in path lists, taking [*] ok?"},
{ERR_APP_ARY,"terminal list not allowed in application path"},
{ERR_MOD_MEM ,"unable to load module '%s', not enough mem?"},
{ERR_MOD_BRK ,"missing bracket, '(' or ')', in module"},
{ERR_MOD_TYP ,"missing module '%s' type declaration"},
{ERR_MOD_COMM,"syntax error in module '%s' parameter list"},
{ERR_PAR_NET ,"syntax error in net module:"},
{ERR_PAR_ARY ,"syntax error in array:"},
{ERR_PAR_FUN ,"syntax error in node function initialisers:"},
{ERR_PAR_STU ,"syntax error in struct initialisers:"},
{ERR_PAR_COM ,"syntax error in communications details:"},
{ERR_PAR_NOP ,"syntax error in node parameters:"},
{ERR_SEC_REC ,"level of recursively defined module '%s' too deep"},
{ERR_NUL_NET ,"null net '%s' only allowed when used as an aux net"},
{ERR_STU_DIS ,"struct '%s' has disappeared!"},
{ERR_STU_ERR ,"struct error, weird internal error"},
{ERR_STU_REC ,"struct '%s' recursively defined"},
{ERR_STU_UDS ,"struct '%s' undefined"},
{ERR_STU_SEM ,"';' expected after struct def"},
{ERR_MAC_SYN ,"macro definition syntax err: name expected"},
{ERR_MAC_RBR ,"'}' expected in macro def"},
{ERR_USR_ARG,"user func(%s) arg either undefined or inconsistently declared"},
{ERR_USR_UID,"user func(%s) arg undefined '%s'"},
{ERR_USR_MID,"user func(%s) arg multiply defined '%s'"},
{ERR_USR_BRK,"')' expected in user func(%s) declaration"},
{ERR_USR_PARMS,"too many arguments to user func(%s)"},
{EXIT_INT,"Exit on interrupt"},
{EXIT_BADFILE,"@Unable to open input/output file:%s"},
{EXIT_OSPACE,"Ran out of memory space:%s"},
{EXIT_ROOT,"Root net 'network' not found"},
{EXIT_FILE,"Access to %s file went wrong"},
{EXIT_INT_ERR,"Internal error?!"},
{EXIT_TYP_EOF ,"EOF encountered in typedef: ',' or ';' expected"},
{EXIT_TYP_LBR ,"EOF encountered in struct: '{' expected"},
{EXIT_TYP_RBR ,"EOF encountered in struct: '}' expected"},
{EXIT_INITIAL ,"Could not initialise complier"},
{EXIT_SUBLVL ,"Too many sublevels in struct"},
{EXIT_SALOVR ,"Too many lists within list"},
{EXIT_ERRS, NULL},
{EXIT_UNDEF ,NULL},
{ERR_OK	,"no error... "},
{0,NULL}};

void sayline()
{ if(err_file_num<0 || source_num==0) fprintf(logfp,"?(?): ");
  else 
  {/* chop off surrounding double-quotes */
	  char *a=source_list[err_file_num],*b=a+strlen(a)-1,chr;
	  chr=(*b); *b='\0'; 
	  fprintf(logfp,"%s(%d) : ",a+1,err_line_num);
	  *b=chr;
  }
}

/* Main parser syntax error handling routine.
*/
void raw_error(args)
  va_list args;
{ int error_no,nn=0,cur_error=1,ma; BOOL skip=FALSE;
  char *ept,*sa,*sb;
  if(firsterr)
    {firsterr=0;
     fprintf(logfp,version,version_num,version_upd);
     fprintf(logfp,"%s\n",input_name);}
  error_no=va_arg(args, int);
  do{if((cur_error=error_list[nn].error_num)==error_no) break;
     ++nn;
    }while(cur_error);
  if(TRUE)
    {if(cur_error==error_no)
       {ept=error_list[nn].error_str;
        if(ept)
          {if(*ept=='@') ++ept; else sayline();
           if(cur_error>=WRN_BASE) fprintf(logfp,"warning ");
		   else fprintf(logfp,"error ");
           fprintf(logfp,"N1%d: ",error_no);
           switch(error_no)
             {case ERR_BRAK:
                ma=va_arg(args,int);
                fprintf(logfp,ept,ma);
                break;
              case ERR_USR_PARMS: case ERR_USR_ARG:
              case ERR_USR_UID: case ERR_USR_MID: case ERR_USR_BRK:
                sa=va_arg(args, char *); sb=va_arg(args, char *);
                fprintf(logfp,ept,sa,sb);
                break;
              case EXIT_BADFILE: case EXIT_OSPACE: case EXIT_FILE:
              case ERR_MESS: case ERR_RED_WORD: case ERR_MOD_MEM:
              case ERR_STU_UDS: case ERR_TYP_UDF: case ERR_STU_REC:
              case ERR_VAR_FIX: case ERR_VAR_BIT: case ERR_VAR_RSB:
              case ERR_FILE: case ERR_STU_DIS: case ERR_MOD_COMM:
              case ERR_COM_DIS: case ERR_SEC_REC: case ERR_BUF_OVR:
              case ERR_NUL_NET: case WRN_TYP_UNK: case WRN_MAC_UNX:
              case WRN_VAR_NAME: case WRN_VAR_SYN: case WRN_OBJ_SIZ:
              case WRN_TYP_MUL: case WRN_OBJ_ARG: case ERR_MOD_TYP:
              case WRN_PAR_MUL: case WRN_PAR_UDS: case WRN_NOD_STAR:
                sa=va_arg(args, char *);
                fprintf(logfp,ept,sa);
                break;
              default: fprintf(logfp,ept);
             }
           fputc('\n',logfp);
       }  }
     else
       {sayline(); cur_error=EXIT_ERRERR;
        fprintf(logfp,"Unknown error number:%d?\n",error_no);
       }
    }
  if(cur_error<WRN_BASE)
    {raise_error=cur_error; ++num_errors; nss_ok=0;
     if(num_errors>100)
       {cur_error=EXIT_ERRS;
        fprintf(logfp,"Number of errors exceeded cut-off limit:%d\n",
                num_errors);
       }
    }
  if(cur_error<ERR_BASE)
    {if(cur_error>EXIT_OK) fprintf(logfp,"Can't continue. Bye!\n");
     exit(cur_error);}
}

/* just display error */
void signal_error(va_alist)
  va_dcl
{ va_list args; va_start(args); raw_error(args); va_end(args);
}

/* See if char is a valid identifier char.
*/
BOOL isident(c)
  char c;
{ return ((BOOL)(isalnum(c) || c=='_'));
}

/* Save string, if enough mem 
*/
char *strsave(s)
  char *s;
{ char *p;
  unsigned int len=strlen(s)+1;
  if(!s) signal_error(EXIT_INT_ERR);
  if( (p=malloc(len)) != NULL) strcpy(p,s);
  else signal_error(EXIT_OSPACE,"saving string");
  return(p);
}

/* Catenate two strings fast.
*  Returns end of catenated string.
*/
char *fcat(a,b)
  register char *a,*b;
{ if(!a) signal_error(EXIT_INT_ERR);
  if(b) while(*b) *(a)++= *(b)++; *(a)='\0';
  return(a);
}

/* Enlarge and array.
*  Returns TRUE if successful
*/
int enlarge_array(basept,elm_size,max_size,num_elms)
  char **basept; unsigned int elm_size; int *max_size,num_elms;
{ unsigned int some_bytes;
  *max_size+=num_elms; 
  some_bytes=(*max_size)*elm_size;
  /* MSC has a a problem with realloc */
  if(!*basept) *basept=(char *)malloc(some_bytes);
  else *basept=(char *)realloc(*basept,some_bytes);
  if(!*basept) signal_error(EXIT_OSPACE,"enlarging array");
  return(*basept!=NULL);
}

/* Look for mask in string.
*  Returns mask position in string, 0 if not found */
int maskinstr(mask,str)
  char *mask,*str;
{ int j,i=0; char chr;
  while(str[i]) /* test "(l|?)*" = "str" */
   {j=0;
    while(chr=mask[j])
     {if(chr=='?' || chr==str[i+j]) j++;
      else break; }
    i++;
    if(!chr) return(i);
   }
  return(0);
}

/* See if string matches pattern.
*  str is a pure ident (_|l|d), pattern is a wild type of ident
*  Returns TRUE or FALSE.
* mask --> (l | ?)*
* pattern --> *mask | mask* | mask | *
* template --> pattern | pattern.template
*/
BOOL match(pattern,str)
  char pattern[],str[];
{ int j,i=(int)strlen(pattern); char chr;
  if(pattern[0]=='*')		/* test "*mask" == "str" */
    { if(!pattern[1] || pattern[1]=='*') return(TRUE); /* "*" */
      if(j=maskinstr((char *)pattern+1,str))
        return((BOOL)(str[j+i-2]=='\0'));
      return(FALSE);
    }
  else if(i==0) return(FALSE);
  j=i; chr='\0'; /* test "mask*" == "str" */
  if(pattern[j-1]=='*') {j--; pattern[j]=chr; chr='*';}
  else if(j!=(int)strlen(str)) return(FALSE);
  i=maskinstr(pattern,str);
  pattern[j]=chr;
  return((BOOL)(i==1));
}

/* work out an id's comms code by consulting comms_list */
char comm_code(id)
  char *id;
{ int kk; char ccode=COMM_NONE,*apt,**cppt=comms_list;
  if(comms_num<1) return(COMM_INOUT);
  for(kk=0;kk<comms_num;++kk) /* a linear search cause we must test all */
     {apt=(*cppt); ++cppt;
      if(match(apt+1,id)) ccode|=(*apt);
     }
  return(ccode);
}

/* Find name str in sorted list of names.
*  Returns pos+1, or -(closest pos) if not found.
*/
int find_name(name,list,hi)
  char *name, **list; int hi;
{ int lo=0,test=0,oldtest,temp;
  if(hi<1) return(0); /* user could have garbage in list even if no elms */
  do {oldtest=test; test=(hi+lo)/2;
    if((temp=strcmp(name,list[test]))==0) return(test+1);
    if(temp<0) hi=test;  else lo=test;
  } while(test!=oldtest);
  return(-hi);
}

/* Search for name str in list of names.
*  Returns pos+1, or -(hi) if not found.
*/
int search_name(name,list,hi)
  char *name, **list; int hi;
{ int oldhi=hi;
  while(hi>0)
   {--hi; if(strcmp(name,list[hi])==0) return(hi+1); }
  return(-oldhi);
}


/* Make hole in list for new element.
*  This could be speeded up with memmove().
*/
void insert_hole(pos,list,num,amaxpt,ainc)
  int pos,num,*amaxpt,ainc; char ***list;
{
  if(num>=(*amaxpt-2))
    {if(!enlarge_array(list,sizeof(char *),amaxpt,ainc))
      signal_error(EXIT_OSPACE,"enlarging array");
    }
  while(num>pos) {*((*list)+num)= *((*list)+num-1); --num;}
}

/* Add name to list if it isn't on it, update size of list.
*  Returns pos+1 if on list, -pos if added to list.
*  Assumes list big enough.
*/
int add_list_name(list,name,numpt,amaxpt,ainc)
  char ***list,*name; int *numpt,*amaxpt,ainc;
{ int pos=find_name(name,*list,*numpt);
  if(pos>0) return(pos);
  /* not on list so add */
  pos=(-pos);
  insert_hole(pos,list,*numpt,amaxpt,ainc);
  *((*list)+pos)=name; ++(*numpt);
  return(-pos);
}

int update_list_name(list,name,numpt,amaxpt,ainc)
  char ***list,*name; int *numpt,*amaxpt,ainc;
{ int pos=find_name(name,*list,*numpt);
  if(pos>0)
    {free((*list)[pos-1]); ((*list)[pos-1])=name; return(pos);}
  pos=(-pos);
  insert_hole(pos,list,*numpt,amaxpt,ainc);
  ((*list)[pos])=name; ++(*numpt);
  return(-pos);
}

void append_list_name(list,name,numpt,amaxpt,ainc)
  char ***list,*name; int *numpt,*amaxpt,ainc;
{
  insert_hole(*numpt,list,*numpt,amaxpt,ainc);
  *((*list)+ *numpt)=name; ++(*numpt);
}

/* Sort a list of strings.
*/
void sort_list(list,num)
  char **list; int num;
{ int pos,sorted=1,a=10,b=0;
  char *temp;
  while(sorted<num)
   { temp=list[sorted];
     pos=find_name(temp,list,sorted);
     if(pos<0) pos=(-pos);
     insert_hole(pos,&list,sorted,&a,&b,0);
     list[pos]=temp; ++sorted;
   }
}

