#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"


bool LibMain::RowNextBank(SurfaceRow & row)
{
    std::string widgetname;

    if (row.NextBank() >= 0) // if next bank exists, set the active bank to it.
    {
        row.IncrementBank(); 
        // scriptLog("Active bank incremented to " + std::to_string(row.ActiveBank), 1);
        return true;
    }
    else
    {
        return false;
    }
}

bool LibMain::RowPreviousBank(SurfaceRow & row)
{
    std::string widgetname;

    if (row.PreviousBank() >= 0) // if next bank exists, set the active bank to it.
    {
        row.DecrementBank();
        // scriptLog("Active bank decremented to " + std::to_string(row.ActiveBank), 1);
        return true;
    }
    else
    {
        return false;
    }
}

void LibMain::ProcessButton(uint8_t button, uint8_t value)  // processes a midi button press
{
    if (value == 127 && button <= 0x3f) // only process button down
    {
        // this is set up so that low 2 bits are switch position (0-3), next two are row (0-3)
        // and fifth bit indicates it's a secondary action, such as double tap or long press release.
        // secondary actions are up/down/row select
        // a row can display buttons, songs/racks, or variations/songparts, which the "row select" botton will cycle between
        uint8_t thisposition = button & 0x03;
        uint8_t thisrow = (button >> 2) & 03;
        uint8_t thisaction = (button >> 4) & 03;
        scriptLog("Process button- position:" + std::to_string(thisposition) + "  row:" + std::to_string(thisrow) + "  action:" + std::to_string(thisaction), 1);

        if (thisaction == MCX_AUX_ACTION)
        {
            // the Aux buttons will duplicate the ROW_ACTION buttons, but we need to adjust for which Page the MC8 bank is on
            if (button == MCX_PAGE1 && Surface.Page == 1)
            {
                Surface.Page = 0;
                SendTextToMCx("", 0x00, 0x02, 0x00); // page toggle
            }
            else if (button == MCX_PAGE2 && Surface.Page == 0)
            {
                Surface.Page = 1;
                SendTextToMCx("", 0x00, 0x02, 0x00); // page toggle
            }
            else if (button < MCX_PAGE1)
            {
                thisrow = thisrow + Surface.Page * 2;
                thisaction = MCX_ROW_ACTION;
                // scriptLog("Process button- position:" + std::to_string(thisposition) + "  row:" + std::to_string(thisrow) + "  action:" + std::to_string(thisaction) + "  page:" + std::to_string(Surface.Page), 1);

            }
        }
        else if (thisaction == MCX_BUTTON_ACTION)
        {
            ToggleButton(button);
        }

        if (thisaction == MCX_ROW_ACTION)
        {
            switch (thisposition) // thisposition items are coded for ACTION_DOWN, ACTION_UP, ACTION_SELECT
            {
            case MCX_ACTION_DOWN:
                switch (Surface.Row[thisrow].Showing)
                {
                case SHOW_BUTTONS:
                    RowPreviousBank(Surface.Row[thisrow]);
                    DisplayRow(Surface.Row[thisrow]);
                    break;
                case SHOW_VARS_PARTS:
                case SHOW_RACKS_SONGS:
                    Surface.Row[thisrow].FirstShown -= 4;
                    DisplayVariations(Surface.Row[thisrow], 0, 4, false);
                }
                break;
            case MCX_ACTION_UP:
                switch (Surface.Row[thisrow].Showing)
                {
                case SHOW_BUTTONS:
                    RowNextBank(Surface.Row[thisrow]);
                    DisplayRow(Surface.Row[thisrow]);
                    break;
                case SHOW_VARS_PARTS:
                case SHOW_RACKS_SONGS:
                    Surface.Row[thisrow].FirstShown += 4;
                    DisplayVariations(Surface.Row[thisrow], 0, 4, false);
                    break;
                }
                break;
            case MCX_ACTION_SELECT:
                switch (Surface.Row[thisrow].Showing)
                {
                case SHOW_BUTTONS:
                    Surface.Row[thisrow].Showing = SHOW_VARS_PARTS;
                    DisplayVariations(Surface.Row[thisrow], 0, 4, true);
                    break;
                case SHOW_VARS_PARTS:
                    Surface.Row[thisrow].Showing = SHOW_RACKS_SONGS;
                    DisplayVariations(Surface.Row[thisrow], 0, 4, true);
                    break;
                default:
                    Surface.Row[thisrow].Showing = SHOW_BUTTONS;
                    DisplayRow(Surface.Row[thisrow]);
                    break;
                }
                break;
            } // switch
        } // if it's a row action */
    } // button down (127)
}


// we just toggle the widgets here and let the OnWidgetValueChanged() callback push the change to the control surface
void LibMain::ToggleButton(uint8_t button)
{
    int x, row;
    std::string widgetname, songname = "";
    double newValue = 0;

    // first get x= button position within control row
    if (button >= MCX_BUTTON_B1 && button < MCX_BUTTON_T1)
    {
        row = BOTTOM_ROW;
        x = button - MCX_BUTTON_B1;
    }
    else if (button >= MCX_BUTTON_T1 && button < MCX_BUTTON_B2)
    {
        row = TOP_ROW;
        x = button - MCX_BUTTON_T1;
    }
    else if (button >= MCX_BUTTON_B2 && button < MCX_BUTTON_T2)
    {
        row = B2_ROW;
        x = button - MCX_BUTTON_B2;
    }
    else if (button >= MCX_BUTTON_T2 && button < MCX_BUTTON_E3)
    {
        row = T2_ROW;
        x = button - MCX_BUTTON_T2;
    }
    else if (button >= MCX_BUTTON_E3 && button < MCX_BUTTON_E4)
    {
        row = E3_ROW;
        x = button - MCX_BUTTON_E3;
    }
    else
    {
        row = E4_ROW;
        x = button - MCX_BUTTON_E4;
    }

    // scriptLog("Toggle sees: button " + std::to_string(button) + "  row " + std::to_string(row), 1);

    if (Surface.Row[row].Showing == SHOW_BUTTONS)
    {
        // scriptLog("Checking for valid bank...", 1);

        if (Surface.Row[row].BankValid())
        {
            widgetname = Surface.Row[row].WidgetPrefix + (std::string) "_" +
                         Surface.Row[row].BankIDs[Surface.Row[row].ActiveBank] + "_" + std::to_string(x);
            newValue = getWidgetValue(widgetname);

            // scriptLog("Toggling: " + widgetname + ", was " + std::to_string(newValue), 1);
            if (newValue != 0.0)
            {
                setWidgetValue(widgetname, (double)0.0);
            }
            else
            {
                setWidgetValue(widgetname, (double)1.0);
            }
        }
    }
    
    else
    {
        x += Surface.Row[row].FirstShown;

        if (Surface.Row[row].Showing == SHOW_RACKS_SONGS)
        {
            if (inSetlistMode() == true)
            {
                if (x < getSongCount())
                {
                    switchToSong(x, 0);
                    songname = getSongName(x);
                }
            }
            else
            {
                if (x < getRackspaceCount())
                {
                    songname = getRackspaceName(x);
                    switchToRackspaceName(songname, "");
                }
            }
        }

        else if (Surface.Row[row].Showing == SHOW_VARS_PARTS)
        {
            if (inSetlistMode() == true)
            {
                if (x < getSongpartCount(getCurrentSongIndex()))
                {
                    songname = getSongpartName(getCurrentSongIndex(), x);
                    switchToSong(getCurrentSongIndex(), x);
                }
            }
            else
            {
                if (x < getVariationCount(getCurrentRackspaceIndex()))
                {
                    songname = getVariationName(getCurrentRackspaceIndex(),x);
                    switchToRackspace(getCurrentRackspaceIndex(), x);
                }
            }
        }
        // if (songname.compare("") != 0)
        //    PresetLongName(songname, x - Surface.Row[row].FirstShown);
    }
}

// we just toggle the widgets here and let the OnWidgetValueChanged() callback push the change to the control surface
void LibMain::ProcessPad(uint8_t button, uint8_t value)
{
    int x, row, invert = 0;
    std::string widgetname, caption, Extras;
    double currentValue = 0;


    if (button >= MKIII_PAD_BASE && button <= MKIII_PAD16 && value > 0 ) 
    {
        x = button - MKIII_PAD_BASE;
        if (x > 7) { x -= 8; } // pad numbers are not contiguous.  There are 8 unused between the top and bottom row.
        row = PAD_ROW;

        if (Surface.Row[row].BankValid())
        {
            widgetname = Surface.Row[row].WidgetPrefix + (std::string)"_" + Surface.Row[row].BankIDs[Surface.Row[row].ActiveBank] + "_" + std::to_string(x);
            currentValue = getWidgetValue(widgetname);
            /*  if (widgetExists(widgetname + "_p"))
            {
                Extras = getWidgetCaption(widgetname + "_p");
                std::vector< std::string>& name_segments = ParseWidgetName(Extras, '_');
            } */

            // scriptLog("Toggling: " + widgetname + ", was " + std::to_string(newValue), 1);
            if (currentValue != 0.0) {
                setWidgetValue(widgetname, (double)0.0);
            }
            else
            {
                setWidgetValue(widgetname, (double)1.0);
            }
        }
    }
}

void LibMain::ProcessKnob(uint8_t column, uint8_t value)  // processes a midi message for a knob turn (0-7) or the on/off of a knob 8
{
    std::string widgetname, caption;
    int resolution = 1000;
    double newValue = 0;

    if (Surface.Row[KNOB_ROW].BankValid())
    {
        widgetname = KNOB_PREFIX + (std::string)"_" + Surface.Row[KNOB_ROW].BankIDs[Surface.Row[KNOB_ROW].ActiveBank] + "_" + std::to_string(column);
        if (widgetExists(widgetname) == true)  // if the widget doesn't exist we ignore the input
        {
            if (column < 8)
            {
                if (widgetExists(widgetname + "_r"))  // if there's a sl_k_1_5_r widget process first caption field as resolution (integer)
                {
                    caption = getWidgetCaption(widgetname + "_r");
                    std::vector< std::string> name_segments = ParseWidgetName(caption, '_');
                    (name_segments.size() >= 1) ? resolution = (int)std::stoi("0" + name_segments[0]) : resolution = 1000;  // default to 1000
                }
                newValue = getWidgetValue(widgetname);
                if (value < 4) {  // small numbers are turns in the clockwise direction
                    newValue = newValue + static_cast<double>(value) / static_cast<double>(resolution);
                    if (newValue > 1) { newValue = 1; }
                }
                else if (value > 125) {  // near 127 is a counter-clockwise move
                    newValue = newValue - static_cast<double>(128 - value) / static_cast<double>(resolution);
                    if (newValue < 0) { newValue = 0; }
                }
                setWidgetValue(widgetname, newValue);  // Move the widget, and the OnWidgetChange callback will move the SLMK3 display
            }
            else
            {
                // if it's above column 7 then something weird is going on and we ignore it
            }
        }
    }
}

void LibMain::ProcessFader(uint8_t column, uint8_t value)  // processes a midi message for a fader
{
    std::string widgetname, caption;
    double newValue = 0;

    if (Surface.Row[FADER_ROW].BankValid())
    {
        widgetname = FADER_PREFIX + (std::string)"_" + Surface.Row[FADER_ROW].BankIDs[Surface.Row[FADER_ROW].ActiveBank] + "_" + std::to_string(column);
        if (widgetExists(widgetname) == true)  // if the widget doesn't exist we ignore the input
        {
            if (column < 8)
            {
                newValue = getWidgetValue(widgetname);

                setWidgetValue(widgetname, static_cast<double>(value) / static_cast<double>(127));
            }
            else
            {
                // if it's above column 7 then something weird is going on and we ignore it
            }
        }
    }
}

bool LibMain::IsKnob(const uint8_t* data, int length)  // Is midi event from a knob turn?
{
    return(data[0] == MIDI_CC_16 && data[1] >= MKIII_KNOB_1 && data[1] <= MKIII_KNOB_8);
}

bool LibMain::IsFader(const uint8_t* data, int length)  // Is midi event from a fader?
{
    return(data[0] == MIDI_CC_16 && data[1] >= MKIII_FADER_1 && data[1] <= MKIII_FADER_8);
}

bool LibMain::IsButton(const uint8_t* data, int length)   // Is midi event from a button?
{
    return(data[0] == MIDI_CC_16 && data[1] >= MCX_BUTTON_BASE && data[1] <= MCX_BUTTON_MAX);
}

bool LibMain::IsPad(const uint8_t* data, int length)   // Is midi event a channel 16 Note on/off from a pad?
{
    return ( ( data[0] == 0x9F || data[0] == 0x8F ) && 
        ( data[1] >= 0x60 && data[1] <= 0x77 ) &&
        ( data[1] <= 0x67 || data[1] >= 0x70 ) ); // must be note on/off, channel 16, in the right pad range
}
