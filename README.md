# Selena Rewritten
## About
Selena Rewritten is a GLSL compiler for the PICA200.<br>
Selena Rewritten is a rewritten version [Selena](https://github.com/machinamentum/Selena), a now sadly discontinued and outdated/unfinished GLSL compiler for the GPU.<br>
All files in the `examples` folder are there demonstrate the compiler.<br>
## How to build
Compiling as an executable requires `gcc` and Makefile support, then the command `make` needs to be used to compile.<br>
Compiling as a library requires `devkitPro` and also Makefile support.<br> 
Then use the command `make -f Library.mk` to use the Makefile required for building as a library.<br>
## Contribution
Commit the your changes via pull request, make sure to document your changes in the pull and at best the in the source code via commits, so other commiters can understand your thinking process.<br>
## Credits
3dbrew - For well documenting the PICA200 SHBIN, registers and instructions.<br>
machinamentum - Of course for making the original Selena.<br>
fincs - For indirectly inspiring to make this via his own attempt, never published though.<br>
Lollio64 - For my disability to make a wave shader in Assembly lol.<br>