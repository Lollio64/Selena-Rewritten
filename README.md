# Selena Rewritten
## About
Selena Rewritten is a GLSL compiler for the PICA200.<br>
It is a rewritten version of [Selena](https://github.com/machinamentum/Selena) by machinamentum.<br> 
Which hasn't been updated in a long time, and is as result of that outdated.<br>
All files in the `examples` folder are there to Demonstranten the various use causes of the compiler.<br>
## How to build
**Dependencies:** devkitPro's 3ds-dev package (required for library compilation), gnu make, gcc<br>

To build as an executable, it is just neccesary to run the Makefile via `make`.<br>
Cleaning up build files is handled via `make clean`, which will remove build folders/files and executables/library files.<br> **It will also remove the installed library from the devkitPro folder.**<br>
With `make library` you can compile the project as a library for usage with the 3ds-dev package.<br> 
Even though the library itself isn't dependent on any libraries in the 3ds-dev package, the Makefile used for compiliation is.<br>
Aside from these methods, it is possible compile a debug build meant to be used with GDB through `make debug`.
## Contribution
All kinds of contributions to improve the compiler are welcome. 
Commit the your changes via pull request, make sure to document your changes in the pull request and at best the in the source code via comments, so other contributers can understand your thinking process.<br>
Issues instead belong into the issue tab, it is best to mark your issue with the according label.<br>
## Credits
3dbrew - For documenting the PICA200 SHBIN format, registers and instructions.<br>
machinamentum - Of course for making the original Selena.<br>
fincs - For indirectly inspiring to make this via his own attempt, never published though.<br>
Lollio64 - For my disability to make a wave shader in Assembly lol.<br>