#version 400

in highp vec2 UV;
out highp vec4 color;

void main() {
  //color = vec4(UV / 2.0 + 0.5, 1.0, 1.0);
  color = vec4(1.0, 1.0, 0.0, 1.0);
}
