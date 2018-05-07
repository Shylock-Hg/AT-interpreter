/*! \brief at command implement 
 *  \author Shylock Hg
 *  \date 2018-04-25
 *  \email tcath2s@gmail.com
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stdlog.h"
#include "at_command.h"

at_cmd_class_t * at_cmd_class_new(size_t HASH_TAB_SIZE, size_t at_cmd_len, size_t at_cmd_param_len, char * delimiter){
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

/* non-portable */
#define AT_CMD_PAT "AT+%[A-Z]"

void at_cmd_handle_str(at_cmd_class_t * instance, const char * cmds){
	if('\0' == cmds[0])
		return;

	char * delimiter = strstr(cmds,instance->delimiter);
	if(NULL == delimiter)
		return;

	//char cmd[AT_CMD_MAX_LEN+1] = {0};
	//char param[AT_CMD_PARAM_MAX_LEN+1] = {0};
	char * cmd   = calloc(sizeof(char),instance->at_cmd_len);
	if(1 == sscanf(cmds, AT_CMD_PAT, cmd)){
		char * anchor = strstr(cmds,cmd);
		anchor += strlen(cmd);
		at_cmd_t * item = NULL;
		switch(*anchor){
		case '=':
			if('?' == *(anchor+1)){  //!< check
				item = at_cmd_lookup(instance, cmd);
				if(NULL != item){
					at_cmd_handler_t * handler = item->value;
					if(NULL != handler){
						if(NULL != handler[AT_CMD_INDEX_CHECK_PARAM])
							handler[AT_CMD_INDEX_CHECK_PARAM](NULL);
					}else
						printf_DBG(DBG_LEVEL_WARN,"unimplement function check parameter\n");
				}else{
					printf_DBG(DBG_LEVEL_WARN,"unregister command-`%s`!\n",cmd);
				}
				
			}else{  //!< command with parameters
				item = at_cmd_lookup(instance, cmd);
				if(NULL != item){
					at_cmd_handler_t * handler = item->value;
					if(NULL != handler){
						if(NULL != handler[AT_CMD_INDEX_WITH_PARAM])
							handler[AT_CMD_INDEX_WITH_PARAM](anchor+1);
					}else
						printf_DBG(DBG_LEVEL_WARN,"unimplement function check parameter\n");
				}else{
					printf_DBG(DBG_LEVEL_WARN,"unregister command-`%s`!\n",cmd);
				}
			
			}
			break;
		case '?':  //!< query
				item = at_cmd_lookup(instance, cmd);
				if(NULL != item){
					at_cmd_handler_t * handler = item->value;
					if(NULL != handler){
						if(NULL != handler[AT_CMD_INDEX_QUERY_PARAM])
							handler[AT_CMD_INDEX_QUERY_PARAM](NULL);
					}else
						printf_DBG(DBG_LEVEL_WARN,"unimplement function check parameter\n");
				}else{
					printf_DBG(DBG_LEVEL_WARN,"unregister command-`%s`!\n",cmd);
				}

			break;
		case '\n':  //!< command without parameters
				item = at_cmd_lookup(instance, cmd);
				if(NULL != item){
					at_cmd_handler_t * handler = item->value;
					if(NULL != handler){
						if(NULL != handler[AT_CMD_INDEX_NO_PARAM])
							handler[AT_CMD_INDEX_NO_PARAM](NULL);
					}else
						printf_DBG(DBG_LEVEL_WARN,"unimplement function check parameter\n");
				}else{
					printf_DBG(DBG_LEVEL_WARN,"unregister command-`%s`!\n",cmd);
				}

			break;
		default:
			printf_DBG(DBG_LEVEL_WARN,"invalid format!\n");
		}
	}else{
		printf_DBG(DBG_LEVEL_WARN,"invalid command format!\n");
	}
	free(cmd);
	
	at_cmd_handle_str(instance, delimiter+strlen(instance->delimiter));
}

void at_cmd_handle_stream(at_cmd_class_t * instance, const char * file){
	FILE * stream = fopen(file,"r");
	if(NULL == stream){
		printf_DBG(DBG_LEVEL_ERR, "Unexist at script file-`%s`!\n",file);
		return;
	}

	char buffer[AT_CMD_PARAM_MAX_LEN] = {0};
	while(NULL != fgets((char*)buffer, sizeof(buffer), stream)){
		at_cmd_handle_str(instance, (char*)buffer);
	}

	fclose(stream);
}

