#ifndef KSOCKET_DEF
#define KSOCKET_DEF

#include "kmacros.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#if KLIB_PLATFORM_WINDOWS

#include <winsock2.h>
#include <ws2tcpip.h>

typedef SOCKET KSocketHandle;

#else

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

typedef int KSocketHandle;

#endif

typedef struct {
    KSocketHandle handle;
    char* ip_address;
    int port;
} KSocket;

KSocket* ksocket_new(const char* ip_address, int port);
bool ksocket_free(KSocket** socket);
bool ksocket_connect(KSocket* socket);
bool ksocket_send(KSocket* socket, const char* data, size_t length);
bool ksocket_receive(KSocket* socket, char* buffer, size_t length);
bool ksocket_close(KSocket* socket);
bool ksocket_is_connected(KSocket* socket);
KSocket* ksocket_accept(KSocket* socket, int seconds, int microseconds);
bool ksocket_bind(KSocket* socket);
bool ksocket_listen(KSocket* socket, int backlog);

#endif
