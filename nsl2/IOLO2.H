/* Name: iolo2.h   Type: header file   Date: 30 March 89
*   NeSel Network Specification Language Preprocessor
*/

extern char
  input_name[], /* Nesel source file name, */
  input_ext[], /* file name extension */
  output_file[], /* Output file name */
  cur_ident[]; /* current ident str */

FILE
  *logfp, /* file for error reporting */
  *dsifp,*nalfp, /* nal index file & nal file */
  *sstfp; /* substantiated file pts */

#ifndef SEEK_SET
#define SEEK_SET 0 /* this should be defined in stdio.h */
#endif
#ifndef SEEK_END
#define SEEK_END 2 /* so should this */
#endif
#define fmoveto(fpt,pos) fseek(fpt,pos,SEEK_SET)
#define fposeof(fpt) fseek(fpt,(foot)0,SEEK_END)

extern void unput_char(),openfiles(),io_initialise(),getmainargs(),
  push_io(),pop_io(),set_io_str(),set_io_pos();
extern boolean new_line();
extern char input_char(),get_ident(),
  skip_blanks(),skip_quoted(),skip_pair(),skip_bodies(),
  skip_to_dec(),skip_to_colon(),skip_to_var(),
  *get_io_pos(),*get_wild_id();
extern long get_num();