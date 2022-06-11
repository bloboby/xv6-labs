#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  char *args[MAXARG];
  char buf[512], c;
  int apos = 0, bpos = 0;

  if (argc < 1) {
    fprintf(2, "usage: xargs [cmd] (args)");
    exit(1);
  }

  // Add argv into args.
  for (int i=1; i<argc; ++i) {
    args[apos++] = argv[i];
  }
  args[apos++] = buf;

  while (read(0, &c, 1) == 1) {
    if (c == '\n') {
      buf[bpos++] = 0;

      // execute command
      if (fork() == 0) {
        exec(args[0], args);
        exit(0);
      } else {
        wait(0);
      }

      // reset buf
      memset(buf, 0, sizeof buf);
      bpos = 0;
    } else {
      buf[bpos++] = c;
    }
  }

  exit(0);
}
