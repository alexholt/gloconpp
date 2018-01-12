#version 400

in highp vec2 UV;
out highp vec3 color;

void main() {
  color = vec3(UV.r / 2.0 + 0.5, UV.g / 2.0 + 0.5, (UV.r + UV.g) / 4.0 + 0.5 );
}
