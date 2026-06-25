#include "woody.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

static int close_file(int fd, const char *filename) {
    if (close(fd) < 0) {
        fprintf(stderr, "'%s': Failed to close file\n", filename);
        return (-1);
    }
    return (0);
}

static ssize_t get_file_size(int fd) {
    ssize_t begin;
    ssize_t size;

    if ((begin = lseek(fd, 0, SEEK_CUR)) == (off_t)-1) {
        return (-1);
    }
    if ((size = lseek(fd, 0, SEEK_END)) == (off_t)-1) {
        return (-1);
    }
    if (lseek(fd, begin, SEEK_SET) == (off_t)-1) {
        return (-1);
    }
    return (size);
}

unsigned char *open_file(const char *filename, size_t *size) {
    unsigned char *input;
    ssize_t file_size;
    int fd;

    if ((fd = open(filename, O_RDONLY)) < 0) {
        fprintf(stderr, "'%s': Failed to open file\n", filename);
        return (NULL);
    }
    if ((file_size = get_file_size(fd)) < 0) {
        close_file(fd, filename);
        return (NULL);
    }
    input = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (input == MAP_FAILED) {
        fprintf(stderr, "'%s': Failed to map file\n", filename);
        close_file(fd, filename);
        return (NULL);
    }
    if (close_file(fd, filename) < 0) {
        return (NULL);
    }
    *size = (size_t)file_size;
    return (input);
}

unsigned char *copy_file(const char *filename, unsigned char *data, size_t size) {
    mode_t permissions = S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;
    unsigned char *output;
    int fd;

    fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, permissions);
    if (fd < 0) {
        fprintf(stderr, "'%s': Failed to open file\n", filename);
        return (NULL);
    }
    if (lseek(fd, size - 1, SEEK_SET) == (off_t)-1) {
        fprintf(stderr, "'%s': Failed to seek to size - 1\n", filename);
        close_file(fd, filename);
        return (NULL);
    }
    if (write(fd, "\0", 1) < 0) {
        fprintf(stderr, "'%s': Failed to write \\0 byte\n", filename);
        close_file(fd, filename);
        return (NULL);
    }
    if ((output = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        fprintf(stderr, "'%s': Failed to map file\n", filename);
        close_file(fd, filename);
        return (NULL);
    }
    ft_memcpy(output, data, size);
    if (close_file(fd, filename) < 0) {
        if (munmap(output, size) < 0) {
            fprintf(stderr, "'%s': Failed to unmap file\n", filename);
        }
        return (NULL);
    }
    return (output);
}
