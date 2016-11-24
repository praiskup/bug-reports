#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 512

void write_chunks (int fd, int count, int fill)
{
    char buffer[BUFFER_SIZE];
    memset (buffer, fill, BUFFER_SIZE);

    for (int i = 0; i < count; ++i) {
        if (write (fd, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
            fprintf (stderr, "please fix the reproducer\n");
            exit (1);
        }
    }
}

void make_hole (int fd, off_t size)
{
  if (lseek (fd, size, SEEK_CUR) != -1) {
    ftruncate (fd, lseek (fd, 0, SEEK_CUR));
    return;
  }

  fprintf (stderr, "lseek fail");
  exit (1);
}

int main (int argc, char **argv)
{
    char buffer[BUFFER_SIZE];
    off_t written;

    for (int i = 1; i < argc; i++) {
      int fd = open (
              "sparsefile",
              O_CREAT | O_RDWR | O_TRUNC,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
      );

      if (!fd) {
          fprintf (stderr, "can't open file %s\n", "sparsefile");
          return 1;
      }

      write_chunks (fd, atoi(argv[i]), 'A');
      make_hole (fd, 1024*1024);
      write_chunks (fd, atoi(argv[i]), 'A');
      close (fd);
    }
}
