# Tiny

## Building & Installing

```bash
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
$ sudo make install
```

## Creating classes

The most basic feature of this library - garbage-collected classes defined by structure. To define such class, define structure like that:

```c
typedef struct Sample {
  // ...
  t_gcunit_use(); // Includes reference-counted garbage collector unit.
} Sample;

Sample* sample_new();
void sample_destroy(Sample* self);
```

Defining it:

```c

#include <stdio.h>
#include <stdlib.h>

Sample* sample_new() {
  // t_malloc is just a global variable which allows choosing your allocator.
  // Arguments for custom allocators must be same as in stdlib's ones.
  Sample* self = (Sample*) t_malloc(sizeof(Sample));
  if (!self) {
    perror("sample_new");
    exit(1);
  }

  // t_gcunit(obj) is a macro for selecting GC Unit in a given object.
  // Underline-suffixed version of t_gcunit_new casts variables (self to void*
  // and sample_destroy to TGCUnitDestructor) automatically.
  // If object's reference count drops to 0, it gets destroyed.
  t_gcunit(self) = t_gcunit_new_(self, sample_destroy);

  // Initialize your stuff here

  return self;
}

void sample_destroy(Sample* self) {
  // Destroy your stuff here by using `t_unref(obj)` on it.
}

```

Now this class can be used like that:

```c
int main() {
  Sample* sample = sample_new();

  t_unref(sample);
  return 0;
}
```

## Errors

Errors are objects used to inform function callers about any errors that may occur
in the function.

Each error has domain (library code, all libraries which use errors (except for
tiny) must get their own at the start of main() function), error code (defined
in libraries, best way to do that is enum) and message (so you can print your
error message).

Usage:

```c
TErrorDomain sample_error_domain = 0; // Warning: 0 is always taken by tiny

typedef enum SampleErrors {
  SampleErrors_SAMPLE = 1,
}

void sample_init() {
  sample_error_domain = t_error_domain_get();
}

void sample_error(TError** err) {
  // Return an error using such pattern:
  *err = t_error_new(sample_error_domain, SampleErrors_SAMPLE, "Sample Error");
  return;
  // return NULL if you return a pointer
  // return -1 if you return a signed integer
}

int main() {
  sample_init(); // sample gets an own error domain

  printf("%u", sample_error_domain);

  TError* err = NULL;
  
  sample_error(&err);
  if (err != NULL) {
    printf("Error: [%u:%u] %s\n", err->domain, err->code, err->message);
  }

  // sets err back to NULL. err's reference count will be decreased automatically
  t_error_clear(err);

  return 0;
}
```

## Examples

All examples can be found in the test.c file