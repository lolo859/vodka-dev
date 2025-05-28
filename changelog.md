# Changelog

## Version 0.4 beta 3

### New features (Vodka Transcoder)

- **Added two new compilations options:**
  - `-t, --show-log-time` : show the number of second since the start of the compilation, in each output
  - `--version` : output the version of the vodka transcoder, vodka-lib, json export and json algorythm version, dependencies and compiler version

### Changes and improvements (Vodka Transcoder)

- **`kernel.invert`, `kernel.free` and `kernel.abs` now support `vodec` datatype**

- **Renammed the functions responsible for reading, analysing and pre-treat the source file.**

### New features (vodka-lib)

- **Added two new `enum class`:**
  - `vodka::variables::VariableDatatype` :  indicate the datatype of a variable with possibles values all datatypes
  - `vodka::syscalls::SyscallsNames` : indicate the name of the syscall with possibles values all syscalls

- **Added two new functions:**
  - `vodka::variables::datatype_to_string` : convert a `vodka::variables::VariableDatatype` into string
  - `vodka::syscalls::syscall_to_string` : convert a `vodka::syscalls::SyscallsNames` to string

- **Added a new class to check existence and datatype of each argument in a line:** `vodka::analyser::ArgumentChecker`

- **vodka-lib now uses environnements variables to transmit information about options selected for output:**
  - `VODKA_SHOW_LOG_TIME`
  - `VODKA_DEBUG_MODE`
  - `VODKA_SHOW_VAR_WARNING`
  - `VODKA_VERBOSE_MODE`
  - See documentation for the role and possibles values of each one

- **Added a new function to `vodka::utilities`:** `double get_process_time()`

### Changes and improvements (vodka-lib)

- **Replaced every singles arguments in functions and methods using `const string&` by `string`**

- **Renamed possibles values for `vodka::json::vodka::VodkaLine::thing`:**
  - `int` : replaced by `instruction`
  - `var` : replaced by `variable_declaration`

- **Nearly every single names of class and some names of attributes and methods has been renamed:**
  - **Please see the documentation for the list of each new names of each arguments**
  - **The roles and amount of each class, arguments and methods hasn't been modified, they still have the same description in the documentation**

- **Added two new functions to `vodka::utilities` that were originally duplicated in each files:**
  - `std::vector<std::string> split(std::string str, std::string delimiter)`
  - `void replaceall(std::string str, std::string from, std::string to)`

- **The metadatas of each variable is now accessible without any check for the datatype being needed. They are now stored in `vodka::variables::VariableContainer::variable_metadata`**

- **Removed `vodka::variables::typess` and `vodka::syscalls::syscall`**

- **The name of each syscall is now stored as a string in `vodka::syscalls::<syscall_name>::name`**

- **To identify the name of the syscall or the datatype of a variable, `vodka::variables::VariableContainer::thing` and `vodka::syscalls::SyscallContainer::thing` are now `VariableDatatype` and `SyscallsNames` objects**

- **Modified JSON output made by methods inside `vodka::json` to match attributes names inside the class of these methods**