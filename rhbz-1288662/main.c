#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>

int main()
{
  char buf[512];
#if 1
  /* this makes the following write() fail */
  int fd = openat (AT_FDCWD, "sparsefile", O_WRONLY | O_CREAT, 420);
#else
  /* this is OK */
  int fd = open ("sparsefile", O_WRONLY | O_CREAT, 420);
#endif
  if ((off_t)-1 == lseek (fd, 8589935104, SEEK_SET)) {
    perror ("lseek");
    return 1;
  }

  if ((ssize_t)-1 == write(fd, buf, 512)) {
    perror ("write");
    return 1;
  }

  return 0;
}
