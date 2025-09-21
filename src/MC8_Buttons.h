// Morningstar MC8 button assignments
// we view the MC8 as a series of rows with buttons in banks of 4
// we use the two least significant bits as button number (0-3, left to right)
// the next two bits are the row (00 = bottom page one, 01 = top page one, 10 = bottom page 2, 11 = top page 2)
// bits four and five indicates an extended message type:  00 = regular button, 01 = long press (bank change functions), 10 = aux buttons
// bank message buttons are 00 = down, 01 = select, 10 = TBD, 11 = up
// bit six indicates it's an expression pedal, with pedal number (0-3) on the two least significant bits
// note - omniport jacks 1 & 2 must be used for aux switches if aux switches are to be used.  

#pragma once

#define MIDI_CC_00              0xB0
#define NOTE_ON_1				0x90
#define NOTE_ON_16				0x9f
#define MCX_BUTTON_BASE 0x00
#define MCX_BUTTON_MAX 0x3f
#define MCX_BUTTON_B1 0x00
#define MCX_BUTTON_T1 0x04
#define MCX_BUTTON_B2 0x08
#define MCX_BUTTON_T2 0x0C

#define MCX_KNOB_1	0x40
#define MCX_KNOB_2	0x41
#define MCX_KNOB_3	0x42
#define MCX_KNOB_4	0x43

#define MCX_BUTTON_ACTION 0x00
#define MCX_ROW_ACTION 0x01
#define MCX_AUX_ACTION 0x02

#define MCX_ACTION_DOWN 0x00
#define MCX_ACTION_SELECT 0x01
#define MCX_ACTION_PAGE 0x02
#define MCX_ACTION_UP 0x03

#define MCX_PAGE1 0x28
#define MCX_PAGE2 0x29

