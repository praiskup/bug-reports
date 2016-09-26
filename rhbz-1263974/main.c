#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

/* 100 MB */
#define SHM_BLOCK_SIZE 100*1024*1024

void die(const char * msg)
{
    printf(" # %s\n", msg);
    exit(1);
}


void my_mmap(int id)
{
    char filename[100];
    sprintf(filename, "/test-shm-mapping-%d", id);
    void *ptr;

    shm_unlink(filename);
    int fd = shm_open(filename, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd == -1)
        die("shm_open fail");

    if (ftruncate(fd, SHM_BLOCK_SIZE) == -1)
        die("ftruncate fail");


    ptr = mmap(NULL, SHM_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (id == 1)
        printf (" * started mmaping from %.8p\n", ptr);
}

int main(int argc, char **argv)
{
    char errmsg[70];
    char cmd[100];
    char *endptr = NULL;
    void * ptr;

    if (argc < 3)
        die("COUNT_MAPS(100M) COUNT_MALLOCS(511k) args expected");

    int count_maps = strtol(argv[1], &endptr, 10);
    if (!endptr || *endptr) {
        sprintf(errmsg, "'%s' is not valid number", argv[1]);
        die(errmsg);
    }

    printf(" * first malloc starts at: %.8p\n", malloc(512*1024));

    int count_mallocs = strtol(argv[2], &endptr, 10);
    if (!endptr || *endptr) {
        sprintf(errmsg, "'%s' is not valid number", argv[1]);
        die(errmsg);
    }

    printf("using %d maps and %d mallocs\n", count_maps, count_mallocs);

    for (int i = 0; i < count_maps; i++)
        my_mmap(i);

    // this is to have [heap] mapping
    for (int i = 0; i < count_mallocs; i++) {
        ptr = malloc(511*1024);
        if (i == 0)
            printf(" * after-mmap malloc() starts from %.8p\n", ptr);
    }
    printf(" * after-mmap malloc() ends on %.8p\n", ptr);

    printf("My PID is %d\n", getpid());

    sprintf(cmd, "cat /proc/%d/maps", getpid());
    system(cmd);

    sleep(1000);
}
