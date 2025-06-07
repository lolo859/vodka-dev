# Changelog

## Version 0.4

### New features (Vodka Transcoder)

- **Added `vodstr` internal library:**
  - This library contain every functions useful for the new `vodstr` datatype
  - Please see Vodka codebase documentation for usage and syntax.

- **Added 8 new functions to Vodka codebase:**
  - `vodstr.length`
  - `vodstr.concat`
  - `vodstr.substring`
  - `vodstr.charat`
  - `vodstr.reverse`
  - `vodstr.escape`
  - `vodstr.insert`
  - `vodstr.find`

- **Added 8 new instructions to Kernel Code codebase:**
  - `LENGTH`
  - `CONCAT`
  - `SUBSTRING`
  - `CHARAT`
  - `REVERSE`
  - `ESCAPE`
  - `INSERT`
  - `FIND`
  - Please see Kernel Code codebase documentation for usage and syntax.

### Changes and improvements (Vodka transcoder)

- **Reorganize some functions inside multiple new internal libraries:**
  - `conversions`:
    - `kernel.toint` and `kernel.todec` has been moved to the new `conversions` library
    - `tostr` instruction has been moved into the `conversions` library as well
  - `math`:
    - `multiply` instruction has been moved to the `math` library
    - In the long term, all arithmetic functions will be moved into this library

- **The following functions and their equivalent in Kernel Code instructions now have a fixed amount of arguments:**
  - `kernel.print`
  - `kernel.add`
  - `kernel.free`

- **The following functions and their equivalent in Kernel Code instructions now have a dedicated output argument:**
  - `kernel.invert`
  - `kernel.abs`

- **`vodint` and `vodec` variables can now be declared as empty by using `null` as value**

### New features (vodka-lib)

- **Added three new namespace with their content:**
  - `vodka::library::conversions`
    - `class vodka::library::conversions::CallTreatement`
  - `vodka::library::math`
    - `class vodka::library::math::CallTreatement`
  - `vodka::library::vodstr`
    - `class vodka::library::vodstr::CallTreatement`

- **Added corresponding class for the new Kernel Code instructions**

### Changes and improvements (vodka-lib)

- **Removed `vodka::instructions` namespace**

- **Modified the following class in `vodka::syscalls` to match new syntax for some syscalls:**
  - `class vodka::syscalls::PRINT`
  - `class vodka::syscalls::ADD`
  - `class vodka::syscalls::FREE`
  - `class vodka::syscalls::INVERT`
  - `class vodka::syscalls::ABS`