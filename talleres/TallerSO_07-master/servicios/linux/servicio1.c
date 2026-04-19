#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <syslog.h>
#include <errno.h>

#define FIFO_IN "/tmp/eco_entrada"
#define FIFO_OUT "/tmp/eco_salida"
#define BUFFER 1024

void transformar(char *msg) {
    for (int i = 0; msg[i] != '\0'; i++) {
        if (islower((unsigned char)msg[i])) {
            msg[i] = toupper((unsigned char)msg[i]);
        } else if (isupper((unsigned char)msg[i])) {
            msg[i] = tolower((unsigned char)msg[i]);
        }
    }
}

int main(void) {
    char buffer[BUFFER];

    if (mkfifo(FIFO_IN, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo eco_entrada");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(FIFO_OUT, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo eco_salida");
        exit(EXIT_FAILURE);
    }

    openlog("servicio1", LOG_CONS | LOG_PID, LOG_USER);

    while (1) {
        int fd_in = open(FIFO_IN, O_RDONLY);
        if (fd_in == -1) {
            syslog(LOG_ERR, "Error abriendo %s", FIFO_IN);
            continue;
        }

        ssize_t n = read(fd_in, buffer, BUFFER - 1);
        close(fd_in);

        if (n <= 0) {
            continue;
        }

        buffer[n] = '\0';

        syslog(LOG_INFO, "Mensaje recibido: %s", buffer);

        transformar(buffer);

        int fd_out = open(FIFO_OUT, O_WRONLY);
        if (fd_out == -1) {
            syslog(LOG_ERR, "Error abriendo %s", FIFO_OUT);
            continue;
        }

        write(fd_out, buffer, strlen(buffer) + 1);
        close(fd_out);
    }

    closelog();
    return 0;
}
