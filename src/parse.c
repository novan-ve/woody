#include "woody.h"

#include <stdio.h>

Elf64_Ehdr *get_elf_header(unsigned char *data, size_t len) {
    Elf64_Ehdr *ehdr;
    
    if (len < sizeof(Elf64_Ehdr)) {
        fprintf(stderr, "File too small to contain ELF header\n");
        return (NULL);
    }
    ehdr = (Elf64_Ehdr*)data;
    if (validate_elf_header(ehdr) < 0) {
        return (NULL);
    }
    return (ehdr);
}

Elf64_Phdr *get_text_section(unsigned char *data, size_t len) {
    Elf64_Phdr *phdr;
    Elf64_Ehdr *ehdr;

    if ((ehdr = get_elf_header(data, len)) == NULL) {
        return (NULL);
    }
    if (len <= ehdr->e_phoff + ehdr->e_phnum * sizeof(Elf64_Phdr)) {
        fprintf(stderr, "File too small to contain all program headers\n");
        return (NULL);
    }
    phdr = (Elf64_Phdr*)(data + ehdr->e_phoff);
    for (Elf64_Half i = 0; i < ehdr->e_phnum; i++) {
        if (phdr->p_type == PT_LOAD && phdr->p_flags == (PF_R | PF_X)) {
            if (phdr->p_offset + phdr->p_filesz > len) {
                fprintf(stderr, "Invalid text segment, offset larger than filesize\n");
                return (NULL);
            }
            return (phdr);
        }
        phdr++;
    }
    fprintf(stderr, "Failed to find text segment\n");
    return (NULL);
}

Elf64_Addr get_gap_size(unsigned char *data, Elf64_Ehdr *ehdr, Elf64_Phdr *text) {
    Elf64_Phdr *phdr;
    Elf64_Addr gap = (Elf64_Addr)-1;
    Elf64_Addr file_end = text->p_offset + text->p_filesz;
    Elf64_Addr mem_end = text->p_vaddr + text->p_memsz;

    phdr = (Elf64_Phdr*)(data + ehdr->e_phoff);
    for (Elf64_Half i = 0; i < ehdr->e_phnum; i++, phdr++) {
        if (phdr->p_type == PT_LOAD &&
            phdr->p_offset > file_end && phdr->p_vaddr > mem_end &&
            phdr->p_offset - file_end < gap && phdr->p_vaddr - mem_end < gap)
        {
            gap = MIN(phdr->p_offset - file_end, phdr->p_vaddr - mem_end);
        }
    }
    if (gap == (Elf64_Addr)-1) {
        fprintf(stderr, "Failed to find gap\n");
    }
    return (gap);
}
