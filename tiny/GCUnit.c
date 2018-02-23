#include <tiny.h>

#include <stdio.h>
#include <stdlib.h>

/*
 * GC Unit
 */

TGCUnit* t_gcunit_new(void* obj, TGCUnitDestructor destructor) {
  TGCUnit* self = (TGCUnit*) t_malloc(sizeof(TGCUnit));
  if (!self) {
    perror("t_gcunit_init");
    exit(1);
  }
  self->ref_count      = 1;
  self->obj            = obj;
  self->destructor     = destructor;
  self->t_gcunit_field = self;
  return self;
}

void t_gcunit_ref(TGCUnit* self) {
  ++self->ref_count;
}

void t_gcunit_unref(TGCUnit* self) {
  if (--self->ref_count == 0 && self->obj != NULL) {
    if (self->destructor) {
      self->destructor(self->obj);
      if (self->destructor != t_free) {
        t_free(self->obj);
      }
    }
  }
  if (self->ref_count == 0) {
    t_free(self);
  }
}