# Changelog  

## Version 0.3 (Beta 2)  

### New Features (Vodka Transcoder)  
- **Added `TOINT` and `TODEC` syscalls to the kernel codebase**  
  - **Usage:** These syscalls are used to format values as integers or decimals.  
  - **Syntax:** `TOINT <output_variable> <input_variable>`  
  - **Note:** `TODEC` follows the same syntax.  

- **Added `kernel.toint` to the Vodka codebase**  
  - **Usage:**  
    - `input_variable` can be of type `vodec` or `vodarg`.  
    - `output_variable` must be of type `vodint`.  
  - **Syntax:** `kernel.toint <output_variable> <input_variable>`  

- **Added `kernel.todec` to the Vodka codebase**  
  - **Usage:**  
    - `input_variable` can be of type `vodint` or `vodarg`.  
    - `output_variable` must be of type `vodec`.  
  - **Syntax:** `kernel.todec <output_variable> <input_variable>`  

### New Features (Vodka Library)  
- **Added the `vodka::errors` namespace:** This namespace provides a standardized way to raise high-quality error tracebacks throughout the Vodka ecosystem.  
  - **Note:** It is compatible even with error types that do not require file and line information.  
- **Added new error handling in functions from `vodka::analyser` and `vodka::type`.**  

### Changes and Improvements  
- **Errors raised by the Vodka transcoder and functions/methods inside `vodka-lib` now display an internal code traceback.**  
  - **Warning:** The traceback is built using the `__FILE__` compiler-managed variable. Its output may vary depending on where (relative to the `comp.sh` file) the transcoder is compiled.  

- **Errors and debug messages now have color-coded output:**  
  - **🟦 Blue:** Folders  
  - **🟦 Cyan:** Files  
  - **🟩 Green:** Namespaces  
  - **🟨 Yellow:** Classes / Final namespace in the code structure  
  - **🟪 Magenta:** Functions  
  - **🟥 Red:** Errors  

- **Most functions inside `vodka-lib` (especially those analyzing Vodka code) now require a call stack managed by `vodka::errors::sources_stack`, which must be passed as an argument.**  
  - If this argument is not provided in the function call, the traceback may be incomplete.  