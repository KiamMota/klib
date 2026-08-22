#include "ksocket.h"
#include "kmacros.h"
#include "kmalloc.h"
#include <string.h>

#if KLIB_PLATFORM_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

KSocket* ksocket_new(const char* ip_address, int port) {
    KSocket* sock = KMALLOC(KSocket);

    if (!sock)
        return NULL;

#if KLIB_PLATFORM_WINDOWS
    sock->handle = INVALID_SOCKET;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        KFREE(sock);
        return NULL;
    }

    sock->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock->handle == INVALID_SOCKET) {
        WSACleanup();
        KFREE(sock);
        return NULL;
    }

#else
    sock->handle = socket(AF_INET, SOCK_STREAM, 0);

    if (sock->handle < 0) {
        KFREE(sock);
        return NULL;
    }
#endif

    sock->ip_address = NULL;
    sock->port = port;

    if (ip_address) {
        sock->ip_address = KMALLOC(strlen(ip_address) + 1);

        if (!sock->ip_address) {
#if KLIB_PLATFORM_WINDOWS
            closesocket(sock->handle);
            WSACleanup();
#else
            close(sock->handle);
#endif
            KFREE(sock);
            return NULL;
        }

        strcpy(sock->ip_address, ip_address);
    }

    return sock;
}

bool ksocket_free(KSocket** socket) {
    if (!socket || !*socket)
        return false;

    ksocket_close(*socket);

    if ((*socket)->ip_address) {
        KFREE((*socket)->ip_address);
        (*socket)->ip_address = NULL;
    }

    KFREE(*socket);
    *socket = NULL;

    return true;
}
bool ksocket_bind(KSocket* sock) {

    struct sockaddr_in address = { 0 };

    address.sin_family = AF_INET;
    address.sin_port = htons(sock->port);

    if (sock->ip_address == NULL || sock->ip_address[0] == '\0') {
        address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else {
#if KLIB_PLATFORM_WINDOWS
        if (inet_pton(AF_INET, sock->ip_address, &address.sin_addr) != 1)
            return false;
#else
        if (inet_pton(AF_INET, sock->ip_address, &address.sin_addr) != 1)
            return false;
#endif
    }

    return bind(
        sock->handle,
        (struct sockaddr*)&address,
        sizeof(address)
    ) == 0;
}

bool ksocket_listen(KSocket* sock, int backlog) {
    return listen(sock->handle, backlog) == 0;
}

KSocket* ksocket_accept(KSocket* sock, int seconds, int microseconds) {
    if (!sock)
        return NULL;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock->handle, &readfds);

    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = microseconds;

#if KLIB_PLATFORM_WINDOWS

    int result = select(
        0,
        &readfds,
        NULL,
        NULL,
        &timeout
    );

    if (result <= 0 || !FD_ISSET(sock->handle, &readfds))
        return NULL;

    SOCKET client_handle = accept(
        sock->handle,
        NULL,
        NULL
    );

    if (client_handle == INVALID_SOCKET)
        return NULL;

#else

    int result = select(
        sock->handle + 1,
        &readfds,
        NULL,
        NULL,
        &timeout
    );

    if (result <= 0 || !FD_ISSET(sock->handle, &readfds))
        return NULL;

    int client_handle = accept(
        sock->handle,
        NULL,
        NULL
    );

    if (client_handle < 0)
        return NULL;

#endif

    KSocket* client = KMALLOC(KSocket);

    if (!client) {
#if KLIB_PLATFORM_WINDOWS
        closesocket(client_handle);
#else
        close(client_handle);
#endif
        return NULL;
    }

    client->handle = client_handle;
    client->ip_address = NULL;
    client->port = 0;

    return client;
}
bool ksocket_connect(KSocket* sock) {
    if (!sock || !sock->ip_address)
        return false;

#if KLIB_PLATFORM_WINDOWS
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return false;
#endif

    sock->handle = socket(AF_INET, SOCK_STREAM, 0);

#if KLIB_PLATFORM_WINDOWS
    if (sock->handle == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }
#else
    if (sock->handle < 0)
        return false;
#endif

    struct sockaddr_in server = { 0 };

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(sock->ip_address);
    server.sin_port = htons(sock->port);

    if (server.sin_addr.s_addr == INADDR_NONE)
        return false;

    if (connect(
        sock->handle,
        (struct sockaddr*)&server,
        sizeof(server)
    ) < 0) {

#if KLIB_PLATFORM_WINDOWS
        closesocket(sock->handle);
        WSACleanup();
#else
        close(sock->handle);
#endif

        return false;
    }

    return true;
}

bool ksocket_send(KSocket* sock, const char* data, size_t length) {
    if (!sock || !data || length == 0)
        return false;

#if KLIB_PLATFORM_WINDOWS
    int result = send(sock->handle, data, (int)length, 0);
#else
    ssize_t result = send(sock->handle, data, length, 0);
#endif

    return result == (int)length;
}

bool ksocket_receive(KSocket* sock, char* buffer, size_t length) {
    if (!sock || !buffer || length == 0)
        return false;

#if KLIB_PLATFORM_WINDOWS
    if (sock->handle == INVALID_SOCKET)
        return false;

    int received = recv(
        sock->handle,
        buffer,
        (int)length,
        0
    );

    return received > 0;

#else
    if (sock->handle < 0)
        return false;

    ssize_t received = recv(
        sock->handle,
        buffer,
        length,
        0
    );

    return received > 0;
#endif
}

bool ksocket_close(KSocket* sock) {
    if (!sock)
        return false;

#if KLIB_PLATFORM_WINDOWS
    if (sock->handle == INVALID_SOCKET)
        return false;

    if (closesocket(sock->handle) != 0)
        return false;

    sock->handle = INVALID_SOCKET;
#else
    if (sock->handle < 0)
        return false;

    if (close(sock->handle) != 0)
        return false;

    sock->handle = -1;
#endif

    return true;
}

bool ksocket_is_connected(KSocket* sock) {
    if (!sock)
        return false;

#if KLIB_PLATFORM_WINDOWS
    return sock->handle != INVALID_SOCKET;
#else
    return sock->handle >= 0;
#endif
}

