/**
 * 4.c -- TCP эхо-сервер. 
 *
 * Copyright (c) 2021, Vladislav Shkut <shkut@cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void childHandler()
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port_number\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    if ((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0) {
        fprintf(stderr,
                "getaddrinfo(NULL, argv[1], &hints, &res) error\n");
        exit(EXIT_FAILURE);
    }

    int sd;
    if ((sd =
         socket(res->ai_family, res->ai_socktype,
                res->ai_protocol)) == -1) {
        perror
            ("socket(res->ai_family, res->ai_socktype, res->ai_protocol) error");
        exit(EXIT_FAILURE);
    }

    sigset_t mask;
    if (sigemptyset(&mask) == -1) {
        perror("sigemptyset(&mask) error");
        exit(EXIT_FAILURE);
    }

    struct sigaction act;
    act.sa_handler = childHandler;
    act.sa_mask = mask;
    act.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &act, 0) == -1) {
        perror("sigaction(SIGCHLD, &act, 0) error");
        exit(EXIT_FAILURE);
    }

    if (bind(sd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind(sd, res->ai_addr, res->ai_addrlen) error");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    if (listen(sd, SOMAXCONN) == -1) {
        perror("listen(sd, SOMAXCONN) error");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket_fd;
        if ((client_socket_fd =
             accept(sd, (struct sockaddr *) &client_address,
                    &client_address_len)) == -1) {
            perror
                ("accept(sd, (struct sockaddr*)&client_address, &client_address_len) error");
            exit(EXIT_FAILURE);
        }

        char *IPv4 = inet_ntoa(client_address.sin_addr);
        int port = ntohs(client_address.sin_port);
        printf("Connection IPv4: %s and port: %d\n", IPv4, port);

        pid_t pid;
        switch (pid = fork()) {
        case (-1):{
                perror("fork() error");
                exit(EXIT_FAILURE);
            }

        case (0):{
                if (close(sd) == -1) {
                    perror("close(sd) error");
                    exit(EXIT_FAILURE);
                }

                char buf2[1024];
                int echo;
                while ((echo =
                        read(client_socket_fd, buf2, sizeof(buf2))) > 0) {
                    if (echo == -1) {
                        perror
                            ("read(client_socket_fd, buf2, sizeof(buf2)) error");
                        exit(EXIT_FAILURE);
                    }
                    if (write(client_socket_fd, buf2, echo) == -1) {
                        perror
                            ("write(client_socket_fd, buf2, echo) error");
                        exit(EXIT_FAILURE);
                    }
                }
                exit(EXIT_SUCCESS);
            }
        default:{
                if (close(client_socket_fd) == -1) {
                    perror("close(client_socket_fd) error");
                    exit(EXIT_FAILURE);
                }
                break;
            }
        }
    }

    return 0;
}