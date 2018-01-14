#version 400

in highp vec2 UV;
out highp vec4 color;

uniform sampler2D sampler;

void main() {
  color = vec4(texture(sampler, UV).rgb, 1.0);
}
