#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"


//  The only thing we do here is set the Preset Long Name to the appropriate value
void LibMain::DisplayKnobs(SurfaceRow row)
{
    std::string widgetname;

    if (row.BankValid())
    {
        widgetname = row.WidgetPrefix + (std::string)"p_" + row.ActiveBankID() + (std::string)"_0";
        if (widgetExists(widgetname)) {
            // DisplayPresetLongname(row, 0, getWidgetCaption(widgetname)); // currently crashes MC8
        }
    }
}


// returns the color of the indicated bankindex of Surface.Row by checking for presense of a "p" widget, expecting color as first field of caption
// generally used for coloring the up/down bank arrows for knobs, button, and pad rows
// deprecated since we GetWidgetFillColor() and GetWidgetBorderColor() were added.  We use text widgets for bank colors now, on _i widgets
uint8_t LibMain::GetBankColor(SurfaceRow row, int bankindex)
{
    uint8_t color = SLMKIII_BLACK;
    std::string widgetname;

    if (row.BankValid() && bankindex >= 0 && bankindex != row.ActiveBank)
    {
        widgetname = row.WidgetPrefix + (std::string)"_" + row.BankIDs[bankindex] + "_p";
        if (widgetExists(widgetname)) {
            std::vector< std::string> name_segments = ParseWidgetName(getWidgetCaption(widgetname), '_');
            if (name_segments.size() >= 1) {
                color = (uint8_t)std::stoi("0" + name_segments[0]);
            }
        }
        else { color = SLMKIII_ORANGE_HALF; }  // bank exists, but doesn't have a defined color
    }
    return color;
}

// returns the color of the indicated bankindex of Surface.Row by checking for presense of an "_i" widget, with GetWidgetFillColor() for the indicator color
// generally used for coloring the up/down bank arrows for knobs, button, and pad rows
int LibMain::GetBankRGBColor(SurfaceRow row, int bankindex)
{
    int color = 0x101010;
    std::string widgetname;

    if (row.BankValid() && bankindex >= 0 && bankindex != row.ActiveBank)
    {
        widgetname = row.WidgetPrefix + (std::string)"_" + row.BankIDs[bankindex] + "_i";
        if (widgetExists(widgetname)) {
            color = getWidgetFillColor(widgetname);
        }
    }
    return color;
}

// Clear the knob bank indicators except bank indicated
void LibMain::ResetBankIndicators(SurfaceRow row)
{
    uint8_t x;
    std::string widgetname, widgetindicator;

    if (row.BankValid())
    {
        for (x = 0; x < row.BankIDs.size(); x++)  // cycle through banks to turn on/off the widget indicators for active bank and set up/down arrows for colors
        {
            widgetindicator = row.WidgetPrefix + (std::string)"_" + row.BankIDs[x] + "_i";
            if (widgetExists(widgetindicator)) { setWidgetValue(widgetindicator, (x == row.ActiveBank) ? (double) 1.0 : (double) 0.3); }
        }
    }
}
