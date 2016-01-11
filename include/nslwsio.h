/* Name: nslwsio.h - Small Model Window I/O Definitions
*  NeSeL v2.0 April 1989  Neural Network Specification Language
*/
#ifndef _nslwsio_h
#define _nslwsio_h

#ifdef __cplusplus
extern "C"
{
#endif

/* wnd command parameter constants */
#define NIO_DEFAULT (0x8000)
#define NIO_FREE 0
#define NIO_CHR 1
#define NIO_PIXEL 2
#define NIO_IMAGE 3
#define NIO_INT 4
#define NIO_FLOAT 5
#define NIO_HIDE 6
#define NIO_REDRAW 7
#define NIO_DRAW 8
#define NIO_ERASE 9
#define NIO_DP_OPEN 10
#define NIO_DP_CLOSE 11
#define NIO_DP_DUMP 12

/* wnd title position specifier constants */
#define NIO_TL 1
#define NIO_TC 2
#define NIO_TR 3
#define NIO_BL 4
#define NIO_BC 5
#define NIO_BR 6
#define NIO_LT 7
#define NIO_LC 8
#define NIO_LB 9
#define NIO_RT 10
#define NIO_RC 11
#define NIO_RB 12

extern int wnd_num_colors,wnd_white,wnd_black,wnd_std,wnd_all;
extern int w_exit_code,w_exit_called;

/* extract x or y number from (long) pacted position value */
extern int w_x(long yx);
extern int w_y(long yx);
extern long w_xy(int x,int y);

extern BOOL w_active(int);/*(int w)*/
extern int w_arc(int,int,int,int,int,int,int,int,int);
 /*(int w,x1,y1,x2,y2,x3,y3,x4,y4)*/
extern void w_blank(int,BOOL);/*(int w,BOOL frame)*/
extern int w_bordercolor(int,int);/*(int w,c)*/
extern int w_circle(int,int);/*(int w,r)*/
extern void w_clear(int);/*(int w)*/
extern void w_clear_line(int);/*(int w)*/
extern void w_close(int);/*(int w)*/
extern void w_dump(int,int,...);/*(int w,cmd,...)*/
extern int w_ellipse(int,int,int);/*(int w,rx,ry)*/
extern void w_exit(int retcode);
extern void w_focus(int w);
extern int w_fill(int);/*(int w)*/
extern int w_fillstate(int,int);/*(int w,state)*/
extern void w_fpcheck();
extern int w_getch(int);/*(int w)*/
extern int w_getpixel(int,int,int);/*(int w,x,y)*/
extern char *w_gets(int,char *);/*(int w,char *bp)*/
extern int w_handle(char *);/*(char *name)*/
extern void w_init();
extern int w_more(int,int);/*(int w,state)*/
extern int w_move(int,int);/*(int w,d)*/
extern int w_moveto(int,int,int);/*(int w,x,y)*/
extern int w_onscreen(int);/*(int w)*/
extern int w_open(char *,BOOL,BOOL,BOOL,BOOL,BOOL,int,int,int,int);
  /*(char *name,BOOL share,graf,frame,scale,iswrap,int lx,ly,fc,bc)*/
extern long w_origin(int w,int x,int y);
extern int w_papercolor(int,int);/*(int w,c)*/
extern int w_penchar(int,int);/*(int w,c)*/
extern int w_pencolor(int,int);/*(int w,c)*/
extern int w_pendown(int,int);/*(int w,state)*/
extern int w_penwidth(int w,int n);
extern int w_poly(int,int,int);/*(int w,num,side)*/
extern long w_position(int,int,int);/*(int w,x,y)*/
extern int w_printf(int,char *,...);/*(int w,char *fmt,...)*/
extern void w_puts(int,char *);/*(int w,char *str)*/
extern int w_rectangle(int,int,int,int,int);/*(int w,x1,y1,x2,y2)*/
extern void w_refresh(int,BOOL);/*(int w,BOOL alltrks)*/
extern void w_report(int);/*(int w)*/
extern long w_resize(int w,int x,int y);
extern int w_setjmp();
extern int w_setpixel(int,int,int);/*(int w,x,y)*/
extern long w_size(int,BOOL);/*(int w,BOOL ischr)*/
extern char *w_title(int,int,int,...);
  /*(int w,pos,cmd,va_list)*/
extern int w_track(int,int,...);/*(int w,cmd,...)*/
extern int w_turn(int,int);/*(int w,angle)*/
extern int w_turnto(int,int);/*(int w,angle)*/
extern void w_update(int w);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_nslwsio_h*/

