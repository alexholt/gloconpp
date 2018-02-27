#version 400

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_texCoord;

out vec2 v_texCoord;

uniform mat4 u_camera;
uniform mat4 u_modelView;

void main(void) {
  v_texCoord = a_texCoord.xy;
  gl_Position = u_camera * u_modelView * vec4(a_position, 1.0);
}
