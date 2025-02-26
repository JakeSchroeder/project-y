#pragma once

typedef struct vertex_buffer {
  unsigned int m_renderer_id;
} vertex_buffer_t;

vertex_buffer_t vertex_buffer_create(const void* data, unsigned int size);

void vertex_buffer_destroy(vertex_buffer_t* buffer);

void vertex_buffer_bind(vertex_buffer_t* buffer);

void vertex_buffer_unbind(void);