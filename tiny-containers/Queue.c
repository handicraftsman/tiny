#include <tiny.h>

#include <stdio.h>
#include <stdlib.h>

TQueue* t_queue_new(size_t prealloc, size_t step) {
  TQueue* self = (TQueue*) t_malloc(sizeof(TQueue));
  if (!self) {
    perror("t_list_new");
    exit(1);
  }

  if (prealloc == 0)
    prealloc = 16;

  if (step == 0)
    step = 16;

  t_gcunit(self) = t_gcunit_new_(self, t_queue_destroy);
  self->alloc = prealloc;
  self->step  = step;
  self->len   = 0;
  self->arr   = (TGCUnit**) t_malloc(sizeof(TGCUnit*) * self->alloc);

  return self;
}

void t_queue_destroy(TQueue* self) {
  for (int i = 0; i < self->len; ++i) {
    if (self->arr[i] != NULL)
      t_unref(self->arr[i]);
  }
  free(self->arr);
}

void t_queue_push(TQueue* self, TGCUnit* unit) {
  t_ref(self);
  t_ref(unit);
  if (self->len == self->alloc) {
    self->arr = t_realloc(self->arr, sizeof(TGCUnit*) * (self->alloc + self->step));
    self->alloc += self->step;
  }
  self->arr[self->len] = unit;
  ++self->len;
  t_unref(self);
}

TGCUnit* t_queue_pop(TQueue* self) {
  t_ref(self);
  if (self->len == 0) {
    t_unref(self);
    return NULL;
  }
  
  TGCUnit* unit = self->arr[0];
  
  for (int i = 0; i < self->len; ++i) {
    if (i != self->len - 1)
      self->arr[i] = self->arr[i+1];
  }
  --self->len;

  if ((self->alloc - self->len) > self->step) {
    self->arr = t_realloc(self->arr, sizeof(TGCUnit*) * (self->alloc - self->step));
    self->alloc -= self->step;
  }

  t_unref(self);
  return unit;
}

void t_queue_clear(TQueue* self) {
  t_ref(self);
  for (int i = 0; i < self->len; ++i) {
    if (self->arr[i] != NULL)
      t_unref(self->arr[i]);
  }
  t_unref(self);
}