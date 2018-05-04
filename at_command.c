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
/*
#define AT_CMD_PAT_CHECK_PARAM "AT+%[A-Z]%[=?]\n"
#define AT_CMD_PAT_WITH_PARAM  "AT+%[A-Z]=%[^\n]\n"
#define AT_CMD_PAT_QUERY_PARAM "AT+%[A-Z]%[?]\n"
#define AT_CMD_PAT_NO_PARAM    "AT+%[A-Z]\n"
*/
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
						printf("[warn]:unimplement function check parameter\n");
				}else{
					printf("[warn1]:unregister command-`%s`!\n",cmd);
				}
				
			}else{  //!< command with parameters
				item = at_cmd_lookup(instance, cmd);
				if(NULL != item){
					at_cmd_handler_t * handler = item->value;
					if(NULL != handler){
						if(NULL != handler[AT_CMD_INDEX_WITH_PARAM])
							handler[AT_CMD_INDEX_WITH_PARAM](anchor+1);
					}else
						printf("[warn]:unimplement function check parameter\n");
				}else{
					printf("[warn1]:unregister command-`%s`!\n",cmd);
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
						printf("[warn]:unimplement function check parameter\n");
				}else{
					printf("[warn1]:unregister command-`%s`!\n",cmd);
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
						printf("[warn]:unimplement function check parameter\n");
				}else{
					printf("[warn1]:unregister command-`%s`!\n",cmd);
				}

			break;
		default:
			printf("[warn]:invalid format!\n");
		}
	}else{
		printf("[warn]:invalid command format!\n");
	}
	//char * param = calloc(sizeof(char),instance->at_cmd_param_len);
	//char temp[10] = {0};
	/*
	if(2 == sscanf(cmds,AT_CMD_PAT_CHECK_PARAM,cmd,temp)){
		printf("[info]:command check parameters\n");
		at_cmd_t * item = at_cmd_lookup(instance, cmd);
		if(NULL != item){
			at_cmd_handler_t * handler = item->value;
			if(NULL != handler){
				if(NULL != handler[AT_CMD_INDEX_CHECK_PARAM])
					handler[AT_CMD_INDEX_CHECK_PARAM](NULL);
			}else
				printf("[warn]:unimplement function check parameter\n");
		}else{
			printf("[warn1]:unregister command-`%s`!\n",cmd);
		}
	
	}
	else if(2 == sscanf(cmds,AT_CMD_PAT_WITH_PARAM,cmd,param)){
		printf("[info]:command with parameters\n");
		at_cmd_t * item = at_cmd_lookup(instance, cmd);
		if(NULL != item){
			at_cmd_handler_t * handler = item->value;
			if(NULL != handler){
				if(NULL != handler[AT_CMD_INDEX_WITH_PARAM])
					handler[AT_CMD_INDEX_WITH_PARAM](param);
			}else
				printf("[warn]:unimplement function with parameter\n");
		}else{
			printf("[warn0]:unregister command-`%s`!\n",cmd);
		}

	}
	else if(2 == sscanf(cmds,AT_CMD_PAT_QUERY_PARAM,cmd,temp)){
		printf("[info]:command query parameters\n");
		at_cmd_t * item = at_cmd_lookup(instance, cmd);
		if(NULL != item){
			at_cmd_handler_t * handler = item->value;
			if(NULL != handler){
				if(NULL != handler[AT_CMD_INDEX_QUERY_PARAM])
					handler[AT_CMD_INDEX_QUERY_PARAM](NULL);
			}else
				printf("[warn]:unimplement function query parameter\n");
		}else{
			printf("[warn2]:unregister command-`%s`!\n",cmd);
		}
	
	}
	else if(1 == sscanf(cmds,AT_CMD_PAT_NO_PARAM,cmd)){
		printf("[info]:command without parameters\n");
		at_cmd_t * item = at_cmd_lookup(instance, cmd);
		if(NULL != item){
			at_cmd_handler_t * handler = item->value;
			if(NULL != handler){
				if(NULL != handler[AT_CMD_INDEX_CHECK_PARAM])
					handler[AT_CMD_INDEX_CHECK_PARAM](NULL);
			}else
				printf("[warn]:unimplement function no parameter\n");
		}else{
			printf("[warn3]:unregister command-`%s`!\n",cmd);
		}
	
	}
	else{
		printf("[warn]:invalid format at command-`%s`\n",cmds);
	}
	*/
	free(cmd);
	//free(param);
	
	at_cmd_handle_str(instance, delimiter+strlen(instance->delimiter));
}

