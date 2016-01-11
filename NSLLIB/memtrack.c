/* mem tracker */
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

typedef struct 
{ void *p;
  int nline;
  size_t nn;
  long count;
  char *nfile;
} my_mem;

my_mem *my_table=NULL;
int my_num=0,my_bad=0,my_rel=0;
long my_count=0;
long my_debug_count=32;

void my_debug_break()
{ int tmp=0;
}

void my_slot(void *p,size_t nn,int nline,char *nfile)
{	int num=0;
	while(num<my_num)
	{	if(!my_table[num].p) goto byebye;
	++num;
	}
	my_table=realloc(my_table,(++my_num)*sizeof(my_mem));
	byebye:
	my_table[num].p=p; my_table[num].nline=nline; 
	my_table[num].nfile=nfile; my_table[num].nn=nn;
	my_table[num].count=my_count++;

	if(my_table[num].count==my_debug_count)
		my_debug_break();
}

void my_free(void *p,int nline,char *nfile)
{	int num=my_num;
	while(num-->0) {if(my_table[num].p==p) {my_table[num].p=NULL; goto byebye;}}
	/* possible bug */
	fprintf(stderr,"Mem bug?%d,%s\n",nline,nfile);
	++my_bad;
	byebye:
	free(p);
}

void *my_malloc(size_t nn,int nline,char *nfile)
{	void *p=malloc(nn);
	my_slot(p,nn,nline,nfile);
	return(p);
}

void *my_realloc(void *p,size_t nn,int nline,char *nfile)
{	int num; void *q;
	if(!p) return(my_malloc(nn,nline,nfile));
	num=my_num;
	while(num-->0) {if(my_table[num].p==p) {my_table[num].p=NULL; goto memok;}}
	fprintf(stderr,"Mem bug?\n");
	++my_rel;
	memok:
	q=realloc(p,nn);
	my_slot(q,nn,nline,nfile);
	return(q);
}

void my_exit()
{	int num,num_open=0; FILE *memfp;
	memfp=fopen("C:\\memtrack.log","w");
	if(!memfp) memfp=stderr;
	for(num=0;num<my_num;++num)
	{if(my_table[num].p)
		{fprintf(memfp,"%d,a=%lX,n=%d,c=%ld,l=%d,f=%s\n",
			num,my_table[num].p,my_table[num].nn,my_table[num].count,
			my_table[num].nline,my_table[num].nfile);
		++num_open;
		}
	}
	fprintf(memfp,"Num open=%d,bad=%d,rel=%d\n",
		num_open,my_bad,my_rel);
	fclose(memfp);
	if(my_table) free(my_table);
}