#include "tlpi_hdr.h"
#include <fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

void writeOutput(int fd, void* buf, size_t count) {
  int numWrote = write(fd, buf, count);
  if (numWrote == -1) {
    errExit("write");
  } else if (numWrote != count) {
    fatal("couldn't write whole buffer to stdout");
  }
}

int main(int argc, char* argv[]) {
  int openFlags, fd;
  ssize_t numRead;
  char buf[BUF_SIZE];
  char* fileName;

  if (argc < 2 || strcmp(argv[1], "--help") == 0) {
    usageErr("%s [-a] file\n", argv[0]);
  }

  int append = strcmp(argv[1], "-a") == 0;
  openFlags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);

  fileName = append ? argv[2] : argv[1];
  fd = open(fileName, openFlags,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH |
                S_IWOTH); /* rw-rw-rw- */

  if (fd == -1) {
    errExit("open");
  }

  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    writeOutput(STDOUT_FILENO, buf, numRead);
    writeOutput(fd, buf, numRead);
  }

  if (numRead == -1) {
    errExit("read");
  }

  if (close(fd) == -1) {
    errExit("close input");
  }

  exit(EXIT_SUCCESS);
}