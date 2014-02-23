Erbium
======

A nifty game engine in C.

Build Instructions
------------------

Erbium uses [CMake](http://cmake.org) to generate system and environment-specific build systems. To build Erbium, make a separate build directory and call CMake to generate the appropriate build system. For example, on a system that uses Make, you can use the following commands:

    # From the top of the Erbium source tree
    mkdir build
    cd build
    cmake ..
    make

Other generators are available. On OS X, for example, you can generate an XCode project by supplying `-GXcode` as an argument to the `cmake` command. Refer to CMake's [documentation for a full list of generators](http://www.cmake.org/cmake/help/v2.8.12/cmake.html#section_Generators).

How to Use
----------

**TODO** Explain how to extend the engine with subsystems and entities. Would do it now but I'm tired.
