# vodka documentation

## Introduction

Welcome to the documentation for the Vodka transcoder and vodka-lib, the C++ library designed to provide gateway for Vodka code transcodage to Kernel Code to the Vodka transcoder and other third-party programs. We recommand you read the [Vodka codebase documentation](https://github.com/lolo859/vodka/docs/vodka-codebase.md) first before aptempting to use 

## Dependencies

To ensure you have a smooth time compiling Vodka, please use the following command download all dependencies :

With apt : 
```sh
sudo apt update && sudo apt install -y libboost-all-dev nlohmann-json3-dev g++
```

With dnf :
```
sudo dnf install -y boost-devel nlohmann-json-devel gcc-c++
```

With pacman :
```
sudo pacman -S --needed boost nlohmann-json gcc
```

## Installation

For the moment, there aren't any official package file so you will have ether to use the binaries included in each release or compile yourself the program. We provide a .sh file for easier compiling.

### vodka

To install vodka, you can clone the repository:

```sh
git clone https://github.com/lolo859/vodka.git
```

Compile the program with:

```sh
comp.sh
```

This generate a static executable in the working directory. Please ensure you are in the root folder of the repository.

Warning : the compiling script use g++ for compiling each file. Compiling with other compiler hasn't been tested and aren't officialy supported.

### vodka-lib

To install vodka-lib, you can clone the repository:

```sh
git clone https://github.com/lolo859/vodka.git
```

Include the header in your C++ source file:

```cpp
#include "vodka-lib.h"
```

Compile the library with ```comp.sh library```. Don't forget to include the .a file by passing ```-L. -lvodka``` to your compiler. Please ensure you are in the root folder of the repository.

## References

Please see any of these file for more details :
- [Vodka language syntax and codebase](https://github.com/lolo859/vodka/docs/vodka-codebase.md)
- [Kernel code syntax and codebase](https://github.com/lolo859/vodka/docs/kernel-codebase.md)
- [vodka-lib docs](https://github.com/lolo859/vodka/docs/vodka-lib-usage.md)