#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fumadores2.h"

char*
duplicar_cadena(const char *s) {
  size_t n = strlen(s) + 1;
  char *r = (char *) malloc(n);
  memcpy(r, s, n);
  return r;
}

int
crear_semaforos_agente2(HANDLE sems[], char const *nom_sems[]) {
  sems[CERILLA] = CreateSemaphoreA(NULL, 0, 1, nom_sems[CERILLA]);
  sems[PAPEL] = CreateSemaphoreA(NULL, 0, 1, nom_sems[PAPEL]);
  sems[TABACO] = CreateSemaphoreA(NULL, 0, 1, nom_sems[TABACO]);
  sems[AGENTE] = CreateSemaphoreA(NULL, 1, 1, nom_sems[AGENTE]);

  for (int i = 0; i < AGENTE + 1; i++) {
    if (sems[i] == NULL) {
      cerrar_semaforos2(sems, AGENTE + 1);
      destruir_semaforos_agente2(nom_sems);
      return -1;
    }
  }

  return 0;
}

int
abrir_semaforos_fumador2(HANDLE sems[], char const *nom_sems[]) {
  sems[0] = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, nom_sems[AGENTE]);
  sems[1] = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, nom_sems[1]);

  if (sems[0] == NULL || sems[1] == NULL) {
    cerrar_semaforos2(sems, 2);
    return -1;
  }

  return 0;
}

void
cerrar_semaforos2(HANDLE sems[], int total) {
  for (int i = 0; i < total; i++) {
    if (sems[i] != NULL) {
      CloseHandle(sems[i]);
      sems[i] = NULL;
    }
  }
}

void
destruir_semaforos_agente2(char const *nom_sems[]) {
  HANDLE h;

  for (int i = 0; i < AGENTE + 1; i++) {
    h = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, nom_sems[i]);
    if (h != NULL) {
      CloseHandle(h);
    }
  }
}

void
ejecutar_agente2(HANDLE sems[]) {
  for (;;) {
    SEM_ID sem = rand() % AGENTE;

    Sleep(rand() % MAX_TIEMPO_PREPARANDO + 1);
    fprintf(stdout, "[Agente2] ");

    switch (sem) {
      case CERILLA:
        fprintf(stdout, "poniendo papel y tabaco\r\n");
        break;
      case PAPEL:
        fprintf(stdout, "poniendo cerilla y tabaco\r\n");
        break;
      case TABACO:
        fprintf(stdout, "poniendo cerilla y papel\r\n");
        break;
      default:
        break;
    }

    ReleaseSemaphore(sems[sem], 1, NULL);
    fprintf(stdout, "[Agente2] Esperando continuar\r\n");
    WaitForSingleObject(sems[AGENTE], INFINITE);
  }
}

void
ejecutar_fumador2(HANDLE sems[], char const *id_fumador) {
  for (;;) {
    fprintf(stdout, "[Fumadores2 con %s] esperando\r\n", id_fumador);
    WaitForSingleObject(sems[1], INFINITE);
    fprintf(stdout,
            "[Fumadores2 con %s] tomo ingredientes, lio cigarrillo y a fumar\r\n",
            id_fumador);
    Sleep(rand() % MAX_TIEMPO_FUMANDO + 1);
    ReleaseSemaphore(sems[0], 1, NULL);
  }
}
