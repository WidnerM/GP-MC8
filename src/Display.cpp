#pragma once

#include <iostream>
#include <string>
#include <sstream>
// #include <format>   // C++20 format not supported on macOS GitHub Actions
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

// append a checksum and f7 to a HEX STRING payload, then convert to a GPMidiMessage
gigperformer::sdk::GPMidiMessage LibMain::makeMCHexMessage(std::string hexpayload, uint8_t type, uint8_t op3, uint8_t op4, uint8_t op5, uint8_t op6)
{
    gigperformer::sdk::GPMidiMessage midimessage;

    // build 16 byte MC prefix + payload + append the 00 f7 into a GPMidiMessage
    midimessage = gigperformer::sdk::GPMidiMessage(Surface.SysexPrefix + hexpayload + "00 f7");
    // scriptLog(textmessage, 1);

    // set opcodes
    midimessage.setValue(OPCODE_2, type);
    midimessage.setValue(OPCODE_3, op3);
    midimessage.setValue(OPCODE_4, op4);
    midimessage.setValue(OPCODE_5, op5);
    midimessage.setValue(OPCODE_6, op6);  // op6, 0x7f tells it MC to save it

    // calculate and place checksum
    midimessage.setValue(midimessage.length() - 2,
        (uint8_t)calculateMCChecksum(midimessage.length(), midimessage.asBytes()));
    return midimessage;
}

std::string LibMain::PresetPayload(uint8_t action, uint8_t toggle, uint8_t number, uint8_t value, uint8_t channel)
{
	// this breaks when compiled for mac on GitHub...  Apparently its c++20 format is not supported
	// std::string hexpayload = (std::string)std::format("{:02x} {:02x} {:02x} {:02x} {:02x} ", action, toggle, number, value, channel);

	std::string hexpayload;
	// build the hex payload for the preset message
	hexpayload = gigperformer::sdk::GPUtils::intToHex(action) +
		gigperformer::sdk::GPUtils::intToHex(toggle) +
		gigperformer::sdk::GPUtils::intToHex(number) +
		gigperformer::sdk::GPUtils::intToHex(value) +
		gigperformer::sdk::GPUtils::intToHex(channel);
	// scriptLog("PresetPayload: " + hexpayload, 1);
	return hexpayload;
}

// sends sysex message to MCx to update a "message" sent by that preset when the appropriate action is taken
// this is intended primarily for changing colors.  We should be able to create a message, then execute that action
// using the EngagePreset(preset, action) function to execute the color change.
void LibMain::UpdatePresetMessage(uint8_t preset, uint8_t msgnum, uint8_t msgtype, uint8_t savetomem, uint8_t action, uint8_t toggle, uint8_t ccnum, uint8_t ccval, uint8_t midichan)
{
    gigperformer::sdk::GPMidiMessage midimessage;

    midimessage = gigperformer::sdk::GPMidiMessage(Surface.SysexPrefix + (std::string) "00 00 00 00 00 00 f7");

    midimessage.setValue(OPCODE_2, (uint8_t) PRESETMSG_OP);
    midimessage.setValue(OPCODE_3, (uint8_t) preset);
    midimessage.setValue(OPCODE_4, (uint8_t) msgnum);
    midimessage.setValue(OPCODE_5, (uint8_t) msgtype);
    midimessage.setValue(OPCODE_6, (uint8_t) savetomem);  // op6, 0x7f tells it MC to save it

    
    // First two bytes of the payload are always(?) action type and toggle type
    midimessage.setValue(16, action);
    midimessage.setValue(17, toggle);
    midimessage.setValue(18, ccnum);
    midimessage.setValue(19, ccval);
    midimessage.setValue(20, midichan);


    midimessage.setValue(midimessage.length() - 2,
        (uint8_t)calculateMCChecksum(midimessage.length(), midimessage.asBytes()));
    sendMidiMessage(midimessage);

    // sendMidiMessage(makeMCHexMessage((std::string) std::format("{:02x} {:02x} ", 
    //    action, toggle) + hexpayload, PRESETMSG_OP, preset, msgnum, msgtype, savetomem));
}

// append a checksum and f7 to a text string payload, then convert to a GPMidiMessage
gigperformer::sdk::GPMidiMessage LibMain::makeMCMessage(std::string payload, uint8_t type, uint8_t column, uint8_t op4, uint8_t op5, uint8_t op6)
{
    gigperformer::sdk::GPMidiMessage midimessage;

    // build 16 byte MC prefix + payload + append the 00 f7 into a GPMidiMessage
    // textmessage = MC8_PREFIX + textToHexString(payload) + " 00 f7";
    midimessage = gigperformer::sdk::GPMidiMessage(Surface.SysexPrefix + (std::string) " " + textToHexString(payload) + " 00 f7");
    // scriptLog(textmessage, 1);

    // set opcodes
    midimessage.setValue(OPCODE_2, type);
    midimessage.setValue(OPCODE_3, column);
    midimessage.setValue(OPCODE_4, op4);
    midimessage.setValue(OPCODE_5, op5);

    // calculate and place checksum
    midimessage.setValue(midimessage.length() - 2,
                         (uint8_t)calculateMCChecksum(midimessage.length(), midimessage.asBytes()));
    return midimessage;
}

gigperformer::sdk::GPMidiMessage LibMain::makeMCMessage(std::string payload, uint8_t type, uint8_t column)
{
    return LibMain::makeMCMessage(payload, type, column, 0, 0);
}

// displays text on MCx based on opcodes supplied
void LibMain::SendTextToMCx(std::string text, uint8_t op2, uint8_t op3, uint8_t op4)
{
    std::string hexmessage, subtext;
    gigperformer::sdk::GPMidiMessage midimessage;

    // subtext = cleanSysex(text);
    // subtext = subtext.substr(0, 19);
    // binmessage = GPUtils::hex2binaryString(hexmessage);
    midimessage = makeMCMessage(text, op2, op3, op4, 0);

    sendMidiMessage(midimessage);
}

// displays a message up to 20 characters for up to 20 seconds measured in 1/10ths of a second
void LibMain::Notify(std::string text, uint8_t duration)
{
    SendTextToMCx(cleanSysex(text).substr(0, 19), MESSAGE_OP, 00, duration);
}

// displays short preset name at position
void LibMain::PresetShortName(std::string text, uint8_t position)
{
    // std::string cleantext = cleanSysex(text) + (std::string) "                                                ";
    // SendTextToMCx(cleantext.substr(0,Surface.ShortNameLen), SHORTNAME_OP, position, 0x00);
    sendMidiMessage(Surface.PresetShortname(position, text));
    if (widgetExists("mcx_d_" + std::to_string(position)))
    {
        setWidgetCaption("mcx_d_" + std::to_string(position), text);
	}
}

void LibMain::PresetToggleName(std::string text, uint8_t position)
{
    std::string cleantext = cleanSysex(text) + (std::string)"                                                ";
    SendTextToMCx(cleantext.substr(0, Surface.ShortNameLen), TOGGLENAME_OP, position, 0x00);
}


// sets preset long name name at position
void LibMain::PresetLongName(std::string text, uint8_t position)
{
    std::string cleantext = cleanSysex(text) + (std::string) "                                                   ";
    SendTextToMCx(cleantext.substr(0, Surface.LongNameLen), LONGNAME_OP, position, 0x7f);
}

// sets all preset Long Names to this so it stays on screen regardless of current preset
void LibMain::LongPresetNames(std::string text)
{
    if (Surface.Color)
    {
        PresetLongName(text, 22); // putting the preset longname (variation or songpart) here
		EngagePreset(32, 1); // engage that preset to display the longname
        // EngagePreset(23, 1); // engage to display the preset longname
        // switch back to whatever preset we're supposed to be on...
    }
    else
    {
        for (uint8_t x = 0; x < 22; x++)
            PresetLongName(text, x);
    }
}

// sets preset long name name at position
void LibMain::CurrentBankName(std::string text)
{
    std::string cleantext = cleanSysex(text) + (std::string) "                                       ";
    SendTextToMCx(cleantext.substr(0, Surface.LongNameLen), BANKNAME_OP, 0, 0);
}



// Initialize SLKM3 screen and any essential variables
void LibMain::InitializeMC8()
{
    std::string hexmessage = UNIVERSAL_QUERY;
    std::string binmessage;

    if (Surface.syncState == 1)
    {
        // may be appropriate to do something based on what kind of MCx it is

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


void LibMain::SetButtonColor(uint8_t button, uint8_t color)  // make and send midi message
{
    uint8_t MidiMessage[] = { 0xBF, 0, 0 };  // CC channel 16 message

    MidiMessage[1] = button;
    MidiMessage[2] = color;
    sendMidiMessage(MidiMessage, sizeof(MidiMessage));
}

// Show value of a widget on its linked control surface item

void LibMain::TogglePreset(uint8_t position, uint8_t value)
{
    uint8_t MidiMessage[3];

    MidiMessage[0] = 0xb0; // cc channel zero
    MidiMessage[1] = value > 0 ? 2 : 3;
    MidiMessage[2] = position;
    sendMidiMessage(MidiMessage, sizeof(MidiMessage));
}

// To engage a preset we send a CC with the number being 10 + the preset position and the value being the action type
void LibMain::EngagePreset(uint8_t position, uint8_t value)
{
    uint8_t MidiMessage[3];

    MidiMessage[0] = 0xb0; // cc channel zero
    MidiMessage[1] = position;
    MidiMessage[2] = value;
    sendMidiMessage(MidiMessage, sizeof(MidiMessage));
}

void LibMain::TogglePage(uint8_t page)
{
    if (Surface.Color) {
        // on MC6 Pro we have to set up an action in a preset to go to the desired absolute page
        EngagePreset(33, page == 0 ? ACTION_LONGDOUBLETAP : ACTION_LONGDOUBLETAPRELEASE);
    }
    else if (Surface.Page != page) EngagePreset(4, 0); // on mc8 we can only toggle page using CC 4 [any]
    Surface.Page = page;
}

void LibMain::DisplayWidgetValue(const SurfaceRow &row, SurfaceWidget widget)
{ 

    PresetShortName(widget.Value > 0 ? widget.ShortNameOn : widget.ShortNameOff, row.FirstID + widget.Column);
    TogglePreset(row.FirstID + widget.Column, widget.Value == 0 ? 0 : 1);
    // PresetLongName(widget.LongName, Row.FirstID + widget.Column);
}

void LibMain::DisplayRow(SurfaceRow & row, bool forcetocurrent) /* forcetocurrent = 0 */
{
    ResetBankIndicators(row);
    // if (row.WidgetID.compare(BOTTOM_TAG) == 0 )
    {
        // scriptLog("DisplayRow: row=" + row.WidgetID + " Showing=" + std::to_string(row.Showing), 1);
        switch (row.Showing)
        {
        case SHOW_BUTTONS:
            DisplayButtons(row, 0, Surface.RowLen);
            break;
        case SHOW_RACKS_SONGS:
            DisplayVariations(row, 0, Surface.RowLen, forcetocurrent);
            break;
        case SHOW_VARS_PARTS:
            DisplayVariations(row, 0, Surface.RowLen, forcetocurrent);
            break;
        case SHOW_KNOBS:
            DisplayKnobs(row);
            break;
        }
    }
    // else if (row.Type == BUTTON_TYPE ) DisplayButtons(row, 0, 4);
}

void LibMain::DisplayRefresh(bool forcetocurrent) /* forcetocurrent = 0 */
{
    DisplayRow(Surface.Row[TOP_ROW], forcetocurrent);
    DisplayRow(Surface.Row[BOTTOM_ROW], forcetocurrent);
    DisplayRow(Surface.Row[T2_ROW], forcetocurrent);
    DisplayRow(Surface.Row[B2_ROW], forcetocurrent);
    // DisplayRow(Surface.Row[E1_ROW]);  // Update preset Long Names for expression pedals
    // DisplayRow(Surface.Row[E2_ROW]);  // not currently implemented in MC8 or MC6 Pro (crashes them)
    // DisplayRow(Surface.Row[E3_ROW]);
    // DisplayRow(Surface.Row[E4_ROW]);
}

void LibMain::ClearDisplayRow(SurfaceRow row)
{
    uint8_t column;
    MC8Color blank;

    blank.makeBlank();

    for (column = 0; column <= Surface.RowLen -1 ; column++)
    {
        PresetShortName("", row.FirstID + column);
        TogglePreset(row.FirstID + column, 0);
		if (Surface.Color) sendMidiMessage(Surface.MakeColorMessage(row.FirstID + column, blank));
    }
}

// populate a MC8Color structure with colors from the specified widgets, or defaults if the widgets don't exist
MC8Color LibMain::PopulateColors(std::string offwidget, std::string onwidget, std::string shiftwidget)
{
	MC8Color colors;

    if (gigperformer::sdk::GigPerformerFunctions::widgetExists(offwidget))
    {
        colors.BackgroundColor[0] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetFillColor(offwidget));
        colors.LedColor[0] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetOutlineColor(offwidget));
        // colors.TextColor[0] = (uint8_t) std::stoi(gigperformer::sdk::GigPerformerFunctions::getWidgetCaption(offwidget)) & 0x7f;
        colors.TextColor[0] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetTextColor(offwidget));
    }
    else
    {
        colors.BackgroundColor[0] = colors.closest_index(MCX_DEFAULT_BACKGROUND);
        colors.LedColor[0] = colors.closest_index(MCX_DEFAULT_LED);
        colors.TextColor[0] = colors.closest_index(MCX_DEFAULT_TEXT);
    }

    if (gigperformer::sdk::GigPerformerFunctions::widgetExists(onwidget))
    {
        colors.BackgroundColor[1] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetFillColor(onwidget));
        colors.LedColor[1] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetOutlineColor(onwidget));
        // colors.TextColor[1] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetTextColor(onwidget));
        colors.TextColor[1] = (uint8_t)std::stoi(gigperformer::sdk::GigPerformerFunctions::getWidgetCaption(onwidget)) & 0x7f;
    }
    else
    {
        colors.BackgroundColor[1] = colors.TextColor[0];
        colors.LedColor[1] = colors.TextColor[0];
        colors.TextColor[1] = colors.BackgroundColor[0];
    }

    if (gigperformer::sdk::GigPerformerFunctions::widgetExists(shiftwidget))
    {
        colors.BackgroundColor[2] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetFillColor(shiftwidget));
        colors.LedColor[2] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetOutlineColor(shiftwidget));
        // colors.TextColor[2] = colors.closest_index(gigperformer::sdk::GigPerformerFunctions::getWidgetTextColor(shiftwidget));
        colors.TextColor[2] = 13;
    }
    else
    {
        colors.BackgroundColor[2] = colors.TextColor[0];
        colors.LedColor[2] = colors.TextColor[0];
        colors.TextColor[2] = colors.BackgroundColor[0];
    }
    // scriptLog("Text colors: " + std::to_string(colors.TextColor[0]) + " " + std::to_string(colors.TextColor[1]), 1);

	return colors;
}