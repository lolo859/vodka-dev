# Changelog

## Version 0.4 beta 2

### New features (Vodka Transcoder)

- **Added `vodstr` datatype:**
  - The value is delimited after the first space after the datatype, until the rest of the line
  - `vodstr` datatype supports `\n` and `\u` for inserting new lines and unicode characters
  - For the moment, `vodstr` values can't be inserted as arguments inside instructions

- **Added `tostr` vodka instruction:**
  - **Syntax:** `tostr <output_variable> <input_variable>`
  - `<output_variable>` must be `vodstr` and `<input_variable>` can be `vodint`, `vodec` or `vodarg`

- **Added the `--license` argument**

### Changes and improvements (Vodka Transcoder)

- **The following functions of the `kernel` internal library are now compatible with the `vodstr` datatype:**
  - `kernel.print`
  - `kernel.free`
  - `kernel.toint` : only as input variable
  - `kernel.todec` : only as input variable

- **Optimized define replacement process in order to prevent a bug with `vodstr` value detection**
  - The new method has been extended to others cells as well

### New features (vodka-lib)

- **Added `vodstr` value to `vodka::variables::VariableDatatype`**

- **Added `vodka::variables::VodstrVariable` class**

- **Added a new library constant `vodka::ConversionsInstructions`**

- **Added a new function `vodka::utilities::string_utilities::strip`**

### Changes and improvements (vodka-lib)

- **Reworked `vodka::utilities` structure:**
  - `cell`, `symbol` and `import` structs has been moved to the new namespace `vodka::utilities::structs`
  - `log`, `debuglog` and `var_warning` functions has been moved to `vodka::utilities::output`
  - `split`, `replaceall` and `strip` functions has been moved to the new namespace `vodka::utilities::string_utilities`