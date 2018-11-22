/*! \brief record structure type
 *  \author Shylock Hg
 *  \date 2018-06-25
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/at_xrecord.h"


/******************** at command record structure method ********************/
at_cmd_xrecord_t * at_cmd_xrecord_new(const char * name ,
                const char * param, enum AT_CMD_INDEX type){

        at_cmd_xrecord_t * xrecord = malloc(sizeof(at_cmd_xrecord_t));

        xrecord->type = type;
        if(name){
                strncpy(xrecord->name, name, sizeof(xrecord->name));
        }
        if(param){
                strncpy(xrecord->param, param, sizeof(xrecord->param));
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
