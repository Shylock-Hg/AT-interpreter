/*! \brief a simple At command parameters parser
 *  \author Shylock Hg
 *  \date 2018-08-20
 *  \email tcath2s@gmail.com
 * */

#ifndef _AT_PARAM_H_
#define _AT_PARAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_AT_PARAM_BUF_LEN 1024

typedef struct at_cmd_param {
    bool is_in_quote;  //!< is parameter string in quote
    char* param;       //!< parameter string in seperating
} at_cmd_param_t;

typedef struct at_cmd_params {
    size_t count;             //!< count of parameters
    at_cmd_param_t** params;  //!< parameter string array
} at_cmd_params_t;

/*! \brief create a param structure from parameter string
 *  \param at_param at command parameter string
 *  \retval at param structure pointer
 * */
at_cmd_param_t* at_cmd_param_new(char* at_param, bool is_in_quote);

/*! \brief release a param structure
 *  \param at_param structure pointer
 * */
void at_cmd_param_release(at_cmd_param_t* at_param);

/*! \brief create a params sturcture from at command parameter string
 *  \param at_param at command parameter string
 *  \param minnum minimal count of at command parameters
 *  \param maxnum maximal count of at command parameters
 *  \retval at command params structure pointer
 * */
at_cmd_params_t* at_cmd_params_new(char* at_param, size_t minnum,
                                   size_t maxnum);

/*! \brief release at command params structure
 *  \param at_params at command params structure pointer
 * */
void at_cmd_params_release(at_cmd_params_t* at_params);

#ifdef __cplusplus
}
#endif

#endif  //!< _AT_PARAM_H_
