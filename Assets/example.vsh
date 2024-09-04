#version 450 core

uniform mat4 projection; 
uniform mat4 modelView;

in vec3 position;
in vec2 texcoord;
in vec3 normal;

void main() {
    out_Texcoord = texcoord;
    out_Position = projection * modelView * vec4(position, 1);
}