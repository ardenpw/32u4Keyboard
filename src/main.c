#include <avr/io.h>

#include "USB_driver.h"

int main(void) {
    USB_interface_init();
    for(;;);
    return 0;
}