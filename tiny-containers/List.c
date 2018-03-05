#include <tiny.h>

#include <stdio.h>
#include <stdlib.h>

/*
 * List Node
 */

TListNode* t_list_node_new(TGCUnit* unit) {
  TListNode* self = (TListNode*) t_malloc(sizeof(TListNode));
  if (!self) {
    perror("t_list_node_new");
    exit(1);
  }

  t_gcunit(self) = t_gcunit_new_(self, t_list_node_destroy);
  
  if (unit != NULL) {
    self->unit = unit;
    t_ref(self->unit);
  } else {
    self->unit = NULL;
  }
  self->prev = NULL;
  self->next = NULL;

  return self;
}

void t_list_node_destroy(TListNode* self) {
  if (self->next != NULL)
    self->next->prev = self->prev;

  if (self->prev != NULL)
    self->prev->next = self->next;

  if (self->unit != NULL) {
    t_unref(self->unit);
  }
}

void t_list_node_set(TListNode* self, TGCUnit* unit) {
  if (self->unit != NULL)
    t_unref(self->unit);
  self->unit = unit;
  t_ref(self->unit);
}

/*
 * List
 */

TList* t_list_new() {
  TList* self = (TList*) t_malloc(sizeof(TList));
  if (!self) {
    perror("t_list_new");
    exit(1);
  }

  t_gcunit(self) = t_gcunit_new_(self, t_list_destroy);
  self->first = NULL;
  self->last = NULL;

  return self;
}

void t_list_destroy(TList* self) {
  TListNode* next = NULL;
  TListNode* n = self->first;
  while (n != NULL) {
    next = n->next;
    t_list_node_unlink(n);
    t_unref(n);
    n = next;
  }
}

TListNode* t_list_append(TList* self, TGCUnit* unit) {
  TListNode* node = t_list_node_new(unit);
  t_ref(node);

  if (self->first == NULL) {
    self->first = node;
  }

  if (self->last == NULL) {
    self->last = node;
  } else {
    t_list_node_link(self->last, node);
    self->last = node;
  }

  node->list = self;

  return node;
}

TListNode* t_list_prepend(TList* self, TGCUnit* unit) {
  TListNode* node = t_list_node_new(unit);
  t_ref(node);

  if (self->first == NULL) {
    self->first = node;
  } else {
    t_list_node_link(node, self->first);
    self->first = node;
  }

  if (self->last == NULL) {
    self->last = node;
  }

  node->list = self;

  return node;
}

TListNode* t_list_insert(TList* self, TGCUnit* unit, size_t idx) {
  TListNode* node = t_list_node_new(unit);
  t_ref(node);
 
  node->list = self;

  if (self->first == NULL && self->last == NULL) {
    self->first = node;
    self->last = node;
    return node;
  }

  if (idx == 0) {
    // Prepend
    t_list_node_link(node, self->first);
    self->first = node;
    return node;
  }

  if (self->first == NULL) {
    // Add first node
    self->first = node;
    return node;
  }

  size_t i = 0;
  TListNode* n = self->first;
  while (true) {
    TListNode* next = n->next;
    
    if (next == NULL) {
      // Append
      t_list_node_link(n, node);
      self->last = node;
      return node;
    }

    if (i == idx) {
      // Insert
      t_list_node_link3(n, node, next);
      return node;
    }
    
    n = next;
    ++i;
  }

  return node;
}

TListNode* t_list_next(TListNode* node) {
  TListNode* next = node->next;
  t_unref(node);
  if (node->next != NULL) {
    t_ref(next);
    return next;
  } else {
    return NULL;
  }
}

TListNode* t_list_get(TList* self, size_t idx) {
  int i = 0;
  for (TListNode* n = t_list_first(self); n != NULL; n = t_list_next(n)) {
    if (i == idx) {
      // n's reference count is already incremented by t_list_next()
      return n;
    }
    ++i;
  }
  return NULL;
}

bool t_list_swap(TList* self, TListNode* n1, TListNode* n2) {
  if (n1->list != self || n2->list != self) return false;

  TListNode* l1 = n1->prev; TListNode* r1 = n1->next;
  TListNode* l2 = n2->prev; TListNode* r2 = n2->next;

  t_list_node_unlink(n1);
  t_list_node_unlink(n2);

  t_list_node_link3(l1, n2, r1);
  t_list_node_link3(l2, n1, r2);

  if (self->first == n1) {
    self->first = n2;
  } else if (self->first == n2) {
    self->first = n1;
  }

  if (self->last == n1) { 
    self->last = n2;
  } else if (self->last == n2) {
    self->last = n1;
  }

  return true;
}

void t_list_remove(TList* self, TListNode* node) {
  if (node->list != self) return;

  if (node == self->first && node == self->last) {
    self->first = NULL;
    self->last = NULL;
  }

  if (node == self->first)
    self->first = node->next;

  if (node == self->last)
    self->last = node->prev;

  TListNode* l = node->prev; TListNode* r = node->next;
  t_list_node_unlink(node);
  t_list_node_link(l, r);

  t_unref(node);
}