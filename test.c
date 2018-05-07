/*! \brief test AT command library
 *  \author Shylock Hg
 *  \date 2018-04-29
 *  \email tcath2s@gmail.com
 * */

#include <string.h>

#include "at_command.h"

#define AT_CMD_MAX_LEN       20
#define AT_CMD_MAX_PARAM_LEN 512
#define AT_CMD_DELIMITER     "\n"
#define HASH_TAB_SIZE        100

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
	//< create at parser instance
	at_cmd_handler_t handlers[HASH_VALUE_COUNT] = {at_cmd_hello_handler0,
		at_cmd_hello_handler1,NULL,at_cmd_hello_handler3};

	at_cmd_class_t * instance = at_cmd_class_new(HASH_TAB_SIZE,
			AT_CMD_MAX_LEN,AT_CMD_MAX_PARAM_LEN,AT_CMD_DELIMITER);

	at_cmd_insert(instance,TEST_AT_CMD_HELLO,handlers);

/*
        //< handle standard input
	//char cmd[200] = {0};
	while(1){
		printf("[info]:please input command :\n");
		scanf("%s",cmd);
		strcat(cmd,instance->delimiter);
		at_cmd_handle_str(instance,cmd);

	}
*/

	//< handle file steam input
	FILE * f = fopen("test.at","r");
	at_cmd_handle_stream(instance, f);

	fclose(f);
	at_cmd_class_release(instance);

	return 0;
}

