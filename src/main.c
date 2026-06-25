#include "woody.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

static void print_key(unsigned char *key, size_t len) {
    printf("key_value: ");
    for (size_t i = 0; i < len; i++) {
        printf("%02X", key[i]);
    }
    printf("\n");
}

static void init_elf_file(Elf_File *file) {
    file->ehdr = NULL;
    file->text = NULL;
    file->input = NULL;
    file->output = NULL;
    file->payload = NULL;
    file->payload_text = NULL;
    file->key = NULL;
    file->size = 0;
    file->gap_size = 0;
    file->payload_size = 0;
}

int main(int argc, char **argv) {
    Elf_File file;

    if (argc != 2) {
        fprintf(stderr, "usage: woody_woodpacker FILE\n");
        return (1);
    }
    init_elf_file(&file);
    if ((file.input = open_file(argv[1], &file.size)) == NULL) {
        return (1);
    }
    if ((file.ehdr = get_elf_header(file.input, file.size)) == NULL) {
        free_file(&file);
        return (1);
    }
    if ((file.text = get_text_section(file.input, file.size)) == NULL) {
        free_file(&file);
        return (1);
    }
    file.gap_size = get_gap_size(file.input, file.ehdr, file.text);
    if (file.gap_size == (Elf64_Addr)-1) {
        free_file(&file);
        return (1);
    }
    if ((file.payload = open_file(PAYLOAD_FILE, &file.payload_size)) == NULL) {
        free_file(&file);
        return (1);
    }
    file.payload_text = get_text_section(file.payload, file.payload_size);
    if (file.payload_text == NULL) {
        free_file(&file);
        return (1);
    }
    if (file.payload_text->p_filesz > file.gap_size) {
        fprintf(stderr, "'%s': Payload too big for file\n", PAYLOAD_FILE);
        free_file(&file);
        return (1);
    }
    if ((file.key = encrypt_text(file.input, file.text)) == NULL) {
        free_file(&file);
        return (1);
    }
    if (patch_payload(&file) < 0) {
        free_file(&file);
        return (1);
    }
    if ((file.output = copy_file(OUTPUT_FILE, file.input, file.size)) == NULL) {
        free_file(&file);
        return (1);
    }
    inject_payload(&file);
    patch_entrypoint(file.output, file.text);
    print_key(file.key, KEY_LEN);
    if (free_file(&file) < 0) {
        return (1);
    }
    return (0);
}
