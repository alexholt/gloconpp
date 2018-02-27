#version 400

in vec2 v_texCoord;

out vec4 color;

uniform vec4 u_innerColor;
uniform vec4 u_outerColor;
uniform float u_radiusInner;
uniform float u_radiusOuter;

void main(void) {
  float dx = v_texCoord.x - 0.5;
  float dy = v_texCoord.y - 0.5;
  float dist = sqrt(dx * dx + dy * dy);
  color = mix(u_innerColor, u_outerColor, smoothstep(u_radiusInner, u_radiusOuter, dist));
}
