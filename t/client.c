#include "types/ksocket.h"
#include <stdio.h>

int main(void) {
    KSocket* client = ksocket_new("127.0.0.1", 8080);

    if (!client) {
        printf("Falha ao criar cliente.\n");
        return 1;
    }

    if (!ksocket_connect(client)) {
        printf("Falha ao conectar.\n");
        ksocket_free(&client);
        return 1;
    }

    printf("Conectado ao servidor!\n");

    char buffer[128] = { 0 };

    if (!ksocket_receive(client, buffer, sizeof(buffer) - 1)) {
        printf("Falha ao receber mensagem.\n");
        ksocket_close(client);
        ksocket_free(&client);
        return 1;
    }

    printf("Servidor: %s\n", buffer);

    ksocket_close(client);
    ksocket_free(&client);

    return 0;
}