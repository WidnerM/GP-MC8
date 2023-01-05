#pragma once

// #include <interfaces/CPP/GigPerformerAPI.h>
// #include <interfaces/CPP/GPUtils.h>
// #include <interfaces/CPP/GPMidiMessages.h>

#include "gigperformer/sdk/GPMidiMessages.h"
#include "gigperformer/sdk/GPUtils.h"
#include "gigperformer/sdk/GigPerformerAPI.h"
#include "gigperformer/sdk/types.h"

#include <vector>
#include <cstdio>
#include <iostream>
#include <string>

#include "MC8_Classes.h"
#include "General_Utils.h"


// define an XML string describing your product
const std::string XMLProductDescription =   
     // Replace with your information            
    "<Library>" 
    "<Product Name=\"MC8 Extension\" Version=\"1.0\" BuildDate=\"12/11/2022\"></Product> "
    "<Description>Control Integration for Morningstar MC8</Description>"
    "</Library>"; 


// Define your class here - it MUST be called LibMain and it must inherit from GigPerformerAPI

class LibMain : public gigperformer::sdk::GigPerformerAPI
{
protected:
    int GetPanelCount() override;
    std::string GetPanelName(int index) override;
    std::string GetPanelXML(int index) override;

    // These are for creating menu items in Gig Performer that can be used to trigger external functions provided by the extension developer
    int GetMenuCount() override;
    std::string GetMenuName(int index) override;
    void InvokeMenu(int itemIndex) override;

public:
    // These must be here but no need to do anything unless you want extra behavior
    LibMain(LibraryHandle handle) : GigPerformerAPI(handle)  {}
    virtual ~LibMain() {}

    //  Global declarations and initializations
    //  [Global to the LibMain class, that is]
    SurfaceClass Surface;

    std::vector<std::string> MidiOut = { MCX_MIDI_OUT };
    std::vector<std::string> MidiIn = { MCX_MIDI_IN };

    // Generic support function definitions and includes


    // Core MC8 Routine declarations (as part of LibMain class)

    // from LibMain.cpp.  We make these to simplify sending the same midi message to all destinations if we have multiple destinations
    void sendMidiMessage(std::string MidiMessage);
    void sendMidiMessage(gigperformer::sdk::GPMidiMessage MidiMessage);
    void sendMidiMessage(const uint8_t* MidiMessage, int length);

    // from Display.cpp - functions for displaying things on the MC8 display
    uint8_t calculateMCChecksum(uint16_t len, uint8_t *ptr);
    gigperformer::sdk::GPMidiMessage makeMCMessage(std::string payload, uint8_t type, uint8_t column, uint8_t op4);
    gigperformer::sdk::GPMidiMessage makeMCMessage(std::string payload, uint8_t type, uint8_t column);
    void SendTextToMCx(std::string text, uint8_t op2, uint8_t op3, uint8_t op4);
    void PresetShortName(std::string text, uint8_t position);
    void PresetLongName(std::string text, uint8_t position);
    void CurrentBankName(std::string text);
    void LongPresetNames(std::string text);
    std::string makeVariationText(SurfaceRow row, int index);
    void Notify(std::string text, uint8_t duration);

    void InitializeMC8();
    void DisplayText(uint8_t column, uint8_t row, std::string text);

    std::string GPColorToSLColorHex(int color);
    void SetButtonColor(uint8_t button, uint8_t color);
    void SetButtonRGBColor(uint8_t button, int color);

    void ShowTopLabelColor(uint8_t position, uint8_t color);
    void ShowBottomLabelColor(uint8_t position, uint8_t color);
    void ShowBottomHighlight(uint8_t position, uint8_t color);

    void ShowKnobColor(uint8_t position, uint8_t color);
    void ShowKnobCaption(uint8_t column, const std::string caption);
    void ShowKnobLabel(uint8_t column, const std::string caption);

    void DisplayWidgetValue(const SurfaceRow &row, SurfaceWidget widget);
    void DisplayPresetLongname(const SurfaceRow& row, uint8_t column, std::string value);

    void DisplayRow(SurfaceRow row);
    void DisplayVariations(SurfaceRow & row, uint8_t firstbutton, uint8_t number, bool forcetocurrent);
    void DisplayRacks(SurfaceRow & row, uint8_t firstbutton, uint8_t number, bool forcetocurrent);

    void ClearDisplayRow(SurfaceRow row);


    // from Inputs.cpp
    void ProcessButton(uint8_t button, uint8_t value);
    void ToggleButton(uint8_t button);
    void ProcessKnob(uint8_t row, uint8_t value);

    bool IsKnob(const uint8_t* data, int length);
    bool IsButton(const uint8_t* data, int length);

    bool RowNextBank(SurfaceRow & row);
    bool RowPreviousBank(SurfaceRow & row);


    // from Knobs.cpp
    void DisplayKnobs(SurfaceRow row);  // Shows the active knob bank (as stored in Surface.Row[].ActiveBank)
    void ResetBankIndicators(SurfaceRow row);
    uint8_t GetBankColor(SurfaceRow row, int bankindex);
    int GetBankRGBColor(SurfaceRow row, int bankindex);

    // from Buttons.cpp
    void DisplayButtons(SurfaceRow row, uint8_t firstbutton, uint8_t number);
    // int getWidgetRGBColor(std::string widgetname, double value); // deprecated
    // int getWidgetRGBColor(SurfaceWidget widgetname, double value);

    SurfaceWidget PopulateWidget(std::string widgetname);
    SurfaceWidget PopulateWidget(std::string widgetname, double passed_value);

    // General routines we define before the varoius callbacks

    void scriptlog(std::string text)
    {
        scriptLog(text, 1);
    }


    bool SetMidiInOutDevices() {
        bool foundin = false, foundout = false;
        std::string name;
        std::vector <std::string> validInPorts = {};
        std::vector <std::string> validOutPorts = {};

        for (int i = 0; i < getMidiInDeviceCount(); i++)
        {
            name = getMidiInDeviceName(i);
            for (int j = 0; j < MidiIn.size(); j++) {
                if (name == MidiIn[j]) {
                    listenForMidi(getMidiInDeviceName(i), 1);
                    foundin = true;
                    validInPorts.push_back(name);
                    scriptLog("MCX:  Using midi in " + name, 0);
                }
            }
        }

        for (int i = 0; i < getMidiOutDeviceCount(); i++)
        {
            name = getMidiOutDeviceName(i);
            // scriptLog("Evaluating midi out " + name, 1);
            for (int j = 0; j < MidiOut.size(); j++) {
                if (name == MidiOut[j]) {
                    foundout = true;
                    validOutPorts.push_back(name);
                    scriptLog("MCX:  Using midi out " + name, 0);
                }
            }
        }
        MidiOut = validOutPorts;
        // scriptLog(foundout ? EXTENSION_IDENTIFIER + (std::string)" using midi out " + MidiOut : EXTENSION_IDENTIFIER + (std::string)"COULD NOT FIND midi out " + MidiOut, 1);
        return (foundin && foundout);
    }

    // Define the callbacks here.
    // For organization, most of the real routines are split out into other files.
    void OnStatusChanged(GPStatusType status) override
    {
        std::string name;

        if (status == GPStatus_GigFinishedLoading)
        {
            // scriptLog("Gig loaded.", 1);

            // Look for names for Midi in and out, preferably in global rack
            if (widgetExists(MIDI_IN_WIDGETNAME)) { MidiIn = ParseWidgetName(getWidgetCaption(MIDI_IN_WIDGETNAME), ','); }
            if (widgetExists(MIDI_OUT_WIDGETNAME)) { MidiOut = ParseWidgetName(getWidgetCaption(MIDI_OUT_WIDGETNAME), ','); }

            registerCallback("OnRackspaceActivated");
            registerCallback("OnVariationChanged");
            registerCallback("OnWidgetValueChanged");
            // registerCallback("OnWidgetCaptionChanged");
            registerCallback("OnWidgetStateChanged");
            registerCallback("OnSongChanged");
            registerCallback("OnSongPartChanged");
            registerCallback("OnModeChanged");
            registerCallback("OnGlobalPlayStateChanged");

            registerCallback("OnMidiIn");

            SetMidiInOutDevices();

            // We look for this INITIAL_CONFIG widget one time when the gig has finished loading
            // if this is the only row configuration widget used then changes made during use will persist through rackspace changes
            if (widgetExists(MCX_INITIAL_CONFIG_WIDGETNAME))
            {
                std::vector <std::string> name_segments = ParseWidgetName(getWidgetCaption(MCX_INITIAL_CONFIG_WIDGETNAME), '_');
                for (uint8_t row = 0; row < Surface.NumRows; row++)
                {
                    if (row < name_segments.size())
                    {
                        // scriptLog("sizeof list:" + std::to_string(sizeof(widgetlist)) + "  row:" + std::to_string(row), 1);
                        if (name_segments[row] == "buttons") Surface.Row[row].Showing = SHOW_BUTTONS;
                        else if (name_segments[row] == "variations") Surface.Row[row].Showing = SHOW_VARS_PARTS;
                        else if (name_segments[row] == "racks") Surface.Row[row].Showing = SHOW_RACKS_SONGS;
                    }
                }
            }

            OnRackspaceActivated();  // We call this to set everything up for the current Rackspace after initial Gig is loaded
        }
    }


        
    void OnGlobalPlayStateChanged(bool playing) override
    {
        // Light the Play transport button according to play state.
        // Having the Play button start/stp the global playhead is controlled in Inputs.cpp via widget named sl_t_p
        // SetButtonColor(MKIII_TRANSPORT_PLAY, (playing ? 0x21 : 0x00));
        // Notify("Play state changed.", 20);

    }

    void OnWidgetStateChanged(const std::string & widgetName, int newState) override
    {
        // scriptLog("Widget state change: " + widgetName + " " + std::to_string(newState), 1);
    }


    // When a widget that we are listening for is changed by any means (screen, midi, OSC, extension) this is called.
    void OnWidgetValueChanged(const std::string & widgetname, double newValue) override
    {
        int x;
        SurfaceWidget widget;
        std::string widget_prefix, control_type, control_bank, control_number;

        // scriptLog("On W.Val changed: " + widgetname + std::to_string (newValue),1);
        widget = PopulateWidget(widgetname, newValue);

        if (widget.IsSurfaceItemWidget)  // some widgets we listen for may not display on the control surface
        {
            // if the GP widget is not a button in an active bank on the control surface we don't need to display it
            if ( widget.BankID.compare(Surface.Row[widget.RowNumber].ActiveBankID()) == 0 && Surface.Row[widget.RowNumber].Showing == SHOW_BUTTONS)
            {
                DisplayWidgetValue(Surface.Row[widget.RowNumber], widget);
                // DisplayPresetLongname(Surface.Row[widget.RowNumber], widget.Column, widget.LongName + widget.TextValue);
            }        
        }
    }


    bool OnMidiIn(const std::string& deviceName, const uint8_t* data, int length) override
    {
        char str[1024];
        char extra[5];

        // scriptLog("In " + deviceName, 1);
        if (IsButton(data, length)) {

            /* sprintf_s(str, "Received button event: ");
            for (auto x = 0; x < length; x++)
            {
                sprintf_s(extra, " %0x", data[x]);
                strcat_s(str, extra);
            }
            scriptLog(str, 1);
            scriptLog(deviceName, 1); */

            ProcessButton(data[1], data[2]);  // it's a button press
        }
        else if (IsKnob(data, length))
        {
            ProcessKnob(data[1] - MCX_KNOB_1,
                        data[2]); // pass the pedal position (0-4) and the value
        }
        else
        {
            sprintf(str, "Unexpected midi event: ");
            for (auto x = 0; x < length; x++)
            {
                sprintf(extra, " %0x", data[x]);
                strcat(str, extra);
            }
            scriptLog(str, 1);
            scriptLog(deviceName, 1);
        }

        return (false);
    }
    


    // Called when a new gig file has been loaded and when song is changed
    void OnSongChanged(int oldIndex, int newIndex) override
    {
        // scriptLog("Song changed to number " + std::to_string(newIndex), 1);
        // Surface.BottomMode = SHOW_SONGPARTS;
        CurrentBankName(getSongName(newIndex));
    } 

    // Called when entering song mode
    void OnModeChanged(int mode) override
    {
        if (mode == 1)
        {
            // scriptLog("Entered setlist mode.", 1);
            // SetButtonColor(MKIII_CLEAR, Surface.BottomColor[SHOW_RACKSPACES]);
            // Surface.BottomMode = SHOW_SONGS;
            DisplayRow(Surface.Row[BOTTOM_ROW]);
            DisplayRow(Surface.Row[TOP_ROW]);
            DisplayRow(Surface.Row[B2_ROW]);
            DisplayRow(Surface.Row[T2_ROW]);
        }
        else
        {
            // scriptLog("Entered rackspace mode.", 1);
            // SetButtonColor(MKIII_CLEAR, Surface.BottomColor[SHOW_SONGS]);
            // Surface.BottomMode = SHOW_RACKSPACES;
            DisplayRow(Surface.Row[BOTTOM_ROW]);
            DisplayRow(Surface.Row[TOP_ROW]);
            DisplayRow(Surface.Row[B2_ROW]);
            DisplayRow(Surface.Row[T2_ROW]);
        }
    }

    // examine a vector of widgets and populate the Surface structure to reflect widgets in the rackspace or global rackspace
    void buildSurfaceModel(std::vector <std::string> widgetlist)
    {
        std::string widgetname, prefix, type, bank, column;

        for (auto index = widgetlist.begin(); index != widgetlist.end(); ++index)
        {
            widgetname = *index;
            widgetname = widgetname.substr(widgetname.find(":") + 1);

            std::vector< std::string> name_segments = ParseWidgetName(widgetname, '_');

            // scriptLog("MCx sees widget " + widgetname, 1);

            if (name_segments.size() == 4)
            {
                prefix = name_segments[0];
                type = name_segments[1];
                bank = name_segments[2];
                column = name_segments[3];

                // if it's a widget we're interested in, add a bank for it if it doesn't already exist, and listen for it
                if (prefix.compare(THIS_PREFIX) == 0 )
                {
                    if (Surface.addSurfaceBank(type, bank) == true) {  /* scriptLog("MCx:  bS added bank for " + widgetname, 1); */ }
                    // we don't listen for bank select widgets or anything else without an integer in the column field
                    if (column == std::to_string(std::stoi("0" + column)) ) { listenForWidget(widgetname, true); }
                }

            }
        }
    }

    // examines a Row of widgets and determines which bank should be active and sets things accordingly
    // this is required because after Rackspace or Variation switching we can end up in states where multiple banks or no banks are flagged as "active"
    bool setActiveBank(SurfaceRow & row)
    {
        std::string widgetname, widgetindicator;
        int index;

        row.ActiveBank = -1;

        if (row.BankIDs.empty())
        {
            return false;
        }
        else
        {
            for (index = 0; index < row.BankIDs.size(); ++index)
            {
                widgetname = row.WidgetPrefix + (std::string)"_" + row.BankIDs[index] + (std::string)"_i";
                // scriptLog("sAFB sees " + widgetname + (std::string)" as " + std::to_string(getWidgetValue(widgetname)), 1);
                if (widgetExists(widgetname))
                {
                    if ((getWidgetValue(widgetname) > 0.99) && (row.ActiveBank == -1))
                    {
                        row.ActiveBank = index;
                        // scriptLog("setActiveBank set active to " + std::to_string(index) + " " + Surface.Row[row].BankIDs[index], 1);
                    }
                    else if (index != row.ActiveBank) { setWidgetValue(widgetname, 0.3); }
                }
            }
            if (row.ActiveBank == -1) { row.ActiveBank = 0; }
            return true;
        }
    }

    // Called when rackspace changed
    void OnRackspaceActivated() override
    {
        std::string widgetname;
        std::vector<std::string> widgetlist, globalwidgetlist;
        int row;

        // scriptLog("Rackspace Changed to " + std::to_string(getCurrentRackspaceIndex()) , 1);

        // Clear the BankIDs and active bank data from the prior rackspace's widget set
        for (row = 0; row < std::size(Surface.Row); row++)
        {
            Surface.Row[row].ActiveBank = -1;
            Surface.Row[row].BankIDs.clear();
        }

        getWidgetList(globalwidgetlist, true);
        getWidgetList(widgetlist, false);
        widgetlist.insert(widgetlist.end(), globalwidgetlist.begin(), globalwidgetlist.end());
        buildSurfaceModel(widgetlist);

        if (widgetExists(MCX_CONFIG_WIDGETNAME))
        {
            widgetlist = ParseWidgetName(getWidgetCaption(MCX_CONFIG_WIDGETNAME), '_');
            for (row = 0; row < Surface.NumRows; row++)
            {
                if (row < widgetlist.size())
                {
                    // scriptLog("sizeof list:" + std::to_string(sizeof(widgetlist)) + "  row:" + std::to_string(row), 1);
                    if (widgetlist[row] == "buttons") Surface.Row[row].Showing = SHOW_BUTTONS;
                    else if (widgetlist[row] == "variations") Surface.Row[row].Showing = SHOW_VARS_PARTS;
                    else if (widgetlist[row] == "racks") Surface.Row[row].Showing = SHOW_RACKS_SONGS;
                }
            }
        }

        // scriptLog("SL identified " + std::to_string(Surface.Row[KNOB_ROW].BankIDs.size()) + " knob banks", 1);
        // scriptLog("SL identified " + std::to_string(Surface.Row[BUTTON_ROW].BankIDs.size()) + " button banks", 1);

        setActiveBank(Surface.Row[TOP_ROW]);
        DisplayRow(Surface.Row[TOP_ROW]);
        
        setActiveBank(Surface.Row[BOTTOM_ROW]);
        DisplayRow(Surface.Row[BOTTOM_ROW]);

        setActiveBank(Surface.Row[B2_ROW]);
        DisplayRow(Surface.Row[B2_ROW]);

        setActiveBank(Surface.Row[T2_ROW]);
        DisplayRow(Surface.Row[T2_ROW]);

        setActiveBank(Surface.Row[E3_ROW]);
        DisplayRow(Surface.Row[E3_ROW]);

        setActiveBank(Surface.Row[E4_ROW]);
        DisplayRow(Surface.Row[E4_ROW]);
     
        if (inSetlistMode() == true)
        {
            CurrentBankName(getSongName(getCurrentSongIndex()));
            LongPresetNames(getSongpartName(getCurrentSongIndex(), getCurrentSongpartIndex()));
        }
        else
        {
            CurrentBankName(getRackspaceName(getCurrentRackspaceIndex()));
            LongPresetNames(getVariationName(getCurrentRackspaceIndex(), getCurrentVariationIndex()));
        }
    } 


    // Called when variation changed
    void OnVariationChanged(int oldIndex, int newIndex) override
    {
        // scriptLog("Variation Changed from " + std::to_string(oldIndex) + " to " + std::to_string(newIndex) + "; GetCurrentVariation says " + std::to_string(getCurrentVariationIndex()), 1);

        //if (Surface.BottomMode == SHOW_VARIATIONS) { DisplayBottom(true); }
        
        // setActiveBank(Surface.Row[KNOB_ROW]);
        // DisplayRow(Surface.Row[KNOB_ROW]);

        setActiveBank(Surface.Row[BOTTOM_ROW]);
        DisplayRow(Surface.Row[BOTTOM_ROW]);

        setActiveBank(Surface.Row[TOP_ROW]);
        DisplayRow(Surface.Row[TOP_ROW]);

        setActiveBank(Surface.Row[B2_ROW]);
        DisplayRow(Surface.Row[B2_ROW]);

        setActiveBank(Surface.Row[T2_ROW]);
        DisplayRow(Surface.Row[T2_ROW]);

        LongPresetNames(getVariationName(getCurrentRackspaceIndex(), newIndex));

        // Notify("Variation: " + newIndex);
    }

    void OnSongPartChanged(int oldIndex, int newIndex) override
    {
        // scriptLog("Songpart Changed", 1);
        LongPresetNames(getSongpartName(getCurrentSongIndex(), newIndex));
    }


    // A midi device was added or removed
    void OnMidiDeviceListChanged(std::vector< std::string>& inputs, std::vector< std::string>& outputs) override
    {
        SetMidiInOutDevices();
    }


    // Called when ititially loading this extension.  Not called again, even on loading a new Gig.
    void OnOpen() override
    {
        // scriptLog("OnOpen called.", 1);
        Surface.Initialize();

        // Notify("Funk it up, Boy!", 20);
    }

    // Called when shutting down
    void OnClose() override
    {
        std::string hexstring, binstring;
        uint8_t x;

        ClearDisplayRow(Surface.Row[BOTTOM_ROW]);
        ClearDisplayRow(Surface.Row[TOP_ROW]);
        ClearDisplayRow(Surface.Row[B2_ROW]);
        ClearDisplayRow(Surface.Row[T2_ROW]);
        CurrentBankName("GigPerformer Extension");
        if (Surface.Page == 1) {
            SendTextToMCx("", 0x00, 0x02, 0x00); // page toggle
        }

    }

    // Initialization of the dll plugin
    void Initialization() override
       {
        // Do any initialization that you need
        // scriptLog("Path to me = " + getPathToMe(), 1);
        Surface.Initialize();
        Surface.syncState = 1;
        InitializeMC8();
              
        // Register callabacks
        registerCallback("OnOpen");
        registerCallback("OnStatusChanged");
        registerCallback("OnMidiDeviceListChanged");
        registerCallback("OnClose");
       }

    // Generally don't touch this - simply define the constant 'XMLProductDescription' at the top of this file with
    // an XML description of your product
    std::string GetProductDescription() override // This MUST be defined in your class
        {
           return XMLProductDescription;
        }
    
};

