#include "woody.h"

#include <stdio.h>
#include <sys/mman.h>

int free_file(Elf_File *file) {
    int ret = 0;

    if (file->input && munmap(file->input, file->size) < 0) {
        fprintf(stderr, "Failed to unmap input\n");
        ret = -1;
    }
    if (file->output && munmap(file->output, file->size) < 0) {
        fprintf(stderr, "Failed to unmap output\n");
        ret = -1;
    }
    if (file->payload && munmap(file->payload, file->payload_size) < 0) {
        fprintf(stderr, "Failed to unmap payload\n");
        ret = -1;
    }
    if (file->key) {
        free(file->key);
        file->key = NULL;
    }
    return (ret);
}
