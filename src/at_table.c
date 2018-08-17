/*! \brief at command register table
 *  \author Shylock Hg
 *  \date 2018-08-17
 *  \email tcath2s@gmail.com
 * */

#include "../inc/at_command.h"
#include "../inc/at_table.h"

void at_table_register(at_cmd_context_t * context, at_cmd_cb_t table[], 
		size_t count){
	for(int i=0; i<count; i++){
		at_cmd_insert(context, table[i].cmd_str, 
				&(table[i].at_set_handler));
	}
}

