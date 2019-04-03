/*! \brief standard log information handle lib
 *  \author Shylock Hg
 *  \date 2018-04-14
 *  \email tcath2s@gmail.com
 * */

#ifndef _STDLOG_H_
#define _STDLOG_H_

#ifdef __cplusplus
	extern "C" {
#endif


/*! \brief stdlog characters format 
 *   `[<level>]:<file-line>:<type>:detail...`
 *   - `[err]:./example-23:char:malloc fail`
 *   - `[warn]:./example-13:char:too long string`
 *   - `[log]:./example-03:bin:132442546ABCFFF`
 *
 *  \param <level> [err,warn,log]
 *  \param <file-line>
 * */

/*! \brief stdlog buffer format
 *  `[<level>]:<file-line>:<type>:buffer...`
 * */

/*! \brief DBG debug behavior macro
 *  \value 
 *         |value|behavior|
 *         |:--|:--|
 *         |0  |retarget debug log to null|
 *         |1  |retarget debug log to stdout|
 * */

#define DBG 1

#if (1 == DBG)  //!< retarget log to stdout

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define DBG_LEVEL_ERR  "[err]"
#define DBG_LEVEL_WARN "[warn]"
#define DBG_LEVEL_LOG  "[log]"

#define DBG_TYPE_CHAR  "char"
#define DBG_TYPE_BIN   "bin"

void println_buf(uint8_t buf[], size_t len);

/*! \brief printf characters log
 *  \param level debug information level
 *  \param format format string
 * */
#define printf_DBG(level,format,...) printf(level ":" __FILE__ "-" "%d" ":" DBG_TYPE_CHAR ":" format ,__LINE__,##__VA_ARGS__)

/*! \brief printf buffer log
 *  \param level debug information level
 *  \param buf buffer pointer 
 *  \param len buffer length
 * */
#define println_buf_DBG(level,buf,len) do{\
		printf(level ":" __FILE__ "-" "%d" ":" DBG_TYPE_BIN ":",__LINE__);\
		println_buf((uint8_t*)buf,len);\
	}while(0);


#elif (0 == DBG)  //!< retarget log to null

#define printf_DBG(level,format,...)
#define printf_buf_DBG(level,buf,len)

#endif

#ifdef __cplusplus
	}
#endif

#endif  //!< _STDLOG_H_

