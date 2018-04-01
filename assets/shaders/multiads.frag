#version 400

in vec3 lightIntensity;
in float height;

out vec4 color;

vec3 getBlendedColor(float h) {
  // materials
  #define c_water vec3(.015, .110, .455)
  #define c_grass vec3(.086, .132, .018)
  #define c_beach vec3(.153, .172, .121)
  #define c_rock  vec3(.080, .050, .030)
  #define c_snow  vec3(1.0, 1.0, 1.0)

  // limits
  #define l_water 0.05
  #define l_shore 0.317
  #define l_grass 0.611
  #define l_rock 0.9

  if (h > l_rock) {
    return c_snow;
  }

  vec3 rock = mix(
    c_rock,
    c_snow,
    smoothstep(l_rock, 1.0, h)
  );

  vec3 grass = mix(
    c_grass,
    rock,
    smoothstep(l_grass, l_rock, h)
  );

  vec3 shoreline = mix(
    c_beach,
    grass,
    smoothstep(l_shore, l_grass, h)
  );

  vec3 water = mix(
    c_water / 2.0,
    c_water,
    smoothstep(0.0, l_water, h)
  );

  return mix(
    water,
    shoreline,
    smoothstep(l_water, l_shore, h)
  );
}

void main(void) {
  float normalizedHeight = min((height - 10.0) / 5.0, 1.0);
  color = vec4(getBlendedColor((height - 10.0) / 5.0), 1.0);
}
