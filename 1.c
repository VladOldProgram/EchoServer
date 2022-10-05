/**
 * 1.c -- программа для иллюстрациии преобразования порядка байт. 
 *
 * Copyright (c) 2021, Vladislav Shkut <shkut@cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <limits.h>

#define twoTo32Degrees 4294967296

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s unsigned_32-bit_integer\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    ulong number = strtoul(argv[1], NULL, 0);
    if (number == LONG_MIN || number == LONG_MAX) {
        perror("strtoul(argv[1], NULL, 0) error");
        exit(EXIT_FAILURE);
    }
    if (number < 0 || number >= twoTo32Degrees) {
        fprintf(stderr,
                "strtoul(%s, NULL, 0): conversion is not possible\n",
                argv[1]);
        exit(EXIT_FAILURE);
    }
    char *memoryCells1 = (char *) &number;
    printf("host byte order:\n");
    printf("s_addr=0x0%lx, ", number);
    printf("0x0%x:0x0%x:0x0%x:0x0%x\n", memoryCells1[0], memoryCells1[1],
           memoryCells1[2], memoryCells1[3]);

    ulong transformedNumber = htonl(number);
    char *memoryCells2 = (char *) &transformedNumber;
    printf("network byte order:\n");
    printf("s_addr=0x0%lx, ", transformedNumber);
    printf("0x0%x:0x0%x:0x0%x:0x0%x\n", memoryCells2[0], memoryCells2[1],
           memoryCells2[2], memoryCells2[3]);
    printf("s=%x.%x.%x.%x\n", memoryCells2[0], memoryCells2[1],
           memoryCells2[2], memoryCells2[3]);

    return 0;
}
