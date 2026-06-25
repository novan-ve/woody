#include "woody.h"

#include <stdio.h>

static int validate_elf_header_ident(Elf64_Ehdr *ehdr) {
    if (ft_memcmp(ehdr->e_ident, ELFMAG, SELFMAG)) {
        fprintf(stderr, "Invalid magic bytes\n");
        return (-1);
    }
    if (ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
        fprintf(stderr, "File architecture not supported. x86_64 only\n");
        return (-1);
    }
    if (ehdr->e_ident[EI_DATA] == ELFDATANONE || ehdr->e_ident[EI_DATA] >= ELFDATANUM) {
        fprintf(stderr, "Invalid encoding\n");
        return (-1);
    }
    if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) {
        fprintf(stderr, "Invalid version\n");
        return (-1);
    }
    return (0);
}

int validate_elf_header(Elf64_Ehdr *ehdr) {
    if (validate_elf_header_ident(ehdr) < 0) {
        return (-1);
    }
    if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN) {
        fprintf(stderr, "Unsupported object file type\n");
        return (-1);
    }
    if (ehdr->e_version != EV_CURRENT) {
        fprintf(stderr, "Invalid version\n");
        return (-1);
    }
    if (ehdr->e_ehsize != sizeof(Elf64_Ehdr)) {
        fprintf(stderr, "Invalid elf header size\n");
    }
    if (ehdr->e_phentsize != sizeof(Elf64_Phdr)) {
        fprintf(stderr, "Invalid program header size\n");
        return (-1);
    }
    if (ehdr->e_shentsize != sizeof(Elf64_Shdr)) {
        fprintf(stderr, "Invalid section header size\n");
        return (-1);
    }
    if (!ehdr->e_phnum && !ehdr->e_shnum) {
        fprintf(stderr, "No program/section headers found\n");
        return (-1);
    }
    return (0);
}
