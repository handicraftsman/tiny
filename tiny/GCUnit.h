#pragma once

#include <stddef.h>

/*
 * GC Unit
 */

typedef void (*TGCUnitDestructor)(void*);

#define t_gcunit_field _t_gcunit

typedef struct TGCUnit {
  size_t ref_count;
  void* obj;
  TGCUnitDestructor destructor;
  struct TGCUnit* _t_gcunit;  
} TGCUnit;

#define t_gcunit_use() TGCUnit* t_gcunit_field
#define t_gcunit(obj) (obj)->t_gcunit_field

TGCUnit* t_gcunit_new(void* obj, TGCUnitDestructor destructor);
#define t_gcunit_new_(obj, destructor) t_gcunit_new((void*) obj, (TGCUnitDestructor) destructor)

void t_gcunit_ref(TGCUnit* self);
#define t_ref(obj) (t_gcunit_ref(t_gcunit(obj)))
#define t_ref_(obj) (t_gcunit_ref(t_gcunit(obj)), (obj))

void t_gcunit_unref(TGCUnit* self);
#define t_unref(obj) (t_gcunit_unref(t_gcunit(obj)))
#define t_unref_(obj) (t_gcunit_unref(t_gcunit(obj)), (obj))