#include "vodka-lib.h"
#include "../dependencies/termcolor.hpp"
#include "../dependencies/base64.h"
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <boost/crc.hpp>
#include <filesystem>
#include <sys/resource.h>
#include <stdexcept>
#include <cstdlib>
using namespace std;
double vodka::utilities::get_process_time() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF,&usage)!=0) {
        throw std::runtime_error("Unable to get process time.");
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
string vodka::utilities::genvyid() {
    const char* format=getenv("VODKA_EXE_HASH");
    string hashexe;
    if (format!=nullptr) {
        hashexe=string(format);
    } else {
        ifstream file("/proc/self/exe",ios::binary);
        vector<unsigned char> exebuff((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
        boost::crc_32_type exehash;
        exehash.process_bytes(exebuff.data(), exebuff.size());
        auto exedigest=exehash.checksum();
        std::stringstream ssexe;
        ssexe<<std::hex<<std::setw(8)<<std::setfill('0')<<exedigest;
        hashexe=ssexe.str().substr(0,8);
        setenv("VODKA_EXE_HASH",hashexe.c_str(),1);
    }
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0, 0xFFFFFFFFFFFFFFFFULL);
    uint64_t random_value1=dist(gen);
    std::stringstream ss1;
    ss1<<std::hex<<std::setfill('0')<<std::setw(16)<< random_value1;
    string g1=ss1.str();
    uint64_t random_value2=dist(gen);
    std::stringstream ss2;
    ss2<<std::hex<<std::setfill('0')<<std::setw(16)<< random_value2;
    string g2=ss2.str();
    uint64_t random_value3=dist(gen);
    std::stringstream ss3;
    ss3<<std::hex<<std::setfill('0')<<std::setw(16)<< random_value3;
    string g3=ss3.str();
    string small1=g1.substr(0,8);
    string small2=g2.substr(0,8);
    string small3=g3.substr(0,8);
    boost::crc_32_type hashother1;
    string other=g2+g3;
    hashother1.process_bytes(other.data(),other.size());
    auto hash1=hashother1.checksum();
    std::stringstream sshash1;
    sshash1<<std::hex<<std::setw(8)<<std::setfill('0')<<hash1;
    auto h1=sshash1.str().substr(0,8);
    boost::crc_32_type hashother2;
    other=g1+g3;
    hashother2.process_bytes(other.data(),other.size());
    auto hash2=hashother2.checksum();
    std::stringstream sshash2;
    sshash2<<std::hex<<std::setw(8)<<std::setfill('0')<<hash2;
    auto h2=sshash2.str().substr(0,8);
    boost::crc_32_type hashother3;
    other=g1+g2;
    hashother3.process_bytes(other.data(),other.size());
    auto hash3=hashother3.checksum();
    std::stringstream sshash3;
    sshash3<<std::hex<<std::setw(8)<<std::setfill('0')<<hash3;
    auto h3=sshash3.str().substr(0,8);
    uint32_t val1=static_cast<uint32_t>(std::stoul(small1,nullptr,16));
    uint32_t val2=static_cast<uint32_t>(std::stoul(small2,nullptr,16));
    uint32_t val3=static_cast<uint32_t>(std::stoul(small3,nullptr,16));
    uint32_t valother1=static_cast<uint32_t>(std::stoul(h1,nullptr,16));
    uint32_t valother2=static_cast<uint32_t>(std::stoul(h2,nullptr,16));
    uint32_t valother3=static_cast<uint32_t>(std::stoul(h3,nullptr,16));
    uint32_t valexe=static_cast<uint32_t>(std::stoul(hashexe,nullptr,16));
    uint32_t enc1=val1^valexe^valother1;
    uint32_t enc2=val2^valexe^valother2;
    uint32_t enc3=val3^valexe^valother3;
    std::stringstream sss1;
    std::stringstream sss2;
    std::stringstream sss3;
    sss1<<std::hex<<std::setw(8)<<std::setfill('0')<<enc1;
    sss2<<std::hex<<std::setw(8)<<std::setfill('0')<<enc2;
    sss3<<std::hex<<std::setw(8)<<std::setfill('0')<<enc3;
    boost::crc_32_type hashtotal;
    string total=sss1.str()+sss2.str()+sss3.str();
    hashtotal.process_bytes(total.data(),total.size());
    auto sumtotal=hashtotal.checksum();
    std::stringstream sstotal;
    sstotal<<std::hex<<std::setw(8)<<std::setfill('0')<<sumtotal;
    auto strtotal=sstotal.str();
    boost::crc_32_type hashgrp;
    hashgrp.process_bytes(strtotal.data(),strtotal.size());
    auto sumgrp=hashgrp.checksum();
    std::stringstream ssgrp;
    ssgrp<<std::hex<<std::setw(8)<<std::setfill('0')<<sumgrp;
    auto strgrp=ssgrp.str();
    string final1=sss1.str()+g1;
    string final2=sss2.str()+g2;
    string final3=sss3.str()+g3;
    return "["+base64::convertHexToBase64(strgrp).substr(0,6)+"-"+base64::convertHexToBase64(final1)+"-"+base64::convertHexToBase64(final2)+"-"+base64::convertHexToBase64(final3)+"-"+base64::convertHexToBase64(strtotal).substr(0,6)+"]";
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