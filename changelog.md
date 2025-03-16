## Changelog

### Version 0.3 (Beta 2)

### New features (vodka transcoder)
- **Added `TOINT` and `TODEC` syscalls to kernel codebase**
  - **Usage :** these syscalls should be use to format value to integer or decimal
  - **Syntax :** `TOINT <output_variable> <input_variable>`
  - **Note :** the syntax is the same for `TODEC`
- **Added `kernel.toint` to vodka codebase**
  - **Usage :** 
    - `input_variable` can be `vodec` or `vodarg` datatype
    - `output_variable` must be `vodint` datatype
  - **Syntax :** `kernel.toint <output_variable> <input_variable>`
- **Added `kernel.todec` to vodka codebase**
  - **Usage :** 
    - `input_variable` can be `vodint` or `vodarg` datatype
    - `output_variable` must be `vodec` datatype
  - **Syntax :** `kernel.todec <output_variable> <input_variable>`

### New features (vodka library)
- **Added `vodka::errors` namespace :** it contain everything to raise high quality error traceback and a new standardised way to raise errors in the vodka ecosystem.
  - **Note :** it's compatible even with error type that don't need to specify and file and line
- **Added new errors to the functions from `vodka::analyser` and `vodka::type`**

## Change and improvements
- **Errors raised by the vodka transcoder and the functions/methods inside vodka-lib now show the internal code traceback**
  - **Warning :** the traceback is built using `__FILE__` compiler-managed variable. It could varie depending on where (relative to where the comp.sh file is) you're compiling the transcoder
- **Errors and debug lines now have a color-coded output :**
  - **blue :** folders
  - **cyan :** files
  - **green :** namespaces
  - **yellow :** class/final namespace in the code structure
  - **magenta :** functions
  - **red :** errors
- **Most of the functions inside vodka-lib (especially those who analyse the vodka code) now require a call stack managed by vodka::errors::sources_stack and passed via argument**
  - If this argument isn't provided in the function call, there is chance that the traceback could be incomplete
