/*! \brief stdlog library
 *  \author Shylock Hg
 *  \date 2018-04-14
 *  \email tcath2s@gmail.com
 * */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void println_buf(uint8_t buf[], size_t len){
	for(size_t i = 0; i<len; i++){
		printf("%2x",buf[i]);
	}
	putchar('\n');
}

