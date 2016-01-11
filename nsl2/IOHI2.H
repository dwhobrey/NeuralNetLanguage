/* Name: iohigh2.h   Type: header file   Date: 30 March 89
*   Nesel Network Specification Language Preprocessor
*/
char **source_list;
extern char vrname[4][5];
int sour_num;

extern void release_net(),claim_net(),flush_nets(),source_free();
extern foot nss_init();