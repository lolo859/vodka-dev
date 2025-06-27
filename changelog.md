# Changelog

## Version 0.4

### New features (Vodka Transcoder)

- **Value replacement (also known as datatype replacement process) is now supported by all functions, with varying levels of flexibility depending on function constraints.**
  - See vodka codebase docs for more infos

- **Added `memory.getmem` function:**
  - Syntax: `memory.getmem <output_variable> <input_variable>`
  - Usage: get the amount of bits occupied by a variable
  - Note: `<output_variable>` must be `vodint` datatype

- **Added the corresponding `GETMEM` syscall**

### Changes and improvements (Vodka transcoder)

- **The following functions now support more arguments than expected by the corresponding syscall:**
  - `memory.print`
  - `memory.free`
  - `math.add`

- **All the mathematic-related functions contained in the `kernel` internal library have been moved to the `math` internal library**

- **`kernel` internal library has been renamed `memory`**

- **The Vodka Transcoder now use VYID in place of uuid v4.** These should be available for testing inside vodka-lib and have their own separate library in the future.

- **The Vodka Transcoder now have `xxhash.h`/`xxhash.c` and `base85.h` as dependencies, all included with the project**

- **Changing the value of a variable with a variable declaration now keep the same VYID of the variable**

### New features (vodka-lib)

- **Added `vodka::IndexStartDatatypeReplacement`:** see vodka-lib-usage docs for usage

- **Added `vodka::syscalls::GETMEM`**

- **Added `vodka::utilities::struct::random_values`**

### Changes and improvements (vodka-lib)

- **vodka-lib now have `XoshiroCpp.hpp`, `xxhash.h`/`xxhash.c` and `base85.h` as dependencies, all included with the project**