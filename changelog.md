# Changelog

## Version 0.4.1

### Changes and improvements (Vodka transcoder)

- **Vodka Transcoder now use blake3 instead of the Boost implementation of sha3-512 for the integrity check, all files for blake3 are included with the project**

- **Vodka Transcoder now use the official VYID library for generating VYID:** the `vodka::utilities::genvyid` function stay in place and is the official gateway for generating VYIDs

### Changes and improvements (vodka-lib)

- **vodka-lib now have `XoshiroCpp.hpp`, `xxhash.h`/`xxhash.c` and `base85.h` as dependencies, all included with the project**

- **Removed `vodka::utilities::struct::random_values`**