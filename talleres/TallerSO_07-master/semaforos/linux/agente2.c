#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>
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

  sem_t *sems[AGENTE + 1] = { NULL };
  char *nom_real_sem[AGENTE + 1] = { NULL };

  while ((opt = getopt(argc, argv, "a:c:hp:t:v")) != -1) {
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

  srand(time(NULL));

  if (crear_semaforos_agente2(sems, nom_real_sem, nom_sems) != 0) {
    _exit(EXIT_FAILURE);
  }

  ejecutar_agente2(sems);

  cerrar_semaforos_agente2(sems, nom_real_sem, AGENTE + 1);

  _exit(EXIT_SUCCESS);
}

void
usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
          "Uso: %s [-a <nombre>] [-c <nombre>] [-p <nombre>] [-t <nombre>]\n",
          progname);
  fprintf(stderr,
          "     %s -h\n",
          progname);

  _exit(codigo_salida);
}
