#include <stdint.h>
namespace base85 {
    constexpr char Z85_CHARS[]=
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        ".~:+=^!/*?&<>()_|{}@%$#";
    inline void uint32_to_b85(uint32_t value,char* out) {
        uint32_t div4=52200625;
        uint32_t div3=614125;
        uint32_t div2=7225;
        uint32_t div1=85;
        out[0]=Z85_CHARS[value/div4];
        value%=div4;
        out[1]=Z85_CHARS[value/div3];
        value%=div3;
        out[2]=Z85_CHARS[value/div2];
        value%=div2;
        out[3]=Z85_CHARS[value/div1];
        value%=div1;
        out[4]=Z85_CHARS[value];
        out[5]='\0';
    }
    inline void uint64_to_b85(uint64_t value,char* out) {
        uint32_to_b85(static_cast<uint32_t>(value>>32),out);
        uint32_to_b85(static_cast<uint32_t>(value & 0xFFFFFFFF),out+5);
        out[10]='\0';
    }
}