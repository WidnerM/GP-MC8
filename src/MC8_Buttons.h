// Morningstar MC8 button assignments
// we view the MC8 as a series of rows with buttons in banks of 4
// we use the two least significant bits as button number (0-4, left to right)
// the next two bits are the row (00 = bottom page one, 01 = top page one, 10 = bottom page 2, 11 = top page 2)
// bit four indicates whether it's a bank message button or just regular button push
// bank message buttons are 00 = down, 01 = up, 10 = select, 11 = TBD

#pragma once

#define MIDI_CC_16              0xB0
#define NOTE_ON_16				0x9f
#define MCX_BUTTON_BASE 0x00
#define MCX_BUTTON_MAX 0x7f
#define MCX_BUTTON_B1 0x00
#define MCX_BUTTON_T1 0x04
#define MCX_BUTTON_B2 0x08
#define MCX_BUTTON_T2 0x0C
#define MCX_BUTTON_E3 0x10
#define MCX_BUTTON_E4 0x13

#define MCX_BUTTON_ACTION 0x00
#define MCX_ROW_ACTION 0x01

#define MCX_ACTION_DOWN 0x00
#define MCX_ACTION_SELECT 0x01
#define MCX_ACTION_OTHER 0x02
#define MCX_ACTION_UP 0x03


// these aren't correct anymore...
#define MCX_BOTTOM_BANK_DOWN 30
#define MCX_BOTTOM_BANK_UP 31
#define MCX_TOP_BANK_DOWN 32
#define MCX_TOP_BANK_UP 33
#define MCX_BOTTOM_SELECT 34
#define MCX_TOP_SELECT 35

#define MCX_B2_BANK_DOWN 36
#define MCX_B2_BANK_UP 37
#define MCX_T2_BANK_DOWN 38
#define MCX_T2_BANK_UP 39
#define MCX_B2_SELECT 40
#define MCX_T2_SELECT 41
#define MCX_PAGE1 50
#define MCX_PAGE2 51


#define MKIII_KNOB_BASE         0x15
#define MKIII_KNOB_1            0x15
#define MKIII_KNOB_2            0x16
#define MKIII_KNOB_3            0x17
#define MKIII_KNOB_4            0x18
#define MKIII_KNOB_5            0x19
#define MKIII_KNOB_6            0x1A
#define MKIII_KNOB_7            0x1B
#define MKIII_KNOB_8            0x1C

#define MKIII_FADER_BASE_SYSEX  0x36
#define MKIII_FADER_BASE        0x29
#define MKIII_FADER_1           0x29
#define MKIII_FADER_2           0x2A
#define MKIII_FADER_3           0x2B
#define MKIII_FADER_4           0x2C
#define MKIII_FADER_5           0x2D
#define MKIII_FADER_6           0x2E
#define MKIII_FADER_7           0x2F
#define MKIII_FADER_8           0x30

#define MKIII_DISPLAY_BUTTON_1  0x33
#define MKIII_DISPLAY_BUTTON_2  0x34
#define MKIII_DISPLAY_BUTTON_3  0x35
#define MKIII_DISPLAY_BUTTON_4  0x36
#define MKIII_DISPLAY_BUTTON_5  0x37
#define MKIII_DISPLAY_BUTTON_6  0x38
#define MKIII_DISPLAY_BUTTON_7  0x39
#define MKIII_DISPLAY_BUTTON_8  0x3A

#define MKIII_BUTTON_BASE_SYSEX 0x0C
#define MKIII_BUTTON_ROW1_1     0x3B
#define MKIII_BUTTON_ROW1_2     0x3C
#define MKIII_BUTTON_ROW1_3     0x3D
#define MKIII_BUTTON_ROW1_4     0x3E
#define MKIII_BUTTON_ROW1_5     0x3F
#define MKIII_BUTTON_ROW1_6     0x40
#define MKIII_BUTTON_ROW1_7     0x41
#define MKIII_BUTTON_ROW1_8     0x42

#define MKIII_BUTTON_ROW2_1     0x43
#define MKIII_BUTTON_ROW2_2     0x44
#define MKIII_BUTTON_ROW2_3     0x45
#define MKIII_BUTTON_ROW2_4     0x46
#define MKIII_BUTTON_ROW2_5     0x47
#define MKIII_BUTTON_ROW2_6     0x48
#define MKIII_BUTTON_ROW2_7     0x49
#define MKIII_BUTTON_ROW2_8     0x4A

#define MKIII_DISPLAY_UP        0x51
#define MKIII_DISPLAY_DOWN      0x52
#define MKIII_DISPLAY_UP_SYSEX  0x3E
#define MKIII_DISPLAY_DOWN_SYSEX 0x3F

#define MKIII_SCENE_1           0x53
#define MKIII_SCENE_2           0x54
#define MKIII_SCENE_UP          0x55
#define MKIII_SCENE_DOWN        0x56
#define MKIII_SCENE_UP_SYSEX    0x00
#define MKIII_SCENE_DOWN_SYSEX  0x01


#define MKIII_BUTTONS_UP        0x57
#define MKIII_BUTTONS_DOWN      0x58
#define MKIII_BUTTONS_UP_SYSEX  0x1C
#define MKIII_BUTTONS_DOWN_SYSEX 0x1D

#define MKIII_GRID              0x59
#define MKIII_OPTIONS           0x5A
#define MKIII_SHIFT             0x5B
#define MKIII_DUPLICATE         0x5C
#define MKIII_CLEAR             0x5D

#define MKIII_TRACK_LEFT        0x66
#define MKIII_TRACK_RIGHT       0x67

#define MKIII_TRANSPORT_REWIND  0x70
#define MKIII_TRANSPORT_FORWARD 0x71
#define MKIII_TRANSPORT_STOP    0x72
#define MKIII_TRANSPORT_PLAY    0x73
#define MKIII_TRANSPORT_LOOP    0x74
#define MKIII_TRANSPORT_RECORD  0x75

// Pad note on numbers.  Can write colors to them by sending note on, channel 16, color as velocity
#define MKIII_PAD_BASE_SYSEX 0x26
#define MKIII_PAD_BASE 0x60
#define MKIII_PAD1 0x60
#define MKIII_PAD2 0x61
#define MKIII_PAD3 0x62
#define MKIII_PAD4 0x63
#define MKIII_PAD5 0x64
#define MKIII_PAD6 0x65
#define MKIII_PAD7 0x66
#define MKIII_PAD8 0x67
#define MKIII_PAD9 0x70
#define MKIII_PAD10 0x71
#define MKIII_PAD11 0x72
#define MKIII_PAD12 0x73
#define MKIII_PAD13 0x74
#define MKIII_PAD14 0x75
#define MKIII_PAD15 0x76
#define MKIII_PAD16 0x77
