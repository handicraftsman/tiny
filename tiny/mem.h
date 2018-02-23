#pragma once

#include <stddef.h>

void* (*t_malloc)(size_t size);
void* (*t_calloc)(size_t nr, size_t size);
void* (*t_realloc)(void* ptr, size_t size);
void (*t_free)(void* ptr);