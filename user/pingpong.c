#include "kernel/types.h"
#include "user/user.h"

int main() {
  int p[2];
  pipe(p);  // read is 0, write is 1

  if (fork() != 0) {
    // parent
    char byte = 'x';

    write(p[1], &byte, 1);
    close(p[1]);
    wait((int*)0);

    read(p[0], &byte, 1);
    close(p[0]);
    printf("%d: received pong\n", getpid());
    exit(0);
  } else {
    // child
    char byte;

    read(p[0], &byte, 1);
    close(p[0]);
    printf("%d: received ping\n", getpid());

    write(p[1], &byte, 1);
    close(p[1]);
    exit(0);
  }
}
