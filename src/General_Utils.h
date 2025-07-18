// Utility functions that don't depend on anything in GP or specific control surfaces


std::vector <std::string> ParseWidgetName(const std::string& s, char delim);

std::string cleanSysex(std::string s);

std::string HexToByteString(const std::string& hex);  // GP SDK also does this with GPMidiMessage(hexstring);