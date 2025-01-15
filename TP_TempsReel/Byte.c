#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Génère un entier aléatoire de 8 bits
unsigned char rand8() {
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
    }
    return rand() & 0xFF; // Masque pour limiter à 8 bits
}

// Génère des données pseudo-aléatoires de 8 bits signés
int8_t generate_random_data() {
    return rand8() % 0xFF;
}

// Inverse les octets dans un tableau de 3 octets (big-endian)
int swap_be_8(uint8_t *data) {
    uint8_t temp = data[0];
    data[0] = data[2];
    data[2] = temp;
    return *data;
}
