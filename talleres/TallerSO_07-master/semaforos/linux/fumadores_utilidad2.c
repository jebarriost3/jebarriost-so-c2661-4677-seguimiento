#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "fumadores2.h"

char*
obtener_nombre_semaforo(char const* prefijo, char const* nom_sem) {
  size_t tam = strlen(prefijo) + strlen(nom_sem) + 1;
  char *valor_ret = malloc(tam);
  valor_ret[0] = '\0';
  strcpy(valor_ret, prefijo);
  strcat(valor_ret, nom_sem);

  return valor_ret;
}

int
crear_semaforos_agente2(sem_t *sems[], char *nom_real_sem[], char const *nom_sems[]) {
  for (int i = 0; i < AGENTE + 1; i++) {
    nom_real_sem[i] = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);

    fprintf(stdout, "[Agente2] Creando semaforo: %s\n", nom_real_sem[i]);

    sems[i] = sem_open(nom_real_sem[i],
                       O_CREAT | O_EXCL,
                       S_IRUSR | S_IWUSR,
                       0);

    if (sems[i] == SEM_FAILED) {
      fprintf(stderr, "Error: creando semaforo %d\n", errno);
      cerrar_semaforos_agente2(sems, nom_real_sem, i);
      return -1;
    }
  }

  return 0;
}

void
cerrar_semaforos_agente2(sem_t *sems[], char *nom_real_sem[], int total) {
  for (int i = 0; i < total; i++) {
    if (sems[i] != NULL && sems[i] != SEM_FAILED) {
      sem_close(sems[i]);
    }

    if (nom_real_sem[i] != NULL) {
      sem_unlink(nom_real_sem[i]);
      free(nom_real_sem[i]);
    }
  }
}

int
abrir_semaforos_fumador2(sem_t *sems[], char const *nom_sems[]) {
  for (int i = 0; i < 2; i++) {
    char *nom_real_sem = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);

    fprintf(stdout,
            "[fumadores2 con %s]: abriendo semaforo %s\r\n",
            nom_sems[1],
            nom_real_sem);

    sems[i] = sem_open(nom_real_sem, 0);

    free(nom_real_sem);

    if (sems[i] == SEM_FAILED) {
      fprintf(stderr, "Error: creando semaforo debido a %d\r\n", errno);
      cerrar_semaforos_fumador2(sems, i);
      return -1;
    }
  }

  return 0;
}

void
cerrar_semaforos_fumador2(sem_t *sems[], int total) {
  for (int i = 0; i < total; i++) {
    if (sems[i] != NULL && sems[i] != SEM_FAILED) {
      sem_close(sems[i]);
    }
  }
}

void
ejecutar_agente2(sem_t *sems[]) {
  for (;;) {
    SEM_ID sem = rand() % AGENTE;

    sleep(rand() % MAX_TIEMPO_PREPARANDO + 1);
    fprintf(stdout, "[Agente2] ");

    switch(sem) {
    case CERILLA:
      fprintf(stdout, "poniendo papel y tabaco\n");
      break;
    case PAPEL:
      fprintf(stdout, "poniendo cerilla y tabaco\n");
      break;
    case TABACO:
      fprintf(stdout, "poniendo cerilla y papel\n");
      break;
    default:
      break;
    }

    sem_post(sems[sem]);
    fprintf(stdout, "[Agente2] Esperando continuar\n");
    sem_wait(sems[AGENTE]);
  }
}

void
ejecutar_fumador2(sem_t *sems[], char const *id_fumador) {
  for (;;) {
    fprintf(stdout, "[Fumadores2 con %s] esperando\r\n", id_fumador);
    sem_wait(sems[1]);
    fprintf(stdout,
            "[Fumadores2 con %s] tomo ingredientes, lio cigarrillo y a fumar\r\n",
            id_fumador);
    sleep(rand() % MAX_TIEMPO_FUMANDO + 1);
    sem_post(sems[0]);
  }
}
