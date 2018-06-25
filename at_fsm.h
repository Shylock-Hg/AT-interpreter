/*! \brief  implement a FSM to parse standard at command record
 *  \author Shylock Hg
 *  \date 2018-06-20
 *  \email tcath2s@gmail.com
 * */

#ifndef _AT_FSM_H_
#define _AT_FSM_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "at_command.h"
#include "queue.h"


/*! \brief at command FSM state
 * */
typedef enum at_cmd_FSM_state {
	AT_CMD_FSM_STATE_START,        //!< start state
	AT_CMD_FSM_STATE_A,            //!< 'A'
	AT_CMD_FSM_STATE_T,            //!< 'T'
	AT_CMD_FSM_STATE_PLUS,         //!< '+'
	AT_CMD_FSM_STATE_QUERY,        //!< '?' after command
	AT_CMD_FSM_STATE_TEST_OR_SET,  //!< '=' after command
	AT_CMD_FSM_STATE_TEST,         //!< '?' after '='
	AT_CMD_FSM_STATE_SET,          //!< [^?] after '='
	AT_CMD_FSM_STATE_PARAM         //!< between '"' and '"'
} at_cmd_FSM_state_t;

#define AT_CMD_FSM_STATE_COUNT 9

/*! \brief typedef the list to at command record list 
 *
 * */
typedef queue_class_t at_cmd_xrecord_queue_t;  //!< value memeber point to at_cmd_xrecord_t node
typedef queue_t       at_cmd_xrecord_node_t;

/*! \brief parse input record string to queue of record structure
 *  \param queue_record the queue of at command record
 *  \param record the standard record string
 *  \retval error retval
 * */
int at_cmd_FSM_parse_record(at_cmd_context_t * context, at_cmd_xrecord_queue_t * queue_record, const char * record);

/*! \brief generate xrecord queue from standard at command record
 *  \param record standard at command record string
 *  \retval at command xrecord queue
 * */
at_cmd_xrecord_queue_t * at_cmd_FSM_gen_xrecord_queue_4_record(at_cmd_context_t * context, const char * record);

/*! \brief log infomation of FSM xrecord queue
 *  \param xrecords xrecord queue
 * */
void at_cmd_xrecord_queue_log(at_cmd_xrecord_queue_t * xrecords);

/*! \brief execute a at command by `at command record queue` and `at command register table`
 * */
void at_cmd_execute(at_cmd_xrecord_queue_t * xrecords, at_cmd_context_t * context);

/*! \brief execute at command script 
 *  \param file the at command srcript file name
 * */
void at_cmd_execute_script(at_cmd_context_t * context, const char * file);

#ifdef __cplusplus
	}
#endif

#endif  //!< _AT_FSM_H_

