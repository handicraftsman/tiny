#include <tiny.h>

#include <stdio.h>
#include <stdlib.h>

TVector* t_vector_new(size_t prealloc, size_t step) {
  TVector* self = (TVector*) t_malloc(sizeof(TVector));
  if (!self) {
    perror("t_vector_new");
    exit(1);
  }

  if (prealloc == 0)
    prealloc = 16;

  if (step == 0)
    step = 16;

  t_gcunit(self) = t_gcunit_new_(self, t_vector_destroy);
  self->alloc = prealloc;
  self->step  = step;
  self->len   = 0;
  self->arr   = (TGCUnit**) t_malloc(sizeof(TGCUnit*) * self->alloc);

  return self;
}

void t_vector_destroy(TVector* self) {
  for (int i = 0; i < self->len; ++i) {
    if (self->arr[i] != NULL)
      t_unref(self->arr[i]);
  }
  free(self->arr);
}

void t_vector_push(TVector* self, TGCUnit* unit) {
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

void t_vector_push_front(TVector* self, TGCUnit* unit) {
  t_ref(self);
  t_vector_insert(self, unit, 0);
  t_unref(self);
}

TGCUnit* t_vector_pop(TVector* self) {
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

TGCUnit* t_vector_pop_back(TVector* self) {
  t_ref(self);

  if (self->len == 0) {
    t_unref(self);
    return NULL;
  }

  TGCUnit* unit = self->arr[self->len-1];
  --self->len;

  if ((self->alloc - self->len) > self->step) {
    self->arr = t_realloc(self->arr, sizeof(TGCUnit*) * (self->alloc - self->step));
    self->alloc -= self->step;
  }

  t_unref(self);
  return unit;
}

void t_vector_insert(TVector* self, TGCUnit* unit, size_t idx) {
  t_ref(self);

  if (idx > self->len) {
    t_vector_push(self, unit);
    t_unref(self);
    return;
  }
  t_ref(unit);

  if (self->len == self->alloc) {
    self->arr = t_realloc(self->arr, sizeof(TGCUnit*) * (self->alloc + self->step));
    self->alloc += self->step;
  }
  for (int i = self->len; i > idx; --i) {
    self->arr[i] = self->arr[i-1];
  }
  self->arr[idx] = unit;
  ++self->len;

  t_unref(self);
}

TGCUnit* t_vector_get(TVector* self, size_t idx) {
  if (idx < self->len) {
    t_ref(self->arr[idx]);
    return self->arr[idx];
  } else {
    return NULL;
  }
}

bool t_vector_remove(TVector* self, size_t idx) {
  if (idx >= self->len) return false;
  t_unref(self->arr[idx]);
  for (int i = idx; i < self->len; ++i) {
    if (i != self->len - 1)
      self->arr[i] = self->arr[i+1];
  }
  --self->len;
  return true;
}

void t_vector_clear(TVector* self) {
  t_ref(self);
  for (int i = 0; i < self->len; ++i) {
    if (self->arr[i] != NULL)
      t_unref(self->arr[i]);
  }
  t_unref(self);
}