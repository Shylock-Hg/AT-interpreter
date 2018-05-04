/*! \brief simple at command implement
 *  \author Shylock Hg
 *  \date 2018-04-25
 *  \email tcath2s@gmail.com
 * */

#ifndef _AT_COMMAND_H_
#define _AT_COMMAND_H_

#ifdef __cplusplus
	extern "C" {
#endif

///! \defgroup at_command
//  @{

/*  \brief 
 *       |at command prototype|description|
 *       |:--|:--|
 *       |AT+<cmd>                  |execute command without parameters|
 *       |AT+<cmd>=p0[,p1[,p2...[]]]| execute command with parameters|
 *       |AT+<cmd>?                 |query current parameters value|
 *       |AT+<cmd>=?                |query possible parameters value|
 * */

///! \defgroup at_command_prototype
/// @{
//
#include <stdio.h>

/*  \brief at command function pointer prototype
 *  \param pointer to command parameters string
 *  \retval custom return code
*/
typedef int (*at_cmd_handler_t)(const char *);

/*  \brief index of at command callback function in hash value array
 * */
#define AT_CMD_INDEX_NO_PARAM    0
#define AT_CMD_INDEX_WITH_PARAM  1
#define AT_CMD_INDEX_QUERY_PARAM 2
#define AT_CMD_INDEX_CHECK_PARAM 3

#include "hash.h"

/*  \brief at_cmd_t based on hash_t
 *         - key the cmd string(char*)
 *         - value array of handler pointers(at_cmd_handler_t[])
 *         - next the hash collision node
 * */
typedef hash_t at_cmd_t;  //!< key,value,next

//< hash value size - 4 at command callback function pointer
#define HASH_VALUE_COUNT 4
#define HASH_VALUE_SIZE  (sizeof(at_cmd_handler_t)*HASH_VALUE_COUNT)

//#define AT_CMD_MAX_LEN       20  //!< at command name string length
#define AT_CMD_PARAM_MAX_LEN 520  //!< at command parameter string length

/*! \brief at command class based on hash class
 *         - array the hash array(hash * [])
 *         - hash_tab_size the count of hash table array(size_t)
*/
//typedef hash_class_t at_cmd_class_t;
typedef struct at_cmd_class {
	hash_class_t * hash_instance;  //!< array,hash_tab_size
	size_t at_cmd_len;  //!< max length of at command name
	size_t at_cmd_param_len;
	char delimiter[8];  //!< at command delimiter
} at_cmd_class_t;

/// @}  //!< at_command_prototype group

///! \defgroup at_command_inherit_method
/// @{
///< inherit all methods of hash_class_t
/*
#define at_cmd_class_new                   hash_new
#define at_cmd_class_release               hash_release
#define at_cmd_insert(instance,key,value) \
	hash_insert(instance,key,value,HASH_VALUE_SIZE)
#define at_cmd_delete                      hash_delete
#define at_cmd_lookup                      hash_lookup
#define at_cmd_class_dump                  hash_dump
#define at_cmd_class_load                  hash_load
*/
at_cmd_class_t * at_cmd_class_new(size_t HASH_TAB_SIZE, size_t at_cmd_len, size_t at_cmd_param_len, char * delimiter);
void at_cmd_class_release(at_cmd_class_t * instance);
void at_cmd_insert(at_cmd_class_t * instance, const char * cmd, at_cmd_handler_t * handlers);
void at_cmd_delete(at_cmd_class_t * instance, const char * cmd);
at_cmd_t * at_cmd_lookup(at_cmd_class_t * instance,const char * cmd);
/// @} //!< at_command_inherit_method group

///! \defgroup at_command_extend_method
/// @{

///< extend methods 
/*  \brief printf at command to stdout
 * */
#define at_cmd_printf printf

/*  \brief snprintf at command to string
 * */
#define at_cmd_snprintf snprintf

/*  \brief handle at commands[split by '\n'] string
 *  \param cmds at commands string
 * */
void at_cmd_handle_str(at_cmd_class_t * instance, const char * cmds);

/*  \brief handle at commands from stream
 *  \parma f at commands characters stream
 * */
void at_cmd_handle_stream(at_cmd_class_t * instance, FILE * f);

/// @}  //!< at_command_extend_method group

/// @} //!< at_command group

#ifdef __cplusplus
	}
#endif

#endif  //!< _AT_COMMAND_H_

