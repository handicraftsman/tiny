#pragma once

#include "tiny.h"

typedef struct TVector {
  size_t alloc;
  size_t step;
  size_t len;
  TGCUnit** arr;
  t_gcunit_use();
} TVector;

TVector* t_vector_new(size_t prealloc, size_t step);
void t_vector_destroy(TVector* self);

void t_vector_push(TVector* self, TGCUnit* unit);
#define t_vector_push_(self, obj) (t_vector_push(self, t_gcunit(obj)))

void t_vector_push_front(TVector* self, TGCUnit* unit);
#define t_vector_push_front_(self, obj) (t_vector_push_front(self, t_gcunit(obj)))

TGCUnit* t_vector_pop(TVector* self);
TGCUnit* t_vector_pop_back(TVector* self);

void t_vector_insert(TVector* self, TGCUnit* unit, size_t idx);
#define t_vector_insert_(self, obj, idx) (t_vector_insert(self, t_gcunit(obj), idx))

TGCUnit* t_vector_get(TVector* self, size_t idx);

bool t_vector_remove(TVector* self, size_t idx);

void t_vector_clear(TVector* self);