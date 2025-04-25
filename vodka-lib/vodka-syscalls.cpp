#include "vodka-lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
using namespace std;
//* Some necessary functions
namespace inside_syscalls {
    std::vector<std::string> split(const std::string& str,const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t start=0;
        size_t end=str.find(delimiter);
        while (end!=std::string::npos) {
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
    void replaceall(std::string &str,const std::string &from,const std::string &to) {
        size_t start_pos=0;
        while ((start_pos=str.find(from, start_pos))!=std::string::npos) {
            str.replace(start_pos,from.length(),to);
            start_pos+=to.length();
        }
    }
}
using namespace inside_syscalls;
//* Syntax function for syscalls
string vodka::syscalls::syscall_container::syntax() {
    if (thing==vodka::syscalls::list_syscall::PRINT) {
        string args;
        for (auto a:printele.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return printele.name+" "+args;
    } else if (thing==vodka::syscalls::list_syscall::ADD) {
        string args;
        for (auto a:addele.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return addele.name+" "+addele.output_uid+" "+args;
    } else if (thing==vodka::syscalls::list_syscall::ASSIGN) {
        return assignele.name+" "+assignele.output_uid+" "+assignele.value;
    } else if (thing==vodka::syscalls::list_syscall::FREE) {
        string args;
        for (auto a:freeele.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return freeele.name+" "+args;
    } else if (thing==vodka::syscalls::list_syscall::INVERT) {
        return invertele.name+" "+invertele.uid;
    } else if (thing==vodka::syscalls::list_syscall::DUPLICATE) {
        return duplicateele.name+" "+duplicateele.output_uid+" "+duplicateele.source_uid;
    } else if (thing==vodka::syscalls::list_syscall::ABS) {
        return absele.name+" "+absele.uid;
    } else if (thing==vodka::syscalls::list_syscall::DIVMOD) {
        return divmodele.name+" "+divmodele.quotient_uid+" "+divmodele.rest_uid+" "+divmodele.dividend_uid+" "+divmodele.divisor_uid;
    } else if (thing==vodka::syscalls::list_syscall::TOINT) {
        return tointele.name+" "+tointele.uid_output+" "+tointele.uid_source;
    } else if (thing==vodka::syscalls::list_syscall::TODEC) {
        return todecele.name+" "+todecele.uid_output+" "+todecele.uid_source;
    } else if (thing==vodka::syscalls::list_syscall::MULINT) {
        return mulintele.name+" "+mulintele.output_uid+" "+mulintele.first_uid+" "+mulintele.second_uid;
    } else if (thing==vodka::syscalls::list_syscall::MULDEC) {
        return muldecele.name+" "+muldecele.output_uid+" "+muldecele.first_uid+" "+muldecele.second_uid+" "+muldecele.precision_uid;
    } else if (thing==vodka::syscalls::list_syscall::DIVIDE) {
        return divideele.name+" "+divideele.output_uid+" "+divideele.first_uid+" "+divideele.second_uid+" "+divideele.precision_uid;
    } else {
        return "error";
    }
}
//* Convert list_syscall to string
string vodka::syscalls::syscall_to_string(list_syscall syscall) {
    switch (syscall) {
        case list_syscall::PRINT:return "PRINT";
        case list_syscall::ADD:return "ADD";
        case list_syscall::ASSIGN:return "ASSIGN";
        case list_syscall::FREE:return "FREE";
        case list_syscall::INVERT:return "INVERT";
        case list_syscall::DUPLICATE:return "DUPLICATE";
        case list_syscall::ABS:return "ABS";
        case list_syscall::DIVMOD:return "DIVMOD";
        case list_syscall::TOINT:return "TOINT";
        case list_syscall::TODEC:return "TODEC";
        case list_syscall::MULDEC:return "MULDEC";
        case list_syscall::MULINT:return "MULINT";
        case list_syscall::DIVIDE:return "DIVIDE";
        default:return "Unknown";
    }
}