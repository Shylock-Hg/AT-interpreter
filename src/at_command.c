/*! \brief at command implement 
 *  \author Shylock Hg
 *  \date 2018-04-25
 *  \email tcath2s@gmail.com
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <at/internal/stdlog.h>
#include <at/at_command.h>

at_cmd_context_t * at_cmd_class_new(size_t HASH_TAB_SIZE, 
        	size_t at_cmd_len, size_t at_cmd_param_len, char delimiter){
        at_cmd_context_t * at_cmd_instance = malloc(sizeof(at_cmd_context_t));

        hash_class_t * hash_instance = hash_new(HASH_TAB_SIZE);
        at_cmd_instance->hash_instance = hash_instance;

        at_cmd_instance->at_cmd_len    = at_cmd_len;

        at_cmd_instance->at_cmd_param_len = at_cmd_param_len;

        //strncpy(at_cmd_instance->delimiter,
        //delimiter,sizeof(at_cmd_instance->delimiter));
        at_cmd_instance->delimiter = delimiter;

        return at_cmd_instance;
}

void at_cmd_class_release(at_cmd_context_t * instance){
        hash_release(instance->hash_instance);

        free(instance);
}

void at_cmd_insert(at_cmd_context_t * instance, const char * cmd, 
        	void * handlers){
        hash_insert(instance->hash_instance,cmd,handlers,AT_CMD_HASH_VALUE_SIZE);
}

void at_cmd_delete(at_cmd_context_t * instance, const char * cmd){
        hash_delete(instance->hash_instance,cmd);
}

at_cmd_t * at_cmd_lookup(at_cmd_context_t * instance,const char * cmd){
        return hash_lookup(instance->hash_instance,cmd);
}
