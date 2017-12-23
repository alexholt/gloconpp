#version 400

in highp vec2 UV;
out highp vec3 color;

uniform sampler2D sampler;

void main() {
  color = texture(sampler, UV).rgb;
  //color = vec3(1.0, 1.0, 0.0);
  //color = vec3(UV, 0.0);
}
