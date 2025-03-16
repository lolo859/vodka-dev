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

## Program types

Programs types are used to tell the kernel interpreter how the program should be executed. There is actually 7 possible types :
- app : for GUI programs
- command : for terminal programs
- shell : for terminal session environnement
- gui : for desktop environnement
- logonui : for logon screen with gui
- logonshell : for logon screen based on terminal
- service : for background programs
Please see [£VODTYPE](#vodtype) on how to declare a type.

## Symbols

Vodka basic structure starts with symbols. Symbols always start with the `£` special caracter and are used to specify some infos about the code or delimit functions, class, etc. Their name are written in full uppercase For the moment, there are only three symbols that are actually used.

### £VODTYPE

This symbol is used to specify which type the program is. Only one `£VODTYPE` can be declared in a file .

Syntax :
```
£VODTYPE <type>
```

### £VODSTART

This symbol is used to start a cell (function). Please see [cells](#cells) for more infos.

Syntax :
```
£VODSTART <cell name> <arg1> <arg2> ... <arg_n>
```

### £VODEND

This symbol is used to end a cell (function). Please see [cells](#cells) for more infos.
No symbols can be used inside a cell.

Syntax :
```
£VODEND
```

### £VODEFINE

This symbol allow for automatic replacement of any string by another in the code of every cell. 

Syntax :
```
£VODEFINE <string to replace> <string to paste>
```

## Comments and debug lines

### Comments

Any line, whether it is a symbol or an actual line of code, can be commented by using the `§` symbol. Anything after this symbol will be ignored. Additionnaly, any line that isn't in a cell and don't start by `£` will be ignored.

### Debug lines

Debug lines are lines that start with `>` or `>>` and will print the rest of the line in the terminal during the transcoding process. These lines will be ignored if the `-d` option isn't passed to the transcoder command line. Using `>` will print the source file as well as the line where the message was located. Using `>>` will only print the message.

## Cells

Cell are the core compoments of any Vodka file. The `main` is the cell that is always transcoded first. Others cells are only transcoded if they are called from `main`