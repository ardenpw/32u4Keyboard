#include "USB_driver.h"

static uint8_t USB_interface_EPSet(uint8_t epNum) {
    epNum = epNum > MAXEP ? MAXEP : epNum;
    UENUM = epNum;
    while ((UENUM & 0x07) != epNum);
    return epNum;
}

void USB_interface_EPConfigure(uint8_t epNum, EPDIR_ENUM EPDIR_E, EPTYPE_ENUM EPTYPE_E, uint16_t epSizeB, EPBK_ENUM EPBK_E, UEIENX_CFG_ENUM CFG_E) {
    USB_interface_EPSet(epNum);

    //USB_interface_EPReset(epNum);

    if (epNum == 0) {
        epSizeB = epSizeB > 64 ? 64 : epSizeB;
    }
    // only allowed to have one ep that is 256B

    if (epSizeB <= 8) epSizeB = 0;
    else if (epSizeB <= 16) epSizeB = 1;
    else if (epSizeB <= 32) epSizeB = 2;
    else if (epSizeB <= 64) epSizeB = 3;
    else if (epSizeB <= 128) epSizeB = 4;
    else epSizeB = 5;

    UECONX |= (1 << EPEN);

    UECFG0X = ((EPTYPE_E << EPTYPE0) | (EPDIR_E << EPDIR));
    UECFG1X = ((epSizeB << EPSIZE0) | (EPBK_E << EPBK0) | (1 << ALLOC)); 

    /*
    if (!(UESTA0X & (1 << CFGOK))) {
        cli();
        PORTB &= ~(1 << PB0);
        return;
    }
    */

    while ((UESTA0X & (1 << CFGOK)) == 0) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);

    UERST = (1 << epNum);
    UERST = 0;

    UEIENX = CFG_E;
}

void USB_interface_EPSend(uint8_t epNum, void* buf, uint8_t len) {
    USB_interface_EPSet(epNum);
    
    // could cause problems
    if ((UEINTX & (1 << RWAL)) == 0) return;

    USB_interface_handshakeSet(TYPE_TXINI);
    
    uint8_t* bytes = (uint8_t*)buf;
    for (uint8_t i = 0; i < len - 1; i++) {
        UEDATX = bytes[i];
    }

    while ((UEINTX & (1 << RWAL)) == 0);

    USB_interface_handshakeSet(TYPE_FIFOCON);
}

void USB_interface_EPRead(uint8_t epNum, uint8_t* buf) {
    USB_interface_EPSet(epNum);

    if ((UEINTX & (1 << RXOUTI)) == 0) return;

    uint16_t byteCount = (UEBCHX << 8) | (UEBCLX);

    for (uint8_t i = 0; i < byteCount; i++) {
        buf[i] = UEDATX;
    }

    USB_interface_handshakeSet(TYPE_RXOUTI);
    USB_interface_handshakeSet(TYPE_FIFOCON);
}
// TODO: Ideally this would be in USB_framework.c, but this works for now
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

