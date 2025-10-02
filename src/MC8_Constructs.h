// For classes and such for SL MK3 controller

#define UNIVERSAL_QUERY "F0 7E 7F 06 01 F7"
#define MFG_ID { 0x00, 0x21, 0x24}

#define MCCOLOR_PRESET 23

#define MC8PRO_PREFIX "f0 00 21 24 08 00 70 00 00 00 00 00 00 00 00 00"
#define MC8PRO_ROWLEN 4
#define MC8PRO_SHORTLEN 32
#define MC8PRO_LONGLEN 32
#define MC8PRO_COLOR true
#define MC8PRO_COLORCHANGE "f0 00 21 24 08 00 70 05 00 00 00 00 00 00 00 00 55 55 55 55 01 02 03 04 05 06 07 08 09"
#define MC8_COLOR_PAYLOAD "55 55 55 55 01 02 03 04 05 06 07 08 09" // last nine are 3 LED colors, 3 text colors, 3 background colors
#define MC8_POS_COLOR_LEDS 20
#define MC8_POS_COLOR_TEXT 23
#define MC8_POS_COLOR_BACK 26

#define MC8_PREFIX "f0 00 21 24 04 00 70 00 00 00 00 00 00 00 00 00"
#define MC8_ROWLEN 4
#define MC8_SHORTLEN 10
#define MC8_LONGLEN 24
#define MC8_COLOR false

#define MC6PRO_PREFIX "f0 00 21 24 06 00 70 00 00 00 00 00 00 00 00 00"
#define MC6PRO_ROWLEN 3
#define MC6PRO_SHORTLEN 32
#define MC6PRO_LONGLEN 32
#define MC6PRO_COLOR true

// I don't know if these are right for the MC6
#define MC6_PREFIX "f0 00 21 24 03 00 70 00 00 00 00 00 00 00 00 00"
#define MC6_ROWLEN 3
#define MC6_SHORTLEN 10
#define MC6_LONGLEN 24
#define MC6_COLOR false

#define MC8_COLOR_ARRAY { 0xcd5c5c, 0xfa8072, 0xff0000, 0xb22222, 0x8b0000, 0xffb6c1, 0xff69b4, 0xff1493, 0xdb7093, 0xffa07a, 0xff6347, 0xff4500, 0xff8c00, 0xffa500, 0xffd700, 0xffff00, 0xffffe0, 0xf0e68c, 0xbdb76b, 0xe6e6fa, 0xd8bfd8, 0xda70d6, 0x663399, 0x8a2be2, 0x800080, 0x4b0082, 0x7b68ee, 0x00ff00, 0x228b22, 0x6400, 0x6b8e23, 0x808000, 0x8fbc8f, 0x8080, 0x0000ff, 0x00ffff, 0x40e0d0, 0x5f9ea0, 0x4682b4, 0xb0c4de, 0x87ceeb, 0xfff8dc, 0xd2b48c, 0xa52a2a, 0x800000, 0xd3d3d3, 0x808080, 0x708090, 0xffffff, 0x0 }

// #define MC8_SYSEX     { 0xf0, 0x00, 0x21, 0x24, 0x04, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define OPCODE_1 6
#define OPCODE_2 7
#define OPCODE_3 8
#define OPCODE_4 9
#define OPCODE_5 10
#define OPCODE_6 11
#define OPCODE_7 12
#define TX_ID 13

#define SHORTNAME_OP 0x01
#define TOGGLENAME_OP 0x02
#define LONGNAME_OP 0x03
#define PRESETMSG_OP 0x04
#define OTHERDATA_OP 0x05
#define BANKNAME_OP 0x10
#define MESSAGE_OP 0x11

#define ACTION_NOTHING 0x00
#define ACTION_PRESS 0x01
#define ACTION_RELEASE 0x02
#define ACTION_LONGPRESS 0x03
#define ACTION_LONGRELEASE 0x04
#define ACTION_DOUBLETAP 0x05
#define ACTION_DOUBLETAPRELEASE 0x06
#define ACTION_LONGDOUBLETAP 0x07
#define ACTION_LONGDOUBLETAPRELEASE 0x08


#define MSG_NOTHING 0x00
#define MSG_PC 0x01
#define MSG_CC 0x02

#define TOGGLE_TYPE_1 0x00
#define TOGGLE_TYPE_2 0x01
#define TOGGLE_TYPE_BOTH 0x02
#define TOGGLE_TYPE_SHIFT 0x03

#define LAYOUT_WIDGETNAME "mcx_layout"
#define EXTENSION_IDENTIFIER "Morningstar MCx Extension"
#define MCX_MIDI_IN "^MIDIIN2.*MC8 Pro.*", "MIDIIN2 (Morningstar MC8)", "MIDIIN2 (Morningstar MC6 Pro)", "MIDIIN2 (Morningstar MC6)", ".*MC6 Pro Port 2.*", ".*MC8 Pro Port 2.*"
#define MIDI_IN_WIDGETNAME "mcx_midiin"
#define MCX_MIDI_OUT "^MIDIOUT2.*MC8 Pro.*", "MIDIOUT2 (Morningstar MC8)", "MIDIOUT2 (Morningstar MC6 Pro)", "MIDIOUT2 (Morningstar MC6)", ".*MC6 Pro Port 2.*", ".*MC8 Pro Port 2.*"
#define MIDI_OUT_WIDGETNAME "mcx_midiout"
#define MCX_CONFIG_WIDGETNAME "mcx_row_config"
#define MCX_INITIAL_CONFIG_WIDGETNAME "mcx_initial_row_config"
#define MCX_COLOR_RACKSPACE_SELECTED "mcx_color_rackspace_selected"
#define MCX_COLOR_RACKSPACE_UNSELECTED "mcx_color_rackspace_unselected"
#define MCX_COLOR_VARIATION_SELECTED "mcx_color_variation_selected"
#define MCX_COLOR_VARIATION_UNSELECTED "mcx_color_variation_unselected"
#define MCX_COLOR_SONG_SELECTED "mcx_color_song_selected"
#define MCX_COLOR_SONG_UNSELECTED "mcx_color_song_unselected"
#define MCX_COLOR_SONGPART_SELECTED "mcx_color_songpart_selected"
#define MCX_COLOR_SONGPART_UNSELECTED "mcx_color_songpart_unselected"
#define MCX_COLOR_WIDGET_SELECTED "mcx_color_widget_selected"
#define MCX_COLOR_WIDGET_UNSELECTED "mcx_color_widget_unselected"

#define MCX_DEFAULT_BACKGROUND 0
#define MCX_DEFAULT_LED 0xffffff
#define MCX_DEFAULT_TEXT 0xffffff

#define KNOB "k"
#define FADER "f"
#define BUTTON "b"
#define PAD "p"


