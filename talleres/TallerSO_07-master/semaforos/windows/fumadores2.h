#pragma once
#include <windows.h>

#define NOM_SEM_AGENTE  "agente2_sem"
#define NOM_SEM_CERILLA "cerilla2_sem"
#define NOM_SEM_PAPEL   "papel2_sem"
#define NOM_SEM_TABACO  "tabaco2_sem"

#define MAX_TIEMPO_PREPARANDO 4000
#define MAX_TIEMPO_FUMANDO 5000

enum SEM_ID { CERILLA, PAPEL, TABACO, AGENTE };

typedef enum SEM_ID SEM_ID;

char* duplicar_cadena(const char *s);
int crear_semaforos_agente2(HANDLE sems[], char const *nom_sems[]);
int abrir_semaforos_fumador2(HANDLE sems[], char const *nom_sems[]);
void cerrar_semaforos2(HANDLE sems[], int total);
void destruir_semaforos_agente2(char const *nom_sems[]);
void ejecutar_agente2(HANDLE sems[]);
void ejecutar_fumador2(HANDLE sems[], char const *id_fumador);
