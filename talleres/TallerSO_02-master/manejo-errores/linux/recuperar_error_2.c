#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

static int abrir_archivo(const char *nombre) {
    int fd = open(nombre, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error abriendo %s debido a %d\n", nombre, errno);
    }
    return fd;
}

int main(void) {
    int estado;

    estado = abrir_archivo("no-existe.txt");

    errno = 0;

    estado = abrir_archivo("existe.txt");

    if (estado >= 0) {
        close(estado);
    }

    exit(0);
}
