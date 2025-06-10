#ifndef HEX_TO_BASE64_HPP
#define HEX_TO_BASE64_HPP
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>
namespace base64 {
    inline std::string convertHexToBase64(const std::string& hexString) {
        if (hexString.length() % 2 != 0) {
            throw std::invalid_argument("La chaîne hexadécimale doit avoir une longueur paire.");
        }
        std::vector<unsigned char> bytes;
        bytes.reserve(hexString.length() / 2);
        for (size_t i = 0; i < hexString.length(); i += 2) {
            std::string byteString = hexString.substr(i, 2);
            unsigned int byte = 0;
            std::stringstream ss;
            ss << std::hex << byteString;
            ss >> byte;
            bytes.push_back(static_cast<unsigned char>(byte));
        }
        std::string out;
        int val = 0;
        int valb = -6;
        static const std::string BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        for (unsigned char c : bytes) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back(BASE64_CHARS[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) {
            out.push_back(BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);
        }
        while (out.size() % 4) {
            out.push_back('=');
        }
        return out;
    }
}
#endif