#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"

void LibMain::ProcessButton(uint8_t button, uint8_t value)  // processes a midi button press
{
    if (value == 127 && button <= 0x3f) // only process button down
    {
        // this is set up so that low 2 bits are switch position (0-3), next two are row (0-3)
        // and fifth bit indicates it's a secondary action, such as double tap or long press release.
        // The way we use Aux buttons complicates the logic here a little.  Because there are only generally 6 aux buttons
        // there is no page one/two distinction.  To avoid uncertainty about what actual button was pressed we map those onto
        // the [32-39], which has 'thisaction' = 2 and 'thisrow' = 0.  We then consult what page we think we're on to decide
        // which actual row (page really) to apply them to.
        // each row can display buttons, songs/racks, or variations/songparts, which the "row select" botton will cycle between

        uint8_t thisposition = button & 0x03;
        uint8_t thisrow = (button >> 2) & 0x03;
        uint8_t thisaction = (button >> 4) & 0x03;
        bool doublesync = false;  // we set this if we want to select and sync both top and bottom rows together
        uint8_t otherrow = 0;

        if (thisrow % 2 == 0) otherrow = thisrow + 1;
        else otherrow = thisrow - 1;

        // scriptLog("Process button- position:" + std::to_string(thisposition) + "  row:" + std::to_string(thisrow) + "  action:" + std::to_string(thisaction), 1);
 
        // Long presses (or double taps) on the "Cycle row select" will set "doublesync" to shift both rows
        if (button == 49 || button == 53)
        {
            thisaction = MCX_ROW_ACTION;
            doublesync = true;
            thisrow = thisrow + Surface.Page * 2;
            otherrow = otherrow + Surface.Page * 2;
            // scriptLog("Double sync using row " + std::to_string(thisrow), 1);
        }

        if (thisaction == MCX_AUX_ACTION)
        {
            // the Aux buttons will duplicate the ROW_ACTION buttons, but we need to adjust for which Page the MC8 bank is on
            if (button == MCX_PAGE1)
            {
                TogglePage(0);
            }
            else if (button == MCX_PAGE2)
            {
                TogglePage(1);
            }
            else if (button < MCX_PAGE1) // this is where we catch buttons 32-29 on the aux switches
            {
                thisrow = thisrow + Surface.Page * 2; // only one set of Aux buttons, so we use Page to map them to rows
                otherrow = otherrow + Surface.Page * 2;
                thisaction = MCX_ROW_ACTION;
            }
        }
        else if (thisaction == MCX_BUTTON_ACTION) // this will capture all our primary button actions (cc 0-15)
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
                    Surface.Row[thisrow].DecrementBank(); // set row to prior bank
                    if (Surface.SyncBankIDs(thisrow)) DisplayRefresh();  // sync other rows to it, display all if others match
                    else DisplayRow(Surface.Row[thisrow]); // only redisplay thisrow if no others match
                    break;
                case SHOW_VARS_PARTS:
                case SHOW_RACKS_SONGS:
                    if (Surface.Row[thisrow].Showing == Surface.Row[otherrow].Showing)
                    {
                        Surface.Row[std::max(thisrow, otherrow)].FirstShown -= 2 * Surface.RowLen;
                        DisplayVariations(Surface.Row[std::max(thisrow, otherrow)], 0, Surface.RowLen, false);
                        DisplayVariations(Surface.Row[std::min(thisrow, otherrow)], 0, Surface.RowLen, false);
                    }
                    else
                    {
                        Surface.Row[thisrow].FirstShown -= Surface.RowLen;
                        DisplayVariations(Surface.Row[thisrow], 0, Surface.RowLen, false);
                    }
                    break;
                }
                break;
            case MCX_ACTION_UP:
                switch (Surface.Row[thisrow].Showing)
                {
                case SHOW_BUTTONS:
                    Surface.Row[thisrow].IncrementBank();
                    if (Surface.SyncBankIDs(thisrow)) DisplayRefresh();
                    else DisplayRow(Surface.Row[thisrow]);
                    break;
                case SHOW_VARS_PARTS:
                case SHOW_RACKS_SONGS:
                    if (Surface.Row[thisrow].Showing == Surface.Row[otherrow].Showing)
                    {
                        Surface.Row[std::max(thisrow, otherrow)].FirstShown += 2* Surface.RowLen;
                        DisplayVariations(Surface.Row[std::max(thisrow, otherrow)], 0, Surface.RowLen, false);
                        DisplayVariations(Surface.Row[std::min(thisrow, otherrow)], 0, Surface.RowLen, false);
                    }
                    else
                    {
                        Surface.Row[thisrow].FirstShown += Surface.RowLen;
                        DisplayVariations(Surface.Row[thisrow], 0, Surface.RowLen, false);
                    }
                    break;
                }
                break;
            case MCX_ACTION_SELECT:
                switch (Surface.Row[thisrow].Showing)
                {
                case SHOW_BUTTONS:
                    Surface.Row[thisrow].Showing = SHOW_RACKS_SONGS;
                    if (doublesync || Surface.Row[otherrow].Showing == SHOW_RACKS_SONGS)
                    {
                        // scriptLog("Going buttons to racks, otherrow = " + std::to_string(otherrow), 1);
                        Surface.Row[otherrow].Showing = SHOW_RACKS_SONGS;
                        DisplayVariations(Surface.Row[std::max(thisrow, otherrow)], 0, Surface.RowLen, true);
                        DisplayVariations(Surface.Row[std::min(thisrow, otherrow)], 0, Surface.RowLen, false);
                    }
                    else DisplayVariations(Surface.Row[thisrow], 0, Surface.RowLen, true);
                    break;
                case SHOW_RACKS_SONGS:
                    Surface.Row[thisrow].Showing = SHOW_VARS_PARTS;
                    if (doublesync || Surface.Row[otherrow].Showing == SHOW_VARS_PARTS)
                    {
                        // scriptLog("Going to Racks to variations, otherrow = " + std::to_string(otherrow), 1);
                        Surface.Row[otherrow].Showing = SHOW_VARS_PARTS;
                        DisplayVariations(Surface.Row[std::max(thisrow, otherrow)], 0, Surface.RowLen, true);
                        DisplayVariations(Surface.Row[std::min(thisrow, otherrow)], 0, Surface.RowLen, false);
                    }
                    else DisplayVariations(Surface.Row[thisrow], 0, Surface.RowLen, true);
                    break;
                case SHOW_VARS_PARTS:
                    if (Surface.Row[thisrow].BankValid())
                    {
                        if (doublesync)
                        {
                            if (Surface.Row[otherrow].BankValid())
                            {
                                Surface.Row[otherrow].Showing = SHOW_BUTTONS;
                                Surface.SyncBankIDs(thisrow);
                                // scriptLog("Going to variations to buttons, otherrow = " + std::to_string(otherrow), 1);
                            }
                            DisplayRow(Surface.Row[otherrow]);
                        }
                        Surface.Row[thisrow].Showing = SHOW_BUTTONS;
                        DisplayRow(Surface.Row[thisrow]);
                    }
                    else
                    {
                        Surface.Row[thisrow].Showing = SHOW_RACKS_SONGS;
                        if (doublesync || Surface.Row[otherrow].Showing == SHOW_RACKS_SONGS)
                        {
                            // scriptLog("Going Variation to Racks, otherrow = " + std::to_string(otherrow), 1);
                            Surface.Row[otherrow].Showing = SHOW_RACKS_SONGS;
                            DisplayVariations(Surface.Row[std::max(thisrow, otherrow)], 0, Surface.RowLen, true);
                            DisplayVariations(Surface.Row[std::min(thisrow, otherrow)], 0, Surface.RowLen, false);
                        }
                        else DisplayVariations(Surface.Row[thisrow], 0, Surface.RowLen, true);
                    }
                    break;
                }
                break;
            case MCX_ACTION_PAGE:
                if (thisrow == 0)
                {
                    TogglePage(1);
                }
                else if (thisrow == 2)
                {
                    TogglePage(0);
                }
                break;
            } // switch
        } // if it's a row action */
    } // button down (cc 0-63 = 127)
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
    else if (button >= MCX_BUTTON_T2 && button < MCX_BUTTON_T2 + 4)
    {
        row = T2_ROW;
        x = button - MCX_BUTTON_T2;
    }
    else
    {
        // row = E4_ROW;
        // x = button - MCX_KNOB_1;
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
        // DisplayRefresh();
        // if (songname.compare("") != 0)
        //    PresetLongName(songname, x - Surface.Row[row].FirstShown);
    }
}


// Processing a "Knob" (expression pedal) just updates the appropriate GP widget
void LibMain::ProcessKnob(uint8_t row, uint8_t value)  // processes a midi message for an expression pedal
{
    std::string widgetname;

    // scriptLog("PK:  R=" + std::to_string(row) + "  V=" + std::to_string(value),1);
    // with MCx expression pedal widgets the column is always zero.  we treat these as four rows of 1 pedal each
    if (row < 4)
    {
        SurfaceRow thisrow = Surface.Row[E1_ROW + row];

        if (thisrow.BankValid())
        {
            widgetname = thisrow.WidgetPrefix + "_" + thisrow.BankIDs[thisrow.ActiveBank] + "_0";
            // scriptLog("widget = " + widgetname, 1);
            if (widgetExists(widgetname) == true)  // if the widget doesn't exist we ignore the input
            {
                setWidgetValue(widgetname, (double) value/127);
            }
        }
    }
}


bool LibMain::IsKnob(const uint8_t* data, int length)  // Is midi event from an expression pedal?
{
    return(data[0] == MIDI_CC_00 && data[1] >= MCX_KNOB_1 && data[1] <= MCX_KNOB_4);
}


bool LibMain::IsButton(const uint8_t* data, int length)   // Is midi event from a button?
{
    return(data[0] == MIDI_CC_00 && data[1] >= MCX_BUTTON_BASE && data[1] <= MCX_BUTTON_MAX);
}

