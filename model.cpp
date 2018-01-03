#include "model.h"

GLuint Model::createProgram(QOpenGLFunctions* renderer, QString& vert, QString& frag) {
  GLuint program = renderer->glCreateProgram();
  GLuint vertShader = createShader(renderer, GL_VERTEX_SHADER, vert);
  GLuint fragShader = createShader(renderer, GL_FRAGMENT_SHADER, frag);

  renderer->glAttachShader(program, vertShader);
  renderer->glAttachShader(program, fragShader);
  renderer->glLinkProgram(program);

  GLint* params = new GLint;
  renderer->glGetProgramiv(program, GL_LINK_STATUS, params);
  if (!*params) {
    int max = 512;
    GLsizei* length = new GLsizei;
    char* infolog = new char[max];
    renderer->glGetProgramInfoLog(program, max, length, infolog);
    renderer->glDeleteProgram(program);
    auto err = std::runtime_error(infolog);
    delete length;
    delete params;
    delete[] infolog;

    throw err;
  }

  return program;
}

GLuint Model::createShader(QOpenGLFunctions* renderer, GLenum type, QString& source) {
  GLuint shader = renderer->glCreateShader(type);
  const std::string stdStr = source.toStdString();
  const char* sourceData = stdStr.c_str();
  renderer->glShaderSource(shader, 1, &sourceData, 0);
  renderer->glCompileShader(shader);

  GLint* params = new GLint;
  renderer->glGetShaderiv(shader, GL_COMPILE_STATUS, params);
  if (!*params) {
    int max = 512;
    GLsizei* length = new GLsizei;
    char* infolog = new char[max];
    renderer->glGetShaderInfoLog(shader, max, length, infolog);
    renderer->glDeleteShader(shader);

    QString typeStr("fragment");
    if (GL_VERTEX_SHADER == type)
      QString typeStr("vertex");

    auto err = std::runtime_error((QString("Problem creating ") + typeStr + " " + infolog).toStdString());
    delete length;
    delete params;
    delete[] infolog;

    throw err;
  }

  return shader;
}

void Model::translate(float x, float y, float z) {
  m_modelViewMatrix.translate(x, y, z);
}
