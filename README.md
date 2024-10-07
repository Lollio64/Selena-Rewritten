# Selena Rewritten
## About
Selena Rewritten is a GLSL compiler for the PICA200.<br>
Selena Rewritten is a rewritten version [Selena](https://github.com/machinamentum/Selena), a now sadly discontinued and outdated/unfinished GLSL compiler for the GPU.<br>
All files in the `examples` folder are there demonstrate the compiler.<br>
## How to build
Right now, you can only through the Command Line Interface, compile the compiler via a C++ 17 or above compiler (I'm not actually sure if lower versions work), I think I'll make a Makefile for that.<br>
In the future, I will provide a devkitPro Makefile to automatically compile the shaders at runtime.<br>
Or you'll be able to compile the shaders at compile time, via a libctru implementation or through compiling the compiler without the `main.cpp` file.<br>
## Contribution
Commit the your changes via pull request, make sure to document your changes in the pull and at best the in the source code via commits, so other commiters can understand your thinking process.<br>
## Credits
3dbrew - For well documenting the PICA200 SHBIN, registers and instructions.<br>
machinamentum - Of course for making the original Selena.<br>
fincs - For indirectly inspiring to make this via his own attempt, never published though.<br>
Lollio64 - For my disability to make a wave shader in Assembly lol.<br>