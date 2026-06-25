#include "woody.h"

#include <stdio.h>

static int patch_address(unsigned char *data, unsigned int old, unsigned int new, size_t len) {
    for (size_t i = 0; i <= len - 4; i++) {
        if (*(unsigned int*)(data + i) == old) {
            *(unsigned int*)(data + i) = new;
            return (0);
        }
    }
    return (-1);
}

static int insert_key(unsigned char *data, unsigned char *placeholder, 
                      unsigned char *key, size_t len) {
    for (size_t i = 0; i <= len - KEY_LEN; i++) {
        if (!ft_memcmp(data + i, placeholder, KEY_LEN)) {
            ft_memcpy(data + i, key, KEY_LEN);
            return (0);
        }
    }
    return (-1);
}

int patch_payload(Elf_File *file) {
    unsigned int jmp_addr = 0;
    unsigned char *data = file->payload + file->payload_text->p_offset;
    size_t len = file->payload_text->p_filesz;
    size_t size;

    size = file->text->p_memsz + (file->text->p_vaddr % file->text->p_align);
    if (patch_address(data, SIZE_PLACEHOLDER, size, len) < 0) {
        fprintf(stderr, "'%04x': Pattern not found in payload\n", SIZE_PLACEHOLDER);
        return (-1);
    }
    if (patch_address(data, OFFSET_PLACEHOLDER, file->text->p_memsz, len) < 0) {
        fprintf(stderr, "'%04x': Pattern not found in payload\n", OFFSET_PLACEHOLDER);
        return (-1);
    }
    jmp_addr -= file->text->p_vaddr + file->text->p_memsz + len - file->ehdr->e_entry;
    if (patch_address(data, JUMP_PLACEHOLDER - 4, jmp_addr, len) < 0) {
        fprintf(stderr, "'%04x': Pattern not found in payload\n", JUMP_PLACEHOLDER);
        return (-1);
    }
    if (insert_key(data, (unsigned char*)KEY_PLACEHOLDER, file->key, len) < 0) {
        fprintf(stderr, "'%s': Pattern not found in payload\n", KEY_PLACEHOLDER);
        return (-1);
    }
    return (0);
}

void inject_payload(Elf_File *file) {
    unsigned char *dst = file->output + file->text->p_offset + file->text->p_filesz;
    unsigned char *src = file->payload + file->payload_text->p_offset;
    size_t len = file->payload_text->p_filesz;

    ft_memcpy(dst, src, len);
}

void patch_entrypoint(unsigned char *data, Elf64_Phdr *text) {
    ((Elf64_Ehdr*)data)->e_entry = text->p_vaddr + text->p_memsz;
}
