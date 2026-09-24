#ifndef udDescriptors_h
#define udDescriptors_h

#include <avr/pgmspace.h>
#include <stdint.h>
//#include "global.h"

#define HID_REPORT_INPUT 0x01
#define HID_REPORT_EFFECT_REPORT 0x02
#define HID_REPORT_CONSTANT_FORCE 0x03
#define HID_REPORT_DEVICE_CONTROL 0x04
#define HID_REPORT_EFFECT_OPERATION 0x05
#define HID_REPORT_PARAM_BLOCK_POOLS 0x06

static const uint8_t udDeviceDescriptor[] PROGMEM = {
   18, // bLength
    1, // bDescriptorType
    0x00,
    0x02, // bcdUSB (2.0)
    0, // bDeviceClass (defined in interface [in cfg. descriptor])
    0, // bDeviceSubclass (above is 0)
    0, 
    UD_EP0_SIZE, // 64
    (0x1209 & 255),
    ((0x1209 >> 8) & 255), // idVendor, https://pid.codes
    (0x0001 & 255),
    ((0x0001 >> 8) & 255), // idProduct https://pid.codes
    0x00,
    0x01, // bcdDevice release number
    1,
    2,
    0,
    1
};

static const uint8_t udHIDReportDescriptor[] PROGMEM = {
    0x05, 0x01, //  Usage Page: Generic Desktop
    0x09, 0x04, //  Usage: Joystick
    0xA1, 0x01, //  Collection: Application
    
    0x09, 0x01, //      Usage: Pointer
    0xA1, 0x00, //      Collection: Physical
    0x85, 0x01, //          Report ID: (1)

    0x09, 0x30, //          Usage: X
    0x09, 0x31, //          Usage: Y
    0x16, 0x01, 0x80, //    Logical Minimum: (-32767)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x36, 0x01, 0x80, //    Physical Minimum: (-32767)
    0x46, 0xFF, 0x7F, //    Physical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x02, //          Report Count: (2)
    0x81, 0x02, //          Input: (Data,Var,Abs)
    0xC0, //            End Collection: (Physical)

    // if nothing works. add physical min/maxes to all logical min/maxes

    0x05, 0x0F, //      Usage Page: Physical Interface Device
    0x09, 0x21, //      Usage: Set Effect Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x02, //          Report ID: (2)    

    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)

    // add more effects to this. maybe that will do it...
    0x09, 0x24, //          Usage: Effect Type
    0xA1, 0x02, //          Collection: Logical
    0x09, 0x26, //              Usage: ET Constant-Force
    0x09, 0x27, //              Usage: ET Ramp
    0x09, 0x30, //              Usage: ET Square 
    0x09, 0x31, //              Usage: ET Sine
    0x09, 0x32, //              Usage: ET Triangle
    0x09, 0x33, //              Usage: ET Sawtooth Up
    0x09, 0x34, //              Usage: ET Sawtooth Down
    0x09, 0x40, //              Usage: ET Spring
    0x09, 0x41, //              Usage: ET Damper
    0x09, 0x42, //              Usage: ET Inertia
    0x09, 0x43, //              Usage: ET Friction
    0x15, 0x01, //              Logical Minimum: (1)
    0x25, 0x0B, //              Logical Maximum: (11)
    0x75, 0x08, //              Report Size: (8)
    0x95, 0x01, //              Report Count: (1)
    0x91, 0x00, //              Output: (Data,Arr,Abs)
    0xC0, //                End Collection: Logical (Effect Type)

    0x09, 0x50, //          Usage: Duration
    0x09, 0x54, //          Usage: Trigger Repeat Interval
    0x09, 0x51, //          Usage: Sample Period
    0x09, 0xA7, //          Usage: Start Delay
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x66, 0x03, 0x10, //    Unit: (Eng Lin:Time)
    0x55, 0xFD, //          Unit Exponent: (-3)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x04, //          Report Count: (4)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0x66, 0x00, 0x00, //    Unit: (None)
    0x55, 0x00, //          Unit Exponent: (0)

    0x09, 0x52, //          Usage: Gain
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x00, //    Logical Maximum: (255)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)

    0x09, 0x53, //          Usage: Trigger Button
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x08, //          Logical Maximum: (8)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    // Axes Enable is overridden by Direction Enable
    // Actually, what is going on here?
    0x09, 0x55, //          Usage: Axes Enable
    0xA1, 0x02, //          Collection: Logical
    0x05, 0x01, //              Usage Page: Generic Desktop
    0x09, 0x30, //              Usage: X
    0x15, 0x00, //              Logical Minimum: (0)
    0x25, 0x01, //              Logical Maximim: (1)
    0x75, 0x01, //              Report Size: (1)
    0x95, 0x01, //              Report Count: (1)
    0x91, 0x02, //              Output: (Data,Var,Abs)
    0xC0, //                End Collection: Logical (Axes Enable)

    0x05, 0x0F, //          Usage Page: Physical Interface Device
    0x09, 0x56, //          Usage: Direction Enable
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0x95, 0x06, //          Report Count: (6)
    0x91, 0x03, //          Output: (Const,Var,Abs)

    // add units to this if it doesnt work
    0x09, 0x57, //          Usage: Direction
    0xA1, 0x02, //          Collection: Logical
    0x0B,0x01,0x00,0x0A,0x00, //Usage Ordinals: Instance 1
    0x0B,0x02,0x00,0x0A,0x00, //Usage Ordinals: Instance 2
    0x15, 0x00, //              Logical Minimum: (0)
    0x26, 0xA0, 0x8C, //        Logical Maximum: (36000)
    0x75, 0x10, //              Report Size: (16)
    0x95, 0x01, //              Report Count: (1)
    0x91, 0x02, //              Output: (Data,Var,Abs)
    0xC0, //                End Collection: Logical

    0x09, 0x58, //          Usage: Type Specific Block Offset 
    0xA1, 0x02, //          Collection: Logical
    0x0B,0x01,0x00,0x0A,0x00, //Usage Ordinals: Instance 1
    0x0B,0x02,0x00,0x0A,0x00, //Usage Ordinals: Instance 2
    0x15, 0x00, //              Logical Minimum: (0)
    0x26, 0xFD, 0x7F, //        Logical Maximum: (32765)
    0x75, 0x10, //              Report Size (16)
    0x95, 0x01, //              Report Count: (1)
    0x91, 0x02, //              Output: (Data,Var,Abs)
    0xC0, //                End Collection: Logical (Type Specific Block Offset)
    
    0xC0, //            End Collection: Logical (Set Effect Report)

    0x09, 0x92, //      Usage: PID State Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x03, //          Report ID: (3)

    0x09, 0x94, //          Usage: Effect Playing
    0x09, 0x9F, //          Usage: Device Paused
    0x09, 0xA0, //          Usage: Actuators Enabled
    0x09, 0xA4, //          Usage: Safety Switch
    0x09, 0xA6, //          Usage: Actuator Power
    0x15, 0x00, //          Logical Minimum: (0)
    0x25, 0x01, //          Logical Maximum: (1)
    0x75, 0x01, //          Report Size: (1)
    0x95, 0x05, //          Report Count: (5)
    0x81, 0x02, //          Input: (Data,Var,Abs)
    0x95, 0x03, //          Report Count: (3)
    0x81, 0x03, //          Input (Const,Var,Abs)
    0xC0, //            End Collection: Logical (PID State Report)

    0x09, 0x5A, //      Usage: Set Envelope Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x04, //          Report ID: (4)
    
    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    0x09, 0x5B, //          Usage: Attack Level
    0x09, 0x5D, //          Usage: Fade Level
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x02, //          Report Count: (2)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    // delete this physical min/max
    0x09, 0x5C, //          Usage: Attack Time
    0x09, 0x5E, //          Usage: Fade Time
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (23767)
    0x35, 0x00, //          Physical Minimum: (0)
    0x46, 0xFF, 0x7F, //    Physical Maximum: (23767)
    0x66, 0x03, 0x10, //    Unit: (Eng Lin:Time)
    0x55, 0x0D, //          Unit Exponent: (-3)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x02, //          Report Count: (2)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0x45, 0x00, //          Physical Maximum: (0)
    0x66, 0x00, 0x00, //    Unit: (None)
    0x55, 0x00, //          Unit Exponent: (0)
    0xC0, //            End Collection: Logical (Set Envelope Report)

    0x09, 0x5F, //      Usage: Set Condition Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x05, //          Report ID: (5)
    
    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0x09, 0x23, //          Usage: Parameter Block Offset
    0x15, 0x00, //          Logical Minimum: (0)
    0x25, 0x03, //          Logical Maximum: (3)
    0x75, 0x06, //          Report Size: (6)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    0x09, 0x58, //          Usage: Type Specific Block Offset
    0xA1, 0x02, //          Collection: Logical
    0x0B,0x01,0x00,0x0A,0x00, //Usage Ordinals: Instance 1
    0x0B,0x02,0x00,0x0A,0x00, //Usage Ordinals: Instance 2
    0x75, 0x02, //              Report Size: (2)
    0x95, 0x01, //              Report Count: (1)
    0x91, 0x02, //              Output: (Data,Var,Abs)
    0xC0, //                End Collection: Logical (Type Specific Block Offset)
    
    0x09, 0x60, //          Usage: CP Offset
    0x16, 0x00, 0x80, //    Logical Minimum: (-32767)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    0x09, 0x61, //          Usage: Positive Coefficient
    0x09, 0x62, //          Usage: Negative Coefficient
    0x16, 0x00, 0x80, //    Logical Minimum: (-32767)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x02, //          Report Count: (2)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    0x09, 0x63, //          Usage: Positive Saturation
    0x09, 0x64, //          Usage: Negative Saturation
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x02, //          Report Count: (2)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    0x09, 0x65, //          Usage: Dead Band
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0xC0, //            End Collection: Logical (Set Condition Report)
    
    0x09, 0x6E, //      Usage: Set Periodic Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x06, //          Report ID: (6)
    
    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    0x09, 0x70, //          Usage: Magnitude
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    0x09, 0x6F, //          Usage: Offset
    0x16, 0x00, 0x80, //    Logical Minimum: (-32767)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    // remove physical min/max if it causes problems
    0x09, 0x71, //          Usage: Phase
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0x9F, 0x8C, //    Logical Maximum: (35999)
    0x35, 0x00, //          Physical Minimum: (0)
    0x46, 0x9F, 0x8C, //    Physical Maximum: (35999)
    0x66, 0x14, 0x00, //    Uints: (Eng:Rot, Deg)
    0x55, 0xFE, //          Unit Exponent: (-2)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0x45, 0x00, //          Physical Maximum: (0)
    0x66, 0x00, 0x00, //    Unit: (None)
    0x55, 0x00, //          Unit Exponent: (0)
    
    0x09, 0x72, //          Usage: Period
    0x15, 0x01, //          Logical Minimum: (1)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x35, 0x01, //          Physical Minimum: (1)
    0x46, 0xFF, 0x7F, //    Physical Maximum: (32767)
    0x66, 0x03, 0x10, //    Unit: (Eng Lin:Time)
    0x55, 0x0D, //          Unit Exponent: (-3)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0x45, 0x00, //          Physical Maximum: (0)
    0x66, 0x00, 0x00, //    Unit: (None)
    0x55, 0x00, //          Unit Exponent: (0)
    0xC0, //            End Collection: Logical (Set Periodic Report)
    
    0x09, 0x73, //      Usage: Set Constant Force Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x07, //          Report ID: (7)
    
    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    // add physical min/max if problems
    0x09, 0x70, //          Usage: Magnitude
    0x16, 0x01, 0x80, //    Logical Minimum: (-32767)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0xC0, //            End Collection: Logical (Set Constant Force Report)
    
    0x09, 0x74, //      Usage: Set Ramp Force Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x08, //          Report ID: (8)
    
    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    // add physical min/maxes
    0x09, 0x75, //          Usage: Ramp Start
    0x09, 0x76, //          Usage: Ramp End
    0x16, 0x01, 0x80, //    Logical Minimum: (-32767)
    0x26, 0xFF, 0x7F, //    Logical Maximum: (32767)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x02, //          Report Count: (2)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0xC0, //            End Collection: Logical (Set Ramp Force Report)
    
    0x09, 0x77, //      Usage: Effect Operation Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x09, //          Report ID: (9)
    
    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    
    0x09, 0x78, //          Usage: Effect Operation
    0xA1, 0x02, //          Collection: Logical
    0x09, 0x79, //              Usage: OP Effect Start
    0x09, 0x7A, //              Usage: OP Effect Start Solo
    0x09, 0x7B, //              Usage: OP Effect Stop
    0x15, 0x01, //              Logical Minimum: (1)
    0x25, 0x03, //              Logical Maximum: (3)
    0x75, 0x08, //              Report Size: (8)
    0x95, 0x01, //              Report Count: (1)
    0x91, 0x00, //              Output: (Data,Arr,Var)
    0xC0, //                End Collection: Logical (Effect Operation)
    
    0x09, 0x7C, //          Usage: Loop Count
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x00, //    Logical Maximum: (255)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0xC0, //            End Collection: Logical (Effect Operation Report)
    
    0x09, 0x90, //      Usage: Block Free Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x0A, //          Report ID: (10)
    
    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0xC0, //            End Collection: Logical (Block Free Report)
    
    0x09, 0x95, //      Usage: PID Device Control Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x0B, //          Report ID: (11)
    
    0x09, 0x96, //          Usage: PID Device Control
    0xA1, 0x02, //          Collection: Logical
    0x09, 0x97, //              Usage: DC Enable Actuators
    0x09, 0x98, //              Usage: DC Dissable Actuators
    0x09, 0x99, //              Usage: DC Stop All Effects
    0x09, 0x9A, //              Usage: DC Deivce Reset
    0x09, 0x9B, //              Usage: DC Device Pause
    0x09, 0x9C, //              Usage: DC Device Continue
    0x15, 0x01, //              Logical Minimum: (1)
    0x25, 0x06, //              Logical Maximum: (6)
    0x75, 0x01, //              Report Size: (1)
    0x95, 0x08, //              Report Count: (8)
    0x91, 0x02, //              Output: (Data,Var,Abs)
    0xC0, //                End Collection: Logical (PID Device Control)
    0xC0, //            End Collection: Logical (PID Device Control Report)
    
    0x09, 0x7D, //      Usage: Device Gain Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x0C, //          Report ID: (12)
    
    // add physical min/maxes
    0x09, 0x7E, //          Usage: Device Gain
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x00, //    Logical Maximum: (255)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0x91, 0x02, //          Output: (Data,Var,Abs)
    0xC0, //            End Collection: Logical (Device Gain Report)
    
    0x09, 0xAB, //      Usage: Create New Effect Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x0D, //      Report ID: (13)
    
    0x09, 0x24, //          Usage: Effect Type
    0xA1, 0x02, //          Collection: Logical
    0x09, 0x26, //              Usage: ET Constant-Force
    0x09, 0x27, //              Usage: ET Ramp
    0x09, 0x30, //              Usage: ET Square 
    0x09, 0x31, //              Usage: ET Sine
    0x09, 0x32, //              Usage: ET Triangle
    0x09, 0x33, //              Usage: ET Sawtooth Up
    0x09, 0x34, //              Usage: ET Sawtooth Down
    0x09, 0x40, //              Usage: ET Spring
    0x09, 0x41, //              Usage: ET Damper
    0x09, 0x42, //              Usage: ET Inertia
    0x09, 0x43, //              Usage: ET Friction
    0x15, 0x01, //              Logical Minimum: (1)
    0x25, 0x0B, //              Logical Maximum: (11)
    0x75, 0x08, //              Report Size: (8)
    0x95, 0x01, //              Report Count: (1)
    0xB1, 0x00, //              Feature: (Data,Arr,Abs)
    0xC0, //                End Collection: Logical (Effect Type)
    
    // add physical min/maxes
    0x05, 0x01, //          Usage Page: Generic Desktop
    0x09, 0x3B, //          Usage: Byte Count
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x01, //    Logical Maximum: (511)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0xB1, 0x01, //          Feature: (Const,Arr,Abs)
    0xC0, //            End Collection: Logical (Create New Effect Report)
    0x05, 0x0F, //      Usage Page: Physical Interface Device
    0x09, 0x89, //      Usage: Block Load Report
    0xA1, 0x02, //      Collection Logical
    0x85, 0x0E, //          Report ID: (14)
    
    0x09, 0x22, //          Usage: Effect Block Index
    0x15, 0x01, //          Logical Minimum: (1)
    0x25, 0x28, //          Logical Maximum: (40)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0xB1, 0x02, //          Feautre: (Data,Var,Abs)
    
    0x09, 0x8B, //          Usage: Block Load Status
    0xA1, 0x02, //          Collection: Logical
    0x09, 0x8C, //              Usage: Block Load Success
    0x09, 0x8D, //              Usage: Block Load Full
    0x09, 0x8E, //              Usage: Block Load Error
    0x15, 0x01, //              Logical Minimum: (1)
    0x25, 0x03, //              Logical Maximum: (3)
    0x75, 0x08, //              Report Size: (8)
    0x95, 0x01, //              Report Count: (1)
    0xB1, 0x00, //              Feature: (Data,Arr,Abs)
    0xC0, //                End Collection: Logical (Block Load Status)
    
    // add physical min/max
    0x09, 0xAC, //          Usage: Pool Available
    0x15, 0x00, //          Logical Minimum: (0)
    0x27, 0xFF, 0xFF,0,0, //Logical Maximum: (65535)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Report Count: (1)
    0xB1, 0x00, //          Feature: (Data,Arr,Abs)
    0xC0, //            End Collection: Logical (Block Load Report)
    
    0x09, 0x7F, //      Usage: PID Pool Report
    0xA1, 0x02, //      Collection: Logical
    0x85, 0x0F, //          Report ID: (15)
    
    0x09, 0x80, //          Usage: Ram Pool Size
    0x15, 0x00, //          Logical Minimum: (0)
    0x27, 0xFF, 0xFF,0,0, //Logical Maximum: (65535)
    0x75, 0x10, //          Report Size: (16)
    0x95, 0x01, //          Rpeort Count: (1)
    0xB1, 0x02, //          Feature: (Data,Var,Abs)
    
    0x09, 0x83, //          Usage: Simultaneous Effects Max
    0x15, 0x00, //          Logical Minimum: (0)
    0x26, 0xFF, 0x00, //    Logical Maximum: (255)
    0x75, 0x08, //          Report Size: (8)
    0x95, 0x01, //          Report Count: (1)
    0xB1, 0x02, //          Feature: (Data,Var,Abs)
    
    0x09, 0xA9, //          Usage: Device Managed Pool
    0x09, 0xAA, //          Usage: Shared Parameter Blocks
    0x15, 0x00, //          Logical Minimum: (0)
    0x25, 0x01, //          Logical Maximum: (1)
    0x75, 0x01, //          Report Size: (1)
    0x95, 0x02, //          Report Count: (2)
    0xB1, 0x02, //          Feature: (Data,Var,Abs)
    0x75, 0x01, //          Report Size: (1)
    0x95, 0x06, //          Report Count: (6)
    0xB1, 0x03, //          Feature: (Const,Var,Abs)
    /* 
    */
    0xC0, //            End Collection: Logical (PID Pool Report)

   0xC0, //        End Collection: Application (Top Level)
   };
   
static const uint8_t udConfigurationDescriptor[] PROGMEM = {
    //configuration descriptor
    
    9, //bLength
    0x02, //const CONFIGURATION
    41, 0x00, //bytes configuration descriptor TOTAL (everything)
    1, // num interfaces
    1, // bConfigurationValue
    0, // iConfiguration
    0b10000000, // bmAttributes
    50, // bMaxPower

    // interface descriptor
    9, // bLength
    0x04, // const INTERFACE
    0x00, // 0th interface
    0x00, // no alternates
    2, // additional endpoints
    0x03, // HID device (yay! [bInterfaceClass]) 
    0x00, // bInterfaceSubClass
    0x00, // bInterfaceProtocol (defined in HID)
    0x00, // iInterface

    // HID descriptor
    9, // bLength
    0x21, // class descriptor type
    0x11, 0x01, // bcdHID release number
    0x00, // country code
    0x01, // num. descriptors
    0x22, // bDescriptorType
    (sizeof(udHIDReportDescriptor) & 0xFF), (sizeof(udHIDReportDescriptor) >> 8),

    // endpoint descriptor (IN)
    7,
    0x05, // bDescriptorType
    0b10000001, // bEndpointAddress (ep 1, in)
    0b00000011, // bmAttributes (int.)
    64, 0, // wMaxPacketSize (in B) (size of bank [ep1 will be 64B])
    1, // once every ms

    // endpoint descriptor (OUT)
    7,
    0x05, // bDescriptorType
    0b00000010, // bEndpointAddress (ep 2, out)
    0b00000011, // bmAttributes (int.)
    64, 0, // wMaxPacketSize (in B) (size of bank [ep2 will be 64B])
    1, // once every ms
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
    42, // bLength
    0x03, // bDescriptorType (string)
    0x41, 0x00, 
    0x72, 0x00, 
    0x64, 0x00, 
    0x65, 0x00, 
    0x6E, 0x00, 
    0x27, 0x00, 
    0x73, 0x00, 
    0x20, 0x00, 
    0x57, 0x00, 
    0x68, 0x00, 
    0x65, 0x00, 
    0x65, 0x00, 
    0x6C, 0x00, 
    0x62, 0x00, 
    0x61, 0x00, 
    0x73, 0x00, 
    0x65, 0x00, 
    0x20, 0x00, 
    0x56, 0x00, 
    0x31, 0x00, 
    0x2E, 0x00, 
    0x30, 0x00
};

static const uint8_t udPIDPoolReport[] PROGMEM = {
    0x0F, // Report ID: (15)
    0xFF, 0x00, // Ram Pool Size: (255)
    0x28, // Simultaneous effects max: (40)
    0b00000011 // 1: Shared Blocks, 0: (1) Device Managed
};

#endif // #ifndef udDescriptors.h