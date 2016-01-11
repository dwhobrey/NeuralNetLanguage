/* Name: general.h   Type: header file   Date: 30 March 89
*  NeSel Network Specification Language Preprocessor
*/

extern void push_cond(),push_bot_cond();
extern stack_elm *on_stack();
extern char *expand_struct(),*cond_append(),*type_cast(),expand_typedef(),
  *buffer_module(),*conv_cast();
extern int cast_type(),get_type_code(),get_base_code(),functypedef();
extern BOOL make_hole(),expand_macro(),do_macro();
