#pragma once

#include "../libft/include/libft.h"

#include <elf.h>

#define OUTPUT_FILE "woody"
#define PAYLOAD_FILE "asm/payload"
#define RAND_FILE "/dev/urandom"
#define KEY_LEN 16
#define NUM_ROUNDS 32
#define SIZE_PLACEHOLDER 0x40404040
#define OFFSET_PLACEHOLDER 0x41414141
#define JUMP_PLACEHOLDER 0x42424242
#define KEY_PLACEHOLDER "_key_placeholder"
#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct {
    Elf64_Ehdr *ehdr;
    Elf64_Phdr *text;
    Elf64_Phdr *payload_text;
    unsigned char *input;
    unsigned char *output;
    unsigned char *payload;
    unsigned char *key;
    size_t size;
    Elf64_Addr gap_size;
    size_t payload_size;
} Elf_File;

int free_file(Elf_File *file);

unsigned char *encrypt_text(unsigned char *data, Elf64_Phdr *text);

int patch_payload(Elf_File *file);
void inject_payload(Elf_File *file);
void patch_entrypoint(unsigned char *data, Elf64_Phdr *text);

Elf64_Ehdr *get_elf_header(unsigned char *data, size_t len);
Elf64_Phdr *get_text_section(unsigned char *data, size_t len);
Elf64_Addr get_gap_size(unsigned char *data, Elf64_Ehdr *ehdr, Elf64_Phdr *text);

ssize_t file_size(int fd);
unsigned char *open_file(const char *filename, size_t *size);
unsigned char *copy_file(const char *filename, unsigned char *data, size_t size);

int validate_elf_header(Elf64_Ehdr *ehdr);
