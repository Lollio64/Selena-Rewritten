# Selena Rewritten
## About
Selena Rewritten is a GLSL compiler for the PICA200.<br>
It is a rewritten version [Selena](https://github.com/machinamentum/Selena) by machinamentum.<br> 
Which hasn't been updated in a long time, and as such is outdated.<br>
All files in the `examples` folder are there demonstrate the compiler.<br>
## How to build
**Dependencies:** devkitPro's 3ds-dev package (required for library compilation)<br>

To build as an executable, it is just neccesary to run the Makefile via `make`.<br>
Cleaning up build files is handled via `make clean`, which will remove build folders/files and executables/library files. **It will also remove the installation from the devkitPro folder.**<br>
With `make library` you can compile the project as a library for usage with the 3ds-dev package.<br> 
Though the library itself isn't dependent on any libraries in the 3ds-dev package, only the Makefile.<br>
## Contribution
Commit the your changes via pull request, make sure to document your changes in the pull request and at best the in the source code via comments, so other contributers can understand your thinking process.<br>
## Credits
3dbrew - For well documenting the PICA200 SHBIN, registers and instructions.<br>
machinamentum - Of course for making the original Selena.<br>
fincs - For indirectly inspiring to make this via his own attempt, never published though.<br>
Lollio64 - For my disability to make a wave shader in Assembly lol.<br>