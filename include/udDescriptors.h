#ifndef udDescriptors_h
#define udDescriptors_h

#include <avr/pgmspace.h>
#include <stdint.h>
//#include "global.h"

static const uint8_t udDeviceDescriptor[] PROGMEM = {
    0x12, // bLength
    0x01, // bDescriptorType
    0x00, 0x02, // bcdUSB (2.0)
    0x00, // bDeviceClass (defined in interface [in cfg. descriptor])
    0x00, // bDeviceSubclass (above is 0)
    0x00, // bDeviceProtocol
    UD_EP0_SIZE, // max packet size for ep0: 64
    (0x1209 & 255), ((0x1209 >> 8) & 255), // idVendor, https://pid.codes
    (0x0001 & 255), ((0x0001 >> 8) & 255), // idProduct https://pid.codes
    0x00, 0x01, // bcdDevice release number
    0x01, // manufacturer
    0x02, // product
    0x00, // serial
    0x01 // num configurations
};

typedef struct udReport {
    uint8_t reportID;
    uint8_t modKeys;
    uint8_t keys[6];
};

static const uint8_t udHIDReportDescriptor[] PROGMEM = { 
    // TODO: add LEDs
    0x05, 0x01, //  Usage Page: Generic Desktop
    0x09, 0x06, //  Usage: Joystick
    0xA1, 0x01, //  Collection: Application
    
    0x85, 0x01, //      Report ID: (1)

    0x05, 0x07, //      Usage Page: Keyboard
    0x19, 0xE0, //      Usage Minimum: Left Control
    0x29, 0xE7, //      Usage Maximum:  Right GUI
    0x15, 0x00, //      Logical Minimum: (0)
    0x25, 0x01, //      Logical Maximum: (1)
    0x75, 0x01, //      Report Size: (1)
    0x95, 0x08, //      Report Count: (8)
    0x81, 0x02, //      Input: (Data, Var, Abs)

    0x19, 0x00, //      Usage Minimum (0)
    0x29, 0x65, //      Usage Maximum (101)
    0x15, 0x00, //      Logical Minimum (0)
    0x25, 0x65, //      Logical Maximum (101)
    0x75, 0x08, //      Report Size (8)
    0x95, 0x06, //      Report Count (6)
    0x81, 0x00, //      Input (Data, Array)

    0xC0, //        End Collection: Application (Top Level)
   };
   
static const uint8_t udConfigurationDescriptor[] PROGMEM = { 
    //configuration descriptor
    0x09, //bLength
    0x02, //const CONFIGURATION
    0x29, 0x00, //bytes configuration descriptor TOTAL (everything)
    0x01, // num interfaces
    0x01, // bConfigurationValue
    0x00, // iConfiguration
    0x80, // bmAttributes
    0x32, // bMaxPower

    // interface descriptor
    0x09, // bLength
    0x04, // const INTERFACE
    0x00, // 0th interface
    0x00, // no alternates
    0x01, // additional endpoints
    0x03, // HID device (yay! [bInterfaceClass]) 
    0x00, // bInterfaceSubClass
    /*        TODO:
    0x01,  // bInterfaceSubClass - 1 (specified by USB-IF) is the constant for
           // the boot subclass - this keyboard can communicate with the BIOS,
           // but is limited to 6KRO, as are most keyboards
    0x01,  // bInterfaceProtocol - 0x01 (specified by USB-IF) is the protcol
           // code for keyboards
    */
    0x00, // bInterfaceProtocol (defined in HID)
    0x00, // iInterface

    // HID descriptor
    0x09, // bLength
    0x21, // class descriptor type
    0x11, 0x01, // bcdHID release number
    0x00, // country code
    0x01, // num. descriptors
    0x22, // bDescriptorType
    (sizeof(udHIDReportDescriptor) & 0xFF), (sizeof(udHIDReportDescriptor) >> 8),

    // endpoint descriptor (IN)
    0x07,
    0x05, // bDescriptorType
    0b10000001, // bEndpointAddress (ep 1, in)
    0b00000011, // bmAttributes (interrupt)
    0x40, 0x00, // wMaxPacketSize (in B) (size of bank [ep1 will be 64B])
    0x01, // once every ms
};

static const uint8_t udStringLanguageDescriptor[] PROGMEM = {
    4, // bLength
    0x03, // bDescriptorType (string)
    0x09, 0x04, // wLANGID (0x0409 = English US)
};

static const uint8_t udStringManufacturerDescriptor[] PROGMEM = {
    16, // bLength
    0x03, // bDescriptorType (string)
    0x61, 0x00,
    0x72, 0x00,
    0x64, 0x00,
    0x65, 0x00,
    0x6E, 0x00,
    0x70, 0x00,
    0x77, 0x00
};

static const uint8_t udStringDeviceDescriptor[] PROGMEM = {
    20,   // bLength
    0x03, // bDescriptorType (string)
    0x73, 0x00,
    0x6C, 0x00,
    0x61, 0x00,
    0x70, 0x00,
    0x61, 0x00,
    0x74, 0x00,
    0x72, 0x00,
    0x6F, 0x00,
    0x6E, 0x00 
};

#endif // #ifndef udDescriptors.h