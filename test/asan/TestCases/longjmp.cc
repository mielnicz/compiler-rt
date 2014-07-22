// RUN: %clangxx_asan -O %s -o %t && %run %t

#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sanitizer/asan_interface.h>

static jmp_buf buf;

void TestLongJmp() {
  char x[32];
  fprintf(stderr, "\nTestLongJmp\n");
  fprintf(stderr, "Before: %p poisoned: %d\n", &x,
          __asan_address_is_poisoned(x + 32));
  assert(__asan_address_is_poisoned(x + 32));
  if (0 == setjmp(buf))
    longjmp(buf, 1);
  fprintf(stderr, "After:  %p poisoned: %d\n",  &x,
          __asan_address_is_poisoned(x + 32));
  // FIXME: Invert this assertion once we fix
  // https://code.google.com/p/address-sanitizer/issues/detail?id=258
  assert(!__asan_address_is_poisoned(x + 32));
}

int main(int argc, char **argv) {
  TestLongJmp();
}