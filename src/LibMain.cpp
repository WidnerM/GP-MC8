#include "LibMain.h"


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
std::vector<std::string> menuNames = { "Reset MC midi ports" };


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


void LibMain::InvokeMenu(int index)
{
    std::vector <std::string> widgetlist;
    std::string widgetname;

    if (index >= 0 && index < menuNames.size())
    {
        switch (index)
        {
        case 0:
			SetMidiInOutDevices();
			Surface.Initialize();
			OnStatusChanged(GPStatus_GigFinishedLoading);
            Surface.syncState = 1;
			Surface.LastRackspace = -1; // force a re-initialization of the surface
			OnRackspaceActivated();
            break;
        case 1:
            SetSurfaceLayout("mc6");
            break;
        case 2:
            SetSurfaceLayout("mc6 pro");
            break;
        case 3:
            EngagePreset(32, 1);
            break;
        case 4:
            // UpdatePresetMessage(0, 10, 3, PRESET_SAVE, ACTION_LONGDOUBLETAP, TOGGLE_TYPE_BOTH, 0x22, 0x33, 0); // doesn't work, but should be the PresetMessage for setting a color
            sendMidiMessage(makeMCHexMessage("55 55 55 55 01 02 03 04 05 06 07 08 09", 5, 0,0,0,0));
            break;
        case 5:
            OnStatusChanged(GPStatus_GigFinishedLoading);
            break;

        default:
            break;
        }
    }
}


void LibMain::sendMidiMessage(std::string MidiMessage) {
    
    for (int i = 0; i < MidiOut.size(); i++) {
        sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage);
    }
}

void LibMain::sendMidiMessage(gigperformer::sdk::GPMidiMessage MidiMessage)
{
    
    for (int i = 0; i < MidiOut.size(); i++) {
        sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage);
    }
}

void LibMain::sendMidiMessage(const uint8_t* MidiMessage, int length) {
    for (int i = 0; i < MidiOut.size(); i++) {
        sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage, length);
    }
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
    }
    else if (config.compare("mc8 pro") == 0)
    {
        Surface.SysexPrefix = MC8PRO_PREFIX;
        Surface.Color = MC8PRO_COLOR;
        Surface.RowLen = MC8PRO_ROWLEN;
        Surface.ShortNameLen = MC8PRO_SHORTLEN;
        Surface.LongNameLen = MC8PRO_LONGLEN;
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


gigperformer::sdk::GigPerformerAPI *gigperformer::sdk::CreateGPExtension(LibraryHandle handle)
{
    return new LibMain(handle);
}
