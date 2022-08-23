#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"


//  Call this when the rackspace changes or the active button bank changes
    //   The SL MK3 takes CC messages to light the buttons, Note On messages for the pads.
    //   This could also be done with sysex for full RGB control.
    //   Buttons can also be put into flash and pulse modes, which we do individually rather than as a bank like this.
 
void LibMain::DisplayButtons(SurfaceRow row, uint8_t firstbutton, uint8_t number)
{
    std::string widgetname;
    std::string Caption = "", Label = "", Extras;
    SurfaceWidget widget;
    double Value = 0;
    std::string hexmessage, subtext, binmessage;
    int x;
    int LitColor = 0, DimColor = 0;
 
    // color the bank up/down arrows
    // SetButtonRGBColor(row.WidgetID == BUTTON_TAG ? MKIII_BUTTONS_UP_SYSEX : MKIII_SCENE_UP_SYSEX, GetBankRGBColor(row, row.PreviousBank() ) );  
    // SetButtonRGBColor(row.WidgetID == BUTTON_TAG ? MKIII_BUTTONS_DOWN_SYSEX : MKIII_SCENE_DOWN_SYSEX, GetBankRGBColor(row, row.NextBank() ) );

    if (row.BankValid())
    {       
        for (x = firstbutton ; x < firstbutton + number; x++)
        {
            widgetname = row.WidgetPrefix + "_" + row.BankIDs[row.ActiveBank] + "_" + std::to_string(x);
            widget = PopulateWidget(widgetname);
            if (widget.IsSurfaceItemWidget)
            {
                DisplayWidgetValue(row, widget);       
            }
            else  // we end up here if the widget doesn't exist.
            {
                widget.ShortNameOff = "";
                widget.ShortNameOn = "";
                widget.LongName = "";
                widget.Column = x;
                DisplayWidgetValue(row, widget);
            }
        }
    }
    else
    {
        // turn them all off if there is no valid bank
        widget.ShortNameOff = "";
        widget.ShortNameOn = "";
        widget.LongName = "";
        widget.Value = 0;
        for (x = 0; x < number; x++)
        {
            widget.Column = x;
            DisplayWidgetValue(row, widget);
        }
    }

}

std::string LibMain::makeVariationText(SurfaceRow row, int index)
{
    std::string fullname = "";

    if (inSetlistMode() == false)
    {
        if (row.Showing == SHOW_RACKS_SONGS)
            fullname = getRackspaceName(index);
        else
            fullname = getVariationName(getCurrentRackspaceIndex(), index);
    }
    else
    {
        if (row.Showing == SHOW_RACKS_SONGS)
            fullname = getSongName(index);
        else
            fullname = getSongpartName(getCurrentSongIndex(), index);
    }
 
    scriptLog("fullname = " + fullname, 1);
    return fullname.substr(0,9);
}

// this displays Racks/Variations/Songs/Songparts
void LibMain::DisplayVariations(SurfaceRow row, uint8_t firstbutton, uint8_t number, bool forcetocurrent)
{
    std::string variationname = "";
    int x, positionindex, current = 0, count = 1;

    if (row.Showing == SHOW_RACKS_SONGS)
    {
        if (inSetlistMode())
        {
            current = getCurrentSongIndex();
            count = getSongCount();
        }
        else
        {
            current = getCurrentRackspaceIndex();
            count = getRackspaceCount();
        }
    }
    else if (row.Showing == SHOW_VARS_PARTS)
    {
        if (inSetlistMode())
        {
            current = getCurrentSongpartIndex();
            count = getSongpartCount(getCurrentSongIndex());
        }
        else
        {
            current = getCurrentVariationIndex();
            count = getVariationCount(getCurrentRackspaceIndex());
        }
    }

    // scriptLog("DisplayVariations: count=" + std::to_string(count) + " curret=" + std::to_string(current), 1);

    if (forcetocurrent == true)
    {
        x = current % 4; // we're going to show in banks of 4, this is the offset
        row.FirstShown = current - x;
    }
    else
    {
        if (row.FirstShown >= count)
        {
            row.FirstShown = count - count % 4; // firstshown is zero based, count can be 0 only if there are no songs
        }
        if (row.FirstShown < 0)
        {
            row.FirstShown = 0;
        }
    }

    positionindex = row.FirstShown;

    for (x = firstbutton; x < firstbutton + number; x++)
    {

        if (positionindex >= count) // clear the text if there's no song this high
        {
            PresetShortName("", row.FirstID + x);
            // SetButtonColor(MKIII_DISPLAY_BUTTON_1 + x, SLMKIII_BLACK); // clear the button if no song/var is here
        }
        else
        {
            variationname = makeVariationText(row, positionindex);
            if (positionindex == current)
            {
                variationname = (std::string) "(" + variationname.substr(0, 8) + ")";
            }
            PresetShortName(variationname, row.FirstID + x);
        }
        
        // scriptLog("DVloop: posindex=" + std::to_string(positionindex) + " name=" + variationname + " count=" + std::to_string(count) +
        //              " curret=" + std::to_string(current), 1);
        positionindex++;
    }
}

// Returns the color to make a widget based on the widget's GetWidgetValue() and any parameters set for the widget on related "_p" widgets
/*  int LibMain::getWidgetRGBColor(std::string widgetname, double value)
{
    int LitColor = 0xA0A0A0, DimColor = 0x202020;
    std::string Caption, Extras, Label;

    // check for an extra properties widget on widgetname_P (e.g., sl_b_1_1_p) 
    if (widgetExists(widgetname + "_p"))
    {
        LitColor = getWidgetFillColor(widgetname + "_p");
        DimColor = getWidgetOutlineColor(widgetname + "_p");
    }

    if (value != (double)0.0) return LitColor;
    else return DimColor;
} */

// Returns the color to make a widget based on the widget's GetWidgetValue() and any parameters set for the widget on related "_p" widgets
/* int LibMain::getWidgetRGBColor(SurfaceWidget widget, double value)
{
    if (value != (double)0.0) return widget.RgbLitColor;
    else return widget.RgbDimColor;
} */

// This extension expects widget names generally in the format "DevicePrefix_WidgetType_Bank_Column" where the "_" character is used as a delimiter.
// An example widget would be "sl_k_1_0" referring to SLMK3 knob bank 1 column 0.
// Extra parameters for widgets are looked for on a "widgetname_p" widget in the Caption.  Typically widget colors or knob resolution
SurfaceWidget LibMain::PopulateWidget(std::string widgetname, double passed_value)
{
    SurfaceWidget widget = PopulateWidget(widgetname);
    widget.Value = passed_value;
    return widget;
}

SurfaceWidget LibMain::PopulateWidget(std::string widgetname)
{
    SurfaceWidget widget;
    std::string control_number, extras, pwidgetname;

    if (widgetExists(widgetname))
    {
        widget.Value = getWidgetValue(widgetname);

        std::vector<std::string> name_segments = ParseWidgetName(widgetname, '_');

        if (name_segments.size() >= 4)
        {
            widget.SurfacePrefix = name_segments[0];
            widget.WidgetID = name_segments[1];
            widget.BankID = name_segments[2];
            control_number = name_segments[3];

            if (widget.SurfacePrefix == THIS_PREFIX)
            {

                widget.RowNumber = Surface.IdentifySurfaceRow(widget.WidgetID);

                // is it a valid row identifier for this Surface?
                if (widget.RowNumber >= 0)
                {

                    // check if it's a row parameters widget, and flag the widget as a RowParameterWidget if it is
                    // we don't actually use this part anymore... deprecated
                    if (control_number == "p")
                    {
                        widget.IsRowParameterWidget = true;
                        pwidgetname = widgetname;
                        if (widget.RowNumber >= 0 && widget.RowNumber < Surface.NumRows)
                            widget.Validated = true;
                    }

                    // if not, check for a valid Column
                    else
                        try
                        {
                            widget.IsRowParameterWidget = false;
                            widget.Column = std::stoi(control_number);
                            if (widget.RowNumber >= 0 && widget.RowNumber < Surface.NumRows)
                            {
                                if (widget.Column < Surface.Row[widget.RowNumber].Columns)
                                {
                                    widget.IsSurfaceItemWidget = true;
                                    widget.TextValue = getWidgetTextValue(widgetname);
                                    widget.Caption = getWidgetCaption(widgetname);

                                    // we've checked for valid Surface prefix, row type, and valid column number for the
                                    // row, so it's a valid widget we don't check that the BankID is valid because by
                                    // definition a bank is valid if there's a valid widget for it
                                    widget.Validated = true;
                                    // pwidgetname = widgetname + "_p";
                                }
                            }
                        }
                        catch (...) // catch blow ups, such as stoi() if the widget name doesn't have a number where we
                                    // need a number
                        {
                            widget.Column = -1;
                            widget.IsSurfaceItemWidget = false;
                            widget.Validated = false;
                        }

                    // look for extra parameters on a parameter widget if it's a valid surface item widget
                    if (widget.Validated)
                    {

                        // if there is a _[x]p_ widget that takes first priority, e.g. mcx_bp_bank_0
                        // we use this for preset ShortNameOn, ShortNameOff, and LongName
                        pwidgetname =
                            widget.SurfacePrefix + "_" + widget.WidgetID + "p_" + widget.BankID + "_" + control_number;
                        if (widgetExists(pwidgetname))
                        {
                            name_segments = ParseWidgetName(getWidgetCaption(pwidgetname), '_');

                            if (name_segments.size() >= 1)
                                widget.ShortNameOff = name_segments[0];
                            if (name_segments.size() >= 2)
                                widget.ShortNameOn = name_segments[1];
                            else
                                widget.ShortNameOn = (std::string) "*" + name_segments[0];
                            if (name_segments.size() >= 3)
                                widget.LongName = name_segments[2];
                            else
                                widget.LongName = name_segments[0];

                            widget.Caption = getWidgetCaption(pwidgetname); // need to stop using this for mcx
                            widget.RgbLitColor = getWidgetFillColor(pwidgetname); // these will be nice when the Pro series arrives
                            widget.RgbDimColor = getWidgetOutlineColor(pwidgetname);
                        }
                        else
                        {
                            // in the absense of an widget specific property widget we'll set short and long names by the caption
                            widget.ShortNameOff = widget.Caption;
                            widget.ShortNameOn = (std::string) "*" + widget.Caption;
                            widget.LongName = widget.Caption;

                            // this searches for a bank _p widget that we use for color.  no purpose now, but for the MCx Pro?
                            // I'm just keeping this code around now for the Pro so we can pick up a generic color set for a bank
                            pwidgetname = widget.SurfacePrefix + "_" + widget.WidgetID + "_" + widget.BankID + "_p";
                            if (widgetExists(pwidgetname))
                            {
                                // widget.Caption = getWidgetCaption(pwidgetname);
                                widget.RgbLitColor = getWidgetFillColor(pwidgetname);
                                widget.RgbDimColor = getWidgetOutlineColor(pwidgetname);
                            }
                            else
                            {
                                // if no individual or bank property widget exists we'll try the bank indicator widget
                                pwidgetname = widget.SurfacePrefix + "_" + widget.WidgetID + "_" + widget.BankID + "_i";
                                if (widgetExists(pwidgetname))
                                {
                                    // widget.Caption = getWidgetCaption(pwidgetname);
                                    widget.RgbLitColor = getWidgetFillColor(pwidgetname);
                                    widget.RgbDimColor = getWidgetOutlineColor(pwidgetname);
                                }
                                else
                                {
                                    // if none of them exist we'll use an orange-ish color
                                    widget.RgbLitColor = 0xff0000;
                                    widget.RgbDimColor = 0x500000;
                                }
                            }
                            // widget.BarColor = widget.LitColor;
                            // widget.KnobColor = widget.LitColor;
                        }
                    }
                }
            }
        }
    }
    else
    {
        widget.Validated = false;
    }
    return widget;
}