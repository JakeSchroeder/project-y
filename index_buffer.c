#include "index_buffer.h"
#include "renderer.h"

index_buffer_t index_buffer_create(const unsigned int* data,
                                   unsigned int count) {
  index_buffer_t buffer;

  // ASSERT(sizeof(unsigned int) == sizeof(GLunit));

  GLCall(glGenBuffers(1, &buffer.m_renderer_id));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.m_renderer_id));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                      data, GL_STATIC_DRAW));

  buffer.m_count = count;

  return buffer;
}

void index_buffer_destroy(index_buffer_t* buffer) {
  if (buffer) {
    GLCall(glDeleteBuffers(1, &buffer->m_renderer_id));
    buffer->m_renderer_id = 0;  // Set to 0 to mark as deleted
  }
}

void index_buffer_bind(index_buffer_t* buffer) {
  if (buffer) {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->m_renderer_id));
  }
}

void index_buffer_unbind(void) {
  // Unbinding doesn't need the buffer pointer - just bind 0
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int index_buffer_get_count(const index_buffer_t* buffer) {
  if (buffer) {
    return buffer->m_count;
  }
  return 0;  // Return 0 if buffer is NULL
}