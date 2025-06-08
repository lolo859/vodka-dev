#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <boost/hash2/sha3.hpp> // pour sha3_512
using namespace std;
bool verify_id(const std::string& id, const std::string& exe_hash) {
    if (id.size() < 2 || id.front() != '[' || id.back() != ']') return false;
    std::string exe_hash_trunc = exe_hash.substr(0, 8); // garde juste les 8 premiers hex

    // Retire les crochets
    std::string content = id.substr(1, id.size() - 2);

    // Sépare en 3 groupes
    size_t pos1 = content.find('-');
    if (pos1 == std::string::npos) return false;
    size_t pos2 = content.find('-', pos1 + 1);
    if (pos2 == std::string::npos) return false;

    std::string part1 = content.substr(0, pos1);
    std::string part2 = content.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string part3 = content.substr(pos2 + 1);

    // Chaque part doit faire 24 chars : 8 hex enc + 16 hex g
    if (part1.size() != 24 || part2.size() != 24 || part3.size() != 24) return false;

    // Extraction
    std::string enc1_str = part1.substr(0, 8);
    std::string g1_str = part1.substr(8, 16);
    std::string enc2_str = part2.substr(0, 8);
    std::string g2_str = part2.substr(8, 16);
    std::string enc3_str = part3.substr(0, 8);
    std::string g3_str = part3.substr(8, 16);

    // Convertir strings hex en uint32_t / uint64_t
    auto hex_to_uint32 = [](const std::string& s) -> uint32_t {
        return static_cast<uint32_t>(std::stoul(s, nullptr, 16));
    };

    auto hex_to_uint64 = [](const std::string& s) -> uint64_t {
        return std::stoull(s, nullptr, 16);
    };

    uint32_t enc1 = hex_to_uint32(enc1_str);
    uint64_t g1 = hex_to_uint64(g1_str);
    uint32_t enc2 = hex_to_uint32(enc2_str);
    uint64_t g2 = hex_to_uint64(g2_str);
    uint32_t enc3 = hex_to_uint32(enc3_str);
    uint64_t g3 = hex_to_uint64(g3_str);

    // Récupérer valexe
    uint32_t valexe = hex_to_uint32(exe_hash_trunc);

    // Extraire petits morceaux (8 premiers hex chars des gX)
    auto low32 = [](uint64_t val) -> uint32_t {
        return static_cast<uint32_t>(val >> 32);
    };
    uint32_t val1 = low32(g1);
    uint32_t val2 = low32(g2);
    uint32_t val3 = low32(g3);

    // Calculer les hash croisés (sha3_512 de concaténations gX)
    auto sha3_8hex = [](const std::string& data) -> std::string {
        boost::hash2::sha3_512 h;
        h.update(reinterpret_cast<const unsigned char*>(data.data()), data.size());
        auto digest = h.result();
        std::stringstream ss;
        for (auto b : digest) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)b;
        }
        return ss.str().substr(0, 8);
    };

    std::string h1_str = sha3_8hex(g2_str + g3_str);
    std::string h2_str = sha3_8hex(g1_str + g3_str);
    std::string h3_str = sha3_8hex(g1_str + g2_str);

    uint32_t valother1 = hex_to_uint32(h1_str);
    uint32_t valother2 = hex_to_uint32(h2_str);
    uint32_t valother3 = hex_to_uint32(h3_str);

    // Calcul des enc attendus
    uint32_t enc1_expected = val1 ^ valexe ^ valother1;
    uint32_t enc2_expected = val2 ^ valexe ^ valother2;
    uint32_t enc3_expected = val3 ^ valexe ^ valother3;

    // Compare
    return (enc1 == enc1_expected) && (enc2 == enc2_expected) && (enc3 == enc3_expected);
}
int main() {
    if (verify_id("[c5fb95d014da4d43b988237b-ea22a82a22839b440c7803b0-2463c0df1800444883e5aded]","2f2e7a48f1b13344c93335f1a137a3dc015b89d663bf80a1dbbe3eb182608753c728d199f57ea5c1702172691057a072a5af131cbeada7b375502eca17fc8f8f")) {
        cout<<"yes"<<endl;
    }
}