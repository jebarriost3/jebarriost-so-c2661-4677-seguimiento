#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

  STARTUPINFOA sa_agente;
  PROCESS_INFORMATION pi_agente;
  STARTUPINFOA sa_fumador[AGENTE];
  PROCESS_INFORMATION pi_fumador[AGENTE];
  char linea_agente[256];
  char linea_fumador[AGENTE][256];

  ZeroMemory(&sa_agente, sizeof(sa_agente));
  ZeroMemory(&pi_agente, sizeof(pi_agente));
  sa_agente.cb = sizeof(sa_agente);

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

  sprintf(linea_agente, "agente2.exe -a %s -c %s -p %s -t %s",
          nom_sems[AGENTE], nom_sems[CERILLA], nom_sems[PAPEL], nom_sems[TABACO]);

  if (!CreateProcessA(NULL, linea_agente, NULL, NULL, FALSE, 0, NULL, NULL, &sa_agente, &pi_agente)) {
    ExitProcess(EXIT_FAILURE);
  }

  Sleep(MAX_TIEMPO_PREPARANDO);

  for (int i = 0; i < AGENTE; i++) {
    ZeroMemory(&sa_fumador[i], sizeof(STARTUPINFOA));
    ZeroMemory(&pi_fumador[i], sizeof(PROCESS_INFORMATION));
    sa_fumador[i].cb = sizeof(STARTUPINFOA);

    sprintf(linea_fumador[i], "fumadores2.exe -a %s -f %s",
            nom_sems[AGENTE], nom_sems[i]);

    if (!CreateProcessA(NULL, linea_fumador[i], NULL, NULL, FALSE, 0, NULL, NULL,
                        &sa_fumador[i], &pi_fumador[i])) {
      ExitProcess(EXIT_FAILURE);
    }
  }

  WaitForSingleObject(pi_agente.hProcess, INFINITE);

  for (int i = 0; i < AGENTE; i++) {
    WaitForSingleObject(pi_fumador[i].hProcess, INFINITE);
  }

  CloseHandle(pi_agente.hProcess);
  CloseHandle(pi_agente.hThread);

  for (int i = 0; i < AGENTE; i++) {
    CloseHandle(pi_fumador[i].hProcess);
    CloseHandle(pi_fumador[i].hThread);
  }

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
