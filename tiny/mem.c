#include <tiny.h>

#include <stdlib.h>

void* (*t_malloc) (size_t size)            = malloc;
void* (*t_calloc) (size_t nr, size_t size) = calloc;
void* (*t_realloc)(void* ptr, size_t size) = realloc;
void  (*t_free)   (void* ptr)              = free;