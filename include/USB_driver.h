#ifndef USB_driver_h
#define USB_driver_h

#include <stdint.h>

#define MAXEP 6

typedef enum {
    EPTYPE_CONTROL,
    EPTYPE_ISOCHRONOUS,
    EPTYPE_BULK,
    EPTYPE_INTERRUPT
} EPTYPE_ENUM;

typedef enum {
    EPDIR_OUT,
    EPDIR_IN
} EPDIR_ENUM;

typedef enum {
    EPBK_ONEBANK,
    EPBK_DOUBLEBANK
} EPBK_ENUM;

typedef enum {
    VBUS_NOPLUGDETECT,
    VBUS_PLUGDETECT
} VBUS_ENUM;

typedef enum {
    USBCLK_FREEZE,
    USBCLK_UNFREEZE
} USB_CLK_ENUM;

typedef enum {
    USB_DETACH,
    USB_CONNECT
} USB_DETACH_ENUM;

void USB_interface_powerOn(VBUS_ENUM VBUS_E);
void USB_interface_EPConfigure(uint8_t epNum, EPDIR_ENUM EPDIR_E, EPTYPE_ENUM EPTYPE_E, uint16_t epSize, EPBK_ENUM EPBK_E);

#endif