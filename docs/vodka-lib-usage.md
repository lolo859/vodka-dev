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