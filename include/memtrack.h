#ifndef _memtrack_h
#define _memtrack_h

#ifdef __cplusplus
extern "C"
{
#endif


extern void my_exit();
extern void *my_realloc(void *p,size_t nn,int nline,char *nfile);
extern void *my_malloc(size_t nn,int nline,char *nfile);
extern void my_free(void *p,int nline,char *nfile);

#define malloc(a) my_malloc(a,__LINE__,__FILE__)
#define realloc(a,b) my_realloc(a,b,__LINE__,__FILE__)
#define free(a) my_free(a,__LINE__,__FILE__)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*_memtrack_h*/
