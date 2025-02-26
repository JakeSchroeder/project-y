#pragma once

typedef struct index_buffer {
  unsigned int m_renderer_id;
  unsigned int m_count;
} index_buffer_t;

index_buffer_t index_buffer_create(const unsigned int* data,
                                   unsigned int count);

void index_buffer_destroy(index_buffer_t* buffer);

void index_buffer_bind(index_buffer_t* buffer);
void index_buffer_unbind(void);

unsigned int index_buffer_get_count(const index_buffer_t* buffer);