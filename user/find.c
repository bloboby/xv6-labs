#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *dir, char *name) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(dir, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", dir);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", dir);
    return;
  }

  if (st.type != T_DIR) {
    fprintf(2, "find: %s is not a directory\n", dir);
    return;
  }

  strcpy(buf, dir);
  p = buf+strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) {
      continue;
    }

    memcpy(p, de.name, sizeof(de.name));
    if (stat(buf, &st) < 0) {
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    switch (st.type) {
      case T_FILE:
        if (strcmp(de.name, name) == 0) {
          printf("%s\n", buf);
        }
        break;

      case T_DIR:
        if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
          find(buf, name);
        }
        break;
    }
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "usage: find [dir] [name]\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
