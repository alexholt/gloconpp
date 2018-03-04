#version 400

subroutine vec3 shadeModelType(vec4 position, vec3 normal);
subroutine uniform shadeModelType shadeModel;

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;

out vec3 lightIntensity;

struct LightInfo {
  vec4 position;
  vec3 la;
  vec3 ld;
  vec3 ls;
};

uniform LightInfo light;

struct MaterialInfo {
   vec3 ka;
   vec3 kd;
   vec3 ks;
   float shininess;
};

uniform MaterialInfo material;

uniform mat4 u_modelView;
uniform mat4 u_camera;

subroutine(shadeModelType)
vec3 phongModel(vec4 position, vec3 norm) {
  vec3 s = normalize(vec3(light.position - position));
  vec3 v = normalize(-position.xyz);
  vec3 r = reflect(-s, norm);

  vec3 ambient = light.la * material.ka;

  float sDotN = max(dot(s, norm), 0.0);
  vec3 diffuse = light.ld * material.kd * sDotN;
  vec3 spec = vec3(0.0);

  if (sDotN > 0.0) {
    spec = light.ls * material.ks * pow(max(dot(r, v), 0.0), material.shininess);
  }

  return ambient + diffuse + spec;
}

subroutine(shadeModelType)
vec3 diffuseOnly(vec4 position, vec3 norm) {
  vec3 s = normalize(vec3(light.position - position));
  return light.ld * material.kd * max(dot(s, norm), 0.0);
}

void getEyeSpace(out vec3 eyeNorm, out vec4 eyePosition) {
  eyeNorm = normalize(inverse(transpose(mat3(u_modelView))) * vertexNormal);
  eyePosition = u_modelView * vec4(vertexPosition, 1.0);
}

void main(void) {
  vec3 eyeNorm;
  vec4 eyePosition;

  getEyeSpace(eyeNorm, eyePosition);

  lightIntensity = shadeModel(eyePosition, eyeNorm);
  gl_Position = u_camera * u_modelView * vec4(vertexPosition, 1.0);
}
