/*	nsl.cpp NeSeL Loader v1.0 Nov 1996 (c)(p) DJRW 
*	Performs the compiler passes: cpp, nsl1, nsl2, nsltox
*	Looks for nsl.ini in executable's directory
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <direct.h>

typedef enum {INI_NSL,INI_CPP,INI_NSL1,INI_NSL2,INI_NSLTOX,INI_NUM} IniType;
#ifndef BOOL
typedef enum {FALSE=0,TRUE=1} BOOL;
#endif

#define IsPathSep(c) ((c)=='\\'||(c)=='/'||(c)==':')
#define IsWhiteSpace(c) ((c)==' '||(c)=='\t')
#define ToUpper(c) (((c)>='a'&&(c)<='z')?((c)^0x20):(c))

IniType IniSection=INI_NSL;
char **NslArgs[INI_NUM]={NULL};
int NslArgsNum[INI_NUM]={0};
char NslPath[256]={0};
char *NslPathName=NslPath,*FileName=NULL;
char FileRoot[256]={0};
BOOL NslPass[INI_NUM]={FALSE};

char *NslCmd[]={"nsl.ini","cpp.exe","nsl1.exe","nsl2.exe","nsltox.exe"};

void ShowUsage()
{	fprintf(stderr,"NeSel Loader v1.0,1996. Syntax: Nsl filename [args]\n");
	fprintf(stderr,"  [args]=[-[no-]cpp [args]] [-[no-]nsl1 [args]] [-[no-]nsl2 [args]]\n");
	fprintf(stderr,"         [-config file] [-shell command] [-filename name] [-chdir path]\n");
	fprintf(stderr,"  replaces %%ENVVAR%%, -config as 1st arg overrides nsl.ini\n"); 
}

/* Convert environment string s to absolute,
*  replaces "...%env_var%..." by its value: "...val..."
*  b pts to a buffer for expanded string.
*  Returns b or null
*/
char *envstr(char *b,char *s)
{ char chr,*r,*p,*e; int n;
  if(!b) return(NULL);
  *b='\0';
  if(!s) return(b);
  strcpy(b,s); s=b; /* move str to buffer to expand inline */
  while(*s)
    {if(*s=='%'&&s[1]!='%') /* got an arg */
       {p=s+1; while(*p&&*p!='%') ++p; chr=(*p); *p='\0';
        e=getenv(s+1); *p=chr;
        if(e) /* hack out var name */
          {r=s; if(*p) ++p; while(*p) {*r=(*p); ++r; ++p;} *r='\0';
           n=strlen(e); p=r; r=r+n; /* make space for its value */
           while(p!=s) {*r=(*p); --r; --p;} *r=(*p);
           while(*e) {*p=(*e); ++p; ++e;}
          }
        else ++s;
       }
     else ++s;
    }
  *s='\0';
  return(b);
}

/* extract path to directory containing nsl.exe */
/* use as initial path to nsl.ini */
/* returns 0 if ok */
BOOL GetPath(char *p)
{	int num=strlen(p);
	if(num>0)
	{	strcpy(NslPath,p);
		p=NslPath+num-1;
		while(p>NslPath&&*p&&!IsPathSep(*p)) --p;
		if(IsPathSep(*p)) {++p; *p='\0';}
	}
	else *NslPath='\0';
	NslPathName=p;
	return((BOOL)(*NslPath!='\0'));
}

/* chop off file extension: result to buffer */
/* returns 0 if .ext found */
BOOL GetFileRoot(char *p)
{	int num=strlen(p);
	if(num>0)
	{	strcpy(FileRoot,p);
		p=FileRoot+num-1;
		while(p>FileRoot&&*p&&!IsPathSep(*p)&&(*p!='.')) --p;
		if(*p=='.') {*p='\0'; return(FALSE);}
	}
	else *FileRoot='\0';
	return(TRUE);
}

/* chop off white space from both ends of string */
/* returns start of first non-whitespace */
char *StrChopSpace(char *p)
{	if(p) 
	{	while(IsWhiteSpace(*p)) ++p;
		if(*p)
		{	char *b=p+strlen(p)-1;
			while(b>p&&*b&&(IsWhiteSpace(*b)||*b=='\n')) --b;
			b[1]='\0';
		}
	}
	return(p);
}

/* see if buffer starts with str, ignore white space & case of letters */
/* returns next if ok else null*/
char *StrMatch(char *b,char *str)
{	b=StrChopSpace(b);
	do{	if(!*str) break;
		if(ToUpper(*b)!=ToUpper(*str)) return(NULL);
		++str; ++b;
	}while(*b&&*str);
	if((!*str)&&(IsWhiteSpace(*b)||*b=='\n'||*b=='\0')) return(b);
	return(NULL);
}

char *StrSave(char *p)
{ return(strcpy((char *)malloc((size_t)(1+strlen(p))),p));
}

 /* add a parameter to section */
void AddArg(int section,char *p)
{	int num=NslArgsNum[section]; 
	p=StrChopSpace(p);
	NslArgs[section]=(char **)
	realloc(NslArgs[section],(size_t)((1+num)*sizeof(char **)));
	NslArgs[section][num]=(p?StrSave(p):NULL);
	++NslArgsNum[section];
}

BOOL LoadConfig(char *path);

/* parse a command line arg or ini file line */
void ParseArg(char *p)
{	char *s, tmp[512];
	p=envstr(tmp,p);
	/* check for comment */
	if(!p||!*p||*p=='#'||*p=='\n'||StrMatch(p,"rem")) return;
	/* check for section switch */
	if(StrMatch(p,"[nsl]")||StrMatch(p,"-nsl"))	IniSection=INI_NSL;
	else if(StrMatch(p,"[cpp]")||StrMatch(p,"-cpp")) 
		NslPass[IniSection=INI_CPP]=TRUE;
	else if(StrMatch(p,"[nsl1]")||StrMatch(p,"-nsl1")) 
		NslPass[IniSection=INI_NSL1]=TRUE;
	else if(StrMatch(p,"[nsl2]")||StrMatch(p,"-nsl2")) 
		NslPass[IniSection=INI_NSL2]=TRUE;
	else if(StrMatch(p,"[nsltox]")||StrMatch(p,"-nsltox")) 
		NslPass[IniSection=INI_NSLTOX]=TRUE;
	else if(StrMatch(p,"-no-cpp")) NslPass[INI_CPP]=FALSE; 
	else if(StrMatch(p,"-no-nsl1")) NslPass[INI_NSL1]=FALSE;
	else if(StrMatch(p,"-no-nsl2")) NslPass[INI_NSL2]=FALSE;
	else if(StrMatch(p,"-no-nsltox")) NslPass[INI_NSLTOX]=FALSE;
	else if(s=StrMatch(p,"-config")) LoadConfig(s);
	else if(s=StrMatch(p,"-chdir")) _chdir(StrChopSpace(s));
	else if(s=StrMatch(p,"-shell")) system(StrChopSpace(s));
	else if(s=StrMatch(p,"-filename")) 
	{	if(FileName) free(FileName); FileName=StrSave(StrChopSpace(s));}
	else if(StrMatch(p,"-help")) ShowUsage();
	else {AddArg(IniSection,p);	NslPass[IniSection]=TRUE;}
}

/* get parameters from a config file */
/* returns 0 if ok */
BOOL LoadConfig(char *path)
{	FILE *inifp; char tmp[512];
	if(!path||!*path) return(TRUE);
	path=StrChopSpace(path);
	if(!(inifp=fopen(path,"r")))
	{	fprintf(stderr,"Couldn't open config file: %s\n",path); 
		return(TRUE);
	}
	/* scan config file */
	while(fgets(tmp,512,inifp)) ParseArg(tmp);
	fclose(inifp);
	return(FALSE);
}

/* run command */
int RunPass(char *name,char *argv[])
{	int status=0;
	strcpy(NslPathName,name);
	status=_spawnvp(_P_WAIT, NslPath,(const char *const *)argv);
	return(status);
}

int main( int argc, char *argv[])
{	int j,k,opts,status=0; BOOL HasNoExt;

	for(k=0;k<INI_NUM;++k) {NslArgs[k]=NULL; NslArgsNum[k]=0;}

	if (argc < 2)
	{	status=1;
		ShowUsage();
		goto byebye;
	}

#if 0 /* for debugging */
	GetPath("C:\\USR\\BIN\\NSL.EXE"); 
#else
	GetPath(argv[0]);
#endif	

	/* set up argv[0] for each arg stack */
	for(k=1;k<INI_NUM;++k) {strcpy(NslPathName,NslCmd[k]); AddArg(k,NslPath);}

	FileName=StrSave(argv[1]);

	/* build up list of args for the various compiler passes */
	if(argc>2&&StrMatch(argv[2],"-config"))
	{	LoadConfig(argv[3]); opts=4;
	}
	else 
	{	strcpy(NslPathName,NslCmd[0]);
		LoadConfig(NslPath); opts=2;
	}
	
	while(opts<argc) ParseArg(argv[opts++]);

	/* now perform each pass in turn */

	HasNoExt=GetFileRoot(FileName);

	if(NslPass[INI_CPP])
	{	char tmp[256],*p=tmp; 
		if(HasNoExt) sprintf(tmp,"%s.n",FileRoot); else p=FileName;
		AddArg(INI_CPP,p); 
		sprintf(tmp,"%s.nsp",FileRoot);
		AddArg(INI_CPP,tmp); AddArg(INI_CPP,NULL); 
		if(status=RunPass(NslCmd[INI_CPP],NslArgs[INI_CPP])) goto byebye;
	}

	if(NslPass[INI_NSL1])
	{	AddArg(INI_NSL1,FileRoot); AddArg(INI_NSL1,NULL); 
		if(status=RunPass(NslCmd[INI_NSL1],NslArgs[INI_NSL1])) goto byebye;
	}

	if(NslPass[INI_NSL2])
	{	AddArg(INI_NSL2,FileRoot); AddArg(INI_NSL2,NULL); 
		if(status=RunPass(NslCmd[INI_NSL2],NslArgs[INI_NSL2])) goto byebye;
	}

	if(NslPass[INI_NSLTOX])
	{	AddArg(INI_NSLTOX,FileRoot); AddArg(INI_NSLTOX,NULL); 
		if(status=RunPass(NslCmd[INI_NSLTOX],NslArgs[INI_NSLTOX])) goto byebye;
	}

	byebye:
	/* free space & exit */
	for(k=0;k<INI_NUM;++k) 
	{	for(j=0;j<NslArgsNum[k];++j) {if(NslArgs[k][j]) free(NslArgs[k][j]);}
		if(NslArgs[k]) free(NslArgs[k]);
	}
	if(FileName) free(FileName);
	/*DEBUG: always return 0, regardless of status */
	return(0);
}
