#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_IN "/tmp/eco_entrada"
#define FIFO_OUT "/tmp/eco_salida"
#define BUFFER 1024

int main(void) {
    char mensaje[BUFFER];
    char respuesta[BUFFER];

    printf("Ingrese mensaje: ");
    if (fgets(mensaje, BUFFER, stdin) == NULL) {
        return EXIT_FAILURE;
    }

    int fd_out = open(FIFO_IN, O_WRONLY);
    if (fd_out == -1) {
        perror("open eco_entrada");
        return EXIT_FAILURE;
    }

    write(fd_out, mensaje, strlen(mensaje) + 1);
    close(fd_out);

    int fd_in = open(FIFO_OUT, O_RDONLY);
    if (fd_in == -1) {
        perror("open eco_salida");
        return EXIT_FAILURE;
    }

    ssize_t n = read(fd_in, respuesta, BUFFER - 1);
    if (n >= 0) {
        respuesta[n] = '\0';
        printf("Respuesta: %s\n", respuesta);
    }

    close(fd_in);

    return EXIT_SUCCESS;
}
