/**
 * 3.c -- программа для получения имени хоста по IP-адресу(если это возможно). 
 *
 * Copyright (c) 2021, Vladislav Shkut <shkut@cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s IP_address\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char buf[NI_MAXHOST];
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;

    int status;
    if ((status = inet_pton(AF_INET, argv[1], &sa.sin_addr)) == 0) {
        fprintf(stderr,
                "inet_pton(AF_INET, argv[1], &sa.sin_addr): conversion is not possible\n");
        exit(EXIT_FAILURE);
    }

    if (status < 0) {
        perror("inet_pton(AF_INET, argv[1], &sa.sin_addr) error");
        exit(EXIT_FAILURE);
    }

    if (getnameinfo
        ((struct sockaddr *) &sa, sizeof(sa), buf, sizeof(buf), NULL, 0,
         0) != 0) {
        perror
            ("getnameinfo((struct sockaddr *)&sa, sizeof(sa), buf, sizeof(buf), NULL, 0, 0) error");
        exit(EXIT_FAILURE);
    } else
        if (getnameinfo
            ((struct sockaddr *) &sa, sizeof(sa), buf, sizeof(buf), NULL,
             0, NI_NAMEREQD) != 0) {
        printf("The machine name cannot be determined\n");
        exit(EXIT_FAILURE);
    } else {
        printf("host: %s\n", (char *) buf);
    }

    return 0;
}