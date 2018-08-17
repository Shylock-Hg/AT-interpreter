/*! \brief test AT command library
 *  \author Shylock Hg
 *  \date 2018-04-29
 *  \email tcath2s@gmail.com
 * */

#include <string.h>

#include "at_command.h"
#include "at_fsm.h"

#define TEST_AT_CMD_MAX_LEN       20
#define TEST_AT_CMD_MAX_PARAM_LEN 512
#define TEST_AT_CMD_DELIMITER     '\n'
#define TEST_HASH_TAB_SIZE        100

#define UNUSED(x) (void)(x)

/********** test at commands **********/
#define TEST_AT_CMD_HELLO "HELLO"
int at_cmd_hello_handler0(const char * parameter){
	UNUSED(parameter);
	printf("hello!\n");
	return 0;
}
int at_cmd_hello_handler1(const char * parameter){
	if(NULL == parameter)
		return -1;

	printf("hello %s!\n",parameter);
	return 0;
}
/*
int at_cmd_hello_handler2(const char * parameter){

}
*/
int at_cmd_hello_handler3(const char * parameter){
	UNUSED(parameter);
	printf("any string\n");
	return 0;
}
/********** test at commands **********/

int main(int argc, char * argv[]){
	//< create at parser context
	at_cmd_handler_t handlers[AT_CMD_HASH_VALUE_COUNT] = {at_cmd_hello_handler0,
		at_cmd_hello_handler1,NULL,at_cmd_hello_handler3};

	at_cmd_context_t * context = at_cmd_class_new(TEST_HASH_TAB_SIZE,
			TEST_AT_CMD_MAX_LEN,TEST_AT_CMD_MAX_PARAM_LEN,TEST_AT_CMD_DELIMITER);

	at_cmd_insert(context,TEST_AT_CMD_HELLO,handlers);

	//at_cmd_execute_script(context, "test.at");
	
	char buffer[1024] = {0};
	FILE * stream = fopen("test.at", "r");
	fread(buffer, sizeof(buffer), sizeof(buffer), stream);
	fclose(stream);
	at_cmd_execute_script_string(context, buffer);

	at_cmd_class_release(context);

	return 0;
}

