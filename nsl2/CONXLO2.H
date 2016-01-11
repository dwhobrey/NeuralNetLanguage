/* Name: conxlo2.h   Type: header file   Date: 30 March 89
*   Nesel Network Specification Language Preprocessor
*/

int
  part_level, /* in pamat mode, level to eval opaths to */
  part_leafs; /* running count of leafs in part */
boolean
  part_exceed; /* set if part_level was eval'd to or exceeded */
extern void adj_tree(),blank_part(),zap_ilk(),tidy_ilk(),
   set_aux_inp(),clr_aux_inp(),set_aux_ilk(),clr_aux_ilk(),
   state_init(),state_save(),state_update(),state_restore(),
   aux_push(),aux_pop(),adj_cx_ds(),get_cx_type();
extern boolean has_exclam(),check_exclam(),adrsequal(),is_read_struct();
extern int inp_to_num(),max_part(),cur_part();
extern word node_adrs(),conx_adrs();
extern path_elm *part_path();
extern inp_elm *num_to_inp();
extern node_elm *struct_adrs(),*copy_adrs();
