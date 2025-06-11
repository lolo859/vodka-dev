#include "vodka-lib.h"
#include "../dependencies/termcolor.hpp"
#include "../dependencies/base64.h"
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <boost/crc.hpp>
#include <filesystem>
#include <sys/resource.h>
#include <stdexcept>
#include <cstdlib>
extern "C" {
    #include "../dependencies/xxhash.h"
};
using namespace std;
double vodka::utilities::get_process_time() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF,&usage)!=0) {
        throw runtime_error("Unable to get process time.");
    }
    double user_time=usage.ru_utime.tv_sec+usage.ru_utime.tv_usec/1e6;
    double sys_time=usage.ru_stime.tv_sec+usage.ru_stime.tv_usec/1e6;
    return user_time+sys_time;
}
//* String utilities
vector<string> vodka::utilities::string_utilities::split(string str,string delimiter) {
    vector<string> tokens;
    size_t start=0;
    size_t end=str.find(delimiter);
    while (end!=string::npos) {
        if (end>start) {
            tokens.push_back(str.substr(start,end-start));
        }
        start=end+delimiter.length();
        end=str.find(delimiter,start);
    }
    if (start<str.length()) {
        tokens.push_back(str.substr(start));
    }
    return tokens;
}
void vodka::utilities::string_utilities::replaceall(string str,string from,string to) {
    size_t start_pos=0;
    while ((start_pos=str.find(from, start_pos))!=string::npos) {
        str.replace(start_pos,from.length(),to);
        start_pos+=to.length();
    }
}
string vodka::utilities::string_utilities::strip(string text,string character) {
    if (character.size()!=1) {
        return nullptr;
    }
    char target=character[0];
    size_t i=0;
    while (i<text.size() && text[i]==target) {
        ++i;
    }
    return text.substr(i);
}
//* UUID generator
string vodka::utilities::genvyid(std::mt19937_64& gen,vodka::utilities::structs::random_values& rand) {
    uniform_int_distribution<uint64_t> dist(0, 0xFFFFFFFFFFFFFFFFULL);
    rand.rand1=dist(gen);
    rand.rand2=dist(gen);
    rand.rand3=dist(gen);
    uint64_t seed=rand.rand1^rand.rand2^rand.rand3;
    static const char* hex_chars = "0123456789abcdef";
    uint32_t small1=rand.rand1>>32;
    uint32_t small2=rand.rand2>>32;
    uint32_t small3=rand.rand3>>32;
    uint64_t sign1=rand.rand2^rand.rand3;
    uint64_t sign2=rand.rand1^rand.rand3;
    uint64_t sign3=rand.rand1^rand.rand2;
    sign1>>=32;
    sign2>>=32;
    sign3>>=32;
    uint32_t enc1=small1^sign1;
    uint32_t enc2=small2^sign2;
    uint32_t enc3=small3^sign3;
    char ench1[9];
    ench1[8]='\0';
    for (int i=7;i>=0;--i) {
        ench1[i]=hex_chars[enc1 & 0xF];
        enc1>>=4;
    }
    char ench2[9];
    ench2[8]='\0';
    for (int i=7;i>=0;--i) {
        ench2[i]=hex_chars[enc2 & 0xF];
        enc2>>=4;
    }
    char ench3[9];
    ench3[8]='\0';
    for (int i=7;i>=0;--i) {
        ench3[i]=hex_chars[enc3 & 0xF];
        enc3>>=4;
    }
    char rand1[17];
    rand1[16]='\0';
    for (int i=15;i>=0;--i) {
        rand1[i]=hex_chars[rand.rand1 & 0xF];
        rand.rand1>>=4;
    }
    char rand2[17];
    rand2[16]='\0';
    for (int i=15;i>=0;--i) {
        rand2[i]=hex_chars[rand.rand2 & 0xF];
        rand.rand2>>=4;
    }
    char rand3[17];
    rand3[16]='\0';
    for (int i=15;i>=0;--i) {
        rand3[i]=hex_chars[rand.rand3 & 0xF];
        rand.rand3>>=4;
    }
    char total[25];
    std::memcpy(total,ench1,8);
    std::memcpy(total+8,ench2,8);
    std::memcpy(total+16,ench3,8);
    total[24]='\0';
    auto sumtotal=XXH64(total,sizeof(total),seed);
    uint32_t trunctotal=sumtotal>>32;
    char strtotal[9];
    strtotal[8]='\0';
    for (int i=7;i>=0;--i) {
        strtotal[i]=hex_chars[trunctotal & 0xF];
        trunctotal>>=4;
    }
    auto sumgrp=XXH64(strtotal,sizeof(strtotal),seed);
    uint32_t truncgrp=sumgrp>>32;
    char strgrp[9];
    strgrp[8]='\0';
    for (int i=7;i>=0;--i) {
        strgrp[i]=hex_chars[truncgrp & 0xF];
        truncgrp>>=4;
    }
    char output[70];
    std::string b64_grp=base64::convertHexToBase64(strgrp).substr(0,6);
    std::string b64_1=base64::convertHexToBase64(ench1);
    std::string b64_2=base64::convertHexToBase64(ench2);
    std::string b64_3=base64::convertHexToBase64(ench3);
    std::string b64_r1=base64::convertHexToBase64(ench1);
    std::string b64_r2=base64::convertHexToBase64(ench2);
    std::string b64_r3=base64::convertHexToBase64(ench3);
    std::string b64_total=base64::convertHexToBase64(strtotal).substr(0,6);
    snprintf(output,sizeof(output),"[%s-%s%s-%s%s-%s%s-%s]",b64_grp.c_str(),b64_1.c_str(),b64_r1.c_str(),b64_2.c_str(),b64_r2.c_str(),b64_3.c_str(),b64_r3.c_str(),b64_total.c_str());
    return string(output);
}
string vodka::utilities::genvyid() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937_64 gen(rd());
    static thread_local vodka::utilities::structs::random_values rands;
    return genvyid(gen,rands);
}
//* Logs functions
void vodka::utilities::output::log(string text,int log_main_step,string last,int sublevel,vector<int> substep,vector<unsigned long> subtotal) {
    const char* format=getenv("VODKA_SHOW_LOG_TIME");
    string time;
    if (format!=nullptr && string(format)=="TRUE") {
        time="["+to_string(get_process_time())+"]";
    } else {
        time="";
    }
    format=getenv("VODKA_VERBOSE_MODE");
    string verbose;
    if (format!=nullptr) {
        if (string(format)=="e" || string(format)=="a" || string(format)=="r") {
            verbose=string(format);
        } else {
            verbose="e";
        }
    } else {
        verbose="e";
    }
    if (verbose=="a" || verbose=="r") {
        if (sublevel==0) {
            if (verbose=="a" || verbose=="r") {
                cout<<endl<<time+"[LOG]     ";
                string texti;
                if (verbose=="r" && text.substr(text.length()-1,1)==":") {
                    texti=text.substr(0,text.length()-2)+"...";
                } else {
                    texti=text;
                }
                last="("+to_string(log_main_step)+"/19) "+texti;
                cout<<last;
            }
        } else {
            if (verbose=="a") {
                cout<<endl<<time+"[LOG]     ";
                last="("+to_string(log_main_step)+"/19) ";
                for (int i=0;i<sublevel;++i) {
                    last=last+"("+to_string(substep[i])+"/"+to_string(subtotal[i])+") ";
                }
                last=last+text;
                cout<<last;
            }
        }
    }
}
void vodka::utilities::output::debuglog(string text,int line,string cell,string file,bool debug_info) {
    const char* format=getenv("VODKA_SHOW_LOG_TIME");
    string time;
    if (format!=nullptr && string(format)=="TRUE") {
        time="["+to_string(get_process_time())+"]";
    } else {
        time="";
    }
    format=getenv("VODKA_DEBUG_MODE");
    bool debug_mode;
    if (format!=nullptr && string(format)=="TRUE") {
        debug_mode=true;
    } else {
        debug_mode=false;
    }
    format=getenv("VODKA_VERBOSE_MODE");
    string verbose;
    if (format!=nullptr) {
        if (string(format)=="e" || string(format)=="a" || string(format)=="r") {
            verbose=string(format);
        } else {
            verbose="e";
        }
    } else {
        verbose="e";
    }
    if (debug_mode==true) {
        if (debug_info==true) {
            if (verbose=="e") {
                string texti=text.substr(1,text.length()-1);
                cout<<time+"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<time+"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<time+"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti;
            }
        } else {
            if (verbose=="e") {
                string texti=text.substr(2,text.length()-2);
                cout<<time+"[DEBUG]   "<<texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<time+"[DEBUG]   "<<texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<time+"[DEBUG]   "<<texti;
            }
        }
    }
}
void vodka::utilities::output::var_warning(string namevar,vodka::variables::VariableDatatype typevar,string namecell,string line) {
    const char* format=getenv("VODKA_SHOW_LOG_TIME");
    string time;
    if (format!=nullptr && string(format)=="TRUE") {
        time="["+to_string(get_process_time())+"]";
    } else {
        time="";
    }
    format=getenv("VODKA_SHOW_VAR_WARNING");
    bool var_warning_enabled;
    if (format!=nullptr && string(format)=="TRUE") {
        var_warning_enabled=true;
    } else {
        var_warning_enabled=false;
    }
    format=getenv("VODKA_VERBOSE_MODE");
    string verbose;
    if (format!=nullptr) {
        if (string(format)=="e" || string(format)=="a" || string(format)=="r") {
            verbose=string(format);
        } else {
            verbose="e";
        }
    } else {
        verbose="e";
    }
    if (var_warning_enabled==true) {
        if (verbose=="e") {
            cout<<time+"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+vodka::variables::datatype_to_string(typevar)+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        } else if (verbose=="r") {
            cout<<endl<<time+"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+vodka::variables::datatype_to_string(typevar)+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        } else if (verbose=="a") {
            cout<<endl<<time+"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+vodka::variables::datatype_to_string(typevar)+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        }
    }
}