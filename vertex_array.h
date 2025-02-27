#pragma once

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

typedef struct vertex_array {
  unsigned int m_renderer_id;
} vertex_array_t;

// Create a new vertex array
vertex_array_t vertex_array_create(void);

// Destroy a vertex array and free its resources
void vertex_array_destroy(vertex_array_t* array);

// Bind this vertex array
void vertex_array_bind(vertex_array_t* array);

// Unbind any vertex array
void vertex_array_unbind(void);

// Add a vertex buffer with a specific layout to this vertex array
void vertex_array_add_buffer(vertex_array_t* array, vertex_buffer_t* vb,
                             const vertex_buffer_layout_t* layout);