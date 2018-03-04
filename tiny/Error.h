#pragma once

#include "tiny.h"

#include <stdbool.h>
#include <stddef.h>

/*
 * Error
 */

typedef struct TError {
  char* message;
  bool sys;
  t_gcunit_use();
} TError;

TError* t_error_new(char* message, bool sys);
void t_error_destroy(TError* self);

#define t_error_clear(err) if (err != NULL) { t_unref(err); } err = NULL
#define t_error_handle_sys(err, name, act) \
  if (err != NULL && err->sys == true) { \
    perror(name); \
    (act); \
  }
