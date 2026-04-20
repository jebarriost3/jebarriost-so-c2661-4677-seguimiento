#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "fumadores2.h"

void usage(char const *progname, int codigo_salida);

int
main(int argc, char *argv[]) {
  int opt;

  char const *nom_sems[] = {
    NOM_SEM_AGENTE,
    NULL
  };

  char *id_fumador = NULL;
  sem_t *sems[2] = { NULL };

  while ((opt = getopt(argc, argv, "a:f:h")) != -1) {
    switch(opt) {
    case 'a':
      nom_sems[0] = optarg;
      break;
    case 'f':
      nom_sems[1] = optarg;
      id_fumador = strdup(optarg);
      break;
    case 'h':
      usage(argv[0], EXIT_SUCCESS);
      break;
    }
  }

  if (nom_sems[1] == NULL || id_fumador == NULL) {
    usage(argv[0], EXIT_FAILURE);
  }

  srand(time(NULL));

  if (abrir_semaforos_fumador2(sems, nom_sems) != 0) {
    free(id_fumador);
    _exit(EXIT_FAILURE);
  }

  ejecutar_fumador2(sems, id_fumador);

  cerrar_semaforos_fumador2(sems, 2);
  free(id_fumador);

  _exit(EXIT_SUCCESS);
}

void
usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
          "Uso: %s [-a <nombre>] -f <nombre>\r\n     %s -h\r\n",
          progname, progname);

  _exit(codigo_salida);
}
