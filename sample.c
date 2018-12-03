/*! \brief test AT command library
 *  \author Shylock Hg
 *  \date 2018-04-29
 *  \email tcath2s@gmail.com
 * */

#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#include "inc/at_command.h"
#include "inc/at_fsm.h"
#include "inc/at_table.h"
#include "inc/at_param.h"

/********** test at commands **********/
#define TEST_AT_CMD_HELLO "HELLO"  //!< AT_HELLO=xxx,yyy
int at_cmd_hello_handler0(char * parameter){
        if(NULL == parameter)
                return -1;

        at_cmd_params_t * p_at_cmd_params = at_cmd_params_new(parameter, 2, 2);
        assert(p_at_cmd_params);

        printf("parameters :`%d`-`%s` `%d`-`%s`!\n", 
       	        p_at_cmd_params->params[0]->is_in_quote,
       	        p_at_cmd_params->params[0]->param, 
       	        p_at_cmd_params->params[1]->is_in_quote,
       	        p_at_cmd_params->params[1]->param);

        at_cmd_params_release(p_at_cmd_params);

        //printf("set hello %s!\n",parameter);
        return 0;
}
int at_cmd_hello_handler1(void){
        printf("read hello!\n");
        return 0;
}
/*
int at_cmd_hello_handler2(const char * parameter){

}
*/
int at_cmd_hello_handler3(void){
        printf("exec hello!\n");
        return 0;
}

#define TEST_AT_CMD_HI "HI"
int at_cmd_hi_handler0(char * parameter){
        if(NULL == parameter)
                return -1;

        at_cmd_params_t * p_at_cmd_params = at_cmd_params_new(parameter, 2, 2);
        assert(p_at_cmd_params);

        printf("parameters :`%d`-`%s` `%d`-`%s`!\n", 
       	        p_at_cmd_params->params[0]->is_in_quote,
       	        p_at_cmd_params->params[0]->param, 
       	        p_at_cmd_params->params[1]->is_in_quote,
       	        p_at_cmd_params->params[1]->param);

        at_cmd_params_release(p_at_cmd_params);

        return 0;
}
int at_cmd_hi_handler1(void){
        printf("read hi!\n");
        return 0;
}
/*
int at_cmd_hi_handler2(const char * parameter){

}
*/
int at_cmd_hi_handler3(void){
        printf("exec hi!\n");
        return 0;
}


/********** test at commands **********/

#define TEST_AT_CMD_MAX_LEN       20
#define TEST_AT_CMD_MAX_PARAM_LEN 512
#define TEST_AT_CMD_DELIMITER     '\n'
#define TEST_HASH_TAB_SIZE        100

#define UNUSED(x) (void)(x)

static bool runcond = true;

void sighandler(int signum){
        if(SIGINT == signum){
                runcond = false;
        }
}

#define STR_PARAM "f:h"

/*  \brief mode flag
 *  \enum MODE_SCRIPT interpret a brainfuck script file
 *  \enum MODE_INTERACTIVE run the brainfuck interpreter in interactive mode
 * */
typedef enum {
        MODE_SCRIPT,
        MODE_INTERACTIVE
} app_mode_t;

typedef struct param {
        char * file;
        app_mode_t mode;
} param_t;

int parse_params(int argc, char * argv[], param_t * param) {
	int c = 0;  //!< opt character
	//!< handle command line parameters
	while((c = getopt(argc,argv,STR_PARAM)) != -1){
		switch(c){
			case 'f':
				//!< interpret brainfuck script
				if(access(optarg, F_OK) != -1){
					param->mode = MODE_SCRIPT;
					param->file = optarg;
				}else{
					fprintf(stderr,"Err:unexist file %s!\n",optarg);
                                        return -EINVAL;
				}
				break;
                        case 'h':
                                fprintf(stderr, "Usage: at -f <script>\n"
                                        "       at\n");
                                exit(0);
			case '?':
				if(optopt == 'f') {
					fprintf(
                                                stderr,
                                                "Option -%c requires an argument.\n",optopt);
                                } else if(isprint(optopt)) {
					fprintf(
                                                stderr,
                                                "Unkown option `-%c`.\n",optopt);
                                } else {
					fprintf(
                                                stderr,
                                                "Unkown option character `\\x%x`.\n",optopt);
                                }
			default:
                                return -EINVAL;
				break;
		}
	}
        return 0;
}

int main(int argc, char * argv[]){
        int err = 0;
        //< parse command line input arguments
        param_t param = {
                .file = NULL,
                .mode = MODE_INTERACTIVE
        };
        err = parse_params(argc, argv, &param);
        if (0 > err) {
                return -err;
        }

        //< AT command register table
        at_cmd_cb_t at_cmd_table[2] = {
       	        {AT_FLAG_VISIABLE, TEST_AT_CMD_HI, at_cmd_hi_handler0,
       	                at_cmd_hi_handler1,NULL,at_cmd_hi_handler3},
       	        {AT_FLAG_VISIABLE, TEST_AT_CMD_HELLO, at_cmd_hello_handler0,
       	        at_cmd_hello_handler1,NULL,at_cmd_hello_handler3},
        };

        //< create AT command parser context
        at_cmd_context_t * context = at_cmd_class_new(TEST_HASH_TAB_SIZE,
       	        TEST_AT_CMD_MAX_LEN,TEST_AT_CMD_MAX_PARAM_LEN,TEST_AT_CMD_DELIMITER);

        //< register AT command to context
        at_table_register(context, at_cmd_table, 
       	        sizeof(at_cmd_table)/sizeof(at_cmd_table[0]));
        
        if (MODE_INTERACTIVE == param.mode) {  //!< REPL
                signal(SIGINT, sighandler);

                char buffer[1024] = {0};
                while(runcond){
                        if (NULL != fgets(buffer, sizeof(buffer), stdin)) {
                                at_cmd_execute_script_string(context, buffer);
                        } else {
                                fprintf(stderr, "Err: Read line failed!\n");
                        }
                }
        }
        else {  //!< script
                at_cmd_execute_script(context, param.file);
        }

        at_cmd_class_release(context);

        return 0;
}
