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
    /*
    When writing or reading a FIFO, since they are not memory mapped, so you do everything
    through UEDATX (there is no descrirbed procedure for moving data). Basically, when you 
    setup your endpoint, the ALLOC command allocates stuff required for your ep config., 
    including your bank. When the USB controller receives a new packet, it fires the required 
    interrupt, in which we begin to read (or write) to UEDATX. Any subsiquent read or write 
    to UEDATX, increments an internal pointer to point UEDATX at the next byte in our DPRAM 
    bank for processing. 
    */
    UENUM = 0;    
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
                /*
                Host sent a configuration request for endpoint wValue,
                specified by the low byte in wValue.
                bmRequestType: 0
                bRequest: 9
                wValue: 1
                wIndex: 0
                wLength: 0
                */
                UEINTX &= ~(1 << TXINI); //handshake
               
                // ep1
                UENUM = 1; // = wValue
                UECONX = (1 << EPEN);
                UECFG0X = 0b11000001; // interrupt, in
                UECFG1X = 0b00110010; // One Bank: 64B, Allocate
                if (!(UESTA0X & (1 << CFGOK))) {
                    cli();
                    //_blink(100);
                    return;
                }
                
                // ep2
                UENUM = 2;
                UECONX = (1 << EPEN);
                UECFG0X = 0b11000000; // interrupt, out
                UECFG1X = 0b00110010; // One Bank: 64B, Allocate
                if (!(UESTA0X & (1 << CFGOK))) {
                    cli();
                    //_blink(100);
                    return;
                }
                
                UERST = 0x7E; // 0b01111110
                UERST = 0;
                
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
            return;
        }   
    }

    UECONX |= (1 << STALLRQ);
}