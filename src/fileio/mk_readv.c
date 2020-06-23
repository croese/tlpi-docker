#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>

#define STR_SIZE 100

int main(int argc, char* argv[]) {
  int fd;
  struct stat myStruct;             // first buffer
  int x = 42;                       // second buffer
  char* str = "hello readv world!"; // third buffer

  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    usageErr("%s file\n", argv[0]);
  }

  int statResult = stat("tfile", &myStruct);
  if (statResult == -1) {
    errExit("stat");
  }

  fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  if (fd == -1) {
    errExit("open");
  }

  ssize_t numWrote = write(fd, &myStruct, sizeof(struct stat));
  if (numWrote == -1) {
    errExit("write stat");
  }
  numWrote = write(fd, &x, sizeof(x));
  if (numWrote == -1) {
    errExit("write int");
  }
  numWrote = write(fd, str, strlen(str) + 1);
  if (numWrote == -1) {
    errExit("write str");
  }

  if (close(fd) == -1) {
    errExit("close");
  }

  exit(EXIT_SUCCESS);
}