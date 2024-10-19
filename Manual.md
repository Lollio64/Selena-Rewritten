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
There is the usual way of using the command line interface to compile the shader to a SHBIN (Shader Binary) which can be run through bin2s (located in the devkitPro/tools) to create a header file.<br>
On the other hand the output can be formatted as picasso assembly which can be optimized, changed and then run through picasso (located in devkitPro/tools).<br>
(If speed is a necessity, this step is recommended, due to the compiler not optimizing code at all for now).<br>
Alternatively the shader can be compiled at runtime, this allows for easy code compilation, at the cost of speed.<br> 
Right now, this feature is exclusive to C++, because of the usage of C++ only features.<br> 
(The entire runtime compilation process has to be profiled first, before deciding if C should be supported).
## Differences between the GLSL standard
Due to hardware limitation and other, there are a lot of differences and removals and additions between the Khronos standard and this version:<br>
- Fragment, Tesselation Control, Tesselation Evaluation and Compute Shaders have been support removed, due to lack of hardware support.
- For now, all `atomic` functions/keywords have been removed due to involvement with memory management.
- Unlike GLSL where `enum` is just a reserved keyword, enumerations will be supported in the forseeable future.
