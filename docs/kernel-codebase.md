# Kernel code language codebase and syntax

## Introduction

Kernel code is the low-level, intermediate language designed to be the final target of Vodka code transcoding. It features a minimalistic but precise instruction set, focused on execution rather than readability. Kernel code is what actually gets interpreted and run. Please note that this documentation only covers currently implemented features (including beta-stage ones), and does not include any planned or theoretical additions.

## Index

- [Sections](#sections)
- [Arguments](#arguments)
- [Datas](#datas)
- [Variables]
- [Instructions]

## Sections

The kernel code is divided into sections define with their name followed by `:`. A section is ended by the keyword `end` followed immediately by the name of the section. Sections can't have any arguments. 

Code example:
```
main:
<some code>
endmain
```

The name of the first section to be executed is always `main`. Sections can't be named `args` or `data`

## Arguments

Arguments are stored in the order they are passed to the processus. The variable UID where they are stored is located in the `args` section. If too many arguments are passed to the processus, they will ignored.

Code example:
```
args:
<uid_1>
<uid_2>
endargs
```

## Datas

Datas are basically [kernel constants](vodka-codebase.md#kernel-constants). These are read-only variables that can't be deleted, modified or added during runtime. They are accessible to all sections inside the file and can be duplicated. They are stored inside the `data` section.

Code example:
```
data:
<uid>=<value>
<uid>=<value>
enddata
```

## Variables

Variables are the core of the kernel code. They are declared using the [`ASSIGN`](#assign) syscall. Variables are identified by UID in order to be unique and can contain a infinity of caracters.