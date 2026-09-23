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
    EPBKTYPE_ONEBANK,
    EPBKTYPE_DOUBLEBANK
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

typedef enum {
    /* Enable an endpoint interrupt when an isochronous overflow or underflow condition is detected (OVERFI or UNDERFI). */
    CFG_FLERRE = 128,
    /* Enable an endpoint interrupt when the host sends an IN token but the endpoint cannot provide data, causing a NAK (NAKINI). */
    CFG_NAKINE = 64,
    /* Enable an endpoint interrupt when the host sends an OUT token but the endpoint cannot accept the received data, causing a NAK (NAKOUTI). */
    CFG_NAKOUTE = 16,
    /* Enable an endpoint interrupt after a SETUP packet is received. Normally used by control endpoint zero (RXSTPI). */
    CFG_RXSTPE = 8,
    /* Enable an endpoint interrupt after an OUT packet is received and its payload is available in the endpoint FIFO (RXOUTI). */
    CFG_RXOUTE = 4,
    /* Enable an endpoint interrupt after the endpoint returns a STALL handshake to the host (STALLEDI). */
    CFG_STALLEDE = 2,
    /* Enable an endpoint interrupt when the IN FIFO/bank is ready to accept the next packet from firmware (TXINI). */
    CFG_TXINE = 1
} UEIENX_CFG_ENUM;

typedef enum {
    PLL_STOP,
    PLL_START,
    PLL_CPUEXT
} PLLCFG_ENUM;

typedef enum {
    USBFUNC_RETURN = 2,
    USBFUNC_SETONE = 1,
    USBFUNC_SETZERO = 0
} USB_FUNCTION_RETURN_ENUM;

typedef enum {
    SM_SLEEPDISABLE,
    SM_SLEEPENABLE,
    SM_IDLE = (1 << 1),
    SM_ADCNOISEREDUC = (2 << 1),
    SM_POWERDOWN = (3 << 1),
    SM_POWERSAVE = (4 << 1),
    SM_STANDBY = (5 << 1),
    SM_EXTENDSTANDBY = (6 << 1)
} SM_ENUM;

void USB_interface_powerOn(VBUS_ENUM VBUS_E);
void USB_interface_EPConfigure(uint8_t epNum, EPDIR_ENUM EPDIR_E, EPTYPE_ENUM EPTYPE_E, uint16_t epSize, EPBK_ENUM EPBK_E, UEIENX_CFG_ENUM CFG_E);

static uint8_t USB_interface_configStatus(USB_FUNCTION_RETURN_ENUM USBFUNC_E) {
    static uint8_t status = 0;
    switch (USBFUNC_E) {
        case 0:
            status = 0;
            break;
        case 1:
            status = 1;
            break;
        case 2: 
            break;
    }
    return status;
}

#endif