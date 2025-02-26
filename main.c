#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(x) \
  if (!(x)) __debugbreak();

#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() { while (glGetError() != GL_NO_ERROR); }

static bool GLLogCall(const char* function, const char* file, int line) {
  GLenum error;
  while (error = glGetError()) {
    printf("[OpenGL Error] (%u) %s %s %d\n", error, function, file, line);
    return false;
  }

  return true;
}

struct ShaderProgramSource {
  char* VertexSource;
  char* FragmentSource;
};

struct ShaderProgramSource parse_shader(const char* filepath) {
  FILE* file = fopen(filepath, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open shader file: %s\n", filepath);
    struct ShaderProgramSource empty = {NULL, NULL};
    return empty;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = malloc(length + 1);
  fread(buffer, 1, length, file);
  buffer[length] = '\0';
  fclose(file);

  struct ShaderProgramSource result = {NULL, NULL};
  char* vertex_start = NULL;
  char* fragment_start = NULL;
  char* current = buffer;

  // Find shader sections
  while (*current) {
    if (strstr(current, "#shader vertex") == current) {
      current += strlen("#shader vertex");
      // Skip whitespace and newlines
      while (*current && (*current == ' ' || *current == '\n' ||
                          *current == '\r' || *current == '\t'))
        current++;
      vertex_start = current;
    } else if (strstr(current, "#shader fragment") == current) {
      if (vertex_start) {
        // Properly terminate vertex shader
        *current = '\0';
        // Remove trailing whitespace from vertex shader
        char* end = current - 1;
        while (end > vertex_start &&
               (*end == ' ' || *end == '\n' || *end == '\r' || *end == '\t'))
          *end-- = '\0';
      }
      current += strlen("#shader fragment");
      // Skip whitespace and newlines
      while (*current && (*current == ' ' || *current == '\n' ||
                          *current == '\r' || *current == '\t'))
        current++;
      fragment_start = current;
    }
    current++;
  }

  // Copy the shaders if found
  if (vertex_start) {
    size_t vertex_len = strlen(vertex_start);
    result.VertexSource = malloc(vertex_len + 1);
    strcpy(result.VertexSource, vertex_start);
  }

  if (fragment_start) {
    size_t fragment_len = strlen(fragment_start);
    result.FragmentSource = malloc(fragment_len + 1);
    strcpy(result.FragmentSource, fragment_start);
  }

  free(buffer);
  return result;
}

unsigned int compile_shader(unsigned int type, char* source) {
  unsigned int id = glCreateShader(type);
  const char* src = source;
  GLCall(glShaderSource(id, 1, &src, NULL));
  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)malloc(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    fprintf(stderr, "Failed to compile shader: %s\n", message);
    free(message);
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

unsigned int create_shader(char* vertextShader, char* fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertextShader);
  unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

int main(void) {
  GLFWwindow* window;

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(600, 400, "Hello World", NULL, NULL);

  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  // Initialize GLAD after creating OpenGL context
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    return -1;
  }

  float positions[] = {
      -0.5f, -0.5f,  // bottom left
      0.5f,  -0.5f,  // bottom right
      0.5f,  0.5f,   // top right
      -0.5f, 0.5f    // top left
  };

  unsigned int indicies[] = {0, 1, 2, 2, 3, 0};

  unsigned int vao;
  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  unsigned int buffer;
  GLCall(glGenBuffers(1, &buffer));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions,
                      GL_STATIC_DRAW));

  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

  unsigned int ibo;
  GLCall(glGenBuffers(1, &ibo));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
                      indicies, GL_STATIC_DRAW));

  struct ShaderProgramSource source = parse_shader("res/shaders/basic.shader");
  unsigned int shader =
      create_shader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(shader));

  GLCall(int location = glGetUniformLocation(shader, "u_Color"));
  ASSERT(location != -1);
  GLCall(glUniform4f(location, 1.0f, 0.5f, 0.3f, 1.0f));

  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

  float r = 0.0f;
  float increment = 0.05f;

  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

  // Main render loop
  while (!glfwWindowShouldClose(window)) {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    GLCall(glUseProgram(shader));
    GLCall(glUniform4f(location, r, 0.5f, 0.3f, 1.0f));

    GLCall(glBindVertexArray(vao))
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    if (r > 1.0f)
      increment = -.05f;
    else if (r < 0.0f)
      increment = 0.05f;

    r += increment;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  GLCall(glDeleteProgram(shader));

  glfwTerminate();

  return 0;
}