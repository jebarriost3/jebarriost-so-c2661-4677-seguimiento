#include <windows.h>
#include <stdlib.h>
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

  jardin->h_sem_mutex = CreateSemaphore(NULL, 1, 1, NULL);

  if (jardin->h_sem_mutex == NULL) {
    free(jardin);
    return NULL;
  }

  if (jardin->limitado) {
    jardin->h_sem_limite = CreateSemaphore(NULL, jardin->limite, jardin->limite, NULL);
  } else {
    jardin->h_sem_limite = CreateSemaphore(NULL, jardin->maximo, jardin->maximo, NULL);
  }

  if (jardin->h_sem_limite == NULL) {
    CloseHandle(jardin->h_sem_mutex);
    free(jardin);
    return NULL;
  }

  return jardin;
}

void
entrar_jardin_general(PJardinGeneral jardin) {
  if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_limite, INFINITE);
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    jardin->contador++;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
  }
}

void
salir_jardin_general(PJardinGeneral jardin) {
  if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    jardin->contador--;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
    ReleaseSemaphore(jardin->h_sem_limite, 1, NULL);
  }
}

int
obtener_miembros_jardin_general(PJardinGeneral jardin) {
  int valor_retornar = -1;

  if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    valor_retornar = jardin->contador;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
  }

  return valor_retornar;
}

void
cerrar_jardin_general(PJardinGeneral jardin) {
  if (jardin != NULL) {
    CloseHandle(jardin->h_sem_mutex);
    CloseHandle(jardin->h_sem_limite);
    free((void *) jardin);
  }
}
