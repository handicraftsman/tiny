#pragma once

typedef struct TQueue {
  t_gcunit_use();
  size_t alloc;
  size_t step;
  size_t len;
  TGCUnit** arr;
} TQueue;

TQueue* t_queue_new(size_t prealloc, size_t step);
void t_queue_destroy(TQueue* self);

void t_queue_push(TQueue* self, TGCUnit* unit);
#define t_queue_push_(self, obj) (t_queue_push(self, t_gcunit(obj)))

TGCUnit* t_queue_pop(TQueue* self);

void t_queue_clear(TQueue* self);