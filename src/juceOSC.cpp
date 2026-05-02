#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "juceOSC.h"
#include "LibMain.h"

LibMain *xlib = new LibMain(nullptr); // mrw - this is needed as a reference to LibMain

void AnOSCReceiver::oscMessageReceived(const juce::OSCMessage &message)
{
    xlib->ProcessOSC(message);
}

void AnOSCReceiverWithAddress::oscMessageReceived(const juce::OSCMessage &message)
{
    auto msg = message.getAddressPattern().toString().toStdString();

    // auto msgx = msg.toString();
    // auto msgy = msgx.toStdString();
    // if (message.size() == 1 && message[0].isFloat32())                           // [5]
    // rotaryKnob.setValue(juce::jlimit(0.0f, 10.0f, message[0].getFloat32())); // [6]
    // xlib->scriptLog(msg, 0);
}