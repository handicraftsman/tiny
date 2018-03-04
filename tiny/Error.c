#include <tiny.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 * Error
 */

TError* t_error_new(char* message, bool sys) {
  TError* self = (TError*) t_malloc(sizeof(TError));
  if (!self) {
    perror("t_error_new");
    exit(1);
  }

  t_gcunit(self) = t_gcunit_new_(self, t_error_destroy);

  self->message = strdup(message);
  self->sys     = sys;

  return self;
}

void t_error_destroy(TError* self) {
  t_free(self->message);
}
