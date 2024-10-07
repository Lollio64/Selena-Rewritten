// Example PICA200 vertex shader

// Uniform
uniform mat4 projection; 

// Inputs, locations match attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

void main() {
    gl_FrontColor = color;
    gl_Position = projection * modelView * vec4(position, 1.0);
}