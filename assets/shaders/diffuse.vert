#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;

out vec3 lightIntensity;

uniform vec4 u_lightPosition;
uniform vec3 u_kd;
uniform vec3 u_ld;

uniform mat4 u_modelView;
uniform mat4 u_camera;
uniform mat3 u_normalMatrix;

void main(void) {
  vec3 tnorm = normalize(u_normalMatrix * vertexNormal);
  vec4 eyeCoords = u_modelView * vec4(vertexPosition, 1.0);
  vec3 s = normalize(vec3(u_lightPosition - eyeCoords));
  lightIntensity = u_ld * u_kd * max(dot(s, tnorm), 0.0);
  gl_Position = u_camera * u_modelView * vec4(vertexPosition, 1.0);
}
