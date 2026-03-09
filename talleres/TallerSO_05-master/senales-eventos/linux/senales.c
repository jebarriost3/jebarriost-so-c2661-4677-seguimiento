#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void manejar_senal(int sig);
void esperar_senal(void);
void verifica_estado(int estado);

void manejar_senal(int sig) {
  if (sig == SIGALRM)
    printf("Se capturo SIGALRM\n");
  else if (sig == SIGHUP)
    printf("Se capturo SIGHUP\n");
  else if (sig == SIGQUIT)
    printf("Se capturo SIGQUIT\n");

  fflush(stdout);
  _exit(EXIT_SUCCESS);
}

int main(int argc, char* const argv[], char* const env[]) {
  signal(SIGALRM, manejar_senal);
  signal(SIGHUP, manejar_senal);
  signal(SIGQUIT, manejar_senal);

  int estado;

  pid_t pid1 = fork();
  if (pid1 == 0) {
    esperar_senal();
    _exit(EXIT_FAILURE);
  }
  kill(pid1, SIGALRM);
  waitpid(pid1, &estado, 0);
  verifica_estado(estado);

  pid_t pid2 = fork();
  if (pid2 == 0) {
    esperar_senal();
    _exit(EXIT_FAILURE);
  }
  kill(pid2, SIGHUP);
  waitpid(pid2, &estado, 0);
  verifica_estado(estado);

  pid_t pid3 = fork();
  if (pid3 == 0) {
    esperar_senal();
    _exit(EXIT_FAILURE);
  }
  kill(pid3, SIGQUIT);
  waitpid(pid3, &estado, 0);
  verifica_estado(estado);

  return EXIT_SUCCESS;
}

void esperar_senal(void) {
  pause();
}

void verifica_estado(int estado) {
  if (WIFEXITED(estado)) {
    fprintf(stdout, "Proceso termino invocando _exit: %d\n",
            WEXITSTATUS(estado));
  } else if (WIFSIGNALED(estado)) {
    fprintf(stdout, "Proceso senalizado por: %d\n",
            WTERMSIG(estado));
  }
}
