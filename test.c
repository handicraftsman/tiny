/*
 * Includes
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tiny.h>

/*
 * Sample class
 */

typedef struct Sample {
  t_gcunit_use();
  TGCUnit* str;
} Sample;

Sample* sample_new(char* str);
void sample_destroy(Sample* self);
void sample_print(Sample* self);
void sample_error(Sample*, TError** err);

Sample* sample_new(char* str) {
  Sample* self = (Sample*) t_malloc(sizeof(Sample));
  if (!self) {
    perror("sample_new");
    exit(1);
  }

  t_gcunit(self) = t_gcunit_new_(self, sample_destroy);

  self->str = t_gcunit_new_(strdup(str), t_free);

  return self;
}

void sample_destroy(Sample* self) {
  t_unref(self->str);
}

void sample_print(Sample* self) {
  printf("%s\n", self->str->obj);
}

void sample_error(Sample* self, TError** err) {
  *err = t_error_new(self->str->obj, false);
}

/*
 * Main function
 */

void test_object_and_error() {
  Sample* sample = sample_new("Hello, World!");
  
  sample_print(sample);

  TError* err = NULL;

  sample_error(sample, &err);
  if (err != NULL) {
    printf("%s\n", err->message);
  }

  t_error_clear(err);

  t_unref(sample);
}

void test_list() {
  TList* list = t_list_new();

  char* arr[] = {
    "Foo",
    "Bar",
    "Baz",
    "Quux",
    NULL
  };

  for (int i = 0; arr[i] != NULL; ++i) {
    TGCUnit* u = t_gcunit_new_(strdup(arr[i]), t_free);
    t_unref(t_list_append_(list, u));
    t_unref(u);
  }

  char* arr2[] = {
    "kek",
    "asdf",
    "spam",
    NULL
  };

  for (int i = 0; arr2[i] != NULL; ++i) {
    TGCUnit* u = t_gcunit_new_(strdup(arr2[i]), t_free);
    t_unref(t_list_prepend_(list, u));
    t_unref(u);
  }

  TListNode* n1 = t_list_get(list, 0);
  TListNode* n2 = t_list_get(list, 6);
  t_list_swap(list, n1, n2);

  t_unref(n1);
  t_unref(n2);

  TGCUnit* u1 = t_gcunit_new_(strdup("Last"), t_free);
  t_unref(t_list_insert_(list, u1, 6));
  t_unref(u1);

  TGCUnit* u2 = t_gcunit_new_(strdup("Center"), t_free);
  t_unref(t_list_insert_(list, u2, 3));
  t_unref(u2);

  TGCUnit* u3 = t_gcunit_new_(strdup("First"), t_free);
  t_unref(t_list_insert_(list, u3, 0));
  t_unref(u3);

  t_list_foreach(list, n) {
    // todo
  }

  TListNode* u4 = t_list_get(list, 2);
  t_list_remove(list, u4);
  t_unref(u4);
  
  t_list_foreach(list, n) {
    // todo
  }

  t_unref(list);
}

void test_map() {
  TMap* map = t_map_new(map);

  TGCUnit* u1 = t_gcunit_new_(strdup("Bar"), t_free);
  t_unref(t_map_set_(map, "foo", u1));
  t_unref(u1);

  TMapPair* p1 = t_map_get(map, "foo");
  t_unref(p1);
  
  TGCUnit* u2 = t_gcunit_new(strdup("Baz"), t_free);
  t_unref(t_map_set_(map, "foo", u2));
  t_unref(u2);

  TMapPair* p2 = t_map_get(map, "foo");
  t_unref(p2);

  TGCUnit* u3 = t_gcunit_new(strdup("spam"), t_free);
  t_unref(t_map_set_(map, "quux", u3));
  t_unref(u3);

  TMapPair* p3 = t_map_get(map, "quux");
  t_unref(p3);

  t_unref(t_map_rename(map, "quux", "asdf"));

  TMapPair* p4 = t_map_get(map, "asdf");
  t_unref(p4);

  for (TListNode* n = t_list_first(map->pairs); n != NULL; n = t_list_next(n)) {
    TMapPair* p = n->unit->obj;
  }

  t_map_remove(map, "asdf");

  for (TListNode* n = t_list_first(map->pairs); n != NULL; n = t_list_next(n)) {
    TMapPair* p = n->unit->obj;
  }

  t_unref(map);
}

TVector* get_vector() {
  TVector* vector = t_vector_new(0, 0);

  for (int i = 0; i < 128; ++i) {
    char* str = strdup("foo");
    TGCUnit* u = t_gcunit_new_(str, t_free);
    t_vector_push(vector, u);
    t_unref(u);
  }

  return vector;
}

void test_vector() {
  TGCUnit* u = NULL;

  TVector* v1 = get_vector();
  for (int i = 0; i < v1->len; ++i) {
    TGCUnit* u = v1->arr[i];
    t_ref(u);
    t_unref(u);
  }
  t_unref(v1);

  u = NULL;
  TVector* v2 = get_vector();
  while ((u = t_vector_pop_back(v2), u != NULL)) {
    t_unref(u);
  }
  t_unref(v2);


  TVector* v3 = get_vector();
  t_vector_remove(v3, 0);
  t_vector_remove(v3, 1);
  t_vector_remove(v3, 2);
  for (int i = 0; i < v3->len; ++i) {
    TGCUnit* u = t_vector_get(v3, i);
    if (u == NULL) continue;
    t_unref(u);
  }
  t_unref(v3);
  

  TVector* v4 = get_vector();
  for (int i = 0; i < 16; ++i) {
    char* str = strdup("foo");
    TGCUnit* u = t_gcunit_new_(str, t_free);
    t_vector_insert(v4, u, i*2);
    t_unref(u);
  }
  for (int i = 0; i < v4->len; ++i) {
    TGCUnit* u = t_vector_get(v4, i);
    if (u == NULL) continue;
    t_unref(u);
  }
  t_unref(v4);

  u = NULL;
  TVector* v5 = get_vector();
  while ((u = t_vector_pop_back(v5), u != NULL)) {
    t_unref(u);
  }
  t_unref(v5);

  u = NULL;
  TVector* v6 = t_vector_new(0, 0);
  for (int i = 0; i < 128; ++i) {
    char* str = strdup("foo");
    TGCUnit* u = t_gcunit_new_(str, t_free);
    t_vector_push_front_(v6, u);
    t_unref(u);
  }
  while ((u = t_vector_pop(v6), u != NULL)) {
    t_unref(u);
  }
  t_unref(v6);
}

int main() {
  test_object_and_error();
  test_list();
  test_map();
  test_vector();
  return 0;
}
