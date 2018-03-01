#pragma once

#include <stddef.h>

/*
 * Error Domain
 */

typedef size_t TErrorDomain;
typedef size_t TErrorCode;

TErrorDomain t_error_domain;
TErrorDomain t_error_domain_sys;
TErrorDomain t_error_domain_counter;

inline TErrorDomain t_error_domain_get() { return ++t_error_domain; }

typedef enum TErrors {
  TErrors_TEST = 1
} TErrors;

/*
 * Error
 */

typedef struct TError {
  TErrorDomain domain;
  TErrorCode code;
  char* message;
  t_gcunit_use();
} TError;

TError* t_error_new(TErrorDomain domain, TErrorCode code, char* message);
void t_error_destroy(TError* self);

#define t_error_is(err, domain, code) (err->domain == domain && err->code == code)
#define t_error_clear(err) if (err != NULL) { t_unref(err); } err = NULL
#define t_error_handle_sys(err, name, act) \
  if (err != NULL && err->domain == t_error_domain_sys) { \
    perror(name); \
    (act); \
  }
