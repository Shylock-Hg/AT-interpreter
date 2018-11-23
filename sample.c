/*! \brief test AT command library
 *  \author Shylock Hg
 *  \date 2018-04-29
 *  \email tcath2s@gmail.com
 * */

#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <assert.h>

#include "inc/at_command.h"
#include "inc/at_fsm.h"
#include "inc/at_table.h"
#include "inc/at_param.h"

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

//#define REPL

int main(int argc, char * argv[]){
        //< create at parser context
        /*
        at_cmd_handler_t handlers[2][AT_CMD_HASH_VALUE_COUNT] = {{at_cmd_hello_handler0,
       	        at_cmd_hello_handler1,NULL,at_cmd_hello_handler3},
       	        {at_cmd_hi_handler0,at_cmd_hi_handler1,NULL,at_cmd_hi_handler3}};
        */
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
        
#ifdef REPL

        signal(SIGINT, sighandler);

        char buffer[1024] = {0};
        while(runcond){
                fgets(buffer, sizeof(buffer), stdin);
                at_cmd_execute_script_string(context, buffer);
        }

#else

        at_cmd_execute_script(context, argv[1]);

#endif

        at_cmd_class_release(context);

        return 0;
}
