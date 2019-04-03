/*! \brief record structure type
 *  \author Shylock Hg
 *  \date 2018-06-25
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <at/internal/at_xrecord.h>


/******************** at command record structure method ********************/
at_cmd_xrecord_t * at_cmd_xrecord_new(const char * name ,
                const char * param, enum AT_CMD_INDEX type){

        at_cmd_xrecord_t * xrecord = malloc(sizeof(at_cmd_xrecord_t));
        assert(NULL != xrecord);
        if (NULL == xrecord) {
                return NULL;
        }

        xrecord->type = type;
        if(name){
                const size_t len_str_name = strnlen(name, sizeof(xrecord->name));
                if (len_str_name >= sizeof(xrecord->name)) {
                        free(xrecord);
                        return NULL;
                }
                strncpy(xrecord->name, name, sizeof(xrecord->name));
                xrecord->name[len_str_name+1] = '\0';
        }
        if(param){
                const size_t len_str_param = strnlen(param, sizeof(xrecord->param));
                if (len_str_param >= sizeof(xrecord->param)) {
                        free(xrecord);
                        return NULL;
                }
                strncpy(xrecord->param, param, sizeof(xrecord->param));
                xrecord->param[len_str_param+1] = '\0';
        }

        return xrecord;
}

void at_cmd_xrecord_release(at_cmd_xrecord_t * xrecord){
        free(xrecord);
}

void at_cmd_xrecord_deinit(at_cmd_xrecord_t * xrecord){
        assert(xrecord);

        memset(xrecord->name, 0x00, sizeof(xrecord->name));
        memset(xrecord->param, 0x00, sizeof(xrecord->param));
        xrecord->type = 0;
}
