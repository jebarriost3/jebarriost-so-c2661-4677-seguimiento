#pragma once

#include <semaphore.h>

#define NOM_SEM_AGENTE  "agente"
#define NOM_SEM_CERILLA "cerilla"
#define NOM_SEM_PAPEL   "papel"
#define NOM_SEM_TABACO  "tabaco"

#define PRE_GLOBAL "/"

#define MAX_TIEMPO_PREPARANDO 4
#define MAX_TIEMPO_FUMANDO 5

enum SEM_ID { CERILLA, PAPEL, TABACO, AGENTE };

typedef enum SEM_ID SEM_ID;

char* obtener_nombre_semaforo(char const* prefijo, char const* nom_sem);
int crear_semaforos_agente2(sem_t *sems[], char *nom_real_sem[], char const *nom_sems[]);
void cerrar_semaforos_agente2(sem_t *sems[], char *nom_real_sem[], int total);
int abrir_semaforos_fumador2(sem_t *sems[], char const *nom_sems[]);
void cerrar_semaforos_fumador2(sem_t *sems[], int total);
void ejecutar_agente2(sem_t *sems[]);
void ejecutar_fumador2(sem_t *sems[], char const *id_fumador);
