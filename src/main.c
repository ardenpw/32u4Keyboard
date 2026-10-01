#include <avr/io.h>

#include "USB_framework.h"

int main(void) {
    USB_interface_init();
    doMatrix();
    return 0;
}