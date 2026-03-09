#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void esperar_senal(void);
void verifica_estado(int estado);
void manejar_senal(int sig);

void manejar_senal(int sig) {
  if (sig == SIGALRM) {
    printf("Se capturo SIGALRM\n");
  } else if (sig == SIGCHLD) {
    printf("Se capturo SIGCHLD\n");
  }

  fflush(stdout);
  _exit(EXIT_SUCCESS);
}

int
main(int argc, char* const argv[], char* const env[]) {
  struct sigaction accion;
  sigemptyset(&accion.sa_mask);
  accion.sa_flags = 0;
  accion.sa_handler = manejar_senal;

  int estado;

  pid_t pid_hijo1 = fork();

  if (pid_hijo1 == 0) {
    sigaction(SIGALRM, &accion, NULL);
    esperar_senal();
    _exit(EXIT_FAILURE);
  }

  sleep(1);
  kill(pid_hijo1, SIGALRM);

  if (waitpid(pid_hijo1, &estado, 0) == pid_hijo1) {
    verifica_estado(estado);
  }

  pid_t pid_hijo2 = fork();

  if (pid_hijo2 == 0) {
    sigaction(SIGCHLD, &accion, NULL);
    esperar_senal();
    _exit(EXIT_FAILURE);
  }

  sleep(1);
  kill(pid_hijo2, SIGCHLD);

  if (waitpid(pid_hijo2, &estado, 0) == pid_hijo2) {
    verifica_estado(estado);
  }

  return EXIT_SUCCESS;
}

void esperar_senal(void) {
  pause();
}

void verifica_estado(int estado) {
  if (WIFEXITED(estado)) {
    fprintf(stdout, "Proceso termino invocando _exit: %d\n",
            WEXITSTATUS(estado));
  }
  else if (WIFSIGNALED(estado)) {
    fprintf(stdout, "Proceso senalizado por: %d\n",
            WTERMSIG(estado));
  }
}
