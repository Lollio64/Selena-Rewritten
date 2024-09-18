//#version 450 core

uniform mat4 modelView;
uniform mat4 projection; 

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

void main() {
    out_Texcoord = texcoord;
    out_Position = projection * modelView * vec4(position, 1.0);
}