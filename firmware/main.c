#include <stdio.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "measure.h"
#include "utils.h"
#include "display.h"
#include "sensors.h"
#include "survey.h"
#include "laser.h"
#include "exception.h"
#include "config.h"
//FIXME
#include "beep.h"
#define TXT_LENGTH 50

void display_error(CEXCEPTION_T e) {
    char text[20];
    const char *error;
    const char *reason;
    const char *file;
    int line;
    display_on();
    laser_off();
    display_clear_screen(true);
    error = exception_get_string(e);
    exception_get_details(&reason, &file, &line);
    snprintf(text,18,"Err: %s", error);
    display_write_multiline(0,text, true);
    snprintf(text,18,"%s", reason);
    display_write_multiline(2,text, true);
    if (strlen(reason)>17) {
        snprintf(text,18,"%s", reason+16);
        display_write_multiline(4,text, true);        
    }
    snprintf(text,18,"%s:%d", file, line);
    display_write_multiline(6,text, true);
    delay_ms_safe(5000);
}

void initialise() {
    wdt_clear();
    RTCC_TimeReset(true);
    SYSTEM_Initialize();
    PERIPH_EN_SetHigh();
    exception_init();
    TMR2_Start();
    config_load();
    survey_current_init();
    delay_ms_safe(100);
    display_init();
    sensors_init();
    beep_initialise();
    wdt_clear();
    display_clear_screen(true);    
}

int main(void)
{
    CEXCEPTION_T e;
    initialise();
    beep_happy();
    while (true)
    {
        Try {
            measure();
        }
        Catch(e) {
            display_error(e);
        }
    }
    return 0;
}
