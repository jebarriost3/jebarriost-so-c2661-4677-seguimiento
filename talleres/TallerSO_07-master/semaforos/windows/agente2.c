#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
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

  HANDLE sems[AGENTE + 1] = { NULL };

  while ((opt = getopt(argc, argv, "a:c:hp:t:")) != -1) {
    switch (opt) {
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
      default:
        usage(argv[0], EXIT_FAILURE);
    }
  }

  srand((unsigned int) time(NULL));

  if (crear_semaforos_agente2(sems, nom_sems) != 0) {
    ExitProcess(EXIT_FAILURE);
  }

  ejecutar_agente2(sems);

  cerrar_semaforos2(sems, AGENTE + 1);
  destruir_semaforos_agente2(nom_sems);

  ExitProcess(EXIT_SUCCESS);
}

void
usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
          "Uso: %s [-a <nombre>] [-c <nombre>] [-p <nombre>] [-t <nombre>]\r\n"
          "     %s -h\r\n",
          progname, progname);

  ExitProcess(codigo_salida);
}
