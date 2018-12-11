/* 
 * File:   mem_locations.h
 * Author: phil
 *
 * Created on 04 August 2018, 11:00
 */
#ifndef MEM_LOCATIONS_H
#define	MEM_LOCATIONS_H
/* Definitions used for bootloader */
#define SPECIAL 0x3000 /* includes boot mem, exception mem, debug exec mem */
#define BL_LENGTH 0x4000 /* Length of bootloader's main flash component */

/* Variables to pass into the bootloader. All ad
 * dresses are virtual.
   These represent the base and length of the user or application
   flash memory. */
#define APP_BASE  (0x9D000000 + BL_LENGTH)
#define APP_LENGTH  (0x40000 - BL_LENGTH)
#define FLASH_BLOCK_SIZE 0x0800 /* From the datasheet */
#define CONFIG_WORDS_BASE 0x1fc01740
#define CONFIG_WORDS_TOP  0x1fc017F0

#define APP_CONFIG_LOCATION 0x9D009000
#define APP_LEG_LOCATION    0x9D009800
#define APP_CONFIG_SIZE     0x00000800
#define APP_LEG_SIZE        0x00002800

#ifndef __DEBUG 
#define CONST_STORE const
#else
#define CONST_STORE
#endif


//put data at specified place if on pic, otherwise align with 0x800 boundary
#ifdef __XC32
#ifndef __MPLAB_DEBUGGER_SIMULATOR
#define PLACE_DATA_AT(addr) __attribute__((address(addr), space(data)))
#else
#define PLACE_DATA_AT(addr) __attribute__((aligned(0x800)))
#endif
#else
#define PLACE_DATA_AT(addr) __attribute__((aligned(0x800)))
#endif


#endif	/* MEM_LOCATIONS_H */

