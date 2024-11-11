// Example PICA200 vertex shader

// Uniforms
uniform mat4 modelView;
uniform mat4 projection; 

// Inputs
// Locations match the specified ones
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

void main() {
    gl_Color = vec4(color, 1.0);
    gl_Position = projection  * modelView * vec4(position, 1.0);
}