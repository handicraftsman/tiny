#include <tiny.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Error Domain
 */

TErrorDomain t_error_domain = 0;
TErrorDomain t_error_domain_sys = 1;
TErrorDomain t_error_domain_counter = 1;

/* 
 * Error
 */

TError* t_error_new(TErrorDomain domain, TErrorCode code, char* message) {
  TError* self = (TError*) t_malloc(sizeof(TError));
  if (!self) {
    perror("t_error_new");
    exit(1);
  }

  t_gcunit(self) = t_gcunit_new_(self, t_error_destroy);

  self->domain  = domain;
  self->code    = code;
  self->message = strdup(message);

  return self;
}

void t_error_destroy(TError* self) {
  t_free(self->message);
}
