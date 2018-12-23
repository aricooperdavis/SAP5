/*
 * M-Stack USB Bootloader
 *
 *  M-Stack is free software: you can redistribute it and/or modify it under
 *  the terms of the GNU Lesser General Public License as published by the
 *  Free Software Foundation, version 3; or the Apache License, version 2.0
 *  as published by the Apache Software Foundation.  If you have purchased a
 *  commercial license for this software from Signal 11 Software, your
 *  commerical license superceeds the information in this header.
 *
 *  M-Stack is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this software.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  You should have received a copy of the Apache License, verion 2.0 along
 *  with this software.  If not, see <http://www.apache.org/licenses/>.
 *
 * Alan Ott
 * Signal 11 Software
 * 2016-03-31
 */

#include <xc.h>
#include <string.h>
#include <stdbool.h>


/* Variables from linker script.
 *
 * The way to pass values from the linker script to the program is to create
 * variables in the linker script and extern them in the program.  The only
 * catch is that assignment operations in the linker script only set
 * variable _addresses_, and not variable values.  Thus to get the data in
 * the program, you need to take the _address_ of the variables created by
 * the linker script (and ignore their actual values).  This may seem hacky,
 * but the GNU LD manual cites it as the recommended way to do it.  See
 * section 3.5.5 "Source Code Reference."
 *
 * It's also worth noting that addresses in the Linker Script are virtual,
 * and what we want in most cases in a bootloader are physical addresses.
 */

#include "mcc_generated_files/mcc.h"
#include "app_type.h"
#include "mem_locations.h"

#include "battery.h"
#include "i2c_util.h"
#include "display.h"
#include "usb_callbacks.h"
#include "utils.h"
#include "mcc_generated_files/usb/usb_hal_pic32mm.h"

#define DISPLAY_ADDRESS 0x3C



static int8_t write_display(uint8_t page, uint8_t column, uint8_t* buffer, uint16_t len) {
    write_i2c_data1(DISPLAY_ADDRESS,0xB0+page);
    write_i2c_data1(DISPLAY_ADDRESS,column & 0x0F);
    write_i2c_data1(DISPLAY_ADDRESS,16 + (column / 16));
    return write_i2c_command_block(DISPLAY_ADDRESS,0x40,buffer,len);
}


uint8_t reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void display_show_bat(int charge) {
	static int counter = 0;
	int i;
	// set page and column to top right
	char bat_status[24];
	if (charge==-1) {
		charge = counter;
		counter++;
		if (counter>18) counter = 0;
	}
	bat_status[0] = 0xf8;
	bat_status[1] = 0x04;
	memset(&bat_status[2],0xf4,charge);
	memset(&bat_status[2+charge],0x04,19-charge);
	bat_status[21] = 0xf8;
	bat_status[22] = 0x20;
	bat_status[23] = 0xC0;
	render_data_to_page(0,104,bat_status,24);
	for (i = 0; i< 24; ++i) {
		bat_status[i] = reverse(bat_status[i]);
	}
	render_data_to_page(1,104,bat_status,24);
}


void sleep(void) {
    //TRISA = 0;
    //TRISB = 0;
    //TRISC = 0;
    TRISCbits.TRISC9 = 1;
    TRISBbits.TRISB6 = 1;
    //LATA = 0;
    //LATB = 0;
    //LATC = 0;
    INTCONbits.MVEC = 1;
    //    CNBI: PORT B Change Notification
    //    Priority: 7
        IPC2bits.CNBIP = 7;
    //    Sub Priority: 0
        IPC2bits.CNBIS = 0;
    //    CNCI: PORT C Change Notification
    //    Priority: 7
        IPC2bits.CNCIP = 7;
    //    Sub Priority: 0
        IPC2bits.CNCIS = 0;

    SYSKEY = 0x12345678; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
    PMDCONbits.PMDLOCK = 0;
    PMD1 = 0xffffffff;
    PMD2 = 0xffffffff;
    PMD3 = 0xffffffff;
    PMD4 = 0xffffffff;
    PMD5 = 0xffffffff;
    PMD6 = 0xfffffffe; //leave RTCC on...
    PMD7 = 0xffffffff;
    PMDCONbits.PMDLOCK = 1;
    PWRCONbits.VREGS = 0;
    PWRCONbits.RETEN = 0;    
    OSCCONbits.SLPEN = 1;
    SYSKEY = 0x12345678;
    __builtin_enable_interrupts();
    asm("wait");
    asm("nop;nop;nop;nop;");
    //re-enable modules
    SYSKEY = 0x12345678; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
    PMDCONbits.PMDLOCK = 0;
    PMD1 = 0x0;
    PMD2 = 0x0;
    PMD3 = 0x0;
    PMD4 = 0x0;
    PMD5 = 0x0;
    PMD6 = 0x0;
    PMD7 = 0x0;
    PMDCONbits.PMDLOCK = 1;
    SYSKEY = 0x12345678;
}

void JumpToApp(void)
{       
        void (*fptr)(void);
        PERIPH_EN_SetDigitalOutput();
        PERIPH_EN_SetHigh();
        INTERRUPT_GlobalDisable();
        DMACONbits.ON = 0;
        fptr = (void (*)(void))APP_BASE+1;
        fptr();
}       


void run_usb(void) {
    int counter = 0;
    //Re-enable peripherals...
    
	enum BAT_STATUS bat_status;
    RTCC_TimeReset(true);
    SYSTEM_Initialize();
	/* setup ports */	
/* enable peripherals */
    PIN_MANAGER_Initialize();
    PERIPH_EN_SetHigh();
    TMR2_Start();
    delay_ms(100);
	display_init();
	display_clear_screen();
	delay_ms(3);
	while (1) {
		bat_status = battery_get_status();
		//if (bat_status==DISCHARGING) {
        //    U1PWRCbits.USBPWR = 0;
        //    sys_reset(0);
        //}
		counter++;
		if ((counter & 0xffff)==0) {
			// only update display every 32 cycles
            display_show_bat(-1);
		}
        wdt_clear();
	}
}

int main(void)
{
    PERIPH_EN_SetHigh();
    run_usb();
}

