#include "LibMain.h"


// using PVar = gigperformer::sdk:: GigPerformerFunctions::PersistentVariable;

// List of panels
std::vector<std::string> panelNames = { "MC8 Demo" };
std::vector<std::string> relativePanelLocations = { "MC8 Demo.gppanel" };


std::string pathToMe; // This needs to be initialized from the initialization secttion of the LibMain class so it can be used in the standalone functions directly below

int LibMain::GetPanelCount()
{
    return panelNames.size();
}

std::string  LibMain::GetPanelName(int index)
{
    std::string text;
    if (index >= 0 && index < panelNames.size())
        text = panelNames[index];

    return text;
}

// Return panel layout in XML format
std::string  LibMain::GetPanelXML(int index)
{
    std::string text;
    if (index >= 0 && index < panelNames.size())
    {
        // We assume the panels are in the same folder as the library
        // scriptLog("MC: Path to panels: " + getPathToMe() + relativePanelLocations[index], 0);
        gigperformer::sdk::GPUtils::loadTextFile(getPathToMe() + relativePanelLocations[index], text);
    }
    return text;
}



// List of menu items
std::vector<std::string> menuNames = { "Reset MC midi ports", "Show surface status", "Serialize Colors", "Deserialize Colors",
    "Store string", "Recall string", "Write Color Preset"};


int LibMain::GetMenuCount()
{
    return menuNames.size();
}

std::string  LibMain::GetMenuName(int index)
{
    std::string text;
    if (index >= 0 && index < menuNames.size())
        text = menuNames[index];

    return text;
}


/* void LibMain::oscMessageReceived(const juce::OSCMessage& message)
{
    std::string address, arguments;
    address = message.getAddressPattern().toString().toStdString();

    for (int i = 0; i < message.size(); i++) {
        arguments += message[i].getString().toStdString() + " ";
	}
    // Handle incoming OSC messages here
    scriptLog(address + arguments, 0);
    // You can add more detailed handling based on the message content
} */


void LibMain::InvokeMenu(int index)
{
    std::vector <std::string> widgetlist;
    std::string widgetname, colorstatestring;
    std::stringstream ss;

	// PVar testvar(this, "colorlist", true, false); // this is how you would define a persistent variable in a function


    if (index >= 0 && index < menuNames.size())
    {
        switch (index)
        {
        case 0:
            MidiIn = ParseWidgetName(getWidgetCaption(MIDI_IN_WIDGETNAME), ',');
			MidiOut = ParseWidgetName(getWidgetCaption(MIDI_OUT_WIDGETNAME), ',');
			SetMidiInOutDevices();
			// Surface.Initialize();
			// OnStatusChanged(GPStatus_GigFinishedLoading);
            // Surface.syncState = 1;
			// Surface.LastRackspace = -1; // force a re-initialization of the surface
			// OnRackspaceActivated();
			DisplayRefresh(true);
            break;
        case 1:
            ShowState();
            // SetSurfaceLayout("mc6");
            break;
        case 2:
			// maybe combine all the colors into a single structure for serialization
			// need individual routines to serialize and write out and read back structures using GP state
			// clearAllPersistentVariables(false);
			colorstatestring = Surface.MakeColorStateString();
			scriptLog("Serialized color data size: " + std::to_string(colorstatestring.size()), 1);
            for (int i = 0; i < colorstatestring.size(); i++) {
                scriptLog(" byte " + std::to_string(i) + ": " + std::to_string((uint8_t)colorstatestring[i]), 1);
			}
            storePersistentBinaryVariable("RackColors", colorstatestring, true);
            break;
        case 3:
            // if (nameExists("RackColors", false))
            {
				colorstatestring = recallPersistentBinaryVariable("RackColors", true);
                scriptLog("Serialized color data size: " + std::to_string(colorstatestring.size()), 1);
                for (int i = 0; i < colorstatestring.size(); i++) {
                    scriptLog(" byte " + std::to_string(i) + ": " + std::to_string((uint8_t)colorstatestring[i]), 1);
                }

                if (Surface.LoadColorStateString(colorstatestring)) {
                    scriptLog("Deserialized color data successfully.", 1);
                }
                else {
                    scriptLog("Error deserializing color data.", 1);
                    break;
				}
            }
            break;
        case 4:
			// store string test
			storePersistentStringVariable("TestString", "This is a test string from MCX extension", false);
            storePersistentStringVariable("TestString", "This should overwrite the test string from MCX extension", true);
            storePersistentStringVariable("AnotherString", "This is another string from MCX extension", true);
            break;
        case 5:
			// recall string test
			scriptLog(recallPersistentStringVariable("TestString", true), 1);
            scriptLog(recallPersistentStringVariable("AnotherString", true), 1);
            break;

        case 6:
			// store preset color data
			
            break;

        default:
            break;
        }
    }
}

void LibMain::ShowState()
{
    scriptLog("MCX: Current State:", 0);
    scriptLog("     Input Device: " + Surface.InputDevice, 0);
    scriptLog("     Output Device: " + Surface.OutputDevice, 0);
    scriptLog("     Page: " + std::to_string(Surface.Page), 0);
    for (int row = 0; row < 4; row++)
    {
        scriptLog("     Row " + std::to_string(row) + " showing " + std::to_string(Surface.Row[row].Showing) + ".  Firstshown is " + std::to_string(Surface.Row[row].FirstShown), 0);
    }
}

void LibMain::sendMidiMessage(std::string MidiMessage) {
    
    // for (int i = 0; i < MidiOut.size(); i++) {
    //    sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage);
    // }
    sendMidiMessageToMidiOutDevice(Surface.OutputDevice, MidiMessage);
}

void LibMain::sendMidiMessage(gigperformer::sdk::GPMidiMessage MidiMessage)
{
    
    //for (int i = 0; i < MidiOut.size(); i++) {
    //    sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage);
    //}
    sendMidiMessageToMidiOutDevice(Surface.OutputDevice, MidiMessage);

}

void LibMain::sendMidiMessage(const uint8_t* MidiMessage, int length) {
    // for (int i = 0; i < MidiOut.size(); i++) {
    //    sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage, length);
    //}
    sendMidiMessageToMidiOutDevice(Surface.OutputDevice, MidiMessage, length);
}

// MCx controllers come in different configurations
// We have a configuration variable in the Surface structure so we can define different action keys for different functions
void LibMain::SetSurfaceLayout(std::string config) {

    if (widgetExists(LAYOUT_WIDGETNAME)) { setWidgetCaption(LAYOUT_WIDGETNAME, config); }

    if (config.compare("mc6 pro") == 0)
    {
        Surface.SysexPrefix = MC6PRO_PREFIX;
        Surface.Color = MC6PRO_COLOR;
        Surface.RowLen = MC6PRO_ROWLEN;
        Surface.ShortNameLen = MC6PRO_SHORTLEN;
        Surface.LongNameLen = MC6PRO_LONGLEN;
        Surface.LongNamePreset = MC6PRO_LONGNAME_PRESET;
        Surface.ColorsPreset = MC6PRO_COLOR_PRESET;
    }
    else if (config.compare("mc8 pro") == 0)
    {
        Surface.SysexPrefix = MC8PRO_PREFIX;
        Surface.Color = MC8PRO_COLOR;
        Surface.RowLen = MC8PRO_ROWLEN;
        Surface.ShortNameLen = MC8PRO_SHORTLEN;
        Surface.LongNameLen = MC8PRO_LONGLEN;
        Surface.LongNamePreset = MC8PRO_LONGNAME_PRESET;
        Surface.ColorsPreset = MC8PRO_COLOR_PRESET;
    }
    else if (config.compare("mc4 pro") == 0)
    {
        Surface.SysexPrefix = MC4PRO_PREFIX;
        Surface.Color = MC4PRO_COLOR;
        Surface.RowLen = MC4PRO_ROWLEN;
        Surface.ShortNameLen = MC4PRO_SHORTLEN;
        Surface.LongNameLen = MC4PRO_LONGLEN;
        Surface.LongNamePreset = MC4PRO_LONGNAME_PRESET;
        Surface.ColorsPreset = MC4PRO_COLOR_PRESET;
    }
    else if (config.compare("mc6") == 0)
    {
        Surface.SysexPrefix = MC6_PREFIX;
        Surface.Color = MC6_COLOR;
        Surface.RowLen = MC6_ROWLEN;
        Surface.ShortNameLen = MC6_SHORTLEN;
        Surface.LongNameLen = MC6_LONGLEN;
    }
    else
    {
        Surface.SysexPrefix = MC8_PREFIX;
        Surface.Color = MC8_COLOR;
        Surface.RowLen = MC8_ROWLEN;
        Surface.ShortNameLen = MC8_SHORTLEN;
        Surface.LongNameLen = MC8_LONGLEN;
    }
    Surface.Initialize();
    // OnRackspaceActivated();
    // DisplayRefresh();
}

void LibMain::ProcessOSC(juce::OSCMessage message)
{
    std::string address, widgetname, songname;

    // auto msg = message.getAddressPattern().toString().toStdString();
    // if (message.size() == 1 && message[0].isFloat32())

    address = message.getAddressPattern().toString().toStdString();
    widgetname = "sm_" + address.substr(1);
    scriptLog("Recv " + address, 0);

    if (address == "/play" && message.size() == 1 && message[0].isInt32())
    {
        // setWidgetValue(widgetname, (float) 1.0 * message[0].getInt32());
    }

    if (address == "/barBeat" && message.size() == 3)
    {
        setWidgetCaption("sm_bar", std::to_string(message[0].getInt32()));
        setWidgetCaption("sm_beat", std::to_string(message[1].getInt32()));
        setWidgetCaption("sm_bpm", std::to_string(message[2].getFloat32()));
    }

    if (address == "/songLoaded" && message.size() == 3)
    {
        // scriptLog("Recv " + songname, 0);
        if (inSetlistMode())
        {
            // switchToSongname(message[0].getString().toStdString());
        }
    }


}


gigperformer::sdk::GigPerformerAPI *gigperformer::sdk::CreateGPExtension(LibraryHandle handle)
{
    return new LibMain(handle);
}
