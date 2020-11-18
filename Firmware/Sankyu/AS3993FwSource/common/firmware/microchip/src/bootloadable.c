/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
//#include <p24Fxxxx.h>
#include "p24FJ256DA210.h"
#include "bootloadable.h"

/* the following lines ensure that an application is loadable by
   the ams bootloader version 2.x.y */

extern void _reset();/* so that we can get the address of the reset - code */
const unsigned short userReset __attribute__ ((space(psv),address(USER_PROG_RESET_ADDR))) = (unsigned short)_reset;
const unsigned short appId __attribute__ ((space(psv),address(APP_ID_ADDR))) = APP_ID;

void __attribute__((space(prog),address(ENABLE_BOOTLOADER_ADDR))) enableBootloader()
{
    while ( 1 ) ; /* the bootloader brings this function with real functionality,
		here we just make sure if your application calls this function is never returns; */
}
