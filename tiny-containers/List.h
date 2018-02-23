#pragma once

#include <stdbool.h>

/*
 * List Node
 */

typedef struct TList TList;
typedef struct TListNode TListNode;

typedef struct TListNode {
  TGCUnit* unit;
  TListNode* prev;
  TListNode* next;
  TList* list;
  t_gcunit_use();
} TListNode;

TListNode* t_list_node_new(TGCUnit* unit);
void t_list_node_destroy(TListNode* self);

void t_list_node_set(TListNode* self, TGCUnit* unit);
#define t_list_node_set_(self, obj) t_list_node_set(self, obj->t_gcunit_field)

#define t_list_node_link(l, r) if (l != NULL) { l->next = r; }; if (r != NULL) { r->prev = l; }  
#define t_list_node_link3(l, c, r) t_list_node_link(l, c); t_list_node_link(c, r)
#define t_list_node_unlink(n) if (n != NULL) { if (n->prev != NULL) { n->prev->next = NULL; }; n->prev = NULL; if (n->next != NULL) { n->next->prev = NULL; }; n->next = NULL; }

/*
 * List
 */

typedef struct TList {
  TListNode* first;
  TListNode* last;
  t_gcunit_use();
} TList;

TList* t_list_new();
void t_list_destroy(TList* self);

TListNode* t_list_append(TList* self, TGCUnit* unit);
#define t_list_append_(self, obj) t_list_append(self, (obj)->t_gcunit_field)

TListNode* t_list_prepend(TList* self, TGCUnit* unit);
#define t_list_prepend_(self, obj) t_list_prepend(self, (obj)->t_gcunit_field)

TListNode* t_list_insert(TList* self, TGCUnit* unit, size_t idx);
#define t_list_insert_(self, obj, idx) t_list_insert(self, obj->t_gcunit_field, idx)

#define t_list_first(self) (self->first == NULL ? NULL : t_ref_(self->first))
TListNode* t_list_next(TListNode* node);

TListNode* t_list_get(TList* self, size_t idx);

bool t_list_swap(TList* self, TListNode* n1, TListNode* n2);

void t_list_remove(TList* self, TListNode* node);