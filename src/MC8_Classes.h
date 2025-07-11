#pragma once

#include "gigperformer/sdk/GPMidiMessages.h"
#include "gigperformer/sdk/GPUtils.h"
#include "gigperformer/sdk/GigPerformerAPI.h"
#include "gigperformer/sdk/types.h"

#include "MC8_Constructs.h"
#include "MC8_Colors.h"
#include "MC8_Buttons.h"
#include "General_Utils.h"

// Define identifiers GP user must use to name their widgets
#define THIS_PREFIX "mcx"
#define BOTTOM_PREFIX "mcx_b"
#define TOP_PREFIX "mcx_t"
#define B2_PREFIX "mcx_b2"
#define T2_PREFIX "mcx_t2"
#define E1_PREFIX "mcx_e1"
#define E2_PREFIX "mcx_e2"
#define E3_PREFIX "mcx_e3"
#define E4_PREFIX "mcx_e4"

#define ROW_PREFIX_ARRAY { BOTTOM_PREFIX, TOP_PREFIX, B2_PREFIX, T2_PREFIX, E1_PREFIX, E2_PREFIX, E3_PREFIX, E4_PREFIX}

#define KNOB_TYPE "Knob"
#define BUTTON_TYPE "Button"
#define FADER_TYPE "Fader"
#define PAD_TYPE "Pad"
#define ROW_TYPE_ARRAY { BUTTON_TYPE, BUTTON_TYPE, BUTTON_TYPE, BUTTON_TYPE, KNOB_TYPE, KNOB_TYPE, KNOB_TYPE, KNOB_TYPE  }
#define ROW_LABEL_ARRAY   {  "Button", "Button", "Button", "Button", "Knob", "knob", "Knob", "knob" }

#define KNOB_TAG "k"
#define BUTTON_TAG "n"
#define FADER_TAG "f"
#define PAD_TAG "p"
#define BOTTOM_TAG "b"
#define TOP_TAG "t"
#define B2_TAG "b2"
#define T2_TAG "t2"
#define E1_TAG "e1"
#define E2_TAG "e2"
#define E3_TAG "e3"
#define E4_TAG "e4"
#define TAG_ARRAY  { BOTTOM_TAG, TOP_TAG, B2_TAG, T2_TAG, E1_TAG, E2_TAG, E3_TAG, E4_TAG }

#define BOTTOM_ROW 0
#define TOP_ROW 1
#define B2_ROW 2
#define T2_ROW 3
#define E1_ROW 4
#define E2_ROW 5
#define E3_ROW 6
#define E4_ROW 7

#define KNOB_ROW 0
#define BUTTON_ROW 1
#define FADER_ROW 2
#define PAD_ROW 3

#define BOTTOM_MODES 5 // number of different possible modes for bottom display row
#define SHOW_BUTTONS 4
#define SHOW_RACKS_SONGS 2
#define SHOW_VARS_PARTS 3
#define SHOW_KNOBS 1



#define SONGLIST_UP MKIII_TRACK_LEFT
#define SONGLIST_DOWN MKIII_TRACK_RIGHT

// The SurfaceWidget class is the conduit used for translating GP widget information and changes to control surface display controller_widgettype_bankname_position
// These are always temporary.  We do not store the state of widgets or control surface items in the extension.
// These are created temporarily when GP tells us a widget changed, or we switch to a new rackspace and need to read widgets to update the control surface
class SurfaceWidget
{
public:
	std::string SurfacePrefix;  // typical widget label structure as "controller_widgettype_bankname_position" eg. sl_k_pan_3
	std::string WidgetID;
	std::string BankID;
	uint8_t Column = 255;

	bool Validated = false;  // user may create widgets not on the surface or in banks that don't yet exist - this simplifies detection and crash avoidance
	bool IsSurfaceItemWidget = false;  // indicates the widget maps to a physical surface control
	bool IsRowParameterWidget = false;  // things like names or resolutions on GP widgets that don't correspond to physical surface controls

	int RowNumber = -1;  // If this widget is associated with a Row, used primarily to determine if it's the active bank or not

	// these are GigPerformer values
	double Value = 0.0;
	std::string TextValue = "";
	std::string Caption = "";

	// these are controller specific items we populate from control widgets or associated parameter widgets
    std::string ShortNameOn = "";
    std::string ShortNameOff = "";
    std::string LongName = "";

	int RgbLitColor = 0, RgbDimColor = 0;

	int resolution = 1000;

};

// The SurfaceRow class contain information for a single row of physical controls on a control surface
// The information in this structure is persistent in the extension.  It keeps track of multiple banks, which is active, etc.
class SurfaceRow
{
public:
	std::vector<std::string> BankIDs;
	std::string WidgetID = BUTTON_TAG; // the widget ID that signifies this row, e.g., f, k, b, etc.  Right now these
	std::string Type = BUTTON_TYPE;  // this is internal coding for how the widget behaves.  e.g., pads and buttons may both act like buttons but have different WidgetIDs
    std::string WidgetPrefix = BOTTOM_PREFIX; // the hardware identifier (e.g. "sl") + the WidgetID (e.g. "f") together
	std::string RowLabel = "Switch";  // a friendly name for what the row is, e.g., "Faders", "Knobs", etc.  Used to indicate on the display what bank of controls the displayed labels are for

	int ActiveBank = -1; // the index of the bank shown on and controlled by the controller
	uint8_t Showing = SHOW_BUTTONS; // whether we show buttons, Racks/Songs, or Variations/Parts
    int FirstShown = 0;
	uint8_t Columns = 4;
	// uint8_t MidiCommand = 0x9f; // midi command from the control surface that corresponds to this row
	uint8_t FirstID = 0;  // the first ID that corresponds to the row, eg. note number 0x64.  Elements of a control row must have sequential IDs
	uint8_t FirstIDsysex = 0;  // the first ID if we address it using sysex instead of NoteOn/CC
    int LitColor = 0x4F0000, DimColor = 0x100000;

	bool BankValid() // is the bank indicated by ActiveBank a valid bank?
	{
		return (ActiveBank >= 0 && ActiveBank < BankIDs.size());
	}

	std::string ActiveBankID() // the name of the active bank.  We display this sometimes so the user can see what bank is selected 
	{
		if (BankValid()) return BankIDs[ActiveBank];
		else return "";
	}

	// addBank called when we scan through GP's widget list and discover a new bank
	bool addBank(std::string bank) {
		int index, result;
		bool found = false;

		for (index = 0; index < BankIDs.size(); index++)
		{
			result = BankIDs[index].compare(bank);
			if (result == 0)
			{
				found = true;
				break;
			}
			else if (result > 0) { break; }
		}
		if (!found) {
			BankIDs.insert(BankIDs.begin() + index, bank);
		}
		return (!found);
	}

	// NextBank() and PreviousBank() return appropriate bank numbers without changing the ActiveBank
	int NextBank()
	{
		if (ActiveBank < 0)
		{
			return -1;
		}
		else if (ActiveBank < BankIDs.size() - 1)
		{
			return ActiveBank + 1;
		}
		else
		{
			return 0;
		}

	}

	int PreviousBank()
	{
		if (ActiveBank > 0) { return ActiveBank - 1; }
		else { return BankIDs.size() - 1; }
	}

	// Increment and Decrement the active bank
	bool IncrementBank()
	{
		if (NextBank() >= 0)
		{
			ActiveBank = NextBank();
			return true;
		}
		else return false;
	}

	bool DecrementBank()
	{
		if (PreviousBank() >= 0)
		{
			ActiveBank = PreviousBank();
			return true;
		}
		else return false;
	}

	// Switch active bank based on bank name
	bool makeActiveBank(std::string bank) {
		int index, result;
		bool found = false;

		for (index = 0; index < BankIDs.size(); index++)
		{
			result = BankIDs[index].compare(bank);
			if (result == 0)
			{
				found = true;
				ActiveBank = index;
				break;
			}
			else if (result > 0) { break; }
		}
		return found;
	}

};

// SurfaceClass is our model of the control surface and how it is configured
class SurfaceClass
{
public:
	SurfaceRow Row[8]; // We define the control rows as bottom and top of display, plus a button row for each of the four possible externals
	uint8_t NumRows = 8;
	uint8_t RowLen = MC8_ROWLEN; // mc8 = 4, mc6 pro = 3
	uint8_t ShortNameLen = MC8_SHORTLEN; // mc8 = 10, mc6 pro = 32
	uint8_t LongNameLen = MC8_LONGLEN;  // mc8 = 24, mc6 pro = 32
	bool Color = MC8_COLOR;
	std::string SysexPrefix = MC8_PREFIX;

	bool RowsLinked = false; // MCx this controls display of 2 rows of four (false), or one group of 8 (true)

	// uint8_t BottomMode = SHOW_BUTTONS;  // What to show along bottom row of display
    // uint8_t TopMode = SHOW_VARS_PARTS; // What to show along top row of display

	// uint8_t DisplayLayout = KNOB_LAYOUT; // Not sure if we're going to use this at all 
	// uint8_t SideMode = SHOW_BUTTONS;
	uint8_t Page = 0;

	// FirstShown[x] is used to remember what number is the first shown on the display if we're paging through songs/parts/racks/variations
	// if both Bottom and Top row are displaying the same thing we'll display 8 consecutive rather than different starts for each row
	// uint8_t FirstShown[BOTTOM_MODES] = { 0, 0, 0, 0, 0 };
	// uint8_t BottomColor[BOTTOM_MODES] = { SLMKIII_ORANGE, SLMKIII_BLUE, SLMKIII_PURPLE, SLMKIII_MINT, SLMKIII_GREEN };
	// uint8_t BottomHalfColor[BOTTOM_MODES] = { SLMKIII_ORANGE_HALF, SLMKIII_BLUE_HALF, SLMKIII_PURPLE_HALF, SLMKIII_MINT_HALF, SLMKIII_GREEN_HALF };

	int LastRackspace = -1;
	int syncState = 0;  // is our current model in sync with the device.  semi-deprecated

	// Initialize the surface class as required - specific to a particular control surface
	bool Initialize()
	{
		int x;
		LastRackspace = -1;
		std::string row_prefixes[] = ROW_PREFIX_ARRAY;
		std::string row_tags[] = TAG_ARRAY;
		std::string row_types[] = ROW_TYPE_ARRAY;
		std::string row_labels[] = ROW_LABEL_ARRAY;
        int row_showing[] = {SHOW_BUTTONS, SHOW_VARS_PARTS, SHOW_RACKS_SONGS, SHOW_BUTTONS, SHOW_KNOBS, SHOW_KNOBS, SHOW_KNOBS, SHOW_KNOBS };
        uint8_t midi_commands[] = {MIDI_CC_00, MIDI_CC_00, MIDI_CC_00, MIDI_CC_00, MIDI_CC_00, MIDI_CC_00, MIDI_CC_00, MIDI_CC_00 };
		// uint8_t first_midi[] = { MCX_BUTTON_B1, MCX_BUTTON_T1, MCX_BUTTON_B2, MCX_BUTTON_T2, 22, 23, 24, 25 };
		// int row_columns[] = { 4, 4, 4, 4, 1, 1, 1, 1 };
		// uint8_t first_sysex[] = { MKIII_KNOB_BASE, MKIII_BUTTON_BASE_SYSEX, MKIII_FADER_BASE_SYSEX, MKIII_PAD_BASE_SYSEX };

		// basic Surface structure initializations
		for (x = 0; x < std::size(Row); x++)
		{
			Row[x].WidgetPrefix = row_prefixes[x];
			Row[x].WidgetID = row_tags[x];
			Row[x].RowLabel = row_labels[x];
			Row[x].Type = row_types[x];
			// Row[x].Columns = row_columns[x];
			if (x > 3) { Row[x].FirstID = 24 + x; Row[x].Columns = 1; } // we don't send midi to expression rows, all are 1 column
			else { Row[x].FirstID = x * RowLen; Row[x].Columns = RowLen; }
            Row[x].Showing = row_showing[x];
			// Row[x].FirstIDsysex = first_sysex[x];
			// Row[x].MidiCommand = midi_commands[x];
		}

		return true;
	}

	// adds a bank of the indicated type to our surface model
	bool addSurfaceBank(std::string type, std::string bank) {
		int index;

		for (index = 0; index < std::size(Row); index++) {
			if (Row[index].WidgetID.compare(type) == 0)
			{
				return Row[index].addBank(bank);
			}
		}
		return false;  // if we get here, we didn't find an appropriate Row in the array
	}

	int IdentifySurfaceRow(std::string rowidentifier) {
		int x;

		for (x = 0; x < NumRows; x++)
		{
			if (Row[x].WidgetID.compare(rowidentifier) == 0)
				return x;
		}
		return -1;
	}

	bool RowValid(int rownum)
	{
		return (rownum >= 0 && rownum < NumRows);
	}

	bool SyncBankIDs(uint8_t syncrow)
	{
		bool changes_made = false;

		if (Row[syncrow].BankValid()) {
			std::string rowname = Row[syncrow].ActiveBankID();
			for (uint8_t x = 0; x < std::size(Row); x++)
			{
				if (x != syncrow)
				{
					if (Row[x].makeActiveBank(rowname)) changes_made = true;
				}
			}
		}

		return changes_made;
	}

	gigperformer::sdk::GPMidiMessage PresetShortname(uint8_t position, std::string shortname)
	{
		gigperformer::sdk::GPMidiMessage midimessage;

		std::string cleantext = cleanSysex(shortname) + (std::string)"                                                "; // make sure it cover the full length
		
		midimessage = gigperformer::sdk::GPMidiMessage::makeSysexMessage(gigperformer::sdk::GPUtils::hex2binaryString(SysexPrefix)
			+ cleantext.substr(0,ShortNameLen) + gigperformer::sdk::GPUtils::hex2binaryString("00f7")); // append the csum placeholder + sysex end marker f7

		// set opcodes
		midimessage.setValue((uint8_t) OPCODE_2, (uint8_t) SHORTNAME_OP);
		midimessage.setValue((uint8_t) OPCODE_3, (uint8_t) position);
		midimessage.setValue((uint8_t) OPCODE_4, (uint8_t) 0);
		midimessage.setValue((uint8_t) OPCODE_5, (uint8_t) 0);

		// calculate and place checksum
		midimessage.setValue(midimessage.length() - 2,
			(uint8_t)calculateChecksum(midimessage.length(), midimessage.asBytes()));
		return midimessage;

	}

private:
	// Calculate the checksum required on all MCx messages
	uint8_t calculateChecksum(uint16_t len, uint8_t* ptr)
	{
		uint8_t cSum = *(ptr);
		for (int i = 1; i < len - 2; i++)
		{
			cSum ^= *(ptr + i);
		}
		cSum &= 0x7F;
		return cSum;
	}

	uint8_t calculateChecksum(gigperformer::sdk::GPMidiMessage gpmidimessage)
	{
		return calculateChecksum(gpmidimessage.length(), gpmidimessage.asBytes());
	}
};

