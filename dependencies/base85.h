#include <stdint.h>
namespace base85 {
    constexpr char Z85_CHARS[] = 
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        ".~:+=^!/*?&<>()_|{}@%$#";
    inline void uint32_to_b85(uint32_t value, char* out) {
        uint32_t div=52200625;
        for (int i=0;i<5;++i) {
            uint32_t idx=value/div;
            out[i]=Z85_CHARS[idx];
            value%=div;
            div/=85;
        }
        out[5]='\0';
    }
    inline void uint64_to_b85(uint64_t value,char* out) {
        uint32_to_b85(static_cast<uint32_t>(value>>32), out);
        uint32_to_b85(static_cast<uint32_t>(value & 0xFFFFFFFF),out+5);
        out[10]='\0';
    }
}