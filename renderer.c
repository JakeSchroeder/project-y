#include "renderer.h"
#include <stdbool.h>
#include <stdio.h>

void GLClearError() { while (glGetError() != GL_NO_ERROR); }

bool GLLogCall(const char* function, const char* file, int line) {
  GLenum error;
  while (error = glGetError()) {
    printf("[OpenGL Error] (%u) %s %s %d\n", error, function, file, line);
    return false;
  }

  return true;
}
