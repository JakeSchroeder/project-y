#include "vertex_array.h"
#include <stddef.h>
#include "renderer.h"

vertex_array_t vertex_array_create(void) {
  vertex_array_t array;

  GLCall(glGenVertexArrays(1, &array.m_renderer_id));

  return array;
}

void vertex_array_destroy(vertex_array_t* array) {
  if (array) {
    GLCall(glDeleteVertexArrays(1, &array->m_renderer_id));
    array->m_renderer_id = 0;  // Set to 0 to mark as deleted
  }
}

void vertex_array_bind(vertex_array_t* array) {
  if (array) {
    GLCall(glBindVertexArray(array->m_renderer_id));
  }
}

void vertex_array_unbind(void) {
  // Unbinding doesn't need the array pointer - just bind 0
  GLCall(glBindVertexArray(0));
}

void vertex_array_add_buffer(vertex_array_t* array, vertex_buffer_t* vb,
                             const vertex_buffer_layout_t* layout) {
  if (!array || !vb || !layout) {
    return;
  }

  vertex_array_bind(array);
  vertex_buffer_bind(vb);

  unsigned int offset = 0;
  for (unsigned int i = 0; i < vertex_buffer_layout_get_element_count(layout);
       i++) {
    vertex_buffer_element_t* element =
        vertex_buffer_layout_get_element(layout, i);
    if (!element) continue;

    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(
        i, element->count, element->type, element->normalized,
        vertex_buffer_layout_get_stride(layout), (const void*)(size_t)offset));

    offset += element->size;
  }
}