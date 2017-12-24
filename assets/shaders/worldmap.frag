#version 400

in highp vec2 UV;
out highp vec3 color;

uniform sampler2D sampler;

void main() {
  color = texture(sampler, UV).rgb;
}
