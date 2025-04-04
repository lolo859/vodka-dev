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
Debug line 2 in cell main from file example.vod : this is a detailled debug line
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

To declare a variable, follow the following syntax :
```
vodka <variable name>=<datatype> <value>
```
Spaces before and after the equal sign are supported and optional. These syntax is translated using the ASSIGN syscall in kernel code (see [datatypes](#datatypes) for more infos). Variables are represented using unique identifier in kernel code.

#### Example

Code example:
```
vodka a=vodint 5
```

Kernel code output:
```
ASSIGN <uid> 5
```

### Constants and kernel constants

Since there aren't any kind of constants variables in Kernel code, there is two way of creating immutables variables.

#### Constants

Constant are variables that can only be declared once (and at any time) in a vodka code. To create a constant, use the `$` as first caracter of their name. The equivalent variable without the `$` at the start the variable name is fully independant. The only thing impeaching constants from being modified is the transcoder, because these constants are only effective in the vodka code, not in the kernel code. They also can't be used as syscall output in the vodka code but they can be used as read-only variable.

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
endargs
data:
d26d5244-984b-479a-9b95-08bcef4cd16f=5
enddata
code:
ASSIGN b108f519-0c66-4a74-b307-c4c2c5f8bcf3 5
endcode
```