#include <map>
#include <string>

std::map<char, std::string> xbyte = {
  // escape_characters
  {'\0', "0x00"}, {'\v', "0x0a"}, {'\f', "0x0b"}, {'\b', "0x0c"}, {'\t', "0x0d"},
  {'\r', "0x0e"}, {'\n', "0x0f"}, {'\\', "0x0g"}, {'\"', "0x0h"}, {'\'', "0x0i"},

  // numbers
  {'0', "0xa0"}, {'1', "0xaa"}, {'2', "0xab"}, {'3', "0xac"}, {'4', "0xad"},
  {'5', "0xae"}, {'6', "0xaf"}, {'7', "0xag"}, {'8', "0xah"}, {'9', "0xai"},

  // low_letters
  {'a', "0xb0"}, {'b', "0xba"}, {'c', "0xbb"}, {'d', "0xbc"}, {'e', "0xbd"},
  {'f', "0xbe"}, {'g', "0xbf"}, {'h', "0xbg"}, {'i', "0xbh"}, {'j', "0xbi"},
  {'k', "0xc0"}, {'l', "0xca"}, {'m', "0xcb"}, {'n', "0xcc"}, {'o', "0xcd"},
  {'p', "0xce"}, {'q', "0xcf"}, {'r', "0xcg"}, {'s', "0xch"}, {'t', "0xci"},
  
  // high_letters
  {'u', "0xd0"}, {'v', "0xda"}, {'w', "0xdb"}, {'x', "0xdc"}, {'y', "0xdd"},
  {'z', "0xde"}, {'A', "0xdf"}, {'B', "0xdg"}, {'C', "0xdh"}, {'D', "0xdi"},
  {'E', "0xe0"}, {'F', "0xea"}, {'G', "0xeb"}, {'H', "0xec"}, {'I', "0xed"},
  {'J', "0xee"}, {'K', "0xef"}, {'L', "0xeg"}, {'M', "0xeh"}, {'N', "0xei"},
  
  // special_characters
  {'O', "0xf0"}, {'P', "0xfa"}, {'Q', "0xfb"}, {'R', "0xfc"}, {'S', "0xfd"},
  {'T', "0xfe"}, {'U', "0xff"}, {'V', "0xfg"}, {'W', "0xfh"}, {'X', "0xfi"},
  {'Y', "0xg0"}, {'Z', "0xga"}, {'~', "0xgb"}, {'`', "0xgc"}, {'!', "0xgd"},
  {'@', "0xge"}, {'#', "0xgf"}, {'$', "0xgg"}, {'%', "0xgh"}, {'^', "0xgi"},
  
  // other_special_characters
  {'&', "0xh0"}, {'*', "0xha"}, {'(', "0xhb"}, {')', "0xhc"}, {'-', "0xhd"},
  {'_', "0xhe"}, {'+', "0xhf"}, {'=', "0xhg"}, {'{', "0xhh"}, {'}', "0xhi"},
  {'[', "0xi0"}, {']', "0xia"}, {'|', "0xib"}, {'/', "0xic"}, {':', "0xid"},
  {';', "0xie"}, {'<', "0xif"}, {'>', "0xig"}, {',', "0xih"}, {'.', "0xii"},

  // unknown_character
  {'?', "0x??"},

  // spacebar
  {' ', "0xsb"}
};

std::string encode(const std::string& data) {
  std::string encodexbyte;
  for (char c : data) {
    auto it = xbyte.find(c);
    if (it != xbyte.end()) {
      encodexbyte += it->second;
    }
  }
  return encodexbyte;
}

void setup() {
    Serial.begin(9600);

    // Example usage:
    std::string data = "bad dog! ?";
    std::string encoded = encode(data);

    Serial.print("Encoded value for '");
    Serial.print(data.c_str());
    Serial.print("': ");
    Serial.println(encoded.c_str());
}

void loop() {
    // Your main Arduino loop
}
