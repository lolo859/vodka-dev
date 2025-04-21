# vodka-lib usage

## Introduction

vodka-lib is the static internal C++ library that powers the Vodka transcoder and its ecosystem. It provides the full logic and toolset used to analyze, transform, and validate Vodka code before it's converted into Kernel code. While not directly exposed to end users, it's the backbone of the entire language pipeline. This documentation only covers currently implemented components (including those in beta) and does not describe future or theoretical features.

## Index

- [Main structure](#main-structure)
- [Constants datas](#constants-datas)
  - [vodka::version](#stdstring-vodkaversion)
  - [vodka::json_version](#stdstring-vodkajson_version)
  - [vodka::internal_library](#stdvectorstring-vodkainternal_library)
  - [vodka::internal_library_functions](#mapstringvectorstring-vodkainternal_library_functions)
  - [vodka::internal_type](#vectorstring-vodkainternal_type)
  - [vodka::internal_syscall](#stdvectorstdstring-vodkainternal_syscall)
  - [vodka::vodka_instruction](#stdvectorstdstring-vodkavodka_instruction)
  - [vodka::conversion_syscall](#stdmapstdstring-stdvectorstdstring-vodkaconversion_syscall)
- [vodka::errors](#vodkaerrors)
  - [class vodka::errors::sources_stack](#class-vodkaerrorssources_stack)
  - [class vodka::errors::error_container](#class-vodkaerrorserror_container)
  - [void vodka::errors::raise()](#void-vodkaerrorsraisevodkaerrorserror_container-element)
- [vodka::syscalls](#vodkasyscalls)
  - [class vodka::syscalls::syscall](#class-vodkasyscallssyscall)
  - [class vodka::syscalls::PRINT](#class-vodkasyscallsprint)
  - [class vodka::syscalls::ADD](#class-vodkasyscallsadd)
  - [class vodka::syscalls::ASSIGN](#class-vodkasyscallsassign)
  - [class vodka::syscalls::FREE](#class-vodkasyscallsfree)
  - [class vodka::syscalls::INVERT](#class-vodkasyscallsinvert)
  - [class vodka::syscalls::DUPLICATE](#class-vodkasyscallsduplicate)
  - [class vodka::syscalls::ABS](#class-vodkasyscallsabs)
  - [class vodka::syscalls::DIVMOD](#class-vodkasyscallsdivmod)
  - [class vodka::syscalls::TOINT](#class-vodkasyscallstoint)
  - [class vodka::syscalls::TODEC](#class-vodkasyscallstodec)
  - [class vodka::syscalls::MULINT](#class-vodkasyscallsmulint)
  - [class vodka::syscalls::MULDEC](#class-vodkasyscallsmuldec)
  - [class vodka::syscalls::DIVIDE](#class-vodkasyscallsdivide)
  - [class vodka::syscalls::syscall_container](#class-vodkasyscallssyscall_container)
- [vodka::variables](#vodkavariables)
  - [class vodka::variables::typess](#class-vodkavariablestypess)
  - [class vodka::variables::variable](#class-vodkavariablesvariable)
  - [class vodka::variables::vodint](#class-vodkavariablesvodint)
  - [class vodka::variables::vodec](#class-vodkavariablesvodec)
  - [class vodka::variables::vodarg](#class-vodkavariablesvodarg)
  - [class vodka::variables::element](#class-vodkavariableselement)
- [vodka::analyser](#vodkaanalyser)
  - [std::vector\<std::string> get_arguments()](#stdvectorstdstring-get_argumentsstdstring-line)
  - [class vodka::analyser::line](#class-vodkaanalyserline)
  - [class vodka::analyser::type_analyser](#class-vodkaanalysertype_analyser)
  - [class vodka::analyser::var_dec_analyser](#class-vodkaanalyservar_dec_analyser)
- [vodka::type](#vodkatype)
  - [vodka::type::vodint](#vodkatypevodint)
    - [bool vodka::type::vodint::check_value()](#bool-vodkatypevodintcheck_valuestdstring-value-vodkaanalyserline-context-vodkaerrorssources_stack-lclstack)
    - [std::string vodka::type::vodint::remove_zero()](#stdstring-vodkatypevodintremove_zerostdstring-value-vodkaerrorssources_stack-lclstack)
  - [vodka::type::vodec](#vodkatypevodec)
    - [bool vodka::type::vodec::check_value()](#bool-vodkatypevodeccheck_valuestdstring-value-vodkaanalyserline-context-vodkaerrorssources_stack-lclstack)
    - [std::string vodka::type::vodec::remove_zero()](#stdstring-vodkatypevodecremove_zerostdstring-value-vodkaerrorssources_stack-lclstack)

## Main structure

vodka-lib is divided into several namespaces for ease of coding :
- `vodka::errors` : everything related to error output, used both by the Vodka transcoder and vodka-lib
- `vodka::syscalls` : everything related to kernel code instructions and how to create their syntax, used both by the Vodka transcoder and vodka-lib
- `vodka::variables` : everything related to variables and types storing in the Vodka language, used both by the Vodka transcoder and vodka-lib
- `vodka::analyser` : everything related to the first step of transcoding a line (syntax check and line categorisation) as well as parsing a variable declaration, used both by the Vodka transcoder and vodka-lib
- `vodka::type` : everything related to operation on each datatype, used only in vodka-lib
- `vodka::json` : everything related to json export, used only inside by the Vodka transcoder
- `vodka::utilities` : general utilities used for vodka language structure and logs, used both by the Vodka transcoder and vodka-lib
- `vodka::library` : everything related to vodka code analysis for internal libraries function calls, used only inside by the Vodka transcoder
- `vodka::instructions` : everything related to vodka code analysis for internal instructions calls, used only inside by the Vodka transcoder

## Constants datas

The header file provide some constants variables containing useful information to use vodka-lib and have a better understanding of Vodka codebase. 

### `std::string vodka::version`

The actual version of vodka-lib, also indicate with which version of the Vodka transcoder it should be use.

### `std::string vodka::json_version`

The actual version of the json output format.

### `std::vector<std::string> vodka::internal_library`

The list of internal library for this version of vodka-lib.

### `map<string,vector<string>> vodka::internal_library_functions`

The map that indicate what functions each internal library has.

### `vector<string> vodka::internal_type`

The list of native type that can appear in a variable declaration.

### `std::vector<std::string> vodka::internal_syscall`

The list of all the syscalls implemented for this version of vodka-lib.

### `std::vector<std::string> vodka::vodka_instruction`

The list of all the vodka instructions for this version of vodka-lib.

### `std::map<std::string, std::vector<std::string>> vodka::conversion_syscall`

The map that indicate which vodka datatype can accept each conversion syscall

## `vodka::errors`

That namespace is responsible for errors output.

### `class vodka::errors::sources_stack`

That class is responsible for storing the call stack of an error. It should be declarated at the root of your project and duplicated each time the object is transmited to a function that require it. You can use it in your own functions but it's mandatory for most functions in vodka-lib.

**No attributes need to be set after declaration.**

**Methods:**
- `void add(std::string src, std::string fil)` : add an element to the source stack, `src` should be `__PRETTY_FUNCTION__` and `fil` should be `__FIL__`

---

### `class vodka::errors::error_container`

That class contain all the information necessary to raise an error.

**Attributes that need to be set like a function:**
- `std::string error` : the actual error
- `std::string file` : the .vod file from which the error come. Leave empty (by default) to not display any file
- `std::vector<std::string> lines_content` : the content of each lines that need to be displayed. Leave empty (by default) to not display any line
- `std::vector<int> lines` : the line(s) that caused the error. Leave empty (by default) to not display any line. **It should have the same lenght as lines_content**
- `vodka::errors::sources_stack source` : the source stack
- `bool class_mode` : specifie if the error came from a class method or a function

**This class has no methods.**

---

### `void vodka::errors::raise(vodka::errors::error_container element)`

That function format the error to the Vodka error format and output it.

**Required arguments:**
- `vodka::errors::error_container element` : the error container that contain all the information about the error

## `vodka::syscalls`

That namespace is responsible for syscalls syntax generation.

---

### `class vodka::syscalls::syscall`

That class store the basic information about a syscall.

**Attributes that shouldn't be set after declaration:**
- `std::string name` : the name of the syscall. This is the name that will be written in the output file
- `bool support_multiple_argument` : a indication if the syscall need or support multiple argument

**This class has no methods.**

---

### `class vodka::syscalls::PRINT`

This is the class for the `PRINT` syscall.

**Attributes that must be set after declaration:**
- `std::vector<std::string> argument_uid` : all the variables UID that should be printed
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `PRINT` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::ADD`

This is the class for the `ADD` syscall.

**Attributes that must be set after declaration:**
- `std::vector<std::string> argument_uid` : all the variables UID that should be added together
- `std::string output_uid` : the variable UID for the output of the addition
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `ADD` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::ASSIGN`

This is the class for the `ASSIGN` syscall.

**Attributes that must be set after declaration:**
- `std::string value` : the value to put inside the variable
- `std::string output_uid` : the variable UID for the output of the assignation
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `ASSIGN` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::FREE`

This is the class for the `FREE` syscall.

**Attributes that must be set after declaration:**
- `std::vector<std::string> argument_uid` : all the variables UID to erase
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `FREE` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::INVERT`

This is the class for the `INVERT` syscall.

**Attributes that must be set after declaration:**
- `std::string uid` : the variable UID that should have his sign reversed
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `INVERT` and `info.support_multiple_argument` to `false`

---

### `class vodka::syscalls::DUPLICATE`

This is the class for the `DUPLICATE` syscall.

**Attributes that must be set after declaration:**
- `std::string source_uid` : the UID of the source variable for the duplication
- `std::string output_uid` : the UID of the output variable for the duplication
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `DUPLICATE` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::ABS`

This is the class for the `ABS` syscall.

**Attributes that must be set after declaration:**
- `std::string uid` : the UID of the variable that should be turn into his absolute value
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `ABS` and `info.support_multiple_argument` to `false`

---

### `class vodka::syscalls::DIVMOD`

This is the class for the `DIVMOD` syscall.

**Attributes that must be set after declaration:**
- `std::string quotient_uid` : the UID of the variable that should receive the quotient of the euclidian division
- `std::string rest_uid` : the UID of the variable that should receive the rest of the euclidian division
- `std::string divisor_uid` : the UID of the variable that contain the divisor
- `std::string dividend_uid` : the UID of the variable that contain the dividend
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `DIVMOD` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::TOINT`

This is the class for the `TOINT` syscall.

**Attributes that must be set after declaration:**
- `std::string uid_source` : the UID of the variable that should be converted into integer number syntax
- `std::string uid_output` : the UID of the variable that should receive the converted value
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `TOINT` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::TODEC`

This is the class for the `TODEC` syscall.

**Attributes that must be set after declaration:**
- `std::string uid_source` : the UID of the variable that should be converted into decimal number syntax
- `std::string uid_output` : the UID of the variable that should receive the converted value
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `TODEC` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::MULDEC`

This is the class for the `MULDEC` syscall.

**Attributes that must be set after declaration:**
- `std::string output_uid` : the UID of the variable that should receive the output of the multiplication
- `std::string first_uid` : the UID of the variable that contain the first term of the multiplication
- `std::string second_uid` : the UID of the variable that contain the second term of the multiplication
- `std::string precision_uid` : the UID of the variable that contain the precision of the multiplication
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `MULDEC` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::MULINT`

This is the class for the `MULINT` syscall.

**Attributes that must be set after declaration:**
- `std::string output_uid` : the UID of the variable that should receive the output of the multiplication
- `std::string first_uid` : the UID of the variable that contain the first term of the multiplication
- `std::string second_uid` : the UID of the variable that contain the second term of the multiplication
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `MULINT` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::DIVIDE`

This is the class for the `DIVIDE` syscall.

**Attributes that must be set after declaration:**
- `std::string output_uid` : the UID of the variable that should receive the output of the division
- `std::string first_uid` : the UID of the variable that contain the first term of the division
- `std::string second_uid` : the UID of the variable that contain the second term of the division
- `std::string precision_uid` : the UID of the variable that contain the precision of the division
- `vodka::syscalls::syscall info` : the information about the syscall

**Class constructor:** automatically set `info.name` to `DIVIDE` and `info.support_multiple_argument` to `true`

---

### `class vodka::syscalls::syscall_container`

This is the class that will generate the syntax for each syscall.

**Attributes that must be set after declaration:**
- `std::string thing` : indicate which syscall the container contain, need to be the same as `info.name`

**According to the value in `thing`, one this attribute must be set:**
- `PRINT printele` : if `thing` contain `PRINT`
- `ADD addele` : if `thing` contain `ADD`
- `ASSIGN assignele` : if `thing` contain `ASSIGN`
- `FREE freeele` : if `thing` contain `FREE`
- `INVERT invertele` : if `thing` contain `INVERT`
- `DUPLICATE duplicateele` : if `thing` contain `DUPLICATE`
- `ABS absele` : if `thing` contain `ABS`
- `DIVMOD divmodele` : if `thing` contain `DIVMOD`
- `TOINT tointele` : if `thing` contain `TOINT`
- `TODEC todecele` : if `thing` contain `TODEC`
- `MULINT mulintele` : if `thing` contain `MULINT`
- `MULDEC muldecele` : if `thing` contain `MULDEC`
- `DIVIDE divideele` : if `thing` contain `DIVIDE`

**Methods:**
- `string syntax()` : generate the line to be outputed, according to the value inside `thing`

## `vodka::variables`

That namespace is for internal management of vodka variables. This is not the namespace responsible for transcoding vodka variables declaration into kernel code instructions.

### `class vodka::variables::typess`

This is the class responsible for storing the name of the datatype.

**Attributes that shouldn't be set after declaration:**
- `std::string typenames` : the name of the datatype

**This class has no methods.**

### `class vodka::variables::variable`

This is the class responsible for storing the metadatas common to all variables and datatypes.

**Attributes that must be set after declaration:**
- `std::string varname` : the name of the variable that will be used to identify the variable inside the vodka code
- `std::string uuid` : the UID of the variable that will be used to identify the variable inside the kernel code
- `bool consts` : define if the variable is a vodka constant
- `bool write` : define if the variable should be written inside the `data` section
- `bool define` : define if the variable is a kernel constant, also define if the variable should be written inside the `data` section
- `bool algo_dependant` : define if the variable value is dependant on a function or a user input. **For the moment, the only case where this attribute is set to `true` is when the variable is a cell argument.**

**This class has no methods.**

---

### `class vodka::variables::vodint`

This is the class responsible for storing `vodint` variable.

**Attributes that must be set after declaration:**
- `std::string value` : the value of the variable
- `vodka::variables::variable varinfo` : the main metadata of the variable
- `vodka::variables::typess typeinfo` : the datatype of the variable

**Class constructor:** automatically set `typeinfo.typenames` to `vodint`

**This class has no methods.**

---

### `class vodka::variables::vodec`

This is the class responsible for storing `vodec` variable.

**Attributes that must be set after declaration:**
- `std::string value` : the value of the variable
- `vodka::variables::variable varinfo` : the main metadata of the variable
- `vodka::variables::typess typeinfo` : the datatype of the variable

**Class constructor:** automatically set `typeinfo.typenames` to `vodec`

**This class has no methods.**

### `class vodka::variables::vodarg`

This is the class responsible for storing `vodarg` variable.

**Attributes that must be set after declaration:**
- `vodka::variables::variable varinfo` : the main metadata of the variable
- `vodka::variables::typess typeinfo` : the datatype of the variable

**Class constructor:** automatically set `typeinfo.typenames` to `vodarg`

**This class has no methods.**

**Note:** this datatype represent an argument and shouldn't have a set value on the vodka side.

### `class vodka::variables::element`

This is the class a variable.

**Attributes that must be set after declaration:**
- `std::string thing` : indicate which datatype is stored inside the object

**According to the value in `thing`, one this attribute must be set:**
- `vodint intele` : if `thing` contain `vodint`
- `vodec decele` : if `thing` contain `vodec`
- `vodarg argele` : if `thing` contain `vodarg`

**This class has no methods.**

## `vodka::analyser`

That namespace is responsible for analysing, categorising and parsing lines of vodka code as well as parsing vodka variables declaration.

---

### `std::vector<std::string> get_arguments(std::string line)`

That function return only the arguments of a line (split the line and return all the elements except the first one). **It shouldn't be used on variables declaration.**

---

### `class vodka::analyser::line`

This is the class responsible for checking the basic syntax of each line : all arguments separated by spaces and if the type of line detected (function/instruction call, variable declaration, debug lines) is conform to the expected syntax.

**Attributes that must be set after declaration:**
- `std::string content` : the line to be analysed
- `std::string file` : the source file (in case of an error happening)
- `int line` : the line number (in case of an error happening)

**Attributes that indicate the output:**
- `bool skip` : tell the main program to skip the line. Only happen if the line is empty

**Attribute that should be set by methods:**
- `bool checked` : should be set with the value returned by the `check` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.

**Method:**
- `bool check(vodka::errors::sources_stack lclstack)` : check the line. The output should be put inside the `checked` attribute. **This method raise his own error so you don't need to.**

---

### `class vodka::analyser::type_analyser`

This is the class responsible for deciding of which type the line is (debug lines, vodka instruction call, internal library function call, variable declaration).

**Attributes that must be set after declaration:**
- `vodka::analyser::line line_analyse` : the line to be analysed

**Attributes that indicate the output:**
- `string type` : the result of the test, set by the `line_type_analyse` method. Can be `debug_one`, `debug_two`, `var`, `internal_library` or `vodka_instruction`

**Attribute that should be set by methods:**
- `bool checked` : should be set with the value returned by the `line_type_analyse` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.
- `std::string library_name` : indicate the name of the detected internal library, will only be set if `type` is `internal_library`
- `std::string instruction_name` : indicate the name of the detected instruction, will only be set if `type` is `vodka_instruction`

**Method:**
- `bool line_type_analyse(vodka::errors::sources_stack lclstack)` : check the line. The output should be put inside the `checked` attribute. **This method raise his own error so you don't need to. It will raise an error if `line_analyse.checked` isn't `true`.**

---

### `class vodka::analyser::var_dec_analyser`

This is the class that will parse and convert a variable declaration into a usable `vodka::variables::element` object and a syscall for declaring the variable into kernel code, under the form of `vodka::syscalls::syscall_container`.

**Attributes that must be set after declaration:**
- `vodka::variables::type_analyse line_analyse` : the line to be analysed
- `vodka::variables::element source_duplication` : the source of the duplication. **Should only be specified if `datatype` is `vodka`, after the call of `var_dec_analyse` and `check_type_value`**

**Attributes that shouldn't be modified by user:**
- `std::string name` : the name of the variable
- `std::string datatype` : the datatype of the variable
- `std::string value` : the value of the variable
- `bool is_kernel_const` : if the variable is a kernel constant
- `bool is_vodka_const` : if the variable is a vodka constant
- `vodka::variables::variable var` : the metadata of the variable

**Attributes that give the output:**
- `vodka::variables::element var_object` : the variable in his internal form, ready to be stored and used by other compoments of the library
- `vodka::syscalls::syscall_container vodka_object` : the syscall to put into the kernel code in order to declare the variable

**Attribute that should be set by methods:**
- `bool checked` : should be set with the value returned by the `var_dec_analyse` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.
- `bool checked_type_value` : should be set with the value returned by the `check_type_value` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.
- `bool pre_treated` : should be set with the value returned by the `pre_treatement` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.

**Methods (should be used in order):**
- `bool var_dec_analyse(vodka::errors::sources_stack lclstack)` : parse the variable declaration and extract the five first attributes listed in the one that shouldn't be modified by the user. **This method raise his own error so you don't need to. It will raise an error if `line_analyse.checked` isn't `true`.**
- `bool check_type_info(vodka::errors::sources_stack lclstack, vector<string> context)` : check the type and value of the variable, provide a list of already existing variable in `context`. **This method raise his own error so you don't need to. It will raise an error if `checked` isn't `true`.**
- `bool make_info(vodka::errors::sources_stack lclstack)` : set up the `var` attribute. **This method raise his own error so you don't need to. It will raise an error if `checked_type_value` isn't `true`.**
- `bool pre_treatement(vodka::errors::sources_stack lclstack)` : pre-treat the value to store. **This method raise his own error so you don't need to. It will raise an error if `checked_type_value` isn't `true`.**
- `bool output(vodka::errors::sources_stack lclstack)` : generate the `vodka::variables::element` container in order to store the variable and the `vodka::syscalls::syscall_container` in order to declare/duplicate the variable in the kernel code. **This method raise his own error so you don't need to. It will raise an error if `pre_treated` isn't `true`.**

## `vodka::type`

That namespace is responsible for containing the utilities related to datatypes.

---

### `vodka::type::vodint`

That sub-namespace contain the utilities related to the `vodint` datatype.

---

#### `bool vodka::type::vodint::check_value(std::string value, vodka::analyser::line context, vodka::errors::sources_stack lclstack)`

This function check the value for the `vodint` datatype. The `context` argument need to be set only with the necessary attribute (does not need to be checked). **That function raise his own errors so you don't have to.**

---

#### `std::string vodka::type::vodint::remove_zero(std::string value, vodka::errors::sources_stack lclstack)`

This function remove the useless zeros for a `vodint` value.

---

### `vodka::type::vodec`

That sub-namespace contain the utilities related to the `vodec` datatype.

---

#### `bool vodka::type::vodec::check_value(std::string value, vodka::analyser::line context, vodka::errors::sources_stack lclstack)`

This function check the value for the `vodec` datatype. The `context` argument need to be set only with the necessary attribute (does not need to be checked). **That function raise his own errors so you don't have to.**

---

#### `std::string vodka::type::vodec::remove_zero(std::string value, vodka::errors::sources_stack lclstack)`

This function remove the useless zeros for a `vodec` value.