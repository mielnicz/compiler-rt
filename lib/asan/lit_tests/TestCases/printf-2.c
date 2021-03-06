// RUN: %clang_asan -O2 %s -o %t
// RUN: ASAN_OPTIONS=check_printf=1 not %t 2>&1 | FileCheck --check-prefix=CHECK-ON %s
// RUN: ASAN_OPTIONS=check_printf=0 %t 2>&1 | FileCheck --check-prefix=CHECK-OFF %s
// RUN: %t 2>&1 | FileCheck --check-prefix=CHECK-OFF %s

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
  volatile char c = '0';
  volatile int x = 12;
  volatile float f = 1.239;
  volatile char s[] = "34";
  char *p = strdup((const char *)s);
  free(p);
  printf("%c %d %.3f %s\n", c, x, f, p);
  return 0;
  // Check that %s is sanitized.
  // CHECK-ON: heap-use-after-free
  // CHECK-ON-NOT: 0 12 1.239 34
  // CHECK-OFF: 0 12 1.239 
}
