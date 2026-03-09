#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    if (argc != 5) {
        fprintf(stderr,"Uso: eco <archivo> <origen> <destino> <modulo>\n");
        exit(EXIT_FAILURE);
    }

    int p1[2];
    int p2[2];

    pipe(p1);
    pipe(p2);

    pid_t cat = fork();

    if (cat == 0) {
        dup2(p1[1], 1);

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);

        execl("/bin/cat", "cat", argv[1], NULL);
        exit(EXIT_FAILURE);
    }

    pid_t tr = fork();

    if (tr == 0) {
        dup2(p1[0], 0);
        dup2(p2[1], 1);

        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);

        execl("/usr/bin/tr", "tr", argv[2], argv[3], NULL);
        exit(EXIT_FAILURE);
    }

    close(p1[0]);
    close(p1[1]);
    close(p2[1]);

    FILE *entrada = fdopen(p2[0], "r");

    char linea[1024];
    int contador = 1;
    int modulo = atoi(argv[4]);

    while (fgets(linea, sizeof(linea), entrada)) {
        if (contador % modulo != 0) {
            printf("%s", linea);
        }
        contador++;
    }

    fclose(entrada);

    wait(NULL);
    wait(NULL);

    return 0;
}
