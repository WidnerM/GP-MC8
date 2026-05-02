#pragma once
// #include "LibMain.h"
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_osc/juce_osc.h>


// OSC Receiver class.  Instantiate, then call connectToPort(portnumber)
class AnOSCReceiver : public juce::Component,
                      public juce::OSCReceiver,
                      public juce::OSCReceiver::Listener<juce::OSCReceiver::MessageLoopCallback>
{
  public:
    //==============================================================================
    AnOSCReceiver()
    {
        addListener(this);
    }

    virtual ~AnOSCReceiver()
    {
        disconnect();
    }

    bool connectToPort(int port)
    {
        return connect(port);
    }

  private:
    //==============================================================================
    void oscMessageReceived(const juce::OSCMessage &message) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnOSCReceiver)
};


class AnOSCReceiverWithAddress
    : public juce::Component,
                      public juce::OSCReceiver,
                      public juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>
{
  public:
    //==============================================================================
    AnOSCReceiverWithAddress()
    {
        connect(54321);
        addListener(this, "/play");
    }

    virtual ~AnOSCReceiverWithAddress()
    {
        disconnect();
    }


  private:
    //==============================================================================
    void oscMessageReceived(const juce::OSCMessage &message) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnOSCReceiverWithAddress)
};
