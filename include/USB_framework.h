#ifndef USB_framework_h
#define USB_framework_h

#include "USB.h"
#include "USB_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>

typedef struct { //__attribute__((packed))
    //uint8_t reportID;
    uint8_t modKeys;
    uint8_t keys[7]; // keys[0] always = 0
} udReport;

void doMatrix(void);

#endif