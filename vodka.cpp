#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <bitset>
#include <unistd.h>
#include <cctype>
#include <getopt.h>
#include <chrono>
#include <boost/hash2/sha3.hpp>
#include <cstdlib>
#include "vodka-lib/vodka-lib.h"
#include "dependencies/json.hpp"
//* Some necessary functions
string hash_then_encode(string origin) {
    boost::hash2::sha3_512 hasher;
    hasher.update(origin.data(),origin.size());
    auto digest=hasher.result();
    string out;
    for (auto byte:digest) {
        bitset<8> bits(static_cast<unsigned char>(byte));
        auto str=bits.to_string();
        for (auto a:str) {
            if (a=='0') {
                out=out+u8"\u200B";
            } else {
                out=out+u8"\u2063";
            }
        }
    }
    return out;
}
string encode_to_bin(string input) {
    string binar;
    for (unsigned char a:input) {
        bitset<8> bits(a);
        binar=binar+bits.to_string();
    }
    string out;
    for (auto a:binar) {
        if (a=='0') {
            out=out+u8"\u200B";
        } else {
            out=out+u8"\u2063";
        }
    }
    return hash_then_encode(out);
}
using namespace std;
using namespace vodka::utilities;
using namespace vodka::errors;
//* Some variables
//* Variables for compilation and options
string verbose="e";
bool debug_mode=false;
bool var_warning_enabled=true;
bool disable_integrity_hash=false;
bool show_log_time=false;
int log_main_step=1;
string last;
string file_source;
SourcesStack srcstack;
vector<string> final;
map<string,map<string,string>> json_ints;
map<string,map<string,map<string,string>>> json_ints_v;
//* Vectors and maps for code and code structure analysis
map<string,vodka::variables::VariableContainer> mainargsdict;
map<string,vodka::variables::VariableContainer> variablesdict;
vector<string> variableslist;
vector<string> mainargslist;
map<string,string> replacement;
vector<string> symbollist={"VODSTART","VODEND","VODIMPORT","VODTYPE","VODSTRUCT","VODCLASS","VODENDCLASS","VODEFINE"};
vector<string> typelist={"app","command","shell","gui","logonui","logonshell","service"};
vector<string> content;
vector<string> output_file_to_check;
vector<symbol> symbols;
bool typefound=false;
string program_type;
vector<cell> cells;
vector<string> cellnames;
cell maincell;
//* Read file(s) and removing comments
bool read_file(string output,string mode,SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (output=="") {
        raise(ErrorContainer("vodka.error.file.output_not_provided","<no file>",{"<no line affected>"},{0},lclstack));
        return false;
    }
    if (mode=="check") {
        log("Checking if source and output file exist.",verbose,log_main_step,last);
        if (filesystem::exists(file_source)==false) {
            raise(ErrorContainer("Source file doesn't exist.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        if (filesystem::exists(output)==false) {
            raise(ErrorContainer("Output file doesn't exist.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        log_main_step=log_main_step+1;
        log("Checking files readability.",verbose,log_main_step,last);
        ifstream file_source_stream(file_source.c_str());
        if (file_source_stream.is_open()==false) {
            raise(ErrorContainer("Source file can't be open.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        ifstream file_output_stream(output.c_str());
        if (file_output_stream.is_open()==false) {
            raise(ErrorContainer("Output file can't be open.",output,{"<no line affected>"},{0},lclstack));
            return false;
        }
        log_main_step=log_main_step+1;
        log("Reading files.",verbose,log_main_step,last);
        string line_readed_source;
        while (getline(file_source_stream,line_readed_source)) {
            content.push_back(line_readed_source);
        }
        string line_readed_output;
        while (getline(file_output_stream,line_readed_output)) {
            output_file_to_check.push_back(line_readed_output);
        }
        log_main_step=log_main_step+1;
        log("Removing comments.",verbose,log_main_step,last);
        for (int i=0;i<content.size();++i) {
            if (content[i]!="") {
                if (content[i].find("§",0)==0) {
                    content[i]="";
                } else {
                    content[i]=split(content[i],"§")[0];
                }
            }
        }
        for (int i=0;i<output_file_to_check.size();++i) {
            if (output_file_to_check[i]!="") {
                if (output_file_to_check[i].find("§",0)==0) {
                    output_file_to_check[i]="";
                } else {
                    output_file_to_check[i]=split(output_file_to_check[i],"§")[0];
                }
            }
        }
        file_source_stream.close();
        return true;
    } else {
        log("Checking if source file exist.",verbose,log_main_step,last);
        if (filesystem::exists(file_source)==false) {
            raise(ErrorContainer("Source file doesn't exist.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        log_main_step=log_main_step+1;
        log("Checking file readability.",verbose,log_main_step,last);
        ifstream file_source_stream(file_source.c_str());
        if (file_source_stream.is_open()==false) {
            raise(ErrorContainer("File can't be open.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        log_main_step=log_main_step+1;
        log("Reading file.",verbose,log_main_step,last);
        string line_readed_source;
        while (getline(file_source_stream,line_readed_source)) {
            content.push_back(line_readed_source);
        }
        log_main_step=log_main_step+1;
        log("Removing comments.",verbose,log_main_step,last);
        for (int i=0;i<content.size();++i) {
            if (content[i]!="") {
                if (content[i].find("§",0)==0) {
                    content[i]="";
                } else {
                    content[i]=split(content[i],"§")[0];
                }
            }
        }
        file_source_stream.close();
        return true;
    }
}
//* Detect symbols and process symbols non-related to cells
bool detect_process_symbol(SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    symbol temp;
    log("Detecting symbols.",verbose,log_main_step,last);
    for (size_t i=0;i<content.size();++i) {
        string line=content[i];
        log("Detecting if line "+to_string(i+1)+" contain symbol :",verbose,log_main_step,last,1,{(int)i+1},{content.size()});
        if (line.rfind("£",0)==0) {
            log("Allocating memory.",verbose,log_main_step,last,2,{(int)i+1,1},{content.size(),3});
            temp.line=i+1;
            temp.content=line;
            auto ele=split(line," ");
            temp.type=ele[0].substr(2,ele[0].size());
            log("Checking symbol.",verbose,log_main_step,last,2,{(int)i+1,2},{content.size(),3});
            if (find(symbollist.begin(),symbollist.end(),temp.type)==symbollist.end()) {
                int linenum=i+1;
                raise(ErrorContainer("vodka.error.symbol.unknow_symbol : Unknow symbol found : "+temp.type,file_source,{line},{linenum},lclstack));
                return false;
            } else {
                log("Saving symbol.",verbose,log_main_step,last,2,{(int)i+1,3},{content.size(),3});
                symbols.push_back(temp);
            }
        } else {
            log("No symbol detected.",verbose,log_main_step,last,2,{(int)i+1,1},{content.size(),1});
        }
    }
    log_main_step=log_main_step+1;
    log("Searching define replacement instruction :",verbose,log_main_step,last);
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking symbol type :",verbose,log_main_step,last,1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODEFINE") {
            auto eles=split(symbols[i].content," ");
            if (eles.size()==3) {
                replacement[eles[1]]=eles[2];
                log("Replacement found. "+eles[1]+" will be replaced with "+eles[2]+".",verbose,log_main_step,last,2,{(int)i+1,1},{symbols.size(),1});
            } else {
                raise(ErrorContainer("vodka.error.vodefine.invalid_syntax : Invalid syntax for define replacement declaration.",file_source,{symbols[i].content},{symbols[i].line},lclstack));
                return false;
            }
        } else {
            log("No VODEFINE symbol detected.",verbose,log_main_step,last,2,{(int)i+1,1},{symbols.size(),1});
        }
    }
    log_main_step=log_main_step+1;
    log("Looking for type symbol :",verbose,log_main_step,last);
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking symbol type.",verbose,log_main_step,last,1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODTYPE") {
            if (typefound==false) {
                typefound=true;
            } else {
                raise(ErrorContainer("vodka.error.vodtype.confusion : A vodka program can only contain one VODTYPE symbol.",file_source,{symbols[i].content},{symbols[i].line},lclstack));
                return -1;
            }
        }
    }
    if (typefound==false) {
        raise(ErrorContainer("vodka.error.vodtype.not_found : Can't find VODTYPE symbol.",file_source,{"<no line affected>"},{0},lclstack));
        return false;
    }
    return true;
}
//* Processing VODTYPE symbol
bool detect_program_type(string output,SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Detecting program type :",verbose,log_main_step,last);
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking if line contain type symbol :",verbose,log_main_step,last,1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODTYPE") {
            log("Checking syntax.",verbose,log_main_step,last,2,{(int)i+1,1},{symbols.size(),3});
            auto ele=split(symbols[i].content," ");
            if (ele.size()!=2) {
                raise(ErrorContainer("vodka.error.vodtype.invalid_syntax : Invalid syntax for program type declaration.",file_source,{symbols[i].content},{symbols[i].line},lclstack));
                return false;
            }
            log("Checking program type.",verbose,log_main_step,last,2,{(int)i+1,2},{symbols.size(),3});
            if (find(typelist.begin(),typelist.end(),ele[1])==typelist.end()) {
                raise(ErrorContainer("vodka.error.vodtype.unknow_type : Unknow type : "+ele[1],file_source,{symbols[i].content},{symbols[i].line},lclstack));
                return false;
            }
            log("Saving program type.",verbose,log_main_step,last,2,{(int)i+1,3},{symbols.size(),3});
            program_type=ele[1];
        } else {
            log("The symbol isn't the type symbol.",verbose,log_main_step,last,2,{(int)i+1,1},{symbols.size(),1});
        }
    }
    if (disable_integrity_hash) {
        final.push_back(encode_to_bin(output)+"type "+program_type);
    } else {
        final.push_back("type "+program_type);
    }
    return true;
}
//* Process symbols related to cells
bool detect_cells(SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Detecting cells :",verbose,log_main_step,last);
    for (size_t i=0;i<symbols.size();++i) {
        if (symbols[i].type=="VODSTART") {
            log("Checking start symbol syntax.",verbose,log_main_step,last,1,{1},{10});
            if (symbols.size()>=i+2 && symbols[i+1].type=="VODEND") {
                log("Allocating memory.",verbose,log_main_step,last,1,{2},{10});
                cell temp;
                log("Detecting cell start and end symbol.",verbose,log_main_step,last,1,{3},{10});
                temp.start=symbols[i];
                temp.end=symbols[i+1];
                log("Detecting cell name.",verbose,log_main_step,last,1,{4},{10});
                auto args=split(symbols[i].content," ");
                if (args.size()>1) {
                    temp.name=args[1];
                } else {
                    raise(ErrorContainer("vodka.error.vodstart.name_not_found : Can't find cell's name.",file_source,{symbols[i].content},{symbols[i].line},lclstack));
                    return false;
                }
                log("Checking if an cell with the same name already exist in the program.",verbose,log_main_step,last,1,{5},{10});
                auto it=find(cellnames.begin(),cellnames.end(),temp.name);
                vector<string> contenterror;
                vector<int> lineerror;
                if (it!=cellnames.end()) {
                    for (size_t y=0;y<cells.size();++y) {
                        if (cells[y].name==temp.name) {
                            contenterror.push_back(cells[y].start.content);
                            lineerror.push_back(cells[y].start.line);
                        }
                    } 
                    contenterror.push_back(temp.start.content);
                    lineerror.push_back(temp.start.line);
                    raise(ErrorContainer("vodka.error.cell.confusion : An existing cell with the same name already exists.",file_source,contenterror,lineerror,lclstack));
                    return false;
                }
                log("Detecting cell argument.",verbose,log_main_step,last,1,{6},{10});
                if (args.size()>2) {
                    temp.args=vector<string>(args.begin()+2,args.end());
                }
                log("Detecting cell output.",verbose,log_main_step,last,1,{7},{10});
                auto outs=split(temp.end.content," ");
                if (outs.size()>1) {
                    temp.outs=vector<string>(outs.begin()+1,outs.end());
                }
                log("Saving cell content.",verbose,log_main_step,last,1,{8},{10});
                int startline=temp.start.line;
                int endline=temp.end.line-2;
                for (int i=startline;i<endline+1;++i) {
                    temp.content.push_back(content[i]);
                }
                log("Saving cell.",verbose,log_main_step,last,1,{9},{10});
                cells.push_back(temp);
                cellnames.push_back(temp.name);
                log("Detecting if cell is the main cell.",verbose,log_main_step,last,1,{10},{10});
                if (temp.name=="main") {
                    maincell=temp;
                    for (auto a:maincell.args) {
                        auto uid=to_string(genuid());
                        vodka::variables::VariableMetadata argsinfo;
                        argsinfo.algo_dependant=true;
                        argsinfo.is_vodka_constant=false;
                        argsinfo.is_kernel_constant=false;
                        argsinfo.in_data_section=false;
                        argsinfo.uuid=to_string(genuid());
                        argsinfo.name=a;
                        vodka::variables::VodargVariable argscont;
                        vodka::variables::VariableContainer varcont;
                        varcont.thing=vodka::variables::VariableDatatype::vodarg;
                        varcont.variable_metadata=argsinfo;
                        varcont.vodarg_element=argscont;
                        mainargsdict[a]=varcont;
                        mainargslist.push_back(a);
                        variableslist.push_back(a);
                        variablesdict[a]=varcont;
                    }
                }
            } else {
                raise(ErrorContainer("vodka.error.cell.vodend_not_found : Can't find corresponding VODEND symbol to the previous VODSTART.",file_source,{symbols[i].content},{symbols[i].line},lclstack));
                return false;
            }
        }
    }
    return true;
}
//* Verify if the program contain a main cell
bool verify_main_cell(SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Verifying if program contain a main cell.",verbose,log_main_step,last);
    if (maincell.name!="main") {
        raise(ErrorContainer("vodka.error.cell.main_not_found : Can't find the main cell.",file_source,{"<no line affected>"},{0},lclstack));
        return false;
    }
    return true;
}
//* Apply pre-treatement on the code
bool code_pre_treatement(bool replace,SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Writing args section.",verbose,log_main_step,last);
    for (auto argm:mainargsdict) {
        final.push_back(argm.second.variable_metadata.uuid);
    }
    if (replace==false) {
        log_main_step=log_main_step+1;
        log("Skipping replacement step because of -r.",verbose,log_main_step,last);
    } else {
        log_main_step=log_main_step+1;
        log("Started define replacing process.",verbose,log_main_step,last);
        for (int i=0;i<maincell.content.size();++i) {
            cout<<replacement.size()<<endl;
            auto temp=split(maincell.content[i]," ");
            for (int y=0;y<temp.size();++y) {
                for (auto a:replacement) {
                    if (temp[y]==a.first) {
                        temp[y]=a.second;
                    }
                }
            }
            string newline;
            for (int y=0;y<temp.size();++y) {
                if (y==0) {
                    newline=temp[y];
                } else {
                    newline=newline+" "+temp[y];
                }
            }
            maincell.content[i]=newline;
        }
        for (auto u:cells) {
            for (int i=0;i<u.content.size();++i) {
                for (auto y:replacement) {
                    replaceall(maincell.content[i],y.first,y.second);
                }
            }
        }
    }
    log_main_step=log_main_step+1;
    log("Started datatypes replacement process.",verbose,log_main_step,last);
    vector<string> directs_data;
    for (int i=0;i<maincell.content.size();++i) {
        vodka::analyser::LineSyntaxChecker localline;
        localline.line_number=maincell.start.line+i+1;
        localline.file=file_source;
        localline.content=maincell.content[i];
        auto direct_declared_data=split(maincell.content[i]," ");
        if (direct_declared_data.size()>2) {
            if (direct_declared_data[0]!="vodka") {
                direct_declared_data.erase(direct_declared_data.begin(),direct_declared_data.begin()+1);
                for (auto arg:direct_declared_data) {
                    if (arg.substr(0,1)=="#" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                        if (vodka::type::vodint::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                            vodka::variables::VodintVariable intele;
                            intele.value=vodka::type::vodint::remove_zero(arg.substr(1,arg.size()-1));
                            vodka::variables::VariableContainer contele;
                            vodka::variables::VariableMetadata varinfo;
                            varinfo.is_vodka_constant=true;
                            varinfo.name=arg;
                            varinfo.in_data_section=true;
                            varinfo.algo_dependant=false;
                            varinfo.is_kernel_constant=true;
                            varinfo.uuid=to_string(vodka::utilities::genuid());
                            contele.variable_metadata=varinfo;
                            contele.vodint_element=intele;
                            contele.thing=vodka::variables::VariableDatatype::vodint;
                            variablesdict[arg]=contele;
                            variableslist.push_back(arg);
                            directs_data.push_back(arg);
                        } else {
                            return false;
                        }
                    } else if (arg.substr(0,1)=="%" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                        if (vodka::type::vodec::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                            vodka::variables::VodecVariable decele;
                            decele.value=vodka::type::vodec::remove_zero(arg.substr(1,arg.size()-1));
                            vodka::variables::VariableContainer contele;
                            vodka::variables::VariableMetadata varinfo;
                            varinfo.algo_dependant=false;
                            varinfo.is_vodka_constant=true;
                            varinfo.in_data_section=true;
                            varinfo.is_kernel_constant=true;
                            varinfo.uuid=to_string(vodka::utilities::genuid());
                            varinfo.name=arg;
                            contele.variable_metadata=varinfo;
                            contele.vodec_element=decele;
                            contele.thing=vodka::variables::VariableDatatype::vodec;
                            variablesdict[arg]=contele;
                            variableslist.push_back(arg);
                            directs_data.push_back(arg);
                        } else {
                            return false;
                        }
                    }
                }
            }
        }
    }
    for (auto cell:cells) {
        for (int i=0;i<cell.content.size();++i) {
            vodka::analyser::LineSyntaxChecker localline;
            localline.line_number=cell.start.line+i+1;
            localline.file=file_source;
            localline.content=cell.content[i];
            auto direct_declared_data=split(cell.content[i]," ");
            if (direct_declared_data.size()>2) {
                if (direct_declared_data[0]!="vodka") {
                    direct_declared_data.erase(direct_declared_data.begin(),direct_declared_data.begin()+1);
                    for (auto arg:direct_declared_data) {
                        if (arg.substr(0,1)=="#" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                            if (vodka::type::vodint::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                                vodka::variables::VodintVariable intele;
                                intele.value=vodka::type::vodint::remove_zero(arg.substr(1,arg.size()-1));
                                vodka::variables::VariableContainer contele;
                                vodka::variables::VariableMetadata varinfo;
                                varinfo.algo_dependant=false;
                                varinfo.is_vodka_constant=true;
                                varinfo.in_data_section=true;
                                varinfo.is_kernel_constant=true;
                                varinfo.uuid=to_string(vodka::utilities::genuid());
                                varinfo.name=arg;
                                contele.variable_metadata=varinfo;
                                contele.vodint_element=intele;
                                contele.thing=vodka::variables::VariableDatatype::vodint;
                                variablesdict[arg]=contele;
                                variableslist.push_back(arg);
                                directs_data.push_back(arg);
                            } else {
                                return false;
                            }
                        } else if (arg.substr(0,1)=="%" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                            if (vodka::type::vodec::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                                vodka::variables::VodecVariable decele;
                                decele.value=vodka::type::vodec::remove_zero(arg.substr(1,arg.size()-1));
                                vodka::variables::VariableContainer contele;
                                vodka::variables::VariableMetadata varinfo;
                                varinfo.algo_dependant=false;
                                varinfo.is_vodka_constant=true;
                                varinfo.in_data_section=true;
                                varinfo.is_kernel_constant=true;
                                varinfo.uuid=to_string(vodka::utilities::genuid());
                                varinfo.name=arg;
                                contele.variable_metadata=varinfo;
                                contele.vodec_element=decele;
                                contele.thing=vodka::variables::VariableDatatype::vodec;
                                variablesdict[arg]=contele;
                                variableslist.push_back(arg);
                                directs_data.push_back(arg);
                            } else {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}
//* Main function
int main (int argc,char* argv[]) {
    auto lclstack=srcstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    string output="";
    int option;
    bool replace=true;
    string mode="compile";
    string element_to_find;
    opterr=0;
    struct option options[] = {
        {"help",no_argument,nullptr,'h'},
        {"find",required_argument,nullptr,'f'},
        {"source-file",required_argument,nullptr,'s'},
        {"output-file",required_argument,nullptr,'o'},
        {"verbose-reduced",no_argument,nullptr,'v'},
        {"verbose-full",no_argument,nullptr,'V'},
        {"debug-lines",no_argument,nullptr,'d'},
        {"json-kernel",no_argument,nullptr,'j'},
        {"json-vodka",no_argument,nullptr,'J'},
        {"disable-replacements",no_argument,nullptr,'r'},
        {"disable-all-warnings",no_argument,nullptr,'w'},
        {"disable-variables-warnings",no_argument,nullptr,0},
        {"disable-integrity-hash",no_argument,nullptr,'H'},
        {"check-mode",no_argument,nullptr,'c'},
        {"show-log-time",no_argument,nullptr,'t'},
        {nullptr,0,nullptr,0}
    };
    //* Args management
    while ((option=getopt_long(argc,argv,"hHjJtrdvVcwf:s:o:",options,nullptr))!=-1) {
        switch (option) {
        case 0:
            if (string(argv[optind-1])=="disable-variables-warnings") {
                var_warning_enabled=false;
            }
            break;
        case 'h':
            cout<<"Vodka v0.4 beta 1 - Vodka Objective Dictionary for Kernel Analyser\nOptions :\n  -h, --help :\n    show this help\n  -f, --find object_to_find :\n    (not working for the moment)\n  -s, --source-file source_file :\n    source file \n  -o, --output-file output_file :\n    output file\n  -v, --verbose-reduced :\n    set verbose mode to reduced\n  -V, --verbose-full :\n    set verbose mode to full\n  -d, --debug-lines :\n    enable debug mode\n  -j, --json-kernel :\n    export output to a json file specified with -o\n  -J, --json-vodka :\n    export .vod structure to a json file specified with -o\n  -r, --disable-replacements :\n    disable define replacement\n  -w, --disable-all-warnings :\n    disable warnings\n  --disable-variables-warnings :\n    disable variables warnings\n  -H, --disable-integrity-hash :\n    disable the integrity hash integration into the output\n  -c, --check-mode :\n    verify the integrity between the inputed file witn -s and the output file specified with -o\n  -t, --show-log-time :\n    show time in every log output, should be used alongside -v or -V"<<endl;
            return 0;
        case 'f':
            mode="find";
            element_to_find=optarg;
            break;
        case 's':
            file_source=optarg;
            break;
        case 'o':
            output=optarg;
            break;
        case 'v':
            verbose="r";
            break;
        case 'V':
            verbose="a";
            break;
        case 'd':
            debug_mode=true;
            break;
        case 'j':
            mode="jsonkernel";
            break;
        case 'J':
            mode="jsonvodka";
            break;
        case 'r':
            replace=false;
            break;
        case 'w':
            var_warning_enabled=false;
            break;
        case 'H':
            disable_integrity_hash=true;
            break;
        case 'c':
            mode="check";
            break;
        case 't':
            show_log_time=true;
            break;
        case '?':
            cout<<"Invalid argument."<<endl;
            return -1;
        default:
            return -1;;
        }
    }
    if (show_log_time) {
        setenv("VODKA_SHOW_LOG_TIME","TRUE",1);
    } else {
        setenv("VODKA_SHOW_LOG_TIME","FALSE",1);
    }
    if (debug_mode) {
        setenv("VODKA_DEBUG_MODE","TRUE",1);
    } else {
        setenv("VODKA_DEBUG_MODE","FALSE",1);
    }
    if (var_warning_enabled) {
        setenv("VODKA_SHOW_VAR_WARNING","TRUE",1);
    } else {
        setenv("VODKA_SHOW_VAR_WARNING","FALSE",1);
    }
    if (disable_integrity_hash) {
        setenv("VODKA_DISABLE_INTEGRITY_HASH","TRUE",1);
    } else {
        setenv("VODKA_DISABLE_INTEGRITY_HASH","FALSE",1);
    }
    setenv("VODKA_VERBOSE_MODE",verbose.c_str(),1);
    bool readed_file=read_file(output,mode,lclstack);
    if (readed_file==false) {
        return -1;
    }
    log_main_step=log_main_step+1;
    bool detected_processed_symbols=detect_process_symbol(lclstack);
    if (detected_processed_symbols==false) {
        return -1;
    }
    log_main_step=log_main_step+1;
    bool detected_program_type=detect_program_type(output,lclstack);
    if (detected_program_type==false) {
        return -1;
    }
    log_main_step=log_main_step+1;
    bool detected_cells=detect_cells(lclstack);
    if (detected_cells==false) {
        return -1;
    }
    log_main_step=log_main_step+1;
    bool verified_main_cell=verify_main_cell(lclstack);
    if (verified_main_cell==false) {
        return -1;
    }
    final.push_back("args:");
    log_main_step=log_main_step+1;
    bool code_pre_traited=code_pre_treatement(replace,lclstack);
    if (code_pre_traited==false) {
        return -1;
    }
    log_main_step=log_main_step+1;
    log("Started analysing main cell :",verbose,log_main_step,last);
    //* Here happen all the magic
    final.push_back("endargs");
    vector<vodka::syscalls::SyscallContainer> instructions_main;
    map<string,vodka::variables::VariableContainer> main_variablesdict=variablesdict;
    vector<string> main_variableslist=variableslist;
    map<string,vector<string>> main_vars_used;
    for (size_t i=0;i<maincell.content.size();++i) {
        log("Analysing line "+to_string(i+1)+" :",verbose,log_main_step,last,1,{(int)i+1},{maincell.content.size()});
        string line=maincell.content[i];
        vodka::analyser::LineSyntaxChecker actual_line;
        actual_line.content=line;
        actual_line.line_number=maincell.start.line+(int)i+1;
        actual_line.file=file_source;
        actual_line.checked=actual_line.check(lclstack);
        if (actual_line.checked==false) {
            return -1;
        }
        if (actual_line.shoulb_be_skip==true) {
            continue;
        }
        vodka::analyser::LineTypeChecker type_analyser;
        type_analyser.line_checked=actual_line;
        type_analyser.checked=type_analyser.line_type_analyse(lclstack);
        if (type_analyser.checked==false) {
            return -1;
        }
        //* Debug line processing
        if (type_analyser.type=="debug_one") {
            debuglog(line,maincell.start.line+(int)i+1,"main",verbose,file_source);
        } else if (type_analyser.type=="debug_two") {
            debuglog(line,maincell.start.line+(int)i+1,"main",verbose,file_source,false);
        } 
        //* Vodka instruction processing
        else if (type_analyser.type=="var") {
            log("Checking vodka declaration syntax.",verbose,log_main_step,last,2,{(int)i+1,1},{maincell.content.size(),6});
            vodka::analyser::VariableDeclarationAnalyser VariableDeclarationAnalyser;
            VariableDeclarationAnalyser.line_checked=type_analyser;
            VariableDeclarationAnalyser.checked=VariableDeclarationAnalyser.parser(lclstack);
            if (VariableDeclarationAnalyser.checked==false) {
                return -1;
            }
            if (VariableDeclarationAnalyser.name.substr(0,1)=="$" || VariableDeclarationAnalyser.name.substr(0,2)=="$$") {
                for (auto a:main_variableslist) {
                    if (a==VariableDeclarationAnalyser.name) {
                        raise(ErrorContainer("vodka.error.variables.constant : Can't modify a constant.",file_source,{line},{maincell.start.line+(int)i+1},lclstack));
                        return -1;
                    }
                }
            }
            log("Checking vodka declaration type and value.",verbose,log_main_step,last,2,{(int)i+1,2},{maincell.content.size(),6});
            VariableDeclarationAnalyser.checked_type_value=VariableDeclarationAnalyser.check_type_value(main_variableslist,lclstack);
            if (VariableDeclarationAnalyser.checked_type_value==false) {
                return -1;
            }
            log("Making metadata about vodka declaration.",verbose,log_main_step,last,2,{(int)i+1,3},{maincell.content.size(),6});
            if (VariableDeclarationAnalyser.datatype=="vodka") {
                VariableDeclarationAnalyser.duplication_source_variable=main_variablesdict[VariableDeclarationAnalyser.value];
            }
            bool info=VariableDeclarationAnalyser.make_info(lclstack);
            if (info==false) {
                return -1;
            }
            log("Pre-treating vodka declaration value.",verbose,log_main_step,last,2,{(int)i+1,4},{maincell.content.size(),6});
            VariableDeclarationAnalyser.pre_treated=VariableDeclarationAnalyser.value_pre_treatement(lclstack);
            if (VariableDeclarationAnalyser.pre_treated==false) {
                return -1;
            }
            log("Compiling vodka declaration into variable container.",verbose,log_main_step,last,2,{(int)i+1,5},{maincell.content.size(),6});
            bool output=VariableDeclarationAnalyser.make_output(lclstack);
            if (output==false) {
                return -1;
            }
            log("Registering vodka declaration.",verbose,log_main_step,last,2,{(int)i+1,6},{maincell.content.size(),6});
            main_variablesdict[VariableDeclarationAnalyser.name]=VariableDeclarationAnalyser.variable_container;
            main_variableslist.push_back(VariableDeclarationAnalyser.name);
            if (!VariableDeclarationAnalyser.is_kernel_constant) {
                instructions_main.push_back(VariableDeclarationAnalyser.syscall_container);
                main_vars_used[VariableDeclarationAnalyser.name]={"f",to_string(actual_line.line_number)};
            }
            if (VariableDeclarationAnalyser.is_kernel_constant) {
                variablesdict[VariableDeclarationAnalyser.name]=VariableDeclarationAnalyser.variable_container;
                variableslist.push_back(VariableDeclarationAnalyser.name);   
            }
        //* Kernel function analysis
        } else if (type_analyser.type=="internal_library") {
            auto args=vodka::analyser::get_arguments(line);
            for (auto arg:args) {
                try {
                    main_vars_used.at(arg)={"t",main_vars_used.at(arg)[1]};
                } catch (const std::out_of_range& e) {
                    //* Error is raised later
                }
            }
            if (type_analyser.library_name=="kernel") {
                vodka::library::FunctionCall fcall;
                fcall.verbose_context=verbose;
                fcall.main_logstep_context=log_main_step;
                fcall.last_logstep_context=last;
                fcall.variableslist_context=main_variableslist;
                fcall.cell_context=maincell;
                fcall.iteration_number_context=i;
                fcall.file_name_context=file_source;
                fcall.variablesdict_context=main_variablesdict;
                fcall.line_checked=type_analyser;
                vodka::library::kernel::CallTreatement engine;
                engine.function_call=fcall;
                engine.checked=engine.call_treatement(lclstack);
                if (engine.checked==false) {
                    return -1;
                } else {
                    instructions_main.insert(instructions_main.end(),engine.syscall_output.begin(),engine.syscall_output.end());
                }
                if (engine.var_flag==true) {
                    main_variableslist=engine.function_call.variableslist_context;
                    main_variablesdict=engine.function_call.variablesdict_context;
                }
            }
        } else if (type_analyser.type=="vodka_instruction") {
            auto args=vodka::analyser::get_arguments(line);
            for (auto arg:args) {
                try {
                    main_vars_used.at(arg)={"t",main_vars_used.at(arg)[1]};
                } catch (const std::out_of_range& e) {
                    //* Error is raised later
                }
            }
            vodka::instructions::InstructionCall icall;
            icall.verbose_context=verbose;
            icall.main_logstep_context=log_main_step;
            icall.last_logstep_context=last;
            icall.variableslist_context=main_variableslist;
            icall.cell_context=maincell;
            icall.iteration_number_context=i;
            icall.file_name_context=file_source;
            icall.variablesdict_context=main_variablesdict;
            icall.line_checked=type_analyser;
            vodka::instructions::CallTreatement engine;
            engine.instruction_call=icall;
            engine.checked=engine.call_treatement(lclstack);
            if (engine.checked==false) {
                return -1;
            } else {
                for (auto a:engine.syscalls_output) {
                    instructions_main.push_back(a);
                }
            }
            if (engine.var_flag==true) {
                main_variablesdict=engine.instruction_call.variablesdict_context;
                main_variableslist=engine.instruction_call.variableslist_context;
            }
        } else {
            raise(ErrorContainer("vodka.error.function.unknow : Unknow function.",file_source,{line},{maincell.start.line+(int)i+1},lclstack));
            return -1;
        }
    }
    //* Indicating unused variables
    for (auto var:main_vars_used) {
        if (var.second[0]=="f") {
            vodka::utilities::var_warning(var.first,main_variablesdict[var.first].thing,"main",var.second[1],var_warning_enabled,verbose);
        }
    }
    //* Writing output file
    log_main_step=log_main_step+1;
    log("Writing variables :",verbose,log_main_step,last);
    final.push_back("data:");
    int a=1;
    for (auto i:variablesdict) {
        if (i.second.thing==vodka::variables::VariableDatatype::vodint) {
            if (i.second.variable_metadata.in_data_section==true) {
                log("Writing "+i.second.variable_metadata.uuid+".",verbose,log_main_step,last,1,{a},{variablesdict.size()});
                if (i.second.variable_metadata.is_kernel_constant==true) {
                    final.push_back(i.second.variable_metadata.uuid+"="+i.second.vodint_element.value);
                }
                a=a+1;
            }
        } else if (i.second.thing==vodka::variables::VariableDatatype::vodec) {
            if (i.second.variable_metadata.in_data_section==true) {
                log("Writing "+i.second.variable_metadata.uuid+".",verbose,log_main_step,last,1,{a},{variablesdict.size()});
                if (i.second.variable_metadata.is_kernel_constant==true) {
                    final.push_back(i.second.variable_metadata.uuid+"="+i.second.vodec_element.value);
                }
                a=a+1;
            }
        }
    }
    final.push_back("enddata");
    log_main_step=log_main_step+1;
    log("Writing main code section :",verbose,log_main_step,last);
    a=1;
    vector<string> kernel_sections={"main"};
    vector<int> kernel_sections_line={(int)final.size()};
    map<string,string> kernel_sections_content;
    map<string,string> vodka_cell_hash;
    final.push_back("main:");
    for (auto i:instructions_main) {
        log("Writing "+vodka::syscalls::syscall_to_string(i.thing)+" instruction.",verbose,log_main_step,last,1,{a},{instructions_main.size()});
        final.push_back(i.syntax());
        kernel_sections_content["main"]=kernel_sections_content["main"]+"\n"+i.syntax();
        a=a+1;
    }
    for (auto cell:cells) {
        string content;
        for (auto line:cell.content) {content=content+"\n"+line;}
        vodka_cell_hash[cell.name]=hash_then_encode(content);
    }
    if (disable_integrity_hash==false) {
        final.push_back(vodka_cell_hash["main"]+"endmain");
        for (int i=0;i<kernel_sections_line.size();++i) {
            final[kernel_sections_line[i]]=hash_then_encode(kernel_sections_content[kernel_sections[i]])+final[kernel_sections_line[i]];
        }
    } else {
        final.push_back("endmain");
    }
    if (mode=="check") {
        log_main_step=log_main_step+1;
        if (output_file_to_check[0].substr(0,3)==u8"\u200B" || output_file_to_check[0].substr(0,3)==u8"\u2063") {
            string binfile=split(output_file_to_check[0],"type")[0];
            if (encode_to_bin(output)==binfile) {
                cout<<"The output file hasn't been compiled with integrity checksum enabled. If you think there are checksum in the output file, remove all the invisibles caracters at the start of the first line and then retry."<<endl;
            } else {
                cout<<"The output file may have been compiled with integrity checksum enabled but someone tried to mask it by adding a wrong no-integrity tag. Remove all the invisibles caracters at the start of the first line and then retry."<<endl;
            }
        }
        log("Retrieving checksum.",verbose,log_main_step,last);
        map<string,string> vodka_checksum;
        map<string,string> checksum_line;
        vector<string> founded_sections;
        for (auto line:output_file_to_check) {
            for (auto cellcheck:cells) {
                if (line.substr(line.size()-cellcheck.name.size(),cellcheck.name.size())==cellcheck.name) {
                    checksum_line[cellcheck.name]=line;
                    founded_sections.push_back(cellcheck.name);
                    break;
                }
            }
        }
        if (find(founded_sections.begin(),founded_sections.end(),"main")==founded_sections.end()) {
            cout<<"Can't found main section.";
            return -1;
        } else if (founded_sections.size()==0) {
            cout<<"Doesn't found any section.";
            return -1;
        }
        log_main_step=log_main_step+1;
        log("Extracting checksum.",verbose,log_main_step,last);
        for (auto sect:founded_sections) {
            auto linet=checksum_line.at(sect);
            string checksum=linet.substr(0,1536);
            vodka_checksum[sect]=checksum;
        }
        log_main_step=log_main_step+1;
        log("Checking checksum.",verbose,log_main_step,last);
        cout<<endl<<"Check result:"<<endl;
        for (auto checksum:vodka_checksum) {
            cout<<" - "+checksum.first+": ";
            if (checksum.second==vodka_cell_hash.at(checksum.first)) {
                cout<<"pass"<<endl;
            } else {
                cout<<"failed"<<endl;
            }
        }
    } else if (mode=="compile") {
        log_main_step=log_main_step+1;
        log("Opening output file.",verbose,log_main_step,last);
        ofstream outputfile(output);
        log_main_step=log_main_step+1;
        log("Writing in output file :",verbose,log_main_step,last);
        if (outputfile.is_open()) {
            a=1;
            for (const auto& line:final) {
                log("Writing line number "+to_string(a)+".",verbose,log_main_step,last,1,{a},{final.size()});
                outputfile<<line<<"\n";
                a=a+1;
            }
            log_main_step=log_main_step+1;
            log("Closing output file.",verbose,log_main_step,last);
            outputfile.close();
            if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file_source+" to "+output<<endl;}
        }
    } else if (mode=="jsonkernel") {
        log_main_step=log_main_step+1;
        log("Opening output file.",verbose,log_main_step,last);
        ofstream outputfile(output);
        log_main_step=log_main_step+1;
        log("Converting kernel code to json format :",verbose,log_main_step,last);
        a=1;
        auto now=chrono::system_clock::now();
        time_t now_c=chrono::system_clock::to_time_t(now);
        tm utc=*std::gmtime(&now_c);
        stringstream ss;
        bool endargs=false;
        ss<<put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
        json_ints["metadata"]={{"type","metadata"},{"vodka_version",vodka::LibraryVersion},{"json_version","4"},{"source_file",file_source},{"timestamp",ss.str()},{"json_type","kernel"}};
        vector<string> kernel_symbol={"args:","endargs","data:","enddata"};
        for (auto b:kernel_sections) {
            kernel_symbol.push_back(b+":");
            kernel_symbol.push_back("end"+b);
        }
        for (const string& line:final) {
            string actualcell="main";
            log("Converting line "+to_string(a)+".",verbose,log_main_step,last,1,{a},{final.size()});
            if (find(kernel_symbol.begin(),kernel_symbol.end(),line)==kernel_symbol.end() && line.substr(0,4)!="type") {
                if (std::isalpha(line[0]) && std::isupper(line[0])) {
                    vodka::json::kernel::JsonContainer jsonth;
                    jsonth.type="system_call";
                    jsonth.instruction_name=split(line," ")[0];
                    auto eles=split(line," ");
                    jsonth.args=vector<string>(eles.begin()+1,eles.end());
                    json_ints[to_string(a)+"@"+actualcell+":"+to_string(genuid())]=jsonth.syntax();
                    a=a+1;
                } else {
                    vodka::json::kernel::JsonContainer jsonth;
                    if (endargs) {jsonth.type="constant";} else {jsonth.type="argument";};
                    jsonth.instruction_name=split(line,"=")[0];
                    auto eles=split(line,"=");
                    string otherside;
                    for (int i=1;i<eles.size();++i) {
                        if (i!=1) {
                            otherside=otherside+"="+eles[i];
                        } else {
                            otherside=eles[i];
                        }
                    }
                    jsonth.args={otherside};
                    json_ints[to_string(a)+":"+to_string(genuid())]=jsonth.syntax();
                }
            } else if (line=="endargs") {
                endargs=true;
            }
            a=a+1;
        }
        log_main_step=log_main_step+1;
        log("Writing json file.",verbose,log_main_step,last);
        nlohmann::json j=json_ints;
        outputfile<<j.dump();
        outputfile.close();
        if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file_source+" to "+output<<endl;}
    } else if (mode=="jsonvodka") {
        log_main_step=log_main_step+1;
        log("Opening output file.",verbose,log_main_step,last);
        ofstream outputfile(output);
        log_main_step=log_main_step+1;
        log("Converting vodka code to json format :",verbose,log_main_step,last);
        auto now=chrono::system_clock::now();
        time_t now_c=chrono::system_clock::to_time_t(now);
        tm utc=*std::gmtime(&now_c);
        stringstream ss;
        ss<<put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
        json_ints_v["metadata"]={{"metadata",{{"type","metadata"},{"vodka_version",vodka::LibraryVersion},{"json_version","4"},{"source_file",file_source},{"timestamp",ss.str()},{"json_type","vodka"}}}};
        log("Converting symbols : ",verbose,log_main_step,last,1,{1},{2});
        json_ints_v["symbols"]={};
        bool cellstart=false;
        vodka::json::vodka::VodkaCell actualcell;
        vector<vodka::json::vodka::VodkaCell> idencell;
        int cellcount=0;
        for (int i=0;i<symbols.size();++i) {
            log("Converting \""+symbols[i].content+"\".",verbose,log_main_step,last,2,{1,i+1},{2,symbols.size()});
            vodka::json::vodka::VodkaSymbol symb;
            symb.type=symbols[i].type;
            auto args=split(symbols[i].content," ");
            symb.args=vector<string>(args.begin()+1,args.end());
            symb.uid=to_string(genuid());
            if (symb.type=="VODSTART" && cellstart==false) {
                cellcount=cellcount+1;
                vodka::json::vodka::VodkaCell cell;
                cell.name=symb.args[0];
                cell.uid=to_string(cellcount)+":"+to_string(genuid());
                cell.start=symb;
                cellstart=true;
                actualcell=cell;
            }
            if (symb.type=="VODEND" && cellstart==true) {
                actualcell.end=symb;
                actualcell.end.uid=symb.uid;
                cellstart=false;
                idencell.insert(idencell.begin(),actualcell);
            }
            json_ints_v["symbols"][to_string(i+1)+":"+to_string(genuid())]=symb.syntax();
        }
        log("Converting cells : ",verbose,log_main_step,last,1,{2},{2});
        vector<string> vodkaints=vodka::VodkaInstructions;
        for (int i=0;i<idencell.size();++i) {
            log("Converting cell "+cells[i].name+".",verbose,log_main_step,last,2,{2,i+1},{2,cells.size()});
            auto cellcontent=cells[i].content;
            for (auto a:cellcontent) {
                if (a.substr(0,5)=="vodka") {
                    vodka::json::vodka::VodkaVariableDeclaration vardec;
                    vardec.uid=to_string(genuid());
                    auto eles=split(a,"=");
                    auto namepart=eles[0];
                    auto valuepart=eles[1];
                    vardec.variable_name=split(namepart," ")[1];
                    vardec.variable_datatype=split(valuepart," ")[0];
                    auto vvaluepart=split(valuepart," ");
                    vector<string> valuev=vector<string>(vvaluepart.begin()+1,vvaluepart.end());
                    string value="";
                    for (int b=0;b<valuev.size();++b) {
                        if (b==0) {
                            value=valuev[b];
                        } else {
                            value=value+valuev[b];
                        }
                    }
                    vardec.variable_value=value;
                    vodka::json::vodka::VodkaLine linecont;
                    linecont.thing="variable_declaration";
                    linecont.variable_declaration_element=vardec;
                    idencell[i].lines_content.push_back(linecont);
                } else if (a.substr(0,6)=="kernel") {
                    vodka::json::vodka::VodkaInstruction instr;
                    instr.library="kernel";
                    instr.uid=to_string(genuid());
                    instr.name=split(a," ")[0];
                    instr.source="<builtin>";
                    auto eles=split(a," ");
                    instr.args=vector<string>(eles.begin()+1,eles.end());
                    vodka::json::vodka::VodkaLine linecont;
                    linecont.thing="instruction";
                    linecont.instruction_element=instr;
                    idencell[i].lines_content.push_back(linecont);
                } else if (find(vodkaints.begin(),vodkaints.end(),split(a," ")[0])!=vodkaints.end()) {
                    vodka::json::vodka::VodkaInstruction instr;
                    instr.library="<no_library>";
                    instr.uid=to_string(genuid());
                    instr.name=split(a," ")[0];
                    instr.source="<builtin>";
                    auto eles=split(a," ");
                    instr.args=vector<string>(eles.begin()+1,eles.end());
                    vodka::json::vodka::VodkaLine linecont;
                    linecont.thing="instruction";
                    linecont.instruction_element=instr;
                    idencell[i].lines_content.push_back(linecont);
                }
            }
            json_ints_v[idencell[i].uid]=idencell[i].syntax();
        }
        log_main_step=log_main_step+1;
        log("Writing json file.",verbose,log_main_step,last);
        nlohmann::json j=json_ints_v;
        outputfile<<j.dump();
        outputfile.close();
        if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file_source+" to "+output<<endl;}
    }
    unsetenv("VODKA_SHOW_LOG_TIME");
    unsetenv("VODKA_DEBUG_MODE");
    return 0;
}