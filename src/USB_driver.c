#include "USB_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static void USB_interface_connect(USB_DETACH_ENUM USB_DETACH_E) {
    if (USB_DETACH_E) {
        UDCON |= (1 << DETACH);
        UDCON &= ~(1 << DETACH);
    } else {
        UDCON &= ~(1 << DETACH);
        UDCON |= (1 << DETACH);
    }

    while ((UDCON & (1 << DETACH)) == USB_DETACH_E) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);
}

static void USB_interface_clockManage(USB_CLK_ENUM USBCLK_E) {
    if (USBCLK_E) {
        USBCON &= ~(1 << FRZCLK);
    } else {
        USBCON |= (1 << FRZCLK);
    }

    while ((USBCON & (1 << FRZCLK)) == USBCLK_E) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);
}

static void USB_interface_EPReset(uint8_t epNum) {
    // or interface shutdown. depending on how you look at it.
    epNum = epNum > MAXEP ? MAXEP : epNum;
    UENUM = epNum;
    while ((UENUM & 0x07) != epNum);

    UECONX &= ~(1 << EPEN);
    UERST |= (1 << epNum);
    while (!(UERST & (1 << epNum)));
    UERST &= ~(1 << epNum);
    UECFG1X &= ~(1 << ALLOC);
}

void USB_interface_powerOn(VBUS_ENUM VBUS_E) {
    // begin misc config.
    // debug pd5
    DDRD |= (1 << 5);
    PORTD |= (1 << 5);

    DDRB |= (1 << 0);
    PORTB |= (1 << 0);
    
    // pll stuff
    CLKSEL0 |= (1 << EXTE);
    while ((CLKSTA & (1 << EXTON)) == 0) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);
    CLKSEL0 |= (1 << CLKS);
    CLKSEL1 &= ~(0xf << EXCKSEL0);

    PLLFRQ = 0b01001010;
    PLLCSR = 0b00010010;

    while ((PLLCSR & (1 << PLOCK)) == 0) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);
    
    // begin usb
    UHWCON |= (1 << UVREGE);
    
    USBCON = 0b10110000;
    UDCON = 0b00000001;

    USB_interface_clockManage(USBCLK_UNFREEZE);

    UDIEN = 0b01001101;
    UDADDR = 0;

    if (VBUS_E) {
        while ((USBSTA & (1 << VBUS)) == 0) {
            PORTB &= ~(1 << 0);
        }
        PORTB |= (1 << 0);
    }
    
    for (uint8_t i = 0; i < MAXEP; i++) {
        USB_interface_EPReset(i);
    }
    
    USB_interface_EPConfigure(0, EPDIR_OUT, EPTYPE_CONTROL, 64, EPBK_ONEBANK);
    
    USB_interface_connect(USB_CONNECT);
}

void USB_interface_EPConfigure(uint8_t epNum, EPDIR_ENUM EPDIR_E, EPTYPE_ENUM EPTYPE_E, uint16_t epSize, EPBK_ENUM EPBK_E) {
    // Deconfiugre then reconfigure at epNum with params

    epNum = epNum > MAXEP ? MAXEP : epNum;
    UENUM = epNum;
    while ((UENUM & 0x07) != epNum);

    USB_interface_EPReset(epNum);

    if (epNum == 0) {
        epSize = epSize > 64 ? 64 : epSize;
    }
    // only allowed to have one ep that is 256B

    if (epSize <= 8) epSize = 0;
    else if (epSize <= 16) epSize = 1;
    else if (epSize <= 32) epSize = 2;
    else if (epSize <= 64) epSize = 3;
    else if (epSize <= 128) epSize = 4;
    else epSize = 5;

    UECONX |= (1 << EPEN);

    UECFG0X = ((EPTYPE_E << EPTYPE0) | (EPDIR_E << EPDIR));
    UECFG1X = ((epSize << EPSIZE0) | (EPBK_E << EPBK0) | (1 << ALLOC)); 

    while ((UESTA0X & (1 << CFGOK)) == 0) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);
}
