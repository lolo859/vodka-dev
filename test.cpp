#include <string>
#include <iostream>
#include "dependencies/base64.h"
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;
string hex_to_base64(string hex) {
    vector<unsigned char> bytes;
    for (size_t i=0;i<hex.length();i+=2) {
        string bytestr=hex.substr(i,2);
        unsigned char byte=(unsigned char)strtol(bytestr.c_str(),nullptr,16);
        bytes.push_back(byte);
    }
    string binstr(bytes.begin(),bytes.end());
    return to_base64(binstr);
}
string base64_to_hex(string b64) {
    string bin=from_base64(b64);
    stringstream ss;
    for (unsigned char c:bin) {
        ss<<hex<<setw(2)<<setfill('0')<<(int)c;
    }
    return ss.str();
}
int main() {
    cout<<hex_to_base64("653775ceb0eaae419983c98c")<<endl;
    cout<<base64_to_hex(hex_to_base64("653775ceb0eaae419983c98c"))<<endl;
}