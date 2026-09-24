#ifndef USB_h
#define USB_h

// usb stuff
#define GET_STATUS 0x00
#define CLEAR_FEATURE 0x01
#define SET_FEATURE 0x03
#define SET_ADDRESS 0x05
#define GET_DESCRIPTOR 0x06
#define GET_CONFIGURATION 0x08
#define SET_CONFIGURATION 0x09
#define GET_INTERFACE 0x0A
#define SET_INTERFACE 0x0B2

// HID stuff
#define GET_REPORT 0x01
#define GET_IDLE 0x02
#define GET_PROTOCOL 0x03
#define SET_REPORT 0x09
#define SET_IDLE 0x0A
#define SET_PROTOCOL 0x0B

#define UD_EP0_SIZE 0x40 // 64

void usbInit(void);
void prepUSBReport(void);
void ep1RDY(void);
void ep2Dump(void);

#endif //#ifndef USB_h