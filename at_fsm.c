/*! \brief  implement a FSM to parse standard at command record
 *  \author Shylock Hg
 *  \date 2018-06-20
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "at_fsm.h"


static at_cmd_record_t * at_cmd_record_new(const char * name , const char * param, enum AT_CMD_INDEX type);
static void at_cmd_record_release(at_cmd_record_t * record);


/*! \brief handler function prototype of FSM state handler
 * */
typedef enum at_cmd_FSM_state (*at_cmd_FSM_handler_t)(at_cmd_record_t * record, const char c);

/*! \brief at command FSM object
 *
 * */
typedef struct at_cmd_FSM {
	enum at_cmd_FSM_state state;
	at_cmd_FSM_handler_t  handler;
} at_cmd_FSM_t;

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_start_handler(at_cmd_record_t * record, const char c){

	return 0;
}

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_A_handler(at_cmd_record_t * record, const char c){

	return 0;
}

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_T_handler(at_cmd_record_t * record, const char c){

	return 0;
}

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_PLUS_handler(at_cmd_record_t * record, const char c){

	return 0;
}

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_QUERY_handler(at_cmd_record_t * record, const char c){

	return 0;
}

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_TEST_OR_SET_handler(at_cmd_record_t * record, const char c){

	return 0;
}

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_TEST_handler(at_cmd_record_t * record, const char c){

	return 0;
}

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_SET_handler(at_cmd_record_t * record, const char c){

	return 0;
}

/*! \brief parse input character to record structure
 *  \param record the command record structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_PARAM_handler(at_cmd_record_t * record, const char c){

	return 0;
}

static at_cmd_FSM_t FSM[AT_CMD_FSM_STATE_COUNT] = {
	{AT_CMD_FSM_STATE_START,       at_cmd_FSM_start_handler},
	{AT_CMD_FSM_STATE_A,           at_cmd_FSM_A_handler},
	{AT_CMD_FSM_STATE_T,           at_cmd_FSM_T_handler},
	{AT_CMD_FSM_STATE_PLUS,        at_cmd_FSM_PLUS_handler},
	{AT_CMD_FSM_STATE_QUERY,       at_cmd_FSM_QUERY_handler},
	{AT_CMD_FSM_STATE_TEST_OR_SET, at_cmd_FSM_TEST_OR_SET_handler},
	{AT_CMD_FSM_STATE_TEST,        at_cmd_FSM_TEST_handler},
	{AT_CMD_FSM_STATE_SET,         at_cmd_FSM_SET_handler},
	{AT_CMD_FSM_STATE_PARAM,       at_cmd_FSM_PARAM_handler}
};


int at_cmd_FSM_parse_record(at_cmd_record_queue_t * queue_record, const char * record){
	assert(queue_record);
	assert(record);
	if(NULL == queue_record || NULL == record)
		return -1;

	const char * str = record;
	enum at_cmd_FSM_state state = AT_CMD_FSM_STATE_START;
	//< \TODO create a record structure with no field
	at_cmd_record_t * xrecord = at_cmd_record_new(NULL, NULL, 0);

	while('\0' != *str){
		enum at_cmd_FSM_state pre_state = state;
		state = FSM[state].handler(xrecord, *str);
		//!< return error or out of range
		if(0 > state || AT_CMD_FSM_STATE_COUNT <= state)  
			return -1;
	
		if(AT_CMD_FSM_STATE_T == state && AT_CMD_FSM_STATE_A != pre_state){
			//< \TODO append record to record queue 
			queue_class_enqueue(queue_record, xrecord);
		}
		if(AT_CMD_FSM_STATE_START == state && AT_CMD_FSM_STATE_START != pre_state){
			//< \TODO append record to record queue 
			queue_class_enqueue(queue_record, xrecord);
		}


		str++;
	}

	//< release the xrecord
	at_cmd_record_release(xrecord);

	if(AT_CMD_FSM_STATE_START != state)  //!< error when parse record
		return -1;

	return 0;
}

/******************** at command record structure method ********************/
static at_cmd_record_t * at_cmd_record_new(const char * name , const char * param, enum AT_CMD_INDEX type){
	at_cmd_record_t * record = malloc(sizeof(at_cmd_record_t));

	record->type = type;
	if(name)
		strncpy(record->name, name, sizeof(record->name));
	if(param)
		strncpy(record->param, param, sizeof(record->param));

	return record;
}

static void at_cmd_record_release(at_cmd_record_t * record){
	free(record);
}

