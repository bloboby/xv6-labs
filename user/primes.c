#include "kernel/types.h"
#include "user/user.h"

// read is 0, write is 1.

void sieve(int l[]) {
  close(l[1]);

  int prime;
  if (read(l[0], &prime, 4) == 0) {
    exit(0);
  }
  printf("prime %d\n", prime);

  int r[2];
  pipe(r);
  if (fork() != 0) {
    close(r[0]);

    int n;
    while (read(l[0], &n, 4) == 4) {
      if (n % prime != 0) {
        write(r[1], &n, 4);
      }
    }
    close(l[0]);
    close(r[1]);
    wait((int*)0);
    exit(0);
  } else {
    sieve(r);
  }
}

int main() {
  int p[2];
  pipe(p);

  if (fork() != 0) {
    close(p[0]);

    for (int n=2; n<=35; ++n) {
      write(p[1], &n, 4);
    }
    close(p[1]);
    wait((int*)0);
    exit(0);
  } else {
    sieve(p);
    exit(0);
  }
}
