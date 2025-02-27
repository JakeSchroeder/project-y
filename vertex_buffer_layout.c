#include "vertex_buffer_layout.h"
#include <stdlib.h>
#include <string.h>
#include "renderer.h"

#define INITIAL_CAPACITY \
  1  // this right now is because we only have 1 vertex attribute (position)

vertex_buffer_layout_t vertex_buffer_layout_create(void) {
  vertex_buffer_layout_t layout;

  layout.elements = (vertex_buffer_element_t*)malloc(
      INITIAL_CAPACITY * sizeof(vertex_buffer_element_t));
  layout.element_count = 0;
  layout.elements_capacity = INITIAL_CAPACITY;
  layout.stride = 0;

  return layout;
}

void vertex_buffer_layout_destroy(vertex_buffer_layout_t* layout) {
  if (layout && layout->elements) {
    free(layout->elements);
    layout->elements = NULL;
    layout->element_count = 0;
    layout->elements_capacity = 0;
    layout->stride = 0;
  }
}

static void ensure_capacity(vertex_buffer_layout_t* layout) {
  if (layout->element_count >= layout->elements_capacity) {
    layout->elements_capacity *= 2;
    layout->elements = (vertex_buffer_element_t*)realloc(
        layout->elements,
        layout->elements_capacity * sizeof(vertex_buffer_element_t));
  }
}

static void push_element(vertex_buffer_layout_t* layout, unsigned int type,
                         unsigned int count, unsigned char normalized,
                         unsigned int size_of_type) {
  ensure_capacity(layout);

  vertex_buffer_element_t element;
  element.type = type;
  element.count = count;
  element.normalized = normalized;
  element.size = count * size_of_type;

  layout->elements[layout->element_count] = element;
  layout->element_count++;
  layout->stride += element.size;
}

void vertex_buffer_layout_push_float(vertex_buffer_layout_t* layout,
                                     unsigned int count) {
  push_element(layout, GL_FLOAT, count, GL_FALSE, sizeof(float));
}

void vertex_buffer_layout_push_uint(vertex_buffer_layout_t* layout,
                                    unsigned int count) {
  push_element(layout, GL_UNSIGNED_INT, count, GL_FALSE, sizeof(unsigned int));
}

void vertex_buffer_layout_push_uchar(vertex_buffer_layout_t* layout,
                                     unsigned int count) {
  push_element(layout, GL_UNSIGNED_BYTE, count, GL_TRUE, sizeof(unsigned char));
}

unsigned int vertex_buffer_layout_get_stride(
    const vertex_buffer_layout_t* layout) {
  return layout->stride;
}

vertex_buffer_element_t* vertex_buffer_layout_get_element(
    const vertex_buffer_layout_t* layout, unsigned int index) {
  if (layout && index < layout->element_count) {
    return &layout->elements[index];
  }
  return NULL;
}

unsigned int vertex_buffer_layout_get_element_count(
    const vertex_buffer_layout_t* layout) {
  return layout ? layout->element_count : 0;
}