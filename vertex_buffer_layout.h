#pragma once

typedef struct vertex_buffer_element {
  unsigned int type;         // GL type (e.g., GL_FLOAT, GL_UNSIGNED_INT)
  unsigned int count;        // Number of components per vertex attribute
  unsigned char normalized;  // Whether to normalize
  unsigned int size;         // Size in bytes of this type
} vertex_buffer_element_t;

typedef struct vertex_buffer_layout {
  vertex_buffer_element_t* elements;
  unsigned int element_count;
  unsigned int elements_capacity;
  unsigned int stride;  // Sum of all element sizes
} vertex_buffer_layout_t;

vertex_buffer_layout_t vertex_buffer_layout_create(void);

void vertex_buffer_layout_destroy(vertex_buffer_layout_t* layout);

void vertex_buffer_layout_push_float(vertex_buffer_layout_t* layout,
                                     unsigned int count);

void vertex_buffer_layout_push_uint(vertex_buffer_layout_t* layout,
                                    unsigned int count);

void vertex_buffer_layout_push_uchar(vertex_buffer_layout_t* layout,
                                     unsigned int count);

unsigned int vertex_buffer_layout_get_stride(
    const vertex_buffer_layout_t* layout);

vertex_buffer_element_t* vertex_buffer_layout_get_element(
    const vertex_buffer_layout_t* layout, unsigned int index);

unsigned int vertex_buffer_layout_get_element_count(
    const vertex_buffer_layout_t* layout);