#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>

// Utility functions that don't depend on anything in GP or specific control surfaces

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

std::vector <std::string> ParseWidgetName(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        ltrim(item);
        result.push_back(item);
    }
    return result;
}

// remove elements from a binary string (binary or text) that are outside the acceptable range for inclusion in a MIDI sysex string
int issysexunsafe(int ch) { return ch < 0 || ch > 127; }

std::string cleanSysex(std::string subtext)
{
    // subtext.erase(std::remove_if(subtext.begin(), subtext.end(), [](auto const& c) -> bool { return !issysexsafe(c); }), subtext.end());
    subtext.erase(std::remove_if(subtext.begin(), subtext.end(), issysexunsafe), subtext.end());
    return subtext;
}


// convert a hex string (F07A etc.) to bytes string to send as midi data
// GP SDK will do this with GPMidiMessage(hexstring);
std::string HexToByteString(const std::string& hex) {
    std::string bytes;

    size_t len = hex.length();
    for (size_t i = 0; i < len; i += 2) {
        if (i + 1 >= len) break; // Ignore incomplete byte
        unsigned int byte;
        std::istringstream(hex.substr(i, 2)) >> std::hex >> byte;
        bytes.push_back(static_cast<char>(byte));
    }
    return bytes;
}
