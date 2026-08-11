#include <avr/io.h>

#include "USB_driver.h"

int main(void) {
    USB_interface_powerOn(VBUS_PLUGDETECT);
    for(;;);
    return 0;
}