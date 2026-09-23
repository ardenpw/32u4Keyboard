#include "USB_driver.h"

void USB_interface_init(void) {
    cli();

    DDRD |= (1 << 5);
    PORTD |= (1 << 5);
    
    DDRB |= (1 << 0);
    PORTB |= (1 << 0);

    UERST = 127;
    UDCON |= (1 << DETACH);

    UHWCON |= (1 << UVREGE);

    PLLFRQ = 0b01001010;
    PLLCSR = 0b00010010;
    while(!(PLLCSR & (1 << PLOCK)));

    USBCON = 0xB0;
    UDCON = 1;
    USBCON &= ~(1 << FRZCLK);

    UDIEN = 0xC;
    
    UDCON &= ~(1 << DETACH);

    sei();
}