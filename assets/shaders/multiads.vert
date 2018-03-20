#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;

out vec3 lightIntensity;

struct LightInfo {
  vec4 position;
  vec3 intensity;
};

uniform LightInfo lights[2];

struct MaterialInfo {
   vec3 ka;
   vec3 kd;
   vec3 ks;
   float shininess;
};

uniform MaterialInfo material;

uniform mat4 u_modelView;
uniform mat4 u_camera;

vec3 phongModel(int lightIndex, vec4 position, vec3 norm) {
  vec3 s = normalize(vec3(lights[lightIndex].position - position));
  vec3 v = normalize(vec3(-position));
  vec3 r = reflect(-s, norm);
  vec3 i = lights[lightIndex].intensity;

  return i * (material.ka + material.kd *
    max(dot(s, norm), 0.0) + material.ks * pow(max(dot(r,v), 0.0), material.shininess)
  );
}

void getEyeSpace(out vec3 eyeNorm, out vec4 eyePosition) {
  eyeNorm = normalize(inverse(transpose(mat3(u_modelView))) * vertexNormal);
  eyePosition = u_modelView * vec4(vertexPosition, 1.0);
}

void main(void) {
  vec3 eyeNorm;
  vec4 eyePosition;

  getEyeSpace(eyeNorm, eyePosition);

  lightIntensity = vec3(0.0);

  for (int i = 0; i < 2; i++) {
    lightIntensity += phongModel(i, eyePosition, eyeNorm);
  }

  gl_Position = u_camera * u_modelView * vec4(vertexPosition, 1.0);
}
