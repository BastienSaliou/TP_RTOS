#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "package.h"

ChannelData generate_channel_data(uint8_t header){
    ChannelData channel;
    channel.header = header;          // Définit l'en-tête (0x01, 0x02, 0x03)
    generate_random_data(channel.data); // Génère les 3 octets aléatoires
    return channel;

}

Status generate_status() {
    Status status;
    status.header = 04; // En-tête spécifique au statut
    status.status = 00; // Toujours 0 pour simplifier
    return status;
}

void print_channel_data(const ChannelData *channel) {
    printf("%02X %02X %02X %02X\n",
           channel->header, channel->data[0], channel->data[1], channel->data[2]);
}

// Fonction pour afficher le statut avec le format : Header suivi du statut
void print_acquisition_status(const Status *status) {
    printf("%02X %02X\n", status->header, status->status);
}

// Fonction principale pour générer et afficher les données
void generate_package() {
    // Génération des voies X, Y, Z
    ChannelData x = generate_channel_data(01); // Voie X
    ChannelData y = generate_channel_data(02); // Voie Y
    ChannelData z = generate_channel_data(03); // Voie Z

    // Génération du statut
    Status status = generate_status();

    // Affichage des résultats : chaque voie est affichée avec Header + Data
    printf("Generated Package:\n");
    print_channel_data(&x);
    print_channel_data(&y);
    print_channel_data(&z);
    print_acquisition_status(&status);
}

void print_status(const Status *status) {
    printf("0x%02X 0x%02X\n", status->header, status->status);
}
void test_generate_package() {
    printf("Testing Generated Package:\n");

    // Génération des voies X, Y, Z
    ChannelData x = generate_channel_data(0x01);
    ChannelData y = generate_channel_data(0x02);
    ChannelData z = generate_channel_data(0x03);

    // Génération du statut
    Status status = generate_status();

    // Affichage des résultats
    print_channel_data(&x);
    print_channel_data(&y);
    print_channel_data(&z);
    print_status(&status);
}
