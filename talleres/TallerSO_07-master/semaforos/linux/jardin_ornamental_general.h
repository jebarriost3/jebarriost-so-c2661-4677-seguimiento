#pragma once
#include <semaphore.h>

#define MAX_VISITANTES 20
#define MAX_VISITAS    10
#define TIEMPO_AFUERA  1
#define TIEMPO_VISITA  5

struct JardinGeneral {
  sem_t sem_mutex;
  sem_t sem_limite;
  int contador;
  int maximo;
  int limite;
  int limitado;
};

typedef struct JardinGeneral JardinGeneral;
typedef struct JardinGeneral *PJardinGeneral;

PJardinGeneral obtener_jardin_general(int maximo, int limite, int limitado);
void entrar_jardin_general(PJardinGeneral jardin);
void salir_jardin_general(PJardinGeneral jardin);
int obtener_miembros_jardin_general(PJardinGeneral jardin);
void cerrar_jardin_general(PJardinGeneral jardin);
