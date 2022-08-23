#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"

// Calculate the checksum required on all MCx messages
uint8_t LibMain::calculateMCChecksum(uint16_t len, uint8_t *ptr)
{
    uint8_t cSum = *(ptr);
    for (int i = 1; i < len - 2; i++)
    {
        cSum ^= *(ptr + i);
    }
    cSum &= 0x7F;
    return cSum;
}

// append a checksum and f7 to a hexstring, then convert to a GPMidiMessage
gigperformer::sdk::GPMidiMessage LibMain::makeMCMessage(std::string payload, uint8_t type, uint8_t column, uint8_t op4)
{
    gigperformer::sdk::GPMidiMessage midimessage;

    // build 16 byte MC prefix + payload + append the 00 f7 into a GPMidiMessage
    // textmessage = MC8_PREFIX + textToHexString(payload) + " 00 f7";
    midimessage = gigperformer::sdk::GPMidiMessage(MC8_PREFIX + (std::string) " " + textToHexString(payload) + " 00 f7");
    // scriptLog(textmessage, 1);

    // set opcodes
    midimessage.setValue(OPCODE_2, type);
    midimessage.setValue(OPCODE_3, column);
    midimessage.setValue(OPCODE_4, op4);

    // calculate and place checksum
    midimessage.setValue(midimessage.length() - 2,
                         (uint8_t)calculateMCChecksum(midimessage.length(), midimessage.asBytes()));
    return midimessage;
}

gigperformer::sdk::GPMidiMessage LibMain::makeMCMessage(std::string payload, uint8_t type, uint8_t column)
{
    return LibMain::makeMCMessage(payload, type, column, 0);
}

// displays text on MCx based on opcodes supplied
void LibMain::SendTextToMCx(std::string text, uint8_t op2, uint8_t op3, uint8_t op4)
{
    std::string hexmessage, subtext;
    gigperformer::sdk::GPMidiMessage midimessage;

    // subtext = cleanSysex(text);
    // subtext = subtext.substr(0, 19);
    // binmessage = GPUtils::hex2binaryString(hexmessage);
    midimessage = makeMCMessage(text, op2, op3, op4);

    sendMidiMessage(midimessage);
}

// displays a message up to 20 characters for up to 20 seconds measured in 1/10ths of a second
void LibMain::Notify(std::string text, uint8_t duration)
{
    SendTextToMCx(cleanSysex(text).substr(0,19), 0x11, 00, duration);
}

// displays short preset name at position
void LibMain::PresetShortName(std::string text, uint8_t position)
{
    std::string cleantext = cleanSysex(text) + (std::string) "             ";
    SendTextToMCx(cleantext.substr(0,10), 0x01, position, 0x00);
}

// sets preset long name name at position
void LibMain::PresetLongName(std::string text, uint8_t position)
{

    std::string cleantext = cleanSysex(text) + (std::string) "                          ";
    // SendTextToMCx(cleantext.substr(0, 24), 0x03, position, 0x00);
}

// sets all preset Long Names to this so it stays on screen regardless of current preset
void LibMain::LongPresetNames(std::string text)
{
    for (uint8_t x = 0; x < 8; x++)
        PresetLongName(text, x);
}

// sets preset long name name at position
void LibMain::CurrentBankName(std::string text)
{
    std::string cleantext = cleanSysex(text) + (std::string) "                          ";
    // SendTextToMCx(cleantext.substr(0, 24), 0x10, 0, 0);
}



// Initialize SLKM3 screen and any essential variables
void LibMain::InitializeMC8()
{
    std::string hexmessage = UNIVERSAL_QUERY;
    std::string binmessage;

    if (Surface.syncState == 1)
    {
        // should do something to find out what kind of MCx it is

    }
}

// converts a GP widget color integer to a hex string for SL midi
std::string LibMain::GPColorToSLColorHex(int color)
{
    std::string hexstring;

    hexstring = gigperformer::sdk::GPUtils::intToHex((uint8_t)(color >> 17 & 0x7f)) +
                gigperformer::sdk::GPUtils::intToHex((uint8_t)(color >> 9 & 0x7f)) +
                gigperformer::sdk::GPUtils::intToHex((uint8_t)(color >> 1 & 0x7f));

    return hexstring;
}



// lights keylights on SL MKIII
void LibMain::Keylights(const uint8_t* data, int length)
{
    if (data[1] >= 0x18 && data[1] < 0x54) { // 0x44 to 0x80 for sysex or 00 to 3c  | key range is 0x18 - 0x54 but 0x54 is actually pad up arrow
        // if (data[0] == 0x90) sendMidiMessage(GPMidiMessage::makeNoteOnMessage(data[1], data[2], 16));
        // if (data[0] == 0x80) sendMidiMessage(GPMidiMessage::makeNoteOffMessage(data[1], data[2], 16));
        uint8_t red = data[2];
        uint8_t blue = 0x7f - data[2];
        if (data[0] == 0x90) SetButtonRGBColor(data[1] + 0x2c, ( blue < 90 ? 0x010000 * red : 0x00) + (red > 90 ? 0x00 : blue ));
        if (data[0] == 0x80) SetButtonRGBColor(data[1] + 0x2c, 0x000000);
    }
}

// these should only be called if we're changing one item and not touching the rest of the screen
void LibMain::ShowTopLabelColor(uint8_t position, uint8_t color)
{
    uint8_t knobcolor[] = TOPBAR_SYSEX;
    knobcolor[KNOB_Column] = position;
    knobcolor[KNOB_Data] = color;
    sendMidiMessage(knobcolor, sizeof(knobcolor));
}

void LibMain::ShowBottomLabelColor(uint8_t position, uint8_t color)
{
    uint8_t knobcolor[] = BOTBAR_SYSEX;
    knobcolor[KNOB_Column] = position;
    knobcolor[KNOB_Data] = color;
    sendMidiMessage(knobcolor, sizeof(knobcolor));
}


void LibMain::ShowBottomHighlight(uint8_t position, uint8_t color)  // color is 1 for highlight, 0 for not highlight
{
    uint8_t knobcolor[] = BOTBAR_HILIGHT_SYSEX;
    knobcolor[KNOB_Column] = position;
    knobcolor[KNOB_Data] = color;
    sendMidiMessage(knobcolor, sizeof(knobcolor));
}


void LibMain::DisplayText(uint8_t column, uint8_t row, std::string text)  // top row = 0, bottom = 3
{
    std::string hexmessage, subtext, binmessage;

    // format is Sysex header + property (02) + column (0-8) + 01 (text ID) + row (0-3) + text (up to 9 char) + 00 + end
    subtext = cleanSysex(text);
    subtext = subtext.substr(0, 9);
    hexmessage = SLMK3_SYS_HEADER + (std::string) " 02 " + gigperformer::sdk::GPUtils::intToHex(column) + " 01 " +
                 gigperformer::sdk::GPUtils::intToHex(row) + " " + textToHexString(subtext) + " 00 f7";
    // scriptLog("Text: " + hexmessage, 1);
    binmessage = gigperformer::sdk::GPUtils::hex2binaryString(hexmessage);
    sendMidiMessage(binmessage);
}

// The following three ShowKnobXXX() routines are unused for now because the SLMKIII screen gets glitchy and drops items if we set them all
// in succession using these sysex calls.  Instead we do it all in the DisplayKnobs() routine all in one group.
void LibMain::ShowKnobColor(uint8_t position, uint8_t color)
{
    uint8_t knobcolor[] = KNOB_COLOR_SYSEX;
    knobcolor[KNOB_Column] = position;
    knobcolor[KNOB_Data] = color;
    sendMidiMessage(knobcolor, sizeof(knobcolor));
} 

void LibMain::ShowKnobLabel(uint8_t column, const std::string label)
{
    DisplayText(column, 0, label);
}

void LibMain::ShowKnobCaption(uint8_t column, const std::string label)
{
    DisplayText(column, 1, label);
}

void LibMain::SetButtonColor(uint8_t button, uint8_t color)  // make and send midi message
{
    uint8_t MidiMessage[] = { 0xBF, 0, 0 };  // CC channel 16 message

    MidiMessage[1] = button;
    MidiMessage[2] = color;
    sendMidiMessage(MidiMessage, sizeof(MidiMessage));
}

void LibMain::SetButtonRGBColor(uint8_t button, int value) // int parameter will display RGB color using sysex
{
    sendMidiMessage(gigperformer::sdk::GPMidiMessage(SLMK3_SYS_HEADER + (std::string) " 03 " +
                                                     gigperformer::sdk::GPUtils::intToHex(button) + " 01 " +
                                                     GPColorToSLColorHex(value) + " f7"));
}

// Show value of a widget on its linked control surface item
/*  void LibMain::DisplayWidgetValue(const SurfaceRow &row, uint8_t column, std::string text, double value)
{
    DisplayWidgetValue(row, column, text, (uint8_t)(127 * value));
}

void LibMain::DisplayWidgetValue(const SurfaceRow & row, uint8_t column, std::string text, uint8_t value)
{
    PresetShortName((value != 0 ? (std::string) "*" : "") + text, column);
}

void LibMain::DisplayWidgetValue(const SurfaceRow &row, uint8_t column, std::string text, int value)
{
    PresetShortName((value != 0 ? (std::string) "*" : "") + text, column); 
} */

void LibMain::DisplayWidgetValue(const SurfaceRow &row, SurfaceWidget widget)
{ 
    PresetShortName(widget.Value > 0 ? widget.ShortNameOn : widget.ShortNameOff, row.FirstID + widget.Column);
    // PresetLongName(widget.LongName, Row.FirstID + widget.Column);
}

void LibMain::DisplayPresetLongname(const SurfaceRow& row, uint8_t column, std::string text)
{
    /*  if (Row.Type == KNOB_TYPE)
    {
        DisplayText(column, 1, line1);
        // DisplayText(column, 0, line2);
    }
    else if (Row.Type == BUTTON_TYPE) */
        PresetLongName(text, row.FirstID + column);
    // else if (Row.Type == PAD_TYPE) Notify(line1, 10);
}


void LibMain::DisplayRow(SurfaceRow row)
{
    ResetBankIndicators(row);
    // if (row.WidgetID.compare(BOTTOM_TAG) == 0 )
    {
        scriptLog("DisplayRow: row=" + row.WidgetID + " Showing=" + std::to_string(row.Showing), 1);
        switch (row.Showing)
        {
        case SHOW_BUTTONS:
            DisplayButtons(row, 0, 4);
            break;
        case SHOW_RACKS_SONGS:
            DisplayVariations(row, 0, 4, true);
            break;
        case SHOW_VARS_PARTS:
            DisplayVariations(row, 0, 4, true);
            break;

        }
    }
    // else if (row.Type == BUTTON_TYPE ) DisplayButtons(row, 0, 4);
}

void LibMain::ClearDisplayRow(SurfaceRow row)
{
    uint8_t column;

    for (column = 0; column <= row.Columns -1 ; column++)
    {
        PresetShortName("", row.FirstID + column);
    }
}
