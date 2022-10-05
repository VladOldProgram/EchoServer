/**
 * 2.c -- программа для получения IP-адресов по имени хоста(если это возможно). 
 *
 * Copyright (c) 2021, Vladislav Shkut <shkut@cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s host_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *res, *p;
    char buf[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;        // неважно, IPv4 или IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets

    if (getaddrinfo(argv[1], NULL, &hints, &res) != 0) {
        fprintf(stderr,
                "getaddrinfo(argv[1], NULL, &hints, &res) error\n");
        exit(EXIT_FAILURE);
    }

    for (p = res; p != NULL; p = p->ai_next) {
        void *address;
        if (p->ai_family == AF_INET) {  // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
            address = &(ipv4->sin_addr);
            inet_ntop(p->ai_family, address, buf, sizeof(buf));
            printf("%s has IPv4 address %s\n", argv[1], buf);
        }

        else {                  // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            address = &(ipv6->sin6_addr);
            inet_ntop(p->ai_family, address, buf, sizeof(buf));
            printf("%s has IPv6 address %s\n", argv[1], buf);
        }
    }

    freeaddrinfo(res);
    return 0;
}
