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
  *err = t_error_new(t_error_domain, TErrors_TEST, self->str->obj);
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
    printf("[%zu:%zu] %s\n", err->domain, err->code, err->message);
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

  printf("List Print #1\n");
  for (TListNode* n = t_list_first(list); n != NULL; n = t_list_next(n)) {
    printf("  %s\n", n->unit->obj);
  }

  TListNode* u4 = t_list_get(list, 2);
  t_list_remove(list, u4);
  t_unref(u4);
  
  printf("List Print #2\n");
  for (TListNode* n = t_list_first(list); n != NULL; n = t_list_next(n)) {
    printf("  %s\n", n->unit->obj);
  }

  t_unref(list);
}

void test_map() {
  TMap* map = t_map_new(map);

  TGCUnit* u1 = t_gcunit_new_(strdup("Bar"), t_free);
  t_unref(t_map_set_(map, "foo", u1));
  t_unref(u1);

  TMapPair* p1 = t_map_get(map, "foo");
  printf("%s\n", ((char*) p1->unit->obj));
  t_unref(p1);
  
  TGCUnit* u2 = t_gcunit_new(strdup("Baz"), t_free);
  t_unref(t_map_set_(map, "foo", u2));
  t_unref(u2);

  TMapPair* p2 = t_map_get(map, "foo");
  printf("%s\n", ((char*) p2->unit->obj));
  t_unref(p2);

  TGCUnit* u3 = t_gcunit_new(strdup("spam"), t_free);
  t_unref(t_map_set_(map, "quux", u3));
  t_unref(u3);

  TMapPair* p3 = t_map_get(map, "quux");
  printf("%s\n", ((char*) p3->unit->obj));
  t_unref(p3);

  t_unref(t_map_rename(map, "quux", "asdf"));

  TMapPair* p4 = t_map_get(map, "asdf");
  printf("%s\n", ((char*) p4->unit->obj));
  t_unref(p4);

  for (TListNode* n = t_list_first(map->pairs); n != NULL; n = t_list_next(n)) {
    TMapPair* p = n->unit->obj;
    printf("%s: %s\n", p->key, p->unit->obj);
  }

  t_unref(map);
}

TQueue* get_queue() {
  TQueue* queue = t_queue_new(0, 0);

  for (int i = 0; i < 128; ++i) {
    char* str;
    asprintf(&str, "%d", i);
    TGCUnit* u = t_gcunit_new_(str, t_free);
    t_queue_push(queue, u);
    t_unref(u);
  }

  return queue;
}

void test_queue() {
  TQueue* q1 = get_queue();

  for (int i = 0; i < q1->len; ++i) {
    printf("%s\n", q1->arr[i]->obj);
  }

  t_unref(q1);


  TQueue* q2 = get_queue();

  for (TGCUnit* u = t_queue_pop(q2); u != NULL; u = t_queue_pop(q2)) {
    printf("%s\n", u->obj);
    t_unref(u);
  }

  t_unref(q2);
}

int main() {
  test_object_and_error();
  test_list();
  test_map();
  test_queue();
  return 0;
}