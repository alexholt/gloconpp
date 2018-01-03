#version 400

layout(location = 0) in vec3 a_position;
uniform mat4 u_camera;
uniform mat4 u_modelView;
out vec2 UV;

void main() {
  gl_Position = u_camera * u_modelView * vec4(a_position, 1.0);
  UV = a_position.xy;
}
