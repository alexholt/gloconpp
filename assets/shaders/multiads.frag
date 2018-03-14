#version 400

in vec3 lightIntensity;

out vec4 color;

void main(void) {
  color = vec4(lightIntensity, 1.0);
}
