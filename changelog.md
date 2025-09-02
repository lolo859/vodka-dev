# Changelog

## Version 0.5 beta 1

### Changes and improvements (Vodka transcoder)

- **Moved `encode_to_bin` and `hash_then_encode` functions to vodka-lib**
  - **Note:** A better implementation is planned for the next beta

- **Updated the JSON export version to 5**

- **The kernel code JSON export now use space as separator between the number of line, the name of the section and their VYID**

- **In the data section, the VYID of each kernel constants is now separated with a space from his value.**

### New features (vodka-lib)

- **Added a new class `SafeAttribute` to `vodka::utilities` namespace**
  - **See vodka-lib documentation for usage**

- **Added `vodka::compilation` namespace which contain all the code parsing, symbol and cells processing and code pre-treatement logic**
  - **The classes inside of it are shells for the previous functions located directly inside the Vodka Transcoder**

- **Added `vodka::utilities::encoding` namespace which contain the encoding function from the Vodka Transcoder**

### Changes and improvements (vodka-lib)

- **All classes that previously used public boolean attributes to store methods result now use private `SafeAttribute` object.**
  - **The affected classes include classes from `vodka::analyser`, `vodka::library` and `vodka::compilation`**
  - **See vodka-lib documentation for the usage of the new API**

- **Removed `last` argument from `vodka::utilities::output::log` function**