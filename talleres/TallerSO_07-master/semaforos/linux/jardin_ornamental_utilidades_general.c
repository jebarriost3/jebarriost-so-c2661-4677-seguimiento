#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "jardin_ornamental_general.h"

PJardinGeneral
obtener_jardin_general(int maximo, int limite, int limitado) {
  PJardinGeneral jardin = (PJardinGeneral) malloc(sizeof(JardinGeneral));

  if (jardin == NULL) {
    return NULL;
  }

  jardin->contador = 0;
  jardin->maximo = maximo;
  jardin->limite = limite;
  jardin->limitado = limitado;

  sem_init(&jardin->sem_mutex, 0, 1);

  if (jardin->limitado) {
    sem_init(&jardin->sem_limite, 0, jardin->limite);
  } else {
    sem_init(&jardin->sem_limite, 0, jardin->maximo);
  }

  return jardin;
}

void
entrar_jardin_general(PJardinGeneral jardin) {
  sem_wait(&jardin->sem_limite);

  sem_wait(&jardin->sem_mutex);
  jardin->contador++;
  sem_post(&jardin->sem_mutex);
}

void
salir_jardin_general(PJardinGeneral jardin) {
  sem_wait(&jardin->sem_mutex);
  jardin->contador--;
  sem_post(&jardin->sem_mutex);

  sem_post(&jardin->sem_limite);
}

int
obtener_miembros_jardin_general(PJardinGeneral jardin) {
  int miembros;

  sem_wait(&jardin->sem_mutex);
  miembros = jardin->contador;
  sem_post(&jardin->sem_mutex);

  return miembros;
}

void
cerrar_jardin_general(PJardinGeneral jardin) {
  sem_destroy(&jardin->sem_mutex);
  sem_destroy(&jardin->sem_limite);
  free(jardin);
}
