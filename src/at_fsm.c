/*! \brief  implement a FSM to parse standard at command record
 *  \author Shylock Hg
 *  \date 2018-06-20
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "../inc/at_fsm.h"
#include "../inc/at_xrecord.h"
#include "../inc/stdlog.h"




/*! \brief handler function prototype of FSM state handler
 * */
typedef enum at_cmd_FSM_state (*at_cmd_FSM_handler_t)(
		at_cmd_context_t * context, at_cmd_xrecord_t * record, const char c);

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
static enum at_cmd_FSM_state at_cmd_FSM_start_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if('A' == c){
		return AT_CMD_FSM_STATE_A;
	}else{  //!< match fail
		return -1; //AT_CMD_FSM_STATE_START;
	}

}

/*! \brief parse input character to xrecord structure
 *  \param xrecord the command xrecord structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_A_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if('T' == c){
		return AT_CMD_FSM_STATE_T;
	}else{  //!< match fail
		return -1; //AT_CMD_FSM_STATE_START;
	}
}

/*! \brief parse input character to xrecord structure
 *  \param xrecord the command xrecord structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_T_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if('+' == c){
		return AT_CMD_FSM_STATE_PLUS;
	}else{  //!< match fail
		return -1; //AT_CMD_FSM_STATE_START;
	}
}

/*! \brief append a character to string buffer
 *  \param str string to append character
 *  \param c character to append to string
 *  \note don't check string buffer size, not secure
 * */
//static void strachr(char * str, char c){

//}

/*! \brief append a character to string buffer
 *  \param str string to append a character
 *  \param len size of string buffer
 *  \param c character to append to string
 *  \retval the string changed , NULL for fail
 * */
static char * strnachr(char * str, size_t len, char c){
	assert(str);

	size_t lenstr = strlen(str);

	if(lenstr+1 >= len)  //!< no available buffer memory
		return NULL;

	//< append a character
	str[lenstr] = c;
	str[lenstr+1] = '\0';

	return str;
}

/*! \brief parse input character to xrecord structure
 *  \param xrecord the command xrecord structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_PLUS_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if(isupper(c)){  //!< at command name
		strnachr(xrecord->name, sizeof(xrecord->name), c);
		return AT_CMD_FSM_STATE_PLUS;
	}else if('=' == c){  //!< test or set
		return AT_CMD_FSM_STATE_TEST_OR_SET;
	}else if('?' == c){  //!< query
		return AT_CMD_FSM_STATE_QUERY;
	}else if(';' == c){  //!< set command end
		xrecord->type = AT_CMD_INDEX_NO_PARAM;
		return AT_CMD_FSM_STATE_T;
	}else if(context->delimiter == c){  //!< set command end
		xrecord->type = AT_CMD_INDEX_NO_PARAM;
		return AT_CMD_FSM_STATE_START;
	}else{  //!< match fail
		memset(xrecord->name, 0x00, sizeof(xrecord->name));
		return -1; //AT_CMD_FSM_STATE_START;
	}
}

/*! \brief parse input character to xrecord structure
 *  \param xrecord the command xrecord structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_QUERY_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if(';' == c){
		xrecord->type = AT_CMD_INDEX_QUERY_PARAM;
		return AT_CMD_FSM_STATE_T;
	}else if(context->delimiter == c){
		xrecord->type = AT_CMD_INDEX_QUERY_PARAM;
		return AT_CMD_FSM_STATE_START;
	}else{  //!< match fail
		memset(xrecord->name, 0x00, sizeof(xrecord->name));
		return -1;
	}
}

/*! \brief parse input character to xrecord structure
 *  \param xrecord the command xrecord structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_TEST_OR_SET_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if('?' == c){  //!< test
		return AT_CMD_FSM_STATE_TEST;
	}else if('"' == c){  //!< param
		strnachr(xrecord->param, sizeof(xrecord->param), c);
		return AT_CMD_FSM_STATE_PARAM;
	}else{  //!< set
		strnachr(xrecord->param, sizeof(xrecord->param), c);
		return AT_CMD_FSM_STATE_SET;
	}
}

/*! \brief parse input character to xrecord structure
 *  \param xrecord the command xrecord structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_TEST_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if(';' == c){
		xrecord->type = AT_CMD_INDEX_CHECK_PARAM;
		return AT_CMD_FSM_STATE_T;
	}else if(context->delimiter == c){
		xrecord->type = AT_CMD_INDEX_CHECK_PARAM;
		return AT_CMD_FSM_STATE_START;
	}else{  //!< match fail
		memset(xrecord->name, 0x00, sizeof(xrecord->name));
		return -1;
	}
}

/*! \brief parse input character to xrecord structure
 *  \param xrecord the command xrecord structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_SET_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if(';' == c){
		xrecord->type = AT_CMD_INDEX_WITH_PARAM;
		return AT_CMD_FSM_STATE_T;
	}else if(context->delimiter == c){
		xrecord->type = AT_CMD_INDEX_WITH_PARAM;
		return AT_CMD_FSM_STATE_START;
	}else if('"' == c){
		strnachr(xrecord->param, sizeof(xrecord->param), c);
		return AT_CMD_FSM_STATE_PARAM;
	}else{
		strnachr(xrecord->param, sizeof(xrecord->param), c);
		return AT_CMD_FSM_STATE_SET;
	}
}

/*! \brief parse input character to xrecord structure
 *  \param xrecord the command xrecord structure
 *  \param c input character
 *  \retval next state or negtive error number
 * */
static enum at_cmd_FSM_state at_cmd_FSM_PARAM_handler(
		at_cmd_context_t * context, at_cmd_xrecord_t * xrecord, const char c){
	assert(context);
	assert(xrecord);

	if('"' == c){
		strnachr(xrecord->param, sizeof(xrecord->param), c);
		return AT_CMD_FSM_STATE_SET;
	}else{
		strnachr(xrecord->param, sizeof(xrecord->param), c);
		return AT_CMD_FSM_STATE_PARAM;
	}
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


static const char * at_cmd_state_2_str(at_cmd_FSM_state_t state){
	static const char * at_cmd_state_str[] = {
		"AT_CMD_FSM_STATE_START",
		"AT_CMD_FSM_STATE_A",
		"AT_CMD_FSM_STATE_T",
		"AT_CMD_FSM_STATE_PLUS",
		"AT_CMD_FSM_STATE_QUERY",
		"AT_CMD_FSM_STATE_TEST_OR_SET",
		"AT_CMD_FSM_STATE_TEST",
		"AT_CMD_FSM_STATE_SET",
		"AT_CMD_FSM_STATE_PARAM"
	};

	if(sizeof(at_cmd_state_str) <= state)
		return NULL;

	return at_cmd_state_str[state];
}


int at_cmd_FSM_parse_record(at_cmd_context_t * context, 
		at_cmd_xrecord_queue_t * xrecords, const char * record){
	assert(xrecords);
	assert(record);
	if(NULL == xrecords || NULL == record)
		return -1;

	const char * str = record;
	enum at_cmd_FSM_state state = AT_CMD_FSM_STATE_START;
	//< \TODO create a record structure with no field
	at_cmd_xrecord_t * xrecord = at_cmd_xrecord_new(NULL, NULL, 0);
	at_cmd_xrecord_deinit(xrecord);

	while('\0' != *str){
		//printf_DBG(DBG_LEVEL_LOG, "state-`%s`\n", at_cmd_state_2_str(state));

		enum at_cmd_FSM_state pre_state = state;
		state = FSM[state].handler(context, xrecord, *str);
		//!< return error or out of range
		if(AT_CMD_FSM_STATE_START > state || AT_CMD_FSM_STATE_COUNT <= state){
			at_cmd_xrecord_release(xrecord);
			return -1;
		}
	
		if(AT_CMD_FSM_STATE_T == state && AT_CMD_FSM_STATE_A != pre_state){
			//< \TODO append record to record queue 
			queue_class_enqueue(xrecords, xrecord);
			at_cmd_xrecord_deinit(xrecord);
		}
		if(AT_CMD_FSM_STATE_START == state && AT_CMD_FSM_STATE_START != pre_state){
			//< \TODO append record to record queue 
			queue_class_enqueue(xrecords, xrecord);
			at_cmd_xrecord_deinit(xrecord);
		}


		str++;
	}

	//< release the xrecord
	at_cmd_xrecord_release(xrecord);

	if(AT_CMD_FSM_STATE_START != state)  //!< error when parse record
		return -1;

	return 0;
}

void at_cmd_FSM_gen_xrecord_queue_4_record(
		at_cmd_context_t * context, at_cmd_xrecord_queue_t * xrecords, 
		const char * record){
	assert(context);
	assert(xrecords);
	assert(record);

	///< create a xrecord queue
	//xrecords = queue_class_new(sizeof(at_cmd_xrecord_t));

	at_cmd_FSM_parse_record(context, xrecords, record);
	
	//return xrecords;
}

void at_cmd_xrecord_queue_log(at_cmd_xrecord_queue_t * xrecords){
	assert(xrecords);

	at_cmd_xrecord_node_t * node = xrecords->_head;
	while(node){
		at_cmd_xrecord_t * xrecord = node->value;
		if(xrecord){
			printf_DBG(DBG_LEVEL_LOG, 
					"xrecord->name=`%s`,xrecord->type=`%d`,	xrecord->param=`%s`\n", 
					xrecord->name, xrecord->type, xrecord->param);
		}
		
		node = node->_next;
	}
}

void at_cmd_execute(at_cmd_context_t * context, 
		at_cmd_xrecord_queue_t * xrecords){
	assert(xrecords);
	assert(context);

	at_cmd_xrecord_node_t * node = queue_class_dequeue(xrecords);
	at_cmd_xrecord_t * xrecord = NULL;
	while(NULL != node){
		xrecord = node->value;
		if(xrecord){
			at_cmd_t * item = at_cmd_lookup(context, xrecord->name);
			if(item){
				at_cmd_set_t * handler = item->value;
				if(handler){
					if(handler[xrecord->type]){
						if(AT_CMD_INDEX_NO_PARAM == xrecord->type){
							//!< set
							(at_cmd_set_t)(handler[xrecord->type])(xrecord->param);
						}else{  //!< read test exec
							(at_cmd_read_t)(handler[xrecord->type])(xrecord->param);
						}
					}
				}
			}
			
		}

		//< release the dequeue element
		queue_class_element_release(xrecords, node);
		//< next xrecord queue node
		node = queue_class_dequeue(xrecords);
	}
}

void at_cmd_execute_script(at_cmd_context_t * context, const char * file){
	assert(file);

	FILE * stream = fopen(file, "r");
	//char * buffer = malloc(context->at_cmd_len + context->at_cmd_param_len + 47);
	char buffer[1024] = {0};
	//size_t len = 0;
	at_cmd_xrecord_queue_t * xrecords = queue_class_new(sizeof(at_cmd_xrecord_t));
	while(NULL != fgets(buffer, sizeof(buffer)-1, stream)){
		at_cmd_FSM_gen_xrecord_queue_4_record(context, xrecords, buffer);

		//at_cmd_xrecord_queue_log(xrecords);
	}
	/*
	do{
		len = fread(buffer, sizeof(char), sizeof(buffer)-1, stream);
		//printf("read %ld\n", len);
		buffer[len] = '\0';
		at_cmd_FSM_gen_xrecord_queue_4_record(context, xrecords, buffer);
		at_cmd_xrecord_queue_log(xrecords);
	}while((sizeof(buffer)-1) <= len);
	*/

	at_cmd_execute(context, xrecords);
	queue_class_release(xrecords);

	//free(buffer);
	fclose(stream);
}

void at_cmd_execute_script_string(at_cmd_context_t * context, 
		const char * script){
	assert(context);
	assert(script);

	at_cmd_xrecord_queue_t * xrecords = queue_class_new(sizeof(at_cmd_xrecord_t));
	at_cmd_FSM_gen_xrecord_queue_4_record(context, xrecords, script);
	at_cmd_execute(context, xrecords);
	queue_class_release(xrecords);
	
}

