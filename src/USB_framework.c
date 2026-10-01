#include "USB_framework.h"

static udReport report;

void doMatrix(void) {
    do {
        //report.reportID = 1;
        report.modKeys = 0;
        for (uint8_t i = 0; i < 7; i++) { // just start at i = 1?
            if (i == 0) { report.keys[i] = 0; continue; }
            if (i == 1) { report.keys[i] = 0x05; continue; }
            report.keys[i] = 0;
        }
    } while (1); // TODO: while !shouldStopMatrix
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
        USB_interface_EPConfigure(0, EPDIR_OUT, EPTYPE_CONTROL, UD_EP0_SIZE, EPBKTYPE_ONEBANK, (CFG_RXSTPE));
        USB_interface_configStatus(USBFUNC_SETZERO);
        //PORTD &= ~(1 << 5);
    }
    if (flags & (1 << SOFI)) {
        // sof...
        USB_interface_EPSend(1, &report, sizeof(report));
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

// TODO: turn these actions into functions for readability
ISR(USB_COM_vect) {
    uint8_t epint = UEINT;
    for (uint8_t i = 0; i < 7; i++) {
        if (epint & (1 << i)) {
            UENUM = i;
            while (UENUM != i);
            uint8_t flags = UEINTX;
            PORTB &= ~(1 << PB0);

            //UENUM = 0;    
            if (UEINTX & (1 << RXSTPI)) {
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

                USB_interface_handshakeSet(TYPE_RXSTPI); // TODO: why do we do this?

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
                            break;
                    }
                    descLen = requestLen > descLen ? descLen : requestLen;
                    while (descLen > 0) {
                        while(!(UEINTX & (1 << TXINI))); // wow
                        uint8_t packetLen = descLen > UD_EP0_SIZE ? UD_EP0_SIZE : descLen;
                        for (uint8_t i = 0; i < packetLen; i++) {
                            UEDATX = pgm_read_byte(descriptor + bytesSent++);
                        }

                        if (bytesSent % UD_EP0_SIZE == 0 && bytesSent > 0) {
                            while(!(UEINTX & (1 << TXINI)));
                            USB_interface_handshakeSet(TYPE_TXINI); // ZLP
                        }

                        descLen -= packetLen;
                        USB_interface_handshakeSet(TYPE_TXINI);
                    }

                    PORTB |= (1 << PB0);
                    return;
                }

                if (bRequest == SET_ADDRESS) {
                    USB_interface_handshakeSet(TYPE_TXINI);
                    while(!(UEINTX & (1 << TXINI)));
                    UDADDR = wValue | (1 << ADDEN);
                    PORTB |= (1 << PB0);
                    return;
                }
                
                if (bRequest == SET_CONFIGURATION) {
                    if (bmRequestType == 0 && USB_interface_configStatus(USBFUNC_RETURN) == 0) {
                        USB_interface_handshakeSet(TYPE_TXINI);
                        USB_interface_EPConfigure(1, EPDIR_IN, EPTYPE_INTERRUPT, UD_EP0_SIZE, EPBKTYPE_ONEBANK, CFG_NONE);
                        
                        /*
                        UERST = 0x7E; // 0b01111110
                        UERST = 0;
                        */
                        
                        PORTB |= (1 << PB0);
                        return;
                    }
                }
                
                if (bRequest == SET_IDLE) {
                    /* TODO: this
                    If there is no data change within the idle period,
                    resend the previous packet. If 0, only send data
                    when there is a change (no resends).
                    */
                    USB_interface_handshakeSet(TYPE_TXINI);
                    PORTB |= (1 << PB0);
                    return;
                }

                if (bRequest == GET_REPORT) {
                    USB_interface_handshakeSet(TYPE_TXINI);
                    PORTB |= (1 << PB0);
                    return;
                }   
            }
            if (flags & (1 << RXOUTI)) {
                // read OUT data from UEDATX here first
                UEINTX &= ~(1 << RXOUTI);
            }
            if (flags & (1 << NAKINI)) { UEINTX &= ~(1 << NAKINI); }
            if (flags & (1 << NAKOUTI)) { UEINTX &= ~(1 << NAKOUTI); }
            if (flags & (1 << STALLEDI)) { UEINTX &= ~(1 << STALLEDI); }
            epint &= ~(1 << i);

            UECONX |= (1 << STALLRQ);
        }
    } 
}