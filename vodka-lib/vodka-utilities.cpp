#include "vodka-lib.h"
#include "termcolor.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
#include <cstring>
using namespace std;
//* Some necessary functions
namespace inside_utils {
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
using namespace inside_utils;
//* Utilities
//* Error output function
void vodka::utilities::error(const string& error,const string& file,vector<string> lines_content,vector<int> lines,vector<string> source) {
    string linestr="";
    if (source.size()!=0) {
        for (int i=0;i<source.size();i++) {
            auto path=split(source[i],">");
            auto sourcename=path[path.size()-1];
            path.pop_back();
            auto sourcenamepath=split(sourcename,"::");
            auto functionname=sourcenamepath[sourcenamepath.size()-1];
            if (i!=0) {
                cout<<"["+to_string(i+1)+"] Called function "<<termcolor::green<<termcolor::bold<<functionname<<termcolor::reset<<", located in :"<<endl;
            } else {
                cout<<"[1] Function "<<termcolor::green<<termcolor::bold<<functionname<<termcolor::reset<<", located in :"<<endl;
            }
            for (int y=0;y<path.size();++y) {
                cout<<" ├─ "<<termcolor::blue<<termcolor::bold<<path[y]<<termcolor::reset<<" (file/directory)"<<endl;
            }
            for (int y=0;y<sourcenamepath.size();++y) {
                if (sourcenamepath[y]==functionname) {
                    cout<<" └─ "<<termcolor::green<<termcolor::bold<<sourcenamepath[y]<<termcolor::reset<<" (function)"<<endl;
                } else {
                    cout<<" ├─ "<<termcolor::cyan<<termcolor::bold<<sourcenamepath[y]<<termcolor::reset<<" (namespace)"<<endl;
                }
            }
        }
        cout<<endl<<"Raised the following exception :"<<endl;
    }
    if (lines.size()!=0) {
        if (lines.size()>1) {
            for (size_t i=0;i<lines.size();++i) {
                if (i==lines.size()-1) {
                    linestr=linestr+to_string(lines[i])+",";
                } else {
                    linestr=linestr+to_string(lines[i])+", ";
                }
            }
        } else {
            linestr=to_string(lines[0]);
        }
    }
    if (file!="") {
        cout<<"\nFile "<<termcolor::blue<<termcolor::bold<<file<<termcolor::reset<<":"+to_string(lines[0])+", line(s) "+linestr+" an error occured :"<<endl;
    }
    if (lines_content.size()!=0 && lines_content.size()!=0) {
        for (size_t y=0;y<lines.size();++y) {
            cout<<"  "<<lines[y]<<" | "<<lines_content[y]<<endl;
        }
    }
    cout<<termcolor::red<<termcolor::bold<<error<<termcolor::reset<<endl;
}
//* UUID generator
boost::uuids::uuid vodka::utilities::genuid() {
    boost::uuids::random_generator ran;
    boost::uuids::uuid uuid=ran();
    return uuid;
}
//* Logs functions
void vodka::utilities::log(const string& text,string verbose,int x,string last,int sublevel,vector<int> substep,vector<unsigned long> subtotal) {
    if (verbose=="a" || verbose=="r") {
        if (sublevel==0) {
            if (verbose=="a" || verbose=="r") {
                cout<<"\n";
                string texti;
                if (verbose=="r" && text.substr(text.length()-1,1)==":") {
                    texti=text.substr(0,text.length()-2)+"...";
                } else {
                    texti=text;
                }
                last="("+to_string(x)+"/18) "+texti;
                cout<<last;
            }
        } else {
            if (verbose=="a") {
                cout<<endl;
                last="("+to_string(x)+"/18) ";
                for (int i=0;i<sublevel;++i) {
                    last=last+"("+to_string(substep[i])+"/"+to_string(subtotal[i])+") ";
                }
                last=last+text;
                cout<<last;
            }
        }
    }
}
void vodka::utilities::debuglog(const string& text,int line,const string& cell,bool debugmode,string verbose,string file,bool debug_info) {
    if (debugmode==true) {
        if (debug_info==true) {
            if (verbose=="e") {
                string texti=text.substr(1,text.length()-1);
                cout<<"Debug line "+to_string(line)+" in cell "+cell+" from file "+filesystem::absolute(file).string()+" : "+texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<"Debug line "+to_string(line)+" in cell "+cell+" "+filesystem::absolute(file).string()+" : "+texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<"Debug line "+to_string(line)+" in cell "+cell+" "+filesystem::absolute(file).string()+" : "+texti;
            }
        } else {
            if (verbose=="e") {
                string texti=text.substr(2,text.length()-2);
                cout<<texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<texti;
            }
        }
    }
}