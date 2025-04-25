#ifndef VODKA_LIB_H
#define VODKA_LIB_H
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
#include <iostream>
using namespace std;
//* Vodka standard utilities
//* For documentation, please refer to vodka-lib-usage.md
namespace vodka {
    const string version="0.4 beta 1";
    const string json_version="4";
    //* Every library that has a reserved name inside the transcoder
    const vector<string> internal_library={"kernel"};
    //* Every functions for every internal library
    const map<string,vector<string>> internal_library_functions={{"kernel",{"print","add","assign","free","invert","back","duplicate","abs","divmod","toint","todec","divide","mulint","muldec"}}};
    //* Every internal type
    const vector<string> internal_type={"vodint","vodec","vodarg","vodka"};
    //* Every syscall
    const vector<string> internal_syscall={"PRINT","ADD","ASSIGN","FREE","INVERT","DUPLICATE","ABS","DIVMOD","TOINT","TODEC","MULINT","MULDEC","DIVIDE"};
    //* Every vodka codebase instructions
    const vector<string> vodka_instruction={"multiply"};
    //* Every conversions possible using syscalls
    const map<string,vector<string>> conversion_syscall={{"TOINT",{"vodec","vodarg"}},{"TODEC",{"vodint","vodarg"}}};
    //* Errors handling
    namespace errors {
        //* Contain the call stack of the error
        class sources_stack {
            public:
                vector<string> source;
                vector<string> file;
                //* Add a element to the source stack
                void add(string src,string fil) {source.push_back(src);file.push_back(fil);}
        };
        //* Basic class for errors
        class error_container {
            public:
                string error;
                string file="";
                vector<string> lines_content={};
                vector<int> lines={};
                sources_stack source;
                bool class_mode=false;
                error_container(string error,string file,vector<string> lines_content,vector<int> lines,sources_stack source,bool cm=false) {
                    this->error=error;
                    this->file=file;
                    this->lines=lines;
                    this->lines_content=lines_content;
                    this->source=source;
                    this->class_mode=cm;
                }
        };
        //* Error raising function
        void raise(error_container element);
    }
    //* Syscall utilities (instruction starting with "kernel.<something>")
    namespace syscalls {
        //* Enum class for representating each syscall
        enum class list_syscall {
            PRINT,
            ADD,
            ASSIGN,
            FREE,
            INVERT,
            DUPLICATE,
            ABS,
            DIVMOD,
            TOINT,
            TODEC,
            MULDEC,
            MULINT,
            DIVIDE
        };
        string syscall_to_string(list_syscall syscall);
        //* Each syscall has his own class without heritating from the base one
        class PRINT {
            public:
                vector<string> argument_uid;
                string name="PRINT";
        };
        class ADD {
            public:
                vector<string> argument_uid;
                string output_uid;
                string name="ADD";
        };
        class ASSIGN {
            public:
                string value;
                string output_uid;
                string name="ASSIGN";
        };
        class FREE {
            public:
                vector<string> argument_uid;
                string name="FREE";
        };
        class INVERT {
            public:
                string uid;
                string name="INVERT";
        };
        class DUPLICATE {
            public:
                string source_uid;
                string output_uid;
                string name="DUPLICATE";
        };
        class ABS {
            public:
                string uid;
                string name="ABS";
        };
        class DIVMOD {
            public:
                string quotient_uid;
                string rest_uid;
                string divisor_uid;
                string dividend_uid;
                string name="DIVMOD";
        };
        class TOINT {
            public:
                string uid_source;
                string uid_output;
                string name="TOINT";
        };
        class TODEC {
            public:
                string uid_source;
                string uid_output;
                string name="TODEC";
        };
        class MULDEC {
            public:
                string output_uid;
                string first_uid;
                string second_uid;
                string precision_uid;
                string name="MULDEC";
        };
        class MULINT {
            public:
                string output_uid;
                string first_uid;
                string second_uid;
                string name="MULINT";
        };
        class DIVIDE {
            public:
                string output_uid;
                string first_uid;
                string second_uid;
                string precision_uid;
                string name="DIVIDE";
        };
        //* For registering a syscall, it need to be encapsuled into a syscall_container object
        class syscall_container {
            public:
                //* Type of syscall must be put into thing variable
                list_syscall thing;
                PRINT printele;
                ADD addele;
                ASSIGN assignele;
                FREE freeele;
                INVERT invertele;
                DUPLICATE duplicateele;
                ABS absele;
                DIVMOD divmodele;
                TOINT tointele;
                TODEC todecele;
                MULINT mulintele;
                MULDEC muldecele;
                DIVIDE divideele;
            //* Function for getting the syntax of the syscall
            string syntax();
        };
    }
    //* Variables utilities
    namespace variables {
        //* Base class for datatypes
        class typess {
            public:
                string typenames;
        };
        //* Base class for variables metadatas (excluding value)
        class variable {
            public:
                string varname;
                string uuid;
                bool consts;
                bool write=true;
                bool define=false;
                bool algo_dependant;
        };
        //* Vodint type class : optimize for vodint internal structure
        class vodint {
            public:
                string value;
                variable varinfo;
                typess typeinfo;
            vodint() {
                typeinfo.typenames="vodint";
            }
        };
        //* Vodec type class : optimize for vodec internal structure
        class vodec {
            public:
                string value;
                variable varinfo;
                typess typeinfo;
            vodec() {
                typeinfo.typenames="vodec";
            }
        };
        //* Vodarg type class : optimize for vodarg internal structure
        class vodarg {
            public:
                variable varinfo;
                typess typeinfo;
            vodarg() {
                typeinfo.typenames="vodarg";
            }
        };
        //* For registering a variable, it need to be encapsuled into a element object
        class element {
            public:
                //* Type of variable must be put into thing variable
                string thing="";
                vodint intele;
                vodec decele;
                vodarg argele;
        };
    }
    //* Vodka analyser
    namespace analyser {
        using namespace vodka::errors;
        //* Return the list of variables passed in argument
        vector<string> get_arguments(string line);
        //* Base class to check if a line is conform to vodka syntax
        class line {
            public:
                string content;
                string file;
                int line;
                bool checked=false;
                bool skip=false;
            //* Check if the line is conform to vodka syntax (doesn't check the line argument)
            bool check(sources_stack lclstack={});
        };
        //* Class to direct the analyse to a specialised analyser
        class type_analyser {
            public:
                line line_analyse;
                bool checked=false;
                string type;
                string library_name;
                string instruction_name;
            //* Analyse the type of line (variable declaration, vodka instruction, library instruction, debug line)
            bool line_type_analyse(sources_stack lclstack={});
        };
        //* Class to parse a variable declaration
        class var_dec_analyser {
            public:
                type_analyser line_analyse;
                bool checked=false;
                bool checked_type_value=false;
                bool pre_treated=false;
                string name;
                string datatype;
                string value;
                bool is_kernel_const=false;
                bool is_vodka_const=false;
                vodka::variables::variable var;
                vodka::variables::element var_object;
                vodka::variables::element source_duplication;
                vodka::syscalls::syscall_container vodka_object;
            //* These functions should be used in this order
            //* Parse the variable declaration (name, datatype, value, constant)
            bool var_dec_analyse(sources_stack lclstack={});
            //* Check the type and value of the variable (use vodka::type::<concerned type>::check_value(), if datatype is vodka, please include a list of already declared variables inside the context argument) 
            bool check_type_value(vector<string> context={},sources_stack lclstack={});
            //* Make the corresponding vodka::variables::variable (please specifiy the original variable if datatype is vodka)
            bool make_info(sources_stack lclstack={});
            //* Make a pre-treatement of the value to store
            bool pre_treatement(sources_stack lclstack={});
            //* Output the variable under a vodka::variable::variable_container object (please specifiy the original variable if datatype is vodka)
            bool output(sources_stack lclstack={});
        };
    }
    //* Type utilities
    namespace type {
        using namespace vodka::errors;
        //* Vodint utilities (see vodka-lib.cpp for more informations)
        namespace vodint {
            bool check_value(string value,vodka::analyser::line context,sources_stack lclstack={});
            string remove_zero(string value,sources_stack lclstack={});
        }
        //* Vodec utilities (see vodka-lib.cpp for more informations)
        namespace vodec {
            bool check_value(string value,vodka::analyser::line context,sources_stack lclstack={});
            string remove_zero(string value,sources_stack lclstack={});
        }
    }
    //* Json utilities
    namespace json {
        //* Kernel instructions json functions
        namespace kernel {
            //* Json container for kernel instructions
            class json_container {
                public:
                    string type;
                    string intname;
                    vector<string> args;
                map<string,string> syntax();
            };
        };
        //* Vodka instructions json functions
        namespace vodka {
            //* Json class for vodka instructions
            class instruction {
                public:
                    string name;
                    string source="<builtin>";
                    string library;
                    string uid;
                    vector<string> args;
                map<string,string> syntax();
            };
            //* Json class for vodka symbols
            class symbol {
                public:
                    string type;
                    string uid;
                    vector<string> args;
                map<string,string> syntax();
            };
            //* Json class for vodka variables declaration
            class var_declaration {
                public:
                    string name;
                    string type;
                    string decvalue;
                    string uid;
                map<string,string> syntax();
            };
            //* Json class for vodka lines which contain variables declaration or instructions
            class line_container {
                public:
                    string thing;
                    var_declaration varele;
                    instruction intele;
                map<string,string> syntax();
            };
            //* Json class for vodka cells
            class cell {
                public:
                    string name;
                    string uid;
                    symbol start;
                    symbol end;
                    vector<line_container> lines;
                map<string,map<string,string>> syntax();
            };
        };
    }
    //* General utilities
    namespace utilities {
        //* Vodka structure
        struct symbol {
            int line;
            string content;
            string type;
        };
        struct cellule {
            vector<string> content;
            string name;
            vector<string> args;
            vector<string> outs;
            symbol start;
            symbol end;
        };
        struct import {
            string file;
            string type;
            string importas;
            vector<string> content;
        };
        //* UUID generator
        boost::uuids::uuid genuid();
        //* Logs functions
        void log(string text,string verbose,int x,string last,int sublevel=0,vector<int> substep={},vector<unsigned long> subtotal={});
        void debuglog(string text,int line,string cell,bool debugmode,string verbose,string file,bool debug_info=true);
        void var_warning(string namevar,string typevar,string namecell,string line,bool var_warning_enabled,string verbose);
    }
    //* Internal library
    namespace library {
        //* Class for a line that call a function from an internal library
        class function_call {
            public:
                vodka::analyser::type_analyser type_analyser;
                vector<string> variableslist_context;
                vodka::utilities::cellule cell_context;
                int iteration_number_context;
                string file_name_context;
                string verbose_context;
                int main_logstep_context;
                string last_logstep_context;
                map<string,vodka::variables::element> variablesdict_context;
        };
        //* Kernel internal library
        namespace kernel {
            using namespace vodka::errors;
            //* Main class for parsing line that call kernel internal library
            class treatement {
                public:
                    vodka::library::function_call call;
                    vector<vodka::syscalls::syscall_container> syscall_output;
                    bool checked=false;
                    bool var_flag=false;
                    //* Main function for parsing kernel internal library
                    bool kernel_treatement(sources_stack lclstack={});
                private:
                    //* Every datatypes supported by each function of kernel internal library
                    const map<string,vector<string>> supported_type={{"add",{"vodint","vodec"}},{"abs",{"vodint","vodec"}}};
                    string line;
                    //* Private functions for analysing each instructions
                    bool print_int(sources_stack lclstack={});
                    bool add_int(sources_stack lclstack={});
                    bool invert_int(sources_stack lclstack={});
                    bool free_int(sources_stack lclstack={});
                    bool abs_int(sources_stack lclstack={});
                    bool divmod_int(sources_stack lclstack={});
                    bool toint_int(sources_stack lclstack={});
                    bool todec_int(sources_stack lclstack={});
                    bool divide_int(sources_stack lclstack={});
                    bool mulint_int(sources_stack lclstack={});
                    bool muldec_int(sources_stack lclstack={});
            };
        }
    }
    //* Internal instructions
    namespace instructions {
        using namespace vodka::errors;
        //* Class for a line that call a vodka instruction
        class instruction_call {
            public:
                vodka::analyser::type_analyser type_analyser;
                vector<string> variableslist_context;
                vodka::utilities::cellule cell_context;
                int iteration_number_context;
                string file_name_context;
                string verbose_context;
                int main_logstep_context;
                string last_logstep_context;
                map<string,vodka::variables::element> variablesdict_context;
        };
        class instruction_treatement {
            public:
                vodka::instructions::instruction_call call;
                vector<vodka::syscalls::syscall_container> syscalls_output;
                bool checked=false;
                bool var_flag=false;
                //* Main function for parsing vodka instruction
                bool treatement(sources_stack lclstack={});
            private:
                string line;
                const map<string,vector<string>> supported_type={{"multiply",{"vodint","vodec"}}};
                //* Private functions for analysing each instructions
                bool multiply(sources_stack lclstack={});
        };
    }
}
#endif