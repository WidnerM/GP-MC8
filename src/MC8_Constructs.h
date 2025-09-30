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

#define PRESET_SAVE 0x7f
#define PRESET_DONTSAVE 0

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

#define SLMK3_SYS_HEADER "F0 00 20 29 02 0A 01 "
#define SLMK3_SYS_ENDER "F7"
#define SLMK3_SYS_SPACER "00"

#define SLMK3_EMPTY_LAYOUT "F0 00 20 29 02 0A 01 01 00 F7"
#define SLMK3_KNOB_LAYOUT "F0 00 20 29 02 0A 01 01 01 F7"
#define SLMK3_BOX_LAYOUT "F0 00 20 29 02 0A 01 01 02 F7"

#define SLMK3_ENABLE_LEDS "F0 00 20 29 02 0A 01 05 01 F7"
#define SLMK3_DISABLE_LEDS "F0 00 20 29 02 0A 01 05 00 F7"

#define SLMK3_SYSHEX         { 0xf0, 0x00, 0x20, 0x29, 0x02, 0x0a, 0x01 }
#define KNOB_LAYOUT_SYSEX    { 0xf0, 0x00, 0x20, 0x29, 0x02, 0x0a, 0x01, 0x01, 0x01, 0xf7 }
#define KNOB_POSITION_SYSEX  { 0xf0, 0x00, 0x20, 0x29, 0x02, 0x0a, 0x01, 0x02, 0x00, 0x03, 0x00, 0x64, 0xF7 }
#define TOPBAR_SYSEX         { 0xf0, 0x00, 0x20, 0x29, 0x02, 0x0a, 0x01, 0x02, 0x00, 0x02, 0x00, 0x64, 0xF7 }
#define KNOB_COLOR_SYSEX     { 0xf0, 0x00, 0x20, 0x29, 0x02, 0x0a, 0x01, 0x02, 0x00, 0x02, 0x01, 0x64, 0xF7 }
#define BOTBAR_SYSEX         { 0xf0, 0x00, 0x20, 0x29, 0x02, 0x0a, 0x01, 0x02, 0x00, 0x02, 0x02, 0x64, 0xF7 }
#define BOTBAR_HILIGHT_SYSEX { 0xf0, 0x00, 0x20, 0x29, 0x02, 0x0a, 0x01, 0x02, 0x00, 0x03, 0x01, 0x64, 0xF7 }

#define BOTTOM_COLOR_SYSEX     "f0 00 20 29 02 0a 01 02 00 02 02"
#define BOTTOM_HIGHLIGHT_SYSEX "f0 00 20 29 02 0a 01 02 00 03 01"

#define SLMK3_NOTIFICATION SLMK3_SYS_HEADER + "04 "

#define KNOB_Column		8   // offset in Knob sysex of column
#define KNOB_Data		11  //  offset for position & color in KNOB_Color_sysex & KNOB_Position_Sysex (0-127)

#define TEXT_Column		8
#define TEXT_Row		10
#define TEXT_Text		11
#define NOTIFY_Text		8


#define SLMK3_PROPERTY_CMD SLMK3_SYS_HEADER "02" // followed by row, prop [Text=1, Color=2, Value=3], col, color/text/values
#define PROPERTY_TEXT " 01"
#define PROPERTY_COLOR " 02"
#define PROPERTY_VALUE " 03" // generally 00 for normal, 01 for highlight

#define SLMK3_LED_COMMAND SLMK3_SYS_HEADER "03 "
#define LED_Position 8
#define LED_R 10
#define LED_G 11
#define LED_B 12

#define EMPTY_LAYOUT  0 // display nothing.  Completely clears main display panels including center panel
#define KNOB_LAYOUT  1 // display knobs icons in center of main display panels
#define BOX_LAYOUT	2 // display 3 sets of 2 text lines in each display segment

#define BUTTON_VIEW_MODE	3 // display buttons in center and bottom of main panel
#define FADER_VIEW_MODE		4 // display faders in center of main panels

#define FADER_MODE 6 // diplay racks and variations on the bottom bar


