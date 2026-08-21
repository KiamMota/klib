
#include "types/ksocket.h"
#include <stdio.h>

int main(void) {
    KSocket* server = ksocket_new("127.0.0.1", 8080);

    printf("server=%p\n", (void*)server);
    printf("handle=%llu\n", (unsigned long long)server->handle);
    printf("ip=%p\n", (void*)server->ip_address);
    printf("port=%d\n", server->port);

    printf("new ok\n");

    if (!ksocket_bind(server)) {
        printf("Falha no bind.\n");
        return 1;
    }

    printf("bind ok\n");

    if (!ksocket_listen(server, 10)) {
        printf("Falha no listen.\n");
        return 1;
    }

    printf("listen ok\n");


    printf("antes accept\n");
    getchar();

    KSocket* client = ksocket_accept(server, 30, 0);

    printf("depois accept\n");
    printf("client=%p\n", (void*)client);

    printf("antes close\n");
    ksocket_close(client);
    printf("depois close\n");

    ksocket_free(&client);
}