# Vodka language codebase and syntax

## Introduction

Vodka is a high level language designed to be transcoded to Kernel code. It has very simple but yet very specific syntax. It's the gateway to Kernel code. Please note that this documentation only contain actually implemented features (including betas) and not those planned for future release.

## Index

- [Program types](#program-types)
- [Symbols](#symbols)
  - [£VODTYPE](#vodtype)
  - [£VODSTART](#vodstart)
  - [£VODEND](#vodend)
  - [£VODEFINE](#vodefine)
- [Comments and debug lines](#comments-and-debug-lines)
  - [Comments](#comments)
  - [Debug lines](#debug-lines)
- [Cells](#cells)
  - [Cells arguments](#cells-arguments)
- [Variables](#variables)
  - [Syntax](#syntax)
  - [Constants and kernel constants](#constants-and-kernel-constants)
  - [Variables duplication](#variables-duplication)
- [Datatypes](#datatypes)
  - [vodint](#vodint)
  - [vodec](#vodec)
  - [vodarg](#vodarg)
  - [vodka](#vodka)
  - [vodstr](#vodstr)
- [Internal libraries](#internal-libraries)
  - [memory](#kernel)
    - [print](#print)
    - [free](#free)
    - [getmem](#getmem)
  - [conversions](#conversions)    
    - [toint](#toint)
    - [todec](#todec)
    - [tostr](#tostr)
  - [vodstr](#vodstr)
    - [length](#lenght)
    - [concat](#concat)
    - [substring](#substring)
    - [charat](#charat)
    - [reverse](#reverse)
    - [escape](#escape)
    - [insert](#insert)
    - [find](#find)
  - [math](#math)
    - [multiply](#multiply)
    - [add](#add)
    - [invert](#invert)
    - [abs](#abs)
    - [divmod](#divmod)
    - [divide](#divide)
    - [mulint](#mulint)
    - [muldec](#muldec)

## Program types

Programs types are used to tell the kernel interpreter how the program should be executed. There is actually 7 possible types:
- app: for GUI programs
- command: for terminal programs
- shell: for terminal session environnement
- gui: for desktop environnement
- logonui: for logon screen with gui
- logonshell: for logon screen based on terminal
- service: for background programs
Please see [£VODTYPE](#vodtype) on how to declare a type.

## Symbols

Vodka basic structure starts with symbols. Symbols always start with the `£` special caracter and are used to specify some infos about the code or delimit functions, class, etc. Their name are written in full uppercase For the moment, there are only three symbols that are actually used.

### £VODTYPE

This symbol is used to specify which type the program is. Only one `£VODTYPE` can be declared in a file .

Syntax:
```
£VODTYPE <type>
```

### £VODSTART

This symbol is used to start a cell (function). Please see [cells](#cells) for more infos.

Syntax:
```
£VODSTART <cell name> <arg1> <arg2> ... <arg_n>
```

### £VODEND

This symbol is used to end a cell (function). Please see [cells](#cells) for more infos.
No symbols can be used inside a cell.

Syntax:
```
£VODEND
```

### £VODEFINE

This symbol allow for automatic replacement of any string by another in the code of every cell. 

Syntax:
```
£VODEFINE <string to replace> <string to paste>
```

## Comments and debug lines

### Comments

Any line, whether it is a symbol or an actual line of code, can be commented by using the `§` symbol. Anything after this symbol will be ignored. Additionnaly, any line that isn't in a cell and don't start by `£` will be ignored.

### Debug lines

Debug lines are lines that start with `>` or `>>` and will print the rest of the line in the terminal during the transcoding process. These lines will be ignored if the `-d` option isn't passed to the transcoder command line. Using `>` will print the source file as well as the line where the message was located. Using `>>` will only print the message.

#### Example

Code example:
```
§ this is a comment
>this is a detailled debug line
>>this is a simple debug line
```

Output:
```
Debug line 2 in cell main from file example.vod: this is a detailled debug line
this is a simple debug line
```

## Cells

Cell are the core compoments of any Vodka file. The `main` is the cell that is always transcoded first. Others cells are only transcoded if they are called from `main`.

### Cells arguments

Cells arguments are the main way to share variables between cells. The arguments from the `main` cell are the argument that are obtained from the command line and transmited to the processus. The numbers of arguments is fix.

Arguments are transmited to the `main` under `vodarg` datatype which represent a immutable string. Please see [vodarg datatype](#vodarg) for more info

#### Example

```
£VODTYPE command
£VODSTART main
§ your code
£VODEND
```

## Variables

Variables in Vodka each have a name, a datatype and a value. Values can be undefined in certain case.

### Syntax

To declare a variable, follow the following syntax:
```
vodka <variable name>=<datatype> <value>
```
Spaces before and after the equal sign are supported and optional. These syntax is translated using the ASSIGN syscall in kernel code (see [datatypes](#datatypes) for more infos). Variables are represented using unique identifier in kernel code. Variables names can't start with `#` or `%`.

#### Example

Code example:
```
vodka a=vodint 5
```

Output:
```
ASSIGN <uid> 5
```

### Constants and kernel constants

Since there aren't any kind of constants variables in Kernel code, there is two way of creating immutables variables.

#### Constants

Constant are variables that can only be declared once (and at any time) in a vodka code. To create a constant, use the `$` as first caracter of their name. The equivalent variable without the `$` at the start the variable name is fully independant. The only thing impeaching constants from being modified is the transcoder, because these constants are only effective in the vodka code, not in the kernel code. They also can't be used as syscall output in the vodka code but they can be used as read-only variable. They can't be created from duplication (see [datatypes](#datatypes)) but can be duplicated.

#### Kernel constants

Kernel constants is a super constant variable that has all the property of a normal constant, but is also immutable in the kernel code. It's declared using `$$` instead of `$` at the start of the variable name. They are declared using the `data` section in the kernel code. They are accessible anywhere in kernel code and anywhere after declaration in vodka code.

#### Examples

Code example:
```
vodka $const=vodint 5
vodka $$kernel_const=vodint 5
```

Output:
```
type command
args:
8aede3a1-c624-4b7d-ad6b-e3bd828dd9a3
endargs
data:
067df55e-97b3-453b-9cd0-9b24b5496db3=5
enddata
main:
ASSIGN 5780ef94-b86b-4eca-a72a-3610367a31fc 5
endmain
```

### Variables duplication

Variables can be duplicated using the `vodka` datatype. Constants and kernel constants can't be created using duplication but can be duplicated to an other variable. They are duplicated using the `DUPLICATE` syscall.

Code example:
```
vodka a=vodint 5
vodka b=vodka a
```

Output:
```
type command
args:
e44d121d-33f2-48c3-aeb6-912634e67e02
endargs
data:
enddata
main:
ASSIGN dbb97160-397e-40d6-a226-431ab5e45b2b 5
DUPLICATE 12ac2165-d947-4dfe-acf3-a55672116900 dbb97160-397e-40d6-a226-431ab5e45b2b
endmain
```

## Datatypes

Variables can be of the following datatype with each their own syntax:
- [vodint](#vodint)
- [vodec](#vodec)
- [vodarg](#vodarg)
- [vodka](#vodka)
- [vodstr](#vodstr)

### vodint

The `vodint` datatype is used to store any integer, both positives and negatives numbers, without any decimal part. The number can be any size. A `vodint` variable can be non-declared by using the keyword `null`.

Code example:
```
vodka a=vodint 5
vodka b=vodint 0
vodka c=vodint -5
```

Output:
```
type command
args:
e25191a5-0116-4ef1-89ea-558c645b7ee8
endargs
data:
enddata
main:
ASSIGN 29282575-835d-48a4-a770-1ca1bf96c196 5
ASSIGN f2d921cc-2885-4bd1-82c0-70b99ee5b72b 0
ASSIGN df0d0baa-3d47-456e-9e4c-a17127ef871d -5
endmain
```

Use the `#` to directely insert a `vodint` value in a function/instruction call.

### vodec

The `vodec` datatype is used to store any floating point number, both positives and negatives numbers, with an integer and a decimal part. The number can be any size for both part. A `vodec` variable can be non-declared by using the keyword `null`.

Code example:
```
vodka a=vodec 5.0
vodka b=vodec 5.5
vodka c=vodec 0.0
vodka d=vodec -5.5
```

Code example:
```
type command
args:
7c08a366-a61e-402a-985f-d419bb2bcb91
endargs
data:
enddata
main:
ASSIGN 863685a3-3d3c-47e0-a8ac-f64c14467a6c 5.0
ASSIGN 625b0417-facb-441b-8e58-2d11c9efc670 5.5
ASSIGN 1e4df31c-5f03-411c-bf09-d90c9a238b5d 0.0
ASSIGN a70554c5-ad76-433e-820c-18071534f60d -5.5
endmain
```

Use the `%` to directely insert a `vodint` value in a function/instruction call.

**Note:** scientific notation isn't supported yet.

### vodstr

The `vodstr` datatype is used to store any string. It can include `\n` and `\u` elements.

Code example:
```
vodka a=vodstr hello
vodka b=vodstr   hello  
vodka c=vodstr hello\nworld!
```

Code example:
```
type command
args:
endargs
data:
enddata
main:
ASSIGN 9b8a5f4d-0d4d-4f9b-a688-393e3323cd8c hello
ASSIGN c91ef0e6-f4b7-4e9e-84b2-6f6d60401eda   hello  
ASSIGN ee154182-e4e7-4ad6-87b4-919e79ca0887 hello\nworld!
​⁣​⁣⁣⁣​​⁣​⁣⁣​⁣​​⁣​⁣⁣⁣⁣⁣​​​​⁣​​⁣​⁣⁣​​⁣⁣⁣⁣​​​​​​​⁣​⁣⁣⁣⁣​⁣⁣​⁣⁣⁣​⁣⁣​​⁣​⁣​​⁣⁣​⁣⁣⁣⁣​⁣​​⁣​​​⁣​⁣​⁣​​⁣⁣⁣⁣​​⁣​​⁣⁣⁣⁣⁣​​​​​⁣⁣​⁣​​⁣​⁣⁣​⁣​⁣​​​​⁣​​​​⁣​endmain
```

### vodarg

The `vodarg` datatype is a datatype for cells arguments. Technically, it represent a string but it can be used instead of the following datatype: `vodint`, `vodec` (unless specified).
Variables can't be created with the `vodarg` datatype and cells arguments with the `vodarg` datatype can't be modified or deleted.

### vodka

The `vodka` datatype is used to duplicate variables. See [variables duplication](#variables-duplication) for more infos.

## Internal libraries

Internal librairies are groups of functions directely integrated inside the vodka transcoder.

### memory

The `memory` internal library allow for the direct usage of syscalls which relate to memory management. It contain the following functions: 

- [print](#print)
- [free](#free)
- [getmem](#getmem)

#### print

Used to print variables, each arguments can be `vodint`, `vodec`, `vodstr` or `vodarg`.

Syntax : `memory.print <var1> <var2> ... <var_n>`

Support direct value declaration for all arguments.

#### free

Used to delete variables. Can't be used to delete kernel constants or `vodarg` variable.

Syntax: `memory.free <var1> <var2> ... <var_n>`

Doesn't not support direct value declaration.

#### getmem

Used to get the amount of bits occupied by a variable. `<output_variable>` must be `vodint` datatype and `<input_variable>` can be any datatype.

Syntax: `memory.getmem <output_variable> <input_variable>`

Doesn't not support direct value declaration.

### conversions

The `conversions` internal library allow to convert datatypes between each other. It contain the following functions :

- [toint](#toint)
- [todec](#todec)
- [tostr](#tostr)

#### toint

Used to automatically erase the decimal part of a `vodec`, `vodstr` or `vodarg` input. Ouput must be `vodint` datatype.

Syntax: `conversions.toint <output_variable> <input_variable>`

Doesn't not support direct value declaration.

#### todec

Used to automatically add a empty decimal part to a `vodint`, `vodstr` or `vodarg` input. Ouput must be `vodec` datatype.

Syntax: `conversions.todec <output_variable> <input_variable>`

Doesn't not support direct value declaration.

#### tostr

Used to duplicate the value of a `vodint`, `vodstr` or `vodarg` input into a `vodstr` output.

Syntax: `conversions.tostr <output_variable> <input_variable>`

Doesn't not support direct value declaration.

### vodstr

The `vodstr` internal library allow for basic operations on `vodstr` variables. It contain the following functions:

- [length](#lenght)
- [concat](#concat)
- [substring](#substring)
- [charat](#charat)
- [reverse](#reverse)
- [escape](#escape)
- [insert](#insert)
- [find](#find)

Please note that every time we mention character, we talk about UTF-8 character. Index start at 0.

#### length

Used to get the number of character inside a variable. `<input_variable>` argument must be a `vodstr` variable and `<output_variable>` must be a `vodint` variable.

Syntax: `vodstr.length <output_variable> <input_variable>`

Doesn't not support direct value declaration.

#### concat

Used to concatenate variables together. All arguments must be `vodstr` variables.

Syntax: `vodstr.concat <output_variable> <first_variable> <second_variable> ... <variable>`

Doesn't not support direct value declaration.

#### substring

Used to get a substring from a variable. The first two arguments must be `vodstr` variables. `<start_index>` and  `<substring_length>` must be `vodint` variables.

Syntax: `vodstr.substring <output_variable> <input_variable> <start_index> <substring_length>`

Support direct value declaration for `<start_index>` and `<substring_length>`.

#### charat

Used to get a single character from a variable. The first two arguments must be `vodstr` variables. `<index>` must be a `vodint` variable.

Syntax: `vodstr.charat <output_variable> <source_variable> <index>`

Support direct value declaration only for `<index>`.

#### reverse

Used to reverse a string. All arguments must be `vodstr` variables.

Syntax: `vodstr.reverse <output_variable> <source_variable>`

Doesn't not support direct value declaration.

#### escape

Used to convert every `\n` and `\u` element into text, allowing for considering them as text and not as replacement tag. All arguments must be `vodstr` variables.

Syntax: `vodstr.escape <output_variable> <source_variable>`

Doesn't not support direct value declaration.

#### insert

Used to insert a string inside another string at a certain index. All arguments must be `vodstr` variables except `<index>` which must be a `vodint` variable.

Syntax: `vodstr.insert <output_variable> <source_variable> <index> <string_to_insert>`

Doesn't not support direct value declaration.

#### find

Used to get the index of the first occurence of a character. All arguments must be `vodstr` variables except `<output_variable>` which must be a `vodint` variable.

Syntax: `vodstr.find <output_variable> <source_variable> <char_variable>`

Doesn't not support direct value declaration.

### math

The `math` internal library allow for basic operations on `vodint` and `vodec` variables. It contain the following functions:

- [multiply](#multiply)
- [add](#add)
- [invert](#invert)
- [abs](#abs)
- [divmod](#divmod)
- [divide](#divide)
- [mulint](#mulint)
- [muldec](#muldec)

#### multiply

Used to multiply two numbers together. It has two possibles syntaxs :
- without explicit precision :
  - Usage : multiply two `vodint` or `vodec` variables
  - Syntax : `math.multiply <output_variable> <first_term_variable> <second_term_variable>`
  - Note : all arguments must be of the same datatype, precision is set to 3 in this case
- with explicit precision :
  - Usage : multiply two `vodec` variables
  - Syntax : `math.multiply <output_variable> <first_term_variable> <second_term_variable> <precision_variable>`
  - Note : precision must be `vodint`. All other arguments must be `vodec`.

Support direct value declaration except for `<output_variable>`.

#### add

Used to add `vodint` or `vodec` variables, all arguments must be the same datatype.

Syntax: `math.add <output_variable> <add1> <add2> ... <add_n>`

Support direct value declaration except for `<output_variable>`.

#### invert

Used to invert the sign of a `vodint` or `vodec` variable.

Syntax: `math.invert <variable_name>`

#### abs

Used to get the absolute value of a `vodint` or `vodec` variable.

Syntax: `math.abs <variable_name>`

#### divmod

Used to get the quotient and the rest of a euclidian division. All arguments must be vodint.

Syntax: `math.divmod <result> <rest> <dividend> <divisor>`

Support direct value declaration except for `<result>` and `<rest>`.

#### divide

Used to divide `vodec` variables. All arguments except the precision must be `vodec` datatype and the precision must be `vodint` datatype.

Syntax: `math.divide <output_variable> <dividend_variable> <divisor_variable> <precision_variable>`

Support direct value declaration except for `<output_variable>`.

#### mulint

Used to multiply two integers together. All arguments must be `vodint` datatype.

Syntax: `math.mulint <output_variable> <first_term_variable> <second_term_variable>`

Support direct value declaration except for `<output_variable>`.

#### muldec

Used to multiply `vodec` variables. All arguments except the precision must be `vodec` datatype and the precision must be `vodint` datatype.

Syntax: `math.muldec <output_variable> <first_term_variable> <second_term_variable> <precision_variable>`

Support direct value declaration except for `<output_variable>`.