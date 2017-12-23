#version 400

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;
out vec2 UV;

void main() {
  gl_Position = vec4(a_position, 1.0);
  UV = a_texcoord;
}
