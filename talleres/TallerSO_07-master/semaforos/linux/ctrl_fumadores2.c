#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "fumadores2.h"

void usage(char const *progname, int codigo_salida);

int
main(int argc, char *argv[]) {
  int opt;

  char const *nom_sems[] = {
    NOM_SEM_CERILLA,
    NOM_SEM_PAPEL,
    NOM_SEM_TABACO,
    NOM_SEM_AGENTE
  };

  pid_t procesos[AGENTE + 1];
  char *nom_real_sem[AGENTE + 1] = { NULL };

  while ((opt = getopt(argc, argv, "a:c:hp:t:")) != -1) {
    switch(opt) {
    case 'a':
      nom_sems[AGENTE] = optarg;
      break;
    case 'c':
      nom_sems[CERILLA] = optarg;
      break;
    case 'h':
      usage(argv[0], EXIT_SUCCESS);
      break;
    case 'p':
      nom_sems[PAPEL] = optarg;
      break;
    case 't':
      nom_sems[TABACO] = optarg;
      break;
    }
  }

  for (int i = 0; i < AGENTE + 1; i++) {
    nom_real_sem[i] = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);
  }

  procesos[0] = fork();

  if (procesos[0] == 0) {
    execl("./agente2", "agente2",
          "-a", nom_sems[AGENTE],
          "-c", nom_sems[CERILLA],
          "-p", nom_sems[PAPEL],
          "-t", nom_sems[TABACO],
          NULL);
    _exit(EXIT_FAILURE);
  }

  sleep(MAX_TIEMPO_PREPARANDO);

  for (int i = 0; i < AGENTE; i++) {
    procesos[i + 1] = fork();

    if (procesos[i + 1] == 0) {
      execl("./fumadores2", "fumadores2",
            "-a", nom_sems[AGENTE],
            "-f", nom_sems[i],
            NULL);
      _exit(EXIT_FAILURE);
    }
  }

  sigset_t conjunto;
  int signo = 0;

  sigemptyset(&conjunto);
  sigaddset(&conjunto, SIGINT);
  sigprocmask(SIG_BLOCK, &conjunto, NULL);
  sigwait(&conjunto, &signo);

  for (int i = 0; i < AGENTE + 1; i++) {
    kill(procesos[i], SIGINT);
  }

  for (int i = 0; i < AGENTE + 1; i++) {
    waitpid(procesos[i], NULL, 0);
  }

  for (int i = 0; i < AGENTE + 1; i++) {
    sem_unlink(nom_real_sem[i]);
    free(nom_real_sem[i]);
  }

  _exit(EXIT_SUCCESS);
}

void
usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
          "Uso: %s [-a <nombre>] [-c <nombre>] [-p <nombre>] [-t <nombre>]\r\n     %s -h\r\n",
          progname, progname);

  _exit(codigo_salida);
}
