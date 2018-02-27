#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;

uniform mat4 u_modelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;


void main(void)
{
  vec4 tnorm = normalize(NormalMatrix * VertexNormal);
  vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition, 1.0);
  vec3 s = normalize(vec3(LightPosition - eyeCoords));
  LightIntensity = Ld * Kd * max(dot(s, tnorm), 0.0);
  gl_Position = MVP * vec4(VertexPosition, 1.0);
}
