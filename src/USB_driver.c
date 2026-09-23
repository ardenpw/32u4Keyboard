#include "USB_driver.h"
#include "USB.h"
#include "udDescriptors.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static void USB_interface_connect(USB_DETACH_ENUM USB_DETACH_E) {
    if (USB_DETACH_E) {
        UDCON &= ~(1 << DETACH);
    } else {
        UDCON |= (1 << DETACH);
    }

    /*
    while ((UDCON & (1 << DETACH)) == USB_DETACH_E) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);
    */
}

static void USB_interface_clockManage(USB_CLK_ENUM USBCLK_E) {
    if (USBCLK_E) {
        USBCON &= ~(1 << FRZCLK);
    } else {
        USBCON |= (1 << FRZCLK);
    }

    /*
    while ((USBCON & (1 << FRZCLK)) == USBCLK_E) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);
    */
}

static void USB_interface_EPReset(uint8_t epNum) {
    // or interface shutdown. depending on how you look at it.
    epNum = epNum > MAXEP ? MAXEP : epNum;
    //epNum = 0 cant be reset
    UENUM = epNum;
    //while ((UENUM & 0x07) != epNum);

    if (epNum != 0) { UECONX &= ~(1 << EPEN); }
    UERST |= (1 << epNum);
    //while (!(UERST & (1 << epNum)));
    UERST &= ~(1 << epNum);
    UECFG1X &= ~(1 << ALLOC);
}

static void PLL_configure(PLLCFG_ENUM PLL_E) {
    if (PLL_E & PLL_CPUEXT) {
        CLKSEL0 |= (1 << EXTE);
        while ((CLKSTA & (1 << EXTON)) == 0) {
            PORTB &= ~(1 << 0);
        }
        PORTB |= (1 << 0);
        CLKSEL0 |= (1 << CLKS);
        CLKSEL1 &= ~(0xf << EXCKSEL0);
    }

    if (PLL_E & PLL_START) {
        PLLFRQ = 0b01001010;
        PLLCSR = 0b00010010;

        while ((PLLCSR & (1 << PLOCK)) == 0) {
            PORTB &= ~(1 << 0); 
        }
        PORTB |= (1 << 0);
    } 
    else {
        PLLFRQ = 0;
        PLLCSR &= ~(0x12);
    }
}

static void CPU_sleepMode(SM_ENUM SM_E) {
    if (SM_E > 1) {
        SMCR &= ~0xE;
        SMCR |= (SM_E & 0xE);
    }

    if (SM_E & SM_SLEEPENABLE) {
        SMCR |= 1;
    }
    else {
        SMCR &= ~1;
    }
}

void USB_interface_powerOn(VBUS_ENUM VBUS_E) {
    cli();
    
    // begin misc config.
    // debug pd5
    DDRD |= (1 << 5);
    PORTD |= (1 << 5);
    
    DDRB |= (1 << 0);
    PORTB |= (1 << 0);
    
    // pll stuff
    PLL_configure(PLL_START); //PLL_CPUEXT
    
    // begin usb
    UHWCON |= (1 << UVREGE);

    //UERST = 127;
    // this could cuase probemds 
    for (uint8_t i = 0; i < MAXEP; i++) {
        USB_interface_EPReset(i);
    }

    USB_interface_connect(USB_DETACH);  

    
    if (VBUS_E) {
        while ((USBSTA & (1 << VBUS)) == 0) {
            //PORTB &= ~(1 << 0);
        }
        PORTB |= (1 << 0);
    }
    
    
    USBCON = 0b10110000;
    UDCON = 0b00000001;
    //UDADDR = 0;
    

    //USB_interface_EPConfigure(0, EPDIR_OUT, EPTYPE_CONTROL, 64, EPBKTYPE_ONEBANK, CFG_RXSTPE);

    USB_interface_clockManage(USBCLK_UNFREEZE);

    UDIEN = 0b01001101;

    USB_interface_connect(USB_CONNECT);
    
    //PORTD &= ~(1 << 5);

    sei();
}

void USB_interface_EPConfigure(uint8_t epNum, EPDIR_ENUM EPDIR_E, EPTYPE_ENUM EPTYPE_E, uint16_t epSize, EPBK_ENUM EPBK_E, UEIENX_CFG_ENUM CFG_E) {
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
    else if ((USB_interface_configStatus(USBFUNC_RETURN) & 0x1) == 0) {
        if (epSize <= 128) epSize = 4;
        else epSize = 5;
    }
    else epSize = 3;

    UECONX |= (1 << EPEN);

    UECFG0X = ((EPTYPE_E << EPTYPE0) | (EPDIR_E << EPDIR));
    UECFG1X = ((epSize << EPSIZE0) | (EPBK_E << EPBK0) | (1 << ALLOC)); 

    while ((UESTA0X & (1 << CFGOK)) == 0) {
        PORTB &= ~(1 << 0);
    }
    PORTB |= (1 << 0);
}

ISR(USB_GEN_vect) {
    uint8_t flags = UDINT;
    UDINT = 0;
    if (flags & UPRSMI) {
        // resume usb...
        /*
        CPU_sleepMode(SM_SLEEPDISABLE);
        PLL_configure(PLL_START);
        USB_interface_clockManage(USBCLK_UNFREEZE);
        */
        // clear resume information?
    }
    if (flags & (1 << EORSTI)) {
        USB_interface_EPConfigure(0, EPDIR_OUT, EPTYPE_CONTROL, 64, EPBKTYPE_ONEBANK, (CFG_RXSTPE));
        USB_interface_configStatus(USBFUNC_SETZERO);
        //PORTD &= ~(1 << 5);
    }
    if (flags & (1 << SOFI)) {
        // sof...
    }
    if (flags & (1 << SUSPI)) {
        // clear suspend bit?
        /*
        USB_interface_clockManage(USBCLK_FREEZE);
        PLL_configure(PLL_STOP);
        CPU_sleepMode(SM_POWERDOWN | SM_SLEEPENABLE);
        */
    }
}

ISR(USB_COM_vect) {
    //PORTD &= ~(1 << 5);
    uint8_t epint = UEINT;
    for (uint8_t i = 0; i < 7; i++) {
        if (epint & (1 << i)) {
            UENUM = i;
            uint8_t flags = UEINTX;

            if (flags & (1 << RXSTPI)) {
                // read setup packet from UEDATX here first
                UEINTX &= ~(1 << RXSTPI);
                //UEINTX &= ~(1 << RXSTPI);
                // Setup packet 
                volatile uint8_t bmRequestType = UEDATX;
                volatile uint8_t bRequest = UEDATX; // All USB defined traffic is encoded in little endian order.
                volatile uint16_t wValue = UEDATX;  // Since little endian, LSB is the first byte, 
                wValue |= UEDATX << 8;     // then we move MSB to the left by 8b to get our 2B per 'w.'
                volatile uint16_t wIndex = UEDATX;
                wIndex |= UEDATX << 8;
                volatile uint16_t wLength = UEDATX;
                wLength |= UEDATX << 8;

                UEINTX &= ~((1 << RXSTPI) | (1 << RXOUTI) | (1 << TXINI)); // Clear UEINTX flags to handshake

                PORTB ^= (1 << PB0);

                // send device descriptor
                if (bRequest == GET_DESCRIPTOR) { // bmRequestType == 128 && ...
                    // host sent GET_DESCRIPTOR, requesting only (wLength/8) bytes of the descriptor, 
                    // of type (wValue >> 8)
                    uint16_t descLen; // = sizeof(udDeviceDescriptor);
                    const uint8_t *descriptor;
                    uint16_t requestLen = wLength; // > 512 ? 512 : wLength;
                    uint16_t bytesSent = 0;
                    switch (wValue) {
                        case 0x0100: // getDescriptor(device)
                            descriptor = udDeviceDescriptor;
                            descLen = sizeof(udDeviceDescriptor);
                            break;
                        case 0x0200: // getDescriptor(configuration)
                            descriptor = udConfigurationDescriptor;
                            descLen = sizeof(udConfigurationDescriptor);
                            break;
                        case 0x0300: // getDescriptor(string, index 0) it wants the language (US)
                            descriptor = udStringLanguageDescriptor;
                            descLen = sizeof(udStringLanguageDescriptor);
                            break;
                        case 0x0301: // getDescriptor(string, index 1) it wants the manufacturer String
                            descriptor = udStringManufacturerDescriptor;
                            descLen = sizeof(udStringManufacturerDescriptor);
                            break;
                        case 0x0302: // getDescriptor(string, index 2) it wants the product String
                            descriptor = udStringDeviceDescriptor;
                            descLen = sizeof(udStringDeviceDescriptor);
                            break;
                        case 0x0600: // getDescriptor(deviceQualifier)
                            UECONX |= (1 << STALLRQ);
                            break;
                        case 0x2200: // getDescriptor(HIDReport)
                            descriptor = udHIDReportDescriptor;
                            descLen = sizeof(udHIDReportDescriptor);
                            break;
                        default: // Unknown bDescriptorType
                            UECONX |= (1 << STALLRQ);
                            cli();
                            //unknownPacket = 1;
                            //SH1107_drawString(0, 15, 1, "STALL, unk wVal %u", wValue);
                            break;
                    }
                    descLen = requestLen > descLen ? descLen : requestLen;
                    while (descLen > 0) {
                        while(!(UEINTX & (1 << TXINI))); // wow
                        uint8_t packetLen = descLen > UD_EP0_SIZE ? UD_EP0_SIZE : descLen;
                        for (uint8_t i = 0; i < packetLen; i++) {
                            UEDATX = pgm_read_byte(descriptor + bytesSent++);
                        }
                        descLen -= packetLen;
                        UEINTX &= ~(1 << TXINI);
                    }
                    return;
                }

                if (bRequest == SET_ADDRESS) {
                    UEINTX &= ~(1 << TXINI); //handshake
                    while(!(UEINTX & (1 << TXINI)));
                    UDADDR = wValue | (1 << ADDEN);
                    return;
                }

                if (bRequest == SET_CONFIGURATION) {
                    if (bmRequestType == 0 && USB_interface_configStatus(USBFUNC_RETURN) == 0) {
                        UEINTX &= ~(1 << TXINI); //handshake
                    
                        USB_interface_EPConfigure(1, EPDIR_IN, EPTYPE_INTERRUPT, 64, EPBKTYPE_ONEBANK, CFG_TXINE);
                        UERST = 126;
                        UERST = 0;
                        USB_interface_configStatus(USBFUNC_SETONE);
                        return;
                    }
                }
                
                if (bRequest == SET_IDLE) {
                    /* TODO: this
                    If there is no data change within the idle period,
                    resend the previous packet. If 0, only send data
                    when there is a change (no resends).
                    */
                    UEINTX &= ~(1 << TXINI);
                    return;
                }

                if (bRequest == GET_REPORT) {
                    if (wValue == 0x030F) {
                        // Type: Feature, Report ID: (12)
                        // This is the PID Pool Report
                        while(!(UEINTX & (1 << TXINI)));
                        for (uint8_t i = 0; i < wLength; i++) {
                            UEDATX = pgm_read_byte(udPIDPoolReport + i);
                        }
                        UEINTX &= ~(1 << TXINI);
                        return;
                    }
                }
            }
            if (flags & (1 << RXOUTI)) {
                // read OUT data from UEDATX here first
                UEINTX &= ~(1 << RXOUTI);
            }
            if (flags & (1 << NAKINI))  UEINTX &= ~(1 << NAKINI);
            if (flags & (1 << NAKOUTI)) UEINTX &= ~(1 << NAKOUTI);
            if (flags & (1 << STALLEDI)) UEINTX &= ~(1 << STALLEDI);
            epint &= ~(1 << i);
        }
    }
}

