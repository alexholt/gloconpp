#version 400

in highp vec2 UV;
out highp vec3 color;

void main() {
  color = vec3(UV / 2.0 + 0.5, 1.0);
  color = vec3(1.0, 1.0, 0.0);
}
