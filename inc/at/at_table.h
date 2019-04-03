/*! \brief  AT command register table
 *  \author Shylock Hg
 *  \date 2018-08-17
 *  \email tcath2s@gmail.com
 * */

#ifndef _AT_TABLE_H_
#define _AT_TABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "./at_command.h"

typedef enum AT_FLAG {
    AT_FLAG_UNVISIABLE = 1,
    AT_FLAG_VISIABLE = (1U << 1),
} AT_FLAG_T;

typedef struct at_cmd_cb {
    enum AT_FLAG flag;
    char* cmd_str;
    at_cmd_set_t at_set_handler;
    at_cmd_read_t at_read_handler;
    at_cmd_test_t at_test_handler;
    at_cmd_exec_t at_exec_handler;
} at_cmd_cb_t;

/*! \brief register at command to at comman parser context
 *  \param context at command parser context
 *  \param table at command register table
 *  \param count count of at command register table items
 * */
void at_table_register(at_cmd_context_t* context, at_cmd_cb_t table[],
                       size_t count);

#ifdef __cplusplus
}
#endif

#endif  //!< _AT_TABLE_H_
