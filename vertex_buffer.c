#include "vertex_buffer.h"
#include "renderer.h"
vertex_buffer_t vertex_buffer_create(const void* data, unsigned int size) {
  vertex_buffer_t buffer;

  GLCall(glGenBuffers(1, &buffer.m_renderer_id));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer.m_renderer_id));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

  return buffer;
}

void vertex_buffer_destroy(vertex_buffer_t* buffer) {
  if (buffer) {
    GLCall(glDeleteBuffers(1, &buffer->m_renderer_id));
    buffer->m_renderer_id = 0;  // Set to 0 to mark as deleted
  }
}

void vertex_buffer_bind(vertex_buffer_t* buffer) {
  if (buffer) {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer->m_renderer_id));
  }
}

void vertex_buffer_unbind(void) {
  // Unbinding doesn't need the buffer pointer - just bind 0
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}