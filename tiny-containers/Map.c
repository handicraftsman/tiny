#include <tiny.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint64_t djb2(char* str) {
  uint64_t hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

/*
 * Map Pair
 */

static TMapPair* _t_map_pair_new_(uint64_t hash, char* key, TGCUnit* unit) {
  TMapPair* self = (TMapPair*) t_malloc(sizeof(TMapPair));
  if (!self) {
    perror("t_map_pair_new");
    exit(1);
  }

  t_gcunit(self) = t_gcunit_new_(self, t_map_pair_destroy);
  self->key = key;
  self->hash = hash;
  if (unit != NULL)
    t_ref(unit);
  self->unit = unit;

  return self;
}

TMapPair* t_map_pair_new(char* key, TGCUnit* unit) {
  return _t_map_pair_new_(djb2(key), strdup(key), unit);
}

void t_map_pair_destroy(TMapPair* self) {
  free(self->key);
  if (self->unit != NULL)
    t_unref(self->unit);
}

void t_map_pair_set(TMapPair* self, TGCUnit* unit) {
  if (self->unit != NULL)
    t_unref(self->unit);
  if (unit != NULL)
    t_ref(unit);
  self->unit = unit;
}

void t_map_pair_rename(TMapPair* self, char* key) {
  free(self->key);
  self->key = strdup(key);
  self->hash = djb2(self->key);
}

/*
 * Map
 */

TMap* t_map_new() {
  TMap* self = (TMap*) t_malloc(sizeof(TMap));
  if (!self) {
    perror("t_map_new");
    exit(1);
  }

  t_gcunit(self) = t_gcunit_new_(self, t_map_destroy);
  self->pairs = t_list_new();

  return self;
}

void t_map_destroy(TMap* self) {
  t_unref(self->pairs);
}

static TMapPair* _t_map_get_(TMap* self, uint64_t hash) {
  for (TListNode* n = t_list_first(self->pairs); n != NULL; n = t_list_next(n)) {
    if (((TMapPair*) n->unit->obj)->hash == hash) {
      t_ref((TMapPair*) n->unit->obj);
      t_unref(n);
      return n->unit->obj;
    }
  } 

  return NULL;
}

TMapPair* t_map_get(TMap* self, char* key) {
  return _t_map_get_(self, djb2(key));
}

TMapPair* t_map_set(TMap* self, char* key, TGCUnit* unit) {
  uint64_t hash = djb2(key);
  TMapPair* pair = _t_map_get_(self, hash);
  if (pair == NULL) {
    TMapPair* pair = _t_map_pair_new_(hash, strdup(key), unit);
    TListNode* node = t_list_append_(self->pairs, pair);
    TMapPair* obj = node->unit->obj;
    t_unref(node);
    return obj;
  } else {
    t_map_pair_set(pair, unit); 
    return pair;
  }
}

TMapPair* t_map_rename(TMap* self, char* key, char* new_key) {
  uint64_t hash = djb2(key);
  TMapPair* pair = _t_map_get_(self, hash);
  if (pair != NULL) {
    t_map_pair_rename(pair, new_key);
    return pair;
  } else {
    return NULL;
  }
}