#include "woody.h"

#include <stdio.h>
#include <fcntl.h>

void encipher(unsigned int num_rounds, unsigned char *data, uint32_t key[4]) {
    unsigned int v0, v1;
    unsigned int delta = 0x9E3779B9;
    unsigned int sum = 0;

    v0 = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
    v1 = data[7] << 24 | data[6] << 16 | data[5] << 8 | data[4];

    for (unsigned int i = 0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
	}

    data[0] = (v0 & 0x000000ff);
    data[1] = (v0 & 0x0000ff00) >> 8;
    data[2] = (v0 & 0x00ff0000) >> 16;
    data[3] = (v0 & 0xff000000) >> 24;
    data[4] = (v1 & 0x000000ff);
    data[5] = (v1 & 0x0000ff00) >> 8;
    data[6] = (v1 & 0x00ff0000) >> 16;
    data[7] = (v1 & 0xff000000) >> 24;
}

unsigned char *encrypt_text(unsigned char *data, Elf64_Phdr *text) {
    unsigned char *key;
    unsigned int k[4];
    int fd;

    key = malloc(KEY_LEN);
    if (!key) {
        return (NULL);
    }
    fd = open(RAND_FILE, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "'%s', Failed to open file\n", RAND_FILE);
        free(key);
        return (NULL);
    }
    if (read(fd, key, KEY_LEN) < KEY_LEN) {
        fprintf(stderr, "'%s', Failed to read file\n", RAND_FILE);
        free(key);
        return (NULL);
    }
    if (close(fd) < 0) {
        fprintf(stderr, "'%s', Failed to close file\n", RAND_FILE);
        free(key);
        return (NULL);
    }

    k[0] = key[0]  | key[1]  << 8 | key[2]  << 16 | key[3]  << 24;
    k[1] = key[4]  | key[5]  << 8 | key[6]  << 16 | key[7]  << 24;
    k[2] = key[8]  | key[9]  << 8 | key[10] << 16 | key[11] << 24;
    k[3] = key[12] | key[13] << 8 | key[14] << 16 | key[15] << 24;

    for (size_t i = text->p_offset; i + 8 <= text->p_offset + text->p_filesz; i += 8) {
        encipher(NUM_ROUNDS, data + i, k);
    }
    return (key);
}
