#pragma once

#include <stdint.h>

/*
 * Map Pair
 */

typedef struct TMapPair {
  char* key;
  uint64_t hash;
  TGCUnit* unit;
  t_gcunit_use();
} TMapPair;

TMapPair* t_map_pair_new(char* key, TGCUnit* unit);
void t_map_pair_destroy(TMapPair* self);
void t_map_pair_set(TMapPair* self, TGCUnit* unit);
#define t_map_pair_get(self) (t_ref(self->unit), self->unit)
void t_map_pair_rename(TMapPair* self, char* key);

/*
 * Map
 */

typedef struct TMap {
  TList* pairs;
  t_gcunit_use();
} TMap;

TMap* t_map_new();
void t_map_destroy(TMap* self);
TMapPair* t_map_get(TMap* self, char* key);
TMapPair* t_map_set(TMap* self, char* key, TGCUnit* unit);
#define t_map_set_(self, key, obj) t_map_set(self, key, obj->t_gcunit_field)
TMapPair* t_map_rename(TMap* self, char* key, char* new_key);
