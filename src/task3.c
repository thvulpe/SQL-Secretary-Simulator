#include "task3.h"

#define NR_BLOCKS 4

// Rotunjire in sus
int rotunjire_sus(float x) {
    if (x == (float)((int)x)) {
        return (int)x;
    }
    return (int)(x + 1);
}

// Separarea datelor in NR_BLOCKS blocuri de dimensiune fixa
// si adaugare padding cu 0x00 unde este necesar
void** split_into_blocks_and_pad(student* studenti, int nr_studenti, size_t* block_size) {
    // Copiez intr-un block mare toate elementele vectorului studenti
    void* block = malloc(nr_studenti * sizeof(student));
    memcpy(block, studenti, nr_studenti * sizeof(student));
    // Rotunjesc dimensiunea blockului in sus daca impartirea nu este exacta
    *block_size = rotunjire_sus((float)nr_studenti * sizeof(student) / NR_BLOCKS);

    // Impart in blockuri
    void** blocks = malloc(NR_BLOCKS * sizeof(void*));
    if (blocks == NULL) {
        printf("Nu s-a putut aloca suficienta memorie.\n");
        free(block);
        return NULL;
    }
    for (int i = 0; i < NR_BLOCKS; i++) {
        blocks[i] = malloc(*block_size);
        if (blocks[i] == NULL) {
            printf("Nu s-a putut aloca suficienta memorie.\n");
            for (int j = 0; j < i; j++) {
                free(blocks[j]);
            }
            free(blocks);
            free(block);
            return NULL;
        }
        // Paddingul cu 0x00
        memset(blocks[i], 0, *block_size);
    }

    for (int i = 0 ; i < NR_BLOCKS; i++) {
        // Daca copiez in ultimul block am grija sa nu sparg memoria
        if (i == NR_BLOCKS - 1) {
            int addition = NR_BLOCKS * *block_size - nr_studenti * sizeof(student);
            memcpy(blocks[i], block + i * *block_size, *block_size - addition);
        } else {
            memcpy(blocks[i], block + i * *block_size, *block_size);
        }
    }

    free(block);
    return blocks;
}

// Scriere in fisierul binar
void scriere(char *cale_output, void** blocks, size_t block_size) {
    FILE* out = fopen(cale_output, "wb");
    if (out == NULL) {
        fprintf(stderr, "Nu s-a putut deschide fisierul %s\n", cale_output);
        return;
    }

    for (int i = 0; i < NR_BLOCKS; i++) {
        fwrite(blocks[i], block_size, 1, out);
    }

    fclose(out);
}

// Aplica operatia XOR sau S-BOX
void* XOR(char* block, size_t block_size, char* key, size_t key_len) {
    size_t c = 0;
    for (size_t i = 0; i < block_size; i++) {
        if (c == key_len) {
            c = 0;
        }

        block[i] = (char)(block[i] ^ key[c++]);
    }

    return block;
}

void *P_BOX(char *block, size_t n) {
    char* new_block = malloc(n);
    for (size_t i = 0; i < n; i++) {
        new_block[(i * (n - 1) + 2) % n] = block[i];
    }
    // Eliberez memoria alocata vechiului block
    free(block);
    return new_block;
}

void cripteaza_studenti(secretariat *secretariat, void *key, size_t key_len,
                        void *iv, size_t iv_len, char *cale_output) {
    // TODO(student): Cripteaza vectorul secretariat->studenti si scrie rezultatul in fisierul cale_output
    size_t block_size = 0;
    void** blocks = split_into_blocks_and_pad(secretariat->studenti,
        secretariat->nr_studenti, &block_size);

    blocks[0] = XOR(blocks[0], block_size, iv, iv_len);

    blocks[0] = XOR(blocks[0], block_size, key, key_len);

    blocks[0] = P_BOX(blocks[0], block_size);

    for (int i = 1; i < NR_BLOCKS; i++) {
        blocks[i] = XOR(blocks[i], block_size, blocks[i - 1], block_size);
        blocks[i] = XOR(blocks[i], block_size, key, key_len);
        blocks[i] = P_BOX(blocks[i], block_size);
    }

    scriere(cale_output, blocks, block_size);

    for (int i = 0; i < NR_BLOCKS; i++) {
        free(blocks[i]);
    }
    free(blocks);
}
