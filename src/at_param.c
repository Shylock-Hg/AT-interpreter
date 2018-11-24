/*! \brief a simple at command parameters parser
 *  \author Shylock Hg
 *  \date 2018-08-20
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/at_param.h"

at_cmd_param_t * at_cmd_param_new(char * at_param, bool is_in_quote){
        assert(at_param);

//        size_t len_str = strlen(at_param);
        size_t len_str = strnlen(at_param, MAX_AT_PARAM_BUF_LEN);
        if (len_str >= MAX_AT_PARAM_BUF_LEN) {
                return NULL;
        }

        at_cmd_param_t * p_at_param = malloc(sizeof(at_cmd_param_t));
        assert(p_at_param);
        p_at_param->param = malloc(len_str+1);
        strncpy(p_at_param->param, at_param, len_str+1);
        p_at_param->param[len_str] = '\0';
        p_at_param->is_in_quote = is_in_quote;

        return p_at_param;
}

void at_cmd_param_release(at_cmd_param_t * at_param){
        assert(at_param);

        free(at_param->param);
        free(at_param);
}


at_cmd_params_t * at_cmd_params_new(char * at_param, size_t minnum, 
                size_t maxnum){
        assert(at_param);
        assert(minnum);
        assert(maxnum);

        bool is_in_quote = false;
        char buf[1024] = {0};  //!< max length of one at command parameter string
        size_t pos = 0;  //!< position of buf
        //char * start = at_param;
        char * iter = at_param;
        size_t index = 0;  //!< index of p_at_cmd_params->count
        bool flag_in_quote = false;
        at_cmd_params_t * p_at_cmd_params = malloc(sizeof(at_cmd_params_t));
        p_at_cmd_params->count = 0;
        p_at_cmd_params->params = NULL;

        //printf("at_param == `%s`!\n", at_param);

        //while('\0' != *iter){
        for (; *iter != '\0'; iter++) {
       	        //putchar(*iter);
                if(',' == *iter){  //!< comma
       	                 //printf("is in quote == `%d`!\n", is_in_quote);
                        if(!is_in_quote){
       	                        p_at_cmd_params->count++;
                	}
        	}else if('"' == *iter){  //!< quote
                	is_in_quote = !is_in_quote;
                }else{  //!< others

        	}

       	        //iter++;
        }
        p_at_cmd_params->count++;
        //printf("Count of parmeters is `%d`!\n",	p_at_cmd_params->count);

        if(is_in_quote || p_at_cmd_params->count < minnum || 
       	        p_at_cmd_params->count > maxnum){
       	        //free(p_at_cmd_params);
                at_cmd_params_release(p_at_cmd_params);
       	        return NULL;
        }

        p_at_cmd_params->params = calloc(sizeof(at_cmd_param_t*), 
       	        p_at_cmd_params->count);
        
        //iter = at_param;
        //while('\0' != *iter){
        for (iter = at_param; *iter != '\0'; iter++) {
                if(',' == *iter){  //!< comma
                        if(!is_in_quote){  //!< out of quote
                                buf[pos] = '\0';
                                p_at_cmd_params->params[index++] = 
                                at_cmd_param_new(buf, flag_in_quote);
                                pos = 0;
                                flag_in_quote = false;
                        }else{  //!< in quote
                                buf[pos++] = *iter;
                                //printf("Parameter character is `%c`!\n", *iter);
                        }

                }else if('"' == *iter){  //!< quote
       	                is_in_quote = !is_in_quote;
       	                if(!is_in_quote){
       	                        flag_in_quote = true;
       	                }
                }else{  //!< others
       	                buf[pos++] = *iter;
       	                //printf("Parameter character is `%c`!\n", *iter);
        	}

                //iter++;
        }
        buf[pos] = '\0';
        p_at_cmd_params->params[index++] = at_cmd_param_new(buf, flag_in_quote);

        return p_at_cmd_params;
}

void at_cmd_params_release(at_cmd_params_t * at_params){
        assert(at_params);
        
        for(int i=0; i<at_params->count; i++){
                at_cmd_param_release(at_params->params[i]);
        }
        
        free(at_params->params);
        free(at_params);
}
