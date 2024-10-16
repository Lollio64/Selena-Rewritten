# Selena Manual
## Compiler Usage
Command Line Interface:
```
Usage: selena <input_shader> [options]
Options:
    -o, --output       | Select output file
    -h, --help         | Show this help message
    -v, --verbose      | Print parse and abstract syntax tree structures
    -s, --assembly     | Output picasso assembly
```
## Runtime & Compile Time Compilation
Selena allows for multiple ways of shader compilation.<br> 
There is the usual way of using the command line interface to compile the code to a SHBIN which can be run through bin2s to create a header file.<br>
Or as picasso assembly which can be optimized, changed and then run through picasso.<br>
(If speed is a necessity, this step is recommended, due to the compiler not optimizing code at all for now).<br>
Alternatively the source code can be compiled at runtime, this allows for easy code compilation, at the cost of speed and is currently C++ exclusive.<br> (The entire process has to be profiled first, before deciding if making a C version is a good idea).
## Differences between the GLSL standard
Due to hardware limitation, there are a lot of differences and removals between the Khronos standard and this version:<br>
- Fragment, Tesselation Control, Tesselation Evaluation and Compute Shaders have been support removed, due to lack of hardware support.
- For now, all `atomic` functions are gonna be abscent for now.