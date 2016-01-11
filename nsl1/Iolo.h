/* Name: iolow.h   Type: header file   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/

extern char
 input_name[], /* NeSeL source file name, */
 input_ext[], /* file name extension */
 output_file[]; /* Output file name */


extern FILE 
	*infp,*nalfp, /* input and output file pointers */
	*garfp,*monfp,*sstfp, /* substantiated file pts monitor struct info */
	*hoofp,*funfp,*tmpfp,*logfp;

/* Some macro definitions */

/* Move to position in input source file */
#ifndef SEEK_SET
#define SEEK_SET 0 /* this should be defined in stdio.h */
#endif
#ifndef SEEK_END
#define SEEK_END 2 /* this should be defined in stdio.h */
#endif
#define fmoveto(filept,pos) fseek(filept,(long)pos,(int)SEEK_SET)
#define fposeof(filept) fseek(filept,(long)0,(int)SEEK_END)

/* Get current position in input source file */
#define fcurpos(filept) ftell(filept)

extern char *buffer_on(),*buffer_off(),*get_io_pos(),*get_module();
extern char input_char();
extern void unput_char(),blank_mark(),blank_do(),blank_on(),blank_off();
extern void set_io_str(),hash_line(),pop_io(),push_io(),
   set_io_pos(),getmainargs();
extern void openfiles(),appendfile(),initialise_io();
extern BOOL new_line();
extern char skip_blanks(),skip_quoted(),skip_quoln(),skip_pair(),
     skip_bodies(),skip_to_sep(),skip_to_rbrk(),skip_over(),
     skip_to_cast(),skip_to_scolon(),skip_to_brace(),skip_braces();
extern char get_ident();
extern char *get_arg(),*skip_auxs();
extern BOOL replace_str();
