# vodka-lib usage

## Introduction

vodka-lib is the static internal C++ library that powers the Vodka transcoder and its ecosystem. It provides the full logic and toolset used to analyze, transform, and validate Vodka code before it's converted into Kernel code. While not directly exposed to end users, it's the backbone of the entire language pipeline. This documentation only covers currently implemented components (including those in beta) and does not describe future or theoretical features.

## Index

- [Main structure](#main-structure)
- [Constants datas](#constants-datas)
  - [vodka::LibraryVersion](#stdstring-vodkalibraryversion)
  - [vodka::JsonVersion](#stdstring-vodkajsonversion)
  - [vodka::InternalLibraryList](#stdvectorstdstring-vodkainternallibrarylist)
  - [vodka::InternalLibraryFunctions](#stdvectorstdstring-vodkainternallibrarylist)
  - [vodka::InternalDatatypes](#stdvectorstdstring-vodkainternaldatatypes)
  - [vodka::InternalSyscalls](#stdvectorstdstring-vodkainternalsyscalls)
  - [vodka::VodkaInstructions](#stdvectorstdstring-vodkavodkainstructions)
  - [vodka::ConversionsSyscalls](#stdmapstdstring-stdvectorstdstring-vodkaconversionssyscalls)
- [vodka::errors](#vodkaerrors)
  - [class vodka::errors::SourcesStack](#class-vodkaerrorssourcesstack)
  - [class vodka::errors::ErrorContainer](#class-vodkaerrorserrorcontainer)
  - [void vodka::errors::raise()](#void-vodkaerrorsraisevodkaerrorsErrorContainer-element)
- [vodka::syscalls](#vodkasyscalls)
  - [enum class vodka::syscalls::SyscallsNames](#enum-class-vodkasyscallssyscallsnames)
  - [std::string vodka::syscalls::syscall_to_string()](#stdstring-syscall_to_stringvodkasyscallsSyscallsNames-syscall)
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
  - [class vodka::syscalls::SyscallContainer](#class-vodkasyscallssyscallcontainer)
- [vodka::variables](#vodkavariables)
  - [enum class vodka::variables::VariableDatatype](#enum-class-vodkavariablesvariabledatatype)
  - [std::string vodka::variables::datatype_to_string()](#stdstring-vodkavariablesdatatype_to_stringvodkavariablesvariabledatatype-datatype)
  - [class vodka::variables::VariableMetadata](#class-vodkavariablesvariablemetadata)
  - [class vodka::variables::VodintVariable](#class-vodkavariablesvodintvariable)
  - [class vodka::variables::VodecVariable](#class-vodkavariablesvodecvariable)
  - [class vodka::variables::VodargVariable](#class-vodkavariablesvodargvariable)
  - [class vodka::variables::VariableContainer](#class-vodkavariablesvariablecontainer)
- [vodka::analyser](#vodkaanalyser)
  - [std::vector\<std::string> get_arguments()](#stdvectorstdstring-get_argumentsstdstring-line)
  - [class vodka::analyser::LineSyntaxChecker](#class-vodkaanalyserlinesyntaxchecker)
  - [class vodka::analyser::LineTypeChecker](#class-vodkaanalyserlinetypechecker)
  - [class vodka::analyser::VariableDeclarationAnalyser](#class-vodkaanalyservariabledeclarationanalyser)
- [vodka::type](#vodkatype)
  - [vodka::type::vodint](#vodkatypevodint)
    - [bool vodka::type::vodint::check_value()](#bool-vodkatypevodintcheck_valuestdstring-value-vodkaanalyserline-context-vodkaerrorssourcesstack-lclstack)
    - [std::string vodka::type::vodint::remove_zero()](#stdstring-vodkatypevodintremove_zerostdstring-value-vodkaerrorssourcesstack-lclstack)
  - [vodka::type::vodec](#vodkatypevodec)
    - [bool vodka::type::vodec::check_value()](#bool-vodkatypevodeccheck_valuestdstring-value-vodkaanalyserline-context-vodkaerrorssourcesstack-lclstack)
    - [std::string vodka::type::vodec::remove_zero()](#stdstring-vodkatypevodecremove_zerostdstring-value-vodkaerrorssourcesstack-lclstack)
- [vodka::json](#vodkajson)
  - [vodka::json::kernel](#vodkajsonkernel)
    - [class vodka::json::kernel::JsonContainer](#class-vodkajsonkerneljsoncontainer)
  - [vodka::json::vodka](#vodkajsonvodka)
    - [class vodka::json::vodka::VodkaInstruction](#class-vodkajsonvodkavodkainstruction)
    - [class vodka::json::vodka::VodkaSymbol](#class-vodkajsonvodkavodkasymbol)
    - [class vodka::json::vodka::VodkaVariableDeclaration](#class-vodkajsonvodkavariabledeclaration)
    - [class vodka::json::vodka::VodkaLine](#class-vodkajsonvodkavodkaline)
    - [class vodka::json::vodka::VodkaCell](#class-vodkajsonvodkavodkacell)
- [vodka::utilities](#vodkautilities)
  - [struct vodka::utilities::symbol](#struct-vodkautilitiessymbol)
  - [struct vodka::utilities::cellule](#struct-vodkautilitiescellule)
  - [struct vodka::utilities::import](#struct-vodkautilitiesimport)
  - [boost::uuids::uuid vodka::utilities::genuid()](#boostuuidsuuid-vodkautilitiesgenuid)
  - [void vodka::utilities::log()](#void-vodkautilitieslogstdstring-text-stdstring-verbose-int-x-stdstring-last-int-sublevel-stdvectorint-substep---stdvectorunsigned-long-subtotal)
  - [void vodka::utilities::debuglog()](#void-vodkautilitiesdebuglogstdstring-text-int-line-stdstring-cell-bool-debugmode-stdstring-verbose-stdstring-file-bool-debug_info)
  - [void vodka::utilities::var_warning()](#void-vodkautilitiesvar_warningstdstring-namevar-stdstring-typevar-stdstring-namecell-stdstring-line-bool-var_warning_enabled-stdstring-verbose)
- [vodka::library](#vodkalibrary)
  - [class vodka::library::FunctionCall](#class-vodkalibraryfunctioncall)
  - [vodka::library::kernel](#vodkalibrarykernel)
    - [class vodka::library::kernel::CallTreatement](#class-vodkalibrarykernelcalltreatement)
- [vodka::instruction](#vodkainstruction)
  - [class vodka::instruction::InstructionCall](#class-vodkainstructioninstructioncall)
  - [class vodka::instruction::CallTreatement](#class-vodkainstructioncalltreatement)

## Main structure

vodka-lib is divided into several namespaces for ease of coding:
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

### `std::string vodka::LibraryVersion`

The actual version of vodka-lib, also indicate with which version of the Vodka transcoder it should be use.

### `std::string vodka::JsonVersion`

The actual version of the json output format.

### `std::vector<std::string> vodka::InternalLibraryList`

The list of internal library for this version of vodka-lib.

### `std::map<std::string,std::vector<std::string>> vodka::InternalLibraryFunctions`

The map that indicate what functions each internal library has.

### `std::vector<std::string> vodka::InternalDatatypes`

The list of native type that can appear in a variable declaration.

### `std::vector<std::string> vodka::InternalSyscalls`

The list of all the syscalls implemented for this version of vodka-lib.

### `std::vector<std::string> vodka::VodkaInstructions`

The list of all the vodka instructions for this version of vodka-lib.

### `std::map<std::string, std::vector<std::string>> vodka::ConversionsSyscalls`

The map that indicate which vodka datatype can accept each conversion syscall

## `vodka::errors`

That namespace is responsible for errors output.

### `class vodka::errors::SourcesStack`

That class is responsible for storing the call stack of an error. It should be declarated at the root of your project and duplicated each time the object is transmited to a function that require it. You can use it in your own functions but it's mandatory for most functions in vodka-lib.

**No attributes need to be set after declaration.**

**Methods:**
- `void add(std::string src, std::string fil)` : add an element to the source stack, `src` should be `__PRETTY_FUNCTION__` and `fil` should be `__FIL__`

---

### `class vodka::errors::ErrorContainer`

That class contain all the information necessary to raise an error.

**Attributes that need to be set like a function:**
- `std::string error` : the actual error
- `std::string file` : the .vod file from which the error come. Leave empty (by default) to not display any file
- `std::vector<std::string> lines_content` : the content of each lines that need to be displayed. Leave empty (by default) to not display any line
- `std::vector<int> lines` : the line(s) that caused the error. Leave empty (by default) to not display any line. **It should have the same lenght as lines_content**
- `vodka::errors::SourcesStack source` : the source stack
- `bool class_mode` : specifie if the error came from a class method or a function

**This class has no methods.**

---

### `void vodka::errors::raise(vodka::errors::ErrorContainer element)`

That function format the error to the Vodka error format and output it.

**Required arguments:**
- `vodka::errors::ErrorContainer element` : the error container that contain all the information about the error

## `vodka::syscalls`

That namespace is responsible for syscalls syntax generation.

---

### `enum class vodka::syscalls::SyscallsNames`

That class indicate which syscall is stored.

**Possible values:** `PRINT`, `ADD`, `ASSIGN`, `FREE`, `INVERT`, `DUPLICATE`, `ABS`, `DIVMOD`, `TOINT`, `TODEC`, `MULINT`, `MULDEC`, `DIVIDE`

---

### `std::string syscall_to_string(vodka::syscalls::SyscallsNames syscall)`

Convert `vodka::syscalls::SyscallsNames` object to string.

---
### `class vodka::syscalls::PRINT`

This is the class for the `PRINT` syscall.

**Attributes that must be set after declaration:**
- `std::vector<std::string> argument_uid` : all the variables UID that should be printed
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::ADD`

This is the class for the `ADD` syscall.

**Attributes that must be set after declaration:**
- `std::vector<std::string> argument_uid` : all the variables UID that should be added together
- `std::string output_uid` : the variable UID for the output of the addition
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::ASSIGN`

This is the class for the `ASSIGN` syscall.

**Attributes that must be set after declaration:**
- `std::string value` : the value to put inside the variable
- `std::string output_uid` : the variable UID for the output of the assignation
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::FREE`

This is the class for the `FREE` syscall.

**Attributes that must be set after declaration:**
- `std::vector<std::string> argument_uid` : all the variables UID to erase
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::INVERT`

This is the class for the `INVERT` syscall.

**Attributes that must be set after declaration:**
- `std::string uid` : the variable UID that should have his sign reversed
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::DUPLICATE`

This is the class for the `DUPLICATE` syscall.

**Attributes that must be set after declaration:**
- `std::string source_uid` : the UID of the source variable for the duplication
- `std::string output_uid` : the UID of the output variable for the duplication
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::ABS`

This is the class for the `ABS` syscall.

**Attributes that must be set after declaration:**
- `std::string uid` : the UID of the variable that should be turn into his absolute value
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::DIVMOD`

This is the class for the `DIVMOD` syscall.

**Attributes that must be set after declaration:**
- `std::string quotient_uid` : the UID of the variable that should receive the quotient of the euclidian division
- `std::string rest_uid` : the UID of the variable that should receive the rest of the euclidian division
- `std::string divisor_uid` : the UID of the variable that contain the divisor
- `std::string dividend_uid` : the UID of the variable that contain the dividend
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::TOINT`

This is the class for the `TOINT` syscall.

**Attributes that must be set after declaration:**
- `std::string uid_source` : the UID of the variable that should be converted into integer number syntax
- `std::string uid_output` : the UID of the variable that should receive the converted value
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::TODEC`

This is the class for the `TODEC` syscall.

**Attributes that must be set after declaration:**
- `std::string uid_source` : the UID of the variable that should be converted into decimal number syntax
- `std::string uid_output` : the UID of the variable that should receive the converted value
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::MULDEC`

This is the class for the `MULDEC` syscall.

**Attributes that must be set after declaration:**
- `std::string output_uid` : the UID of the variable that should receive the output of the multiplication
- `std::string first_uid` : the UID of the variable that contain the first term of the multiplication
- `std::string second_uid` : the UID of the variable that contain the second term of the multiplication
- `std::string precision_uid` : the UID of the variable that contain the precision of the multiplication
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::MULINT`

This is the class for the `MULINT` syscall.

**Attributes that must be set after declaration:**
- `std::string output_uid` : the UID of the variable that should receive the output of the multiplication
- `std::string first_uid` : the UID of the variable that contain the first term of the multiplication
- `std::string second_uid` : the UID of the variable that contain the second term of the multiplication
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::DIVIDE`

This is the class for the `DIVIDE` syscall.

**Attributes that must be set after declaration:**
- `std::string output_uid` : the UID of the variable that should receive the output of the division
- `std::string first_uid` : the UID of the variable that contain the first term of the division
- `std::string second_uid` : the UID of the variable that contain the second term of the division
- `std::string precision_uid` : the UID of the variable that contain the precision of the division
- `string name` : the name of the syscall. **Shouldn't be modified.**

---

### `class vodka::syscalls::SyscallContainer`

This is the class that will generate the syntax for each syscall.

**Attributes that must be set after declaration:**
- `vodka::syscalls::SyscallsNames thing` : indicate which syscall the container contain

**According to the value in `thing`, one this attribute must be set:**
- `PRINT print_element` : if `thing` is `vodka::syscalls::SyscallsNames::PRINT`
- `ADD add_element` : if `thing` is `vodka::syscalls::SyscallsNames::ADD`
- `ASSIGN assign_element` : if `thing` is `vodka::syscalls::SyscallsNames::ASSIGN`
- `FREE free_element` : if `thing` is `vodka::syscalls::SyscallsNames::FREE`
- `INVERT invert_element` : if `thing` is `vodka::syscalls::SyscallsNames::INVERT`
- `DUPLICATE duplicate_element` : if `thing` is `vodka::syscalls::SyscallsNames::DUPLICATE`
- `ABS abs_element` : if `thing` is `vodka::syscalls::SyscallsNames::ABS`
- `DIVMOD divmod_element` : if `thing` is `vodka::syscalls::SyscallsNames::DIVMOD`
- `TOINT toint_element` : if `thing` is `vodka::syscalls::SyscallsNames::TOINT`
- `TODEC todec_element` : if `thing` is `vodka::syscalls::SyscallsNames::TODEC`
- `MULINT mulint_element` : if `thing` is `vodka::syscalls::SyscallsNames::MULINT`
- `MULDEC muldec_element` : if `thing` is `vodka::syscalls::SyscallsNames::MULDEC`
- `DIVIDE divide_element` : if `thing` is `vodka::syscalls::SyscallsNames::DIVIDE`

**Methods:**
- `string syntax()` : generate the line to be outputed, according to the value inside `thing`

## `vodka::variables`

That namespace is for internal management of vodka variables. This is not the namespace responsible for transcoding vodka variables declaration into kernel code instructions.

---

### `enum class vodka::variables::VariableDatatype`

This is the class responsible for indicating which datatype is the variable.

**Possibles values:** `vodint`, `vodec`, `vodarg`

---

### `std::string vodka::variables::datatype_to_string(vodka::variables::VariableDatatype datatype)`

Convert `vodka::variables::VariableDatatype` object to string.

---

### `class vodka::variables::VariableMetadata`

This is the class responsible for storing the metadatas common to all variables and datatypes.

**Attributes that must be set after declaration:**
- `std::string name` : the name of the variable that will be used to identify the variable inside the vodka code
- `std::string uuid` : the UID of the variable that will be used to identify the variable inside the kernel code
- `bool is_vodka_constant` : define if the variable is a vodka constant
- `bool in_data_section` : define if the variable should be written inside the `data` section
- `bool define` : define if the variable is a kernel constant, also define if the variable should be written inside the `data` section
- `bool algo_dependant` : define if the variable value is dependant on a function or a user input. **For the moment, the only case where this attribute is set to `true` is when the variable is a cell argument.**

**This class has no methods.**

---

### `class vodka::variables::VodintVariable`

This is the class responsible for storing `vodint` variable.

**Attributes that must be set after declaration:**
- `std::string value` : the value of the variable

**This class has no methods.**

---

### `class vodka::variables::VodecVariable`

This is the class responsible for storing `vodec` variable.

**Attributes that must be set after declaration:**
- `std::string value` : the value of the variable

**This class has no methods.**

---

### `class vodka::variables::VodargVariable`

This is the class responsible for storing `vodarg` variable.

**This class has no attributes.**

**This class has no methods.**

**Note:** this datatype represent an argument and shouldn't have a set value on the vodka side.

---

### `class vodka::variables::VariableContainer`

This is the class for storing a variable.

**Attributes that must be set after declaration:**
- `vodka::variables::VariableDatatype thing` : indicate which datatype is stored inside the object
- `vodka::variables::VariableMetadata variable_metadata` : the main metadata of the variable

**According to the value in `thing`, one this attribute must be set:**
- `vodka::variables::VodintVariable vodint_element` : if `thing` is  `vodka::variables::VariableDatatype::vodint`
- `vodka::variables::VodecVariable vodec_element` : if `thing` is  `vodka::variables::VariableDatatype::vodec`
- `vodka::variables::VodargVariable vodarg_variable` : if `thing` is  `vodka::variables::VariableDatatype::vodarg`

**This class has no methods.**

## `vodka::analyser`

That namespace is responsible for analysing, categorising and parsing lines of vodka code as well as parsing vodka variables declaration.

---

### `std::vector<std::string> get_arguments(std::string line)`

That function return only the arguments of a line (split the line and return all the elements except the first one). **It shouldn't be used on variables declaration.**

---

### `class vodka::analyser::LineSyntaxChecker`

This is the class responsible for checking the basic syntax of each line : all arguments separated by spaces and if the type of line detected (function/instruction call, variable declaration, debug lines) is conform to the expected syntax.

**Attributes that must be set after declaration:**
- `std::string content` : the line to be analysed
- `std::string file` : the source file (in case of an error happening)
- `int line_number` : the line number (in case of an error happening)

**Attributes that indicate the output:**
- `bool should_be_skip` : tell the main program to skip the line. Only happen if the line is empty

**Attributes that should be set by methods:**
- `bool checked` : should be set with the value returned by the `check` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.

**Method:**
- `bool check(vodka::errors::SourcesStack lclstack)` : check the line. The output should be put inside the `checked` attribute. **This method raise his own error so you don't need to.**

---

### `class vodka::analyser::LineTypeChecker`

This is the class responsible for deciding of which type the line is (debug lines, vodka instruction call, internal library function call, variable declaration).

**Attributes that must be set after declaration:**
- `vodka::analyser::LineSyntaxChecker line_checked` : the line to be analysed

**Attributes that indicate the output:**
- `string type` : the result of the test, set by the `line_type_analyse` method. Can be `debug_one`, `debug_two`, `var`, `internal_library` or `vodka_instruction`

**Attributes that should be set by methods:**
- `bool checked` : should be set with the value returned by the `line_type_analyse` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.
- `std::string library_name` : indicate the name of the detected internal library, will only be set if `type` is `internal_library`
- `std::string instruction_name` : indicate the name of the detected instruction, will only be set if `type` is `vodka_instruction`

**Method:**
- `bool line_type_analyse(vodka::errors::SourcesStack lclstack)` : check the line. The output should be put inside the `checked` attribute. **This method raise his own error so you don't need to. It will raise an error if `line_checked.checked` isn't `true`.**

---

### `class vodka::analyser::VariableDeclarationAnalyser`

This is the class that will parse and convert a variable declaration into a usable `vodka::variables::VariableContainer` object and a syscall for declaring the variable into kernel code, under the form of `vodka::syscalls::SyscallContainer`.

**Attributes that must be set after declaration:**
- `vodka::variables::LineTypeChecker line_checked` : the line to be analysed
- `vodka::variables::VariableContainer duplication_source_variable` : the source of the duplication. **Should only be specified if `datatype` is `vodka`, after the call of `parser` and `check_type_value`**

**Attributes that shouldn't be modified by user:**
- `std::string name` : the name of the variable
- `std::string datatype` : the datatype of the variable
- `std::string value` : the value of the variable
- `bool is_kernel_constant` : if the variable is a kernel constant
- `bool is_vodka_const` : if the variable is a vodka constant
- `vodka::variables::VariableMetadata variable_metadata` : the metadata of the variable

**Attributes that give the output:**
- `vodka::variables::VariableContainer variable_container` : the variable in his internal form, ready to be stored and used by other compoments of the library
- `vodka::syscalls::SyscallContainer syscall_container` : the syscall to put into the kernel code in order to declare the variable

**Attributes that should be set by methods:**
- `bool checked` : should be set with the value returned by the `parser` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.
- `bool checked_type_value` : should be set with the value returned by the `check_type_value` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.
- `bool pre_treated` : should be set with the value returned by the `value_pre_treatement` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `check`, the line doesn't pass the test and can't be transmitted to the next step.

**Methods (should be used in order):**
- `bool parser(vodka::errors::SourcesStack lclstack)` : parse the variable declaration and extract the five first attributes listed in the one that shouldn't be modified by the user. **This method raise his own error so you don't need to. It will raise an error if `line_checked.checked` isn't `true`.**
- `bool check_type_info(vodka::errors::SourcesStack lclstack, vector<std::string> context)` : check the type and value of the variable, provide a list of already existing variable in `context`. **This method raise his own error so you don't need to. It will raise an error if `checked` isn't `true`.**
- `bool make_info(vodka::errors::SourcesStack lclstack)` : set up the `var` attribute. **This method raise his own error so you don't need to. It will raise an error if `checked_type_value` isn't `true`.**
- `bool value_pre_treatement(vodka::errors::SourcesStack lclstack)` : pre-treat the value to store. **This method raise his own error so you don't need to. It will raise an error if `checked_type_value` isn't `true`.**
- `bool make_output(vodka::errors::SourcesStack lclstack)` : generate the `vodka::variables::VariableContainer` container in order to store the variable and the `vodka::syscalls::SyscallContainer` in order to declare/duplicate the variable in the kernel code. **This method raise his own error so you don't need to. It will raise an error if `pre_treated` isn't `true`.**

## `vodka::type`

That namespace is responsible for containing the utilities related to datatypes.

---

### `vodka::type::vodint`

That sub-namespace contain the utilities related to the `vodint` datatype.

---

#### `bool vodka::type::vodint::check_value(std::string value, vodka::analyser::line context, vodka::errors::SourcesStack lclstack)`

This function check the value for the `vodint` datatype. The `context` argument need to be set only with the necessary attribute (does not need to be checked). **That function raise his own errors so you don't have to.**

---

#### `std::string vodka::type::vodint::remove_zero(std::string value, vodka::errors::SourcesStack lclstack)`

This function remove the useless zeros for a `vodint` value.

---

### `vodka::type::vodec`

That sub-namespace contain the utilities related to the `vodec` datatype.

---

#### `bool vodka::type::vodec::check_value(std::string value, vodka::analyser::line context, vodka::errors::SourcesStack lclstack)`

This function check the value for the `vodec` datatype. The `context` argument need to be set only with the necessary attribute (does not need to be checked). **That function raise his own errors so you don't have to.**

---

#### `std::string vodka::type::vodec::remove_zero(std::string value, vodka::errors::SourcesStack lclstack)`

This function remove the useless zeros for a `vodec` value.

## `vodka::json`

That namespace is responsible for translating the vodka code input or the kernel code output into a exploitable JSON file.

---

### `vodka::json::kernel`

That namespace is responsible for translating the kernel code output into a exploitable JSON file.

---

#### `class vodka::json::kernel::JsonContainer`

This is the class for translating a line of kernel code into a JSON structure.

**Arguments that should be set just after declaration:**
- `std::string type` : specify the type of line. Could be `system_call`, `constant` or `argument`
- `std::string instruction_name` : the name of the instruction if `type` is `system_call` or the name of the variable if `type` is `constant` or `argument`
- `std::vector<std::string> args` : the list of argument(s) of the line. Leave empty if `type` is `argument` or put one element with all the data behind the `=` if `type` is `constant`

**Method:**
- `std::map<std::string, std::string> syntax()` : generate the map to put inside the JSON file

---

### `vodka::json::vodka`

That namespace is responsible for translating the vodka code input into a JSON structure.

---

#### `class vodka::json::vodka::VodkaInstruction`

This class make the output for a vodka instruction (with or without library).

**Arguments that should be set just after declaration:**
- `std::string name` : the name of the instruction
- `std::string source` : the source of the instruction. **For the moment, all available source is `<builtin>`, signifying that the instruction isn't from a plugin.**
- `std::string library` : the library from which the instruction come from. If the instruction is a cell defined into the source file or a instruction managed by the Vodka transcoder, this attribute should be set as `<no_library>`
- `std::string uid` : the UID of the line
- `std::vector<std::string> args` : the list of argument(s) of the line

**Method:**
- `std::map<std::string, std::string> syntax()` : generate the map to put inside the JSON file

---

#### `class vodka::json::vodka::VodkaSymbol`

This class make the output for a vodka symbol.

**Arguments that should be set just after declaration:**
- `std::string type` : the type of the symbol
- `std::string uid` : the UID of the line
- `std::vector<std::string> args` : the list of argument(s) of the symbol

**Method:**
- `std::map<std::string, std::string> syntax()` : generate the map to put inside the JSON file

---

#### `class vodka::json::vodka::VariableDeclaration`

This class make the output for a vodka variable declaration.

**Arguments that should be set just after declaration:**
- `std::string variable_name` : the name of the variable
- `std::string variable_datatype` : the datatype of the variable
- `std::string variable_value` : the value of the variable into the declaration
- `std::string uid` : the UID of the line

**Method:**
- `std::map<std::string, std::string> syntax()` : generate the map to put inside the JSON file

---

#### `class vodka::json::vodka::VodkaLine`

This class make the output for a line.

**Arguments that should be set just after declaration:**
- `std::string thing` : the type of the line. Can be set to `instruction` for a vodka instruction (in a library or without a library) or `variable_declaration` for a variable declaration
- `vodka::json::vodka::VodkaVariableDeclaration variable_declaration_element` : contain the variable declaration to be outputed. Should only be set if `thing` is `variable_declaration`.
- `vodka::json::vodka::VodkaInstruction instruction_element` : contain the vodka instruction to be outputed. Should only be set if `thing` is `instruction`.

**Method:**
- `std::map<std::string, std::string> syntax()` : generate the map to put inside the JSON file

---

#### `class vodka::json::vodka::VodkaCell`

This class make the output for an entire cell.

**Arguments that should be set just after declaration:**
- `std::string name` : the name of the cell
- `std::string uid` : the UID of the cell
- `vodka::json::vodka::VodkaSymbol start` : the symbol that start the cell (also contain the argument of the cell)
- `vodka::json::vodka::VodkaSymbol end` : the symbol that end the cell
- `std::vector<vodka::json::vodka::VodkaLine> lines` : the lines of the cell, ready to be converted into a JSON structure

**Method:**
- `std::map<std::string,std::map<std::string, std::string>> syntax()` : generate the map to put inside the JSON file

## `vodka::utilities`

That namespace is responsible for storing the necessary structure for early code analysis (symbols and cells detection) as well as logs functions and other utilities.

---

### `struct vodka::utilities::symbol`

This is the structure responsible for storing a vodka symbol.

**Attributes:**
- `int line` : the line of the symbol
- `std::string content` : the content of the line
- `std::string type` : the type of the symbol, without the `£`

---

### `struct vodka::utilities::cellule`

This is the structure responsible for storing a cell.

**Attributes:**
- `std::vector<std::string> content` : the list of each line of the cell
- `std::string name` : the name of the cell
- `std::vector<std::string> args` : the argument(s) of the cell
- `std::vector<std::string> outs` : the output(s) of the cell
- `vodka::utilities::symbol start` : the starting symbol of the cell
- `vodka::utilities::symbol end` : the ending symbol of the cell

---

### `struct vodka::utilities::import`

This is the structure responsible for storing an import. **It's not actually used in the transcoder or the library but will be in the future.**

**Attributes:**
- `std::string file` : the source file
- `std::string type` : the type of import
- `std::string importas` : the name used in the main vodka program to use this import
- `std::vector<std::string> content` : the content imported

---

### `boost::uuids::uuid vodka::utilities::genuid()`

This function is used to generate UID for all the elements inside the Vodka transcoder and vodka-lib. These UID can be directly translated into `std::string` using `std::to_string`.

---

### `void vodka::utilities::log(std::string text, std::string verbose, int x, std::string last, int sublevel, std::vector<int> substep = {}, std::vector<unsigned long> subtotal = {})`

This fonction is used to output logs in a stepped format.

**Arguments:**
- `std::string text` : the text to log
- `std::string verbose` : the verbose mode, used to determine the amount of line break that need to be printed in order to output a readable result. Could be `a` for all or `r` for reduced.
- `int x` : the actual step for the first level of process
- `std::string sublevel` : the number of sub-process inside the actual process (0 if no sub-process)
- `std::string last` : the number of step inside the first level process
- `std::vector<int> substep` : the actual step for each level of process except the first one
- `std::vector<unsigned long> subtotal` : the number of step inside each level of process except the first one

**Example:**
```cpp
log("this is a log","a",6,"12",2,{5,5},{10,10});
```
```
[LOG]     (6/18) (5/10) (5/10) this is a log
```

**Warning:** this log function has been conceived for the Vodka transcoder which combine a specific schema for lines break that will probably result in a mess in your terminal. Use `e` for normal printing mode.

---

### `void vodka::utilities::debuglog(std::string text, int line, std::string cell, bool debugmode, std::string verbose, std::string file, bool debug_info)`

This is the function used to print debug lines in the vodka code.

**Arguments:**
- `std::string text` : the text to print, should include `>` or `>>`
- `int line` : the line number of the debug line
- `std::string cell` : the name of the original cell
- `bool debugmode` : if the debug mode has been actived by the user
- `std::string verbose` : could be `a` for all, `r` for reduced or `e` for error only. This is mainly for deciding how line break should be inserted in the terminal.
- `std::string file` : the source file in which the debug line was detected
- `bool debug_info` : if debug infos such as `file`, `line` or `cell` should be printed as well

**Example:**
```cpp
debuglog(">hello",5,"main",true,"a","file.vod",true);
```
```
[DEBUG]   Debug line 5 in cell main from file /home/lolo859/project/cpp/vodka/file.vod : hello
```

**Warning:** this log function has been conceived for the Vodka transcoder which combine a specific schema for lines break that will probably result in a mess in your terminal. Use `e` for normal printing mode.

### `void vodka::utilities::var_warning(std::string namevar, std::string typevar, std::string namecell, std::string line, bool var_warning_enabled, std::string verbose)`

This is the function used to print unesed variables warning.

**Arguments:**
- `std::string namevar` : the name of the variable
- `std::string typevar` : the datatype of the variable
- `std::string namecell` : the name of the cell where of the variable is declared
- `std::string line` : the line number where the variable is declared
- `bool var_warning_enabled` : if the user enabled warnings about unused variables.
- `std::string verbose` : could be `a` for all, `r` for reduced or `e` for error only. This is mainly for deciding how line break should be inserted in the terminal.

**Example:**
```cpp
var_warning("var","vodint","main","6",true,"e");
```
```
[WARNING] vodka.warnings.unused_variable : Variable var (vodint), declared line 6 in cell main, isn't used anywhere and may take useless memory.
```

**Warning:** this log function has been conceived for the Vodka transcoder which combine a specific schema for lines break that will probably result in a mess in your terminal. Use `e` for normal printing mode.

## `vodka::library`

That namespace is responsible for the transcoding of internal libraries function call.

---

### `class vodka::library::FunctionCall`

This class store all the context needed to transcode an internal library function call.

**Arguments that should be set just after declaration:**
- `vodka::analyser::LineTypeChecker line_checked` : the line to analyse
- `std::vector<std::string> variableslist_context` : the list of variables already existing before this instruction
- `vodka::utilities::cellule cell_context` : the cell structure from which the function call came from
- `int iteration_number_context` : since the analyse of all the lines in the cell should be done in a `for` loop, we need the iterator of this loop to determine the line number.
- `std::string file_name_context` : the name of the file being transcoded
- `std::string verbose_context` : the verbose mode selected by the user, `a`, `r` or `e`. Choose `e` for normal output mode.
- `int main_logstep_context` : the step of the main process of transcoding. Put any number if `verbose_context` is `e`.
- `std::string last_logstep_context` : the number of step in the main process. Put any string if `verbose_context` is `e`.
- `std::map<std::string,vodka::variables::VariableContainer> variablesdict_context` : the map of all the variables already existing before this instruction

---

### `vodka::library::kernel`

This namespace is responsible for the transcoding of internal libraries function call.

---

#### `class vodka::library::kernel::CallTreatement`

This is the class that transcode kernel internal library function call into a list of usable `vodka::syscalls::SyscallContainer`.

**Arguments that should be set just after declaration:**
- `vodka::library::FunctionCall function_call` : the function call with all his context

**Arguments that indicate the output (only set after call_treatement):**
- `std::vector<vodka::syscalls::SyscallContainer> syscall_output` : the list of syscall object that should be added in the kernel code output
- `bool var_flag` : indicate if the main program should replace his list and map of existing variables with the one into the function call object that was modified

**Arguments that should be set by method:**
- `bool checked` : should be set with the value returned by the `call_treatement` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `call_treatement`, the line doesn't pass the test and can't be consideer valid.

**Method:**
- `bool call_treatement(SourcesStack lclstack)` : automatically choose the expected call_treatement for the function call and transcode it into syscall. The output should be put inside the `checked` attribute. **This method raise his own error so you don't need to. It will return `false` if `call.type_analyser.checked` isn't `true` or `call.type_analyser.type` isn't `internal_library`.**

## `vodka::instruction`

That namespace is responsible for the transcoding of vodka instructions call.

---

### `class vodka::instruction::InstructionCall`

This class store all the context needed to transcode a vodka instructions call.

**Arguments that should be set just after declaration:**
- `vodka::analyser::LineTypeChecker line_checked` : the line to analyse
- `std::vector<std::string> variableslist_context` : the list of variables already existing before this instruction
- `vodka::utilities::cellule cell_context` : the cell structure from which the instruction call came from
- `int iteration_number_context` : since the analyse of all the lines in the cell should be done in a `for` loop, we need the iterator of this loop to determine the line number.
- `std::string file_name_context` : the name of the file being transcoded
- `std::string verbose_context` : the verbose mode selected by the user, `a`, `r` or `e`. Choose `e` for normal output mode.
- `int main_logstep_context` : the step of the main process of transcoding. Put any number if `verbose_context` is `e`.
- `std::string last_logstep_context` : the number of step in the main process. Put any string if `verbose_context` is `e`.
- `std::map<std::string,vodka::variables::VariableContainer> variablesdict_context` : the map of all the variables already existing before this instruction

---

### `class vodka::instruction::CallTreatement`

This is the class that transcode vodka instruction call into a list of usable `vodka::syscalls::SyscallContainer`.

**Arguments that should be set just after declaration:**
- `vodka::library::instruction_call instruction_call` : the instruction call with all his context

**Arguments that indicate the output (only set after call_treatement):**
- `std::vector<vodka::syscalls::SyscallContainer> syscall_output` : the list of syscall object that should be added in the kernel code output
- `bool var_flag` : indicate if the main program should replace his list and map of existing variables with the one into the instruction call object that was modified

**Arguments that should be set by method:**
- `bool checked` : should be set with the value returned by the `call_treatement` method (need to be set manually in the main program). Defaulted to `false` but if the value doesn't change after a call on `call_treatement`, the line doesn't pass the test and can't be consideer valid.

**Method:**
- `bool call_treatement(SourcesStack lclstack)` : automatically choose the expected call_treatement for the instruction call and transcode it into syscall. The output should be put inside the `checked` attribute. **This method raise his own error so you don't need to. It will return `false` if `call.type_analyser.checked` isn't `true` or `call.type_analyser.type` isn't `call_treatement`.**