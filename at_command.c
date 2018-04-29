/*! \brief at command implement 
 *  \author Shylock Hg
 *  \date 2018-04-25
 *  \email tcath2s@gmail.com
 * */

/*  \brief 
 *       |at command prototype      |description|
 *       |:--                       |:--|
 *       |AT+<cmd>                  |execute command without parameters|
 *       |AT+<cmd>=p0[,p1[,p2...[]]]| execute command with parameters|
 *       |AT+<cmd>?                 |query current parameters value|
 *       |AT+<cmd>=?                |query possible parameters value|
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "at_command.h"

at_cmd_class_t * at_cmd_class_new(size_t HASH_TAB_SIZE, size_t at_cmd_len, char * delimiter){
	at_cmd_class_t * at_cmd_instance = malloc(sizeof(at_cmd_class_t));

	hash_class_t * hash_instance = hash_new(HASH_TAB_SIZE);
	at_cmd_instance->hash_instance = hash_instance;

	at_cmd_instance->at_cmd_len    = at_cmd_len;
	strncpy(at_cmd_instance->delimiter,delimiter,sizeof(at_cmd_instance->delimiter));

	return at_cmd_instance;
}

void at_cmd_class_release(at_cmd_class_t * instance){
	hash_release(instance->hash_instance);

	free(instance);
}

void at_cmd_insert(at_cmd_class_t * instance, const char * cmd, at_cmd_handler_t * handlers){
	hash_insert(instance->hash_instance,cmd,handlers,HASH_VALUE_SIZE);
}

void at_cmd_delete(at_cmd_class_t * instance, const char * cmd){
	hash_delete(instance->hash_instance,cmd);
}

at_cmd_t * at_cmd_lookup(at_cmd_class_t * instance,const char * cmd){
	return hash_lookup(instance->hash_instance,cmd);
}

/*
void at_cmd_handle_str(at_cmd_class_t * instance, const char * cmds){
	if('\0' == cmds)
		return;

	char * delimiter = strstr(cmds,AT_CMD_DELIMITER);
	if(NULL == delimiter)
		retrun;

	char cmd[20] = {0};
	while('\0' != *cmds){
		if(0 == sscanf(cmds,"AT+%s=",));
	
		cmds++;
	}
}
*/
