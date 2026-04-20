#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "jardin_ornamental_general.h"

void usage(char const *progname, int codigo_salida);
int random_siguiente(int max, int min);

void* visitante(void* pv_arg);
void* admin(void* pv_arg);

int
main(int argc, char *argv[]) {
  int opt;
  int maximo = MAX_VISITANTES;
  int limite = 0;
  int limitado = 0;

  while ((opt = getopt(argc, argv, "hl:m:")) != -1) {
    switch(opt) {
    case 'h':
      usage(argv[0], EXIT_SUCCESS);
      break;
    case 'l':
      limite = atoi(optarg);
      limitado = 1;
      break;
    case 'm':
      maximo = atoi(optarg);
      break;
    default:
      usage(argv[0], EXIT_FAILURE);
    }
  }

  if (maximo <= 0) {
    usage(argv[0], EXIT_FAILURE);
  }

  if (limitado && (limite <= 0 || limite >= maximo)) {
    usage(argv[0], EXIT_FAILURE);
  }

  PJardinGeneral jardin = obtener_jardin_general(maximo, limite, limitado);

  if (jardin == NULL) {
    fprintf(stderr, "Error: Estructura del jardin ornamental no pudo ser creada\n");
    _exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Numero visitantes antes de iniciar: %d\n",
          obtener_miembros_jardin_general(jardin));

  int visitantes = maximo;
  pthread_t hilos[maximo + 1];

  srand(time(NULL));

  for (int i = 0; i < visitantes; i++) {
    if (pthread_create(&hilos[i], NULL, visitante, (void*) jardin) != 0) {
      fprintf(stderr, "Error: No se pudo crear %d-esimo hilo debido a %d\n",
              i, errno);
      _exit(EXIT_FAILURE);
    }
  }

  pthread_create(&hilos[maximo], NULL, admin, (void*) jardin);

  for (int i = 0; i < maximo + 1; i++) {
    pthread_join(hilos[i], NULL);
  }

  fprintf(stdout, "Numero visitantes antes de cerrar jardin: %d\n",
          obtener_miembros_jardin_general(jardin));

  cerrar_jardin_general(jardin);

  _exit(EXIT_SUCCESS);
}

void
usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
          "Uso: %s [-l <limite>] [-m <maximo>]\n"
          "     %s [-m <maximo>]\n"
          "     %s -h\n",
          progname, progname, progname);

  _exit(codigo_salida);
}

void*
visitante(void* pv_arg) {
  PJardinGeneral jardin = (PJardinGeneral) pv_arg;
  pthread_t hilo = pthread_self();

  for (int veces = 0; veces < MAX_VISITAS; veces++) {
    fprintf(stdout, "Visitante %ld afuera\n", hilo);
    sleep(random_siguiente(TIEMPO_AFUERA, 0));

    entrar_jardin_general(jardin);
    fprintf(stdout, "Visitante %ld dentro del jardin\n", hilo);

    sleep(random_siguiente(TIEMPO_VISITA, 0));
    salir_jardin_general(jardin);
  }

  fprintf(stdout, "Visitante %ld termino su jornada\n", hilo);

  return NULL;
}

void*
admin(void* pv_arg) {
  PJardinGeneral jardin = (PJardinGeneral) pv_arg;

  for (int veces = 0; veces < MAX_VISITAS; veces++) {
    sleep(random_siguiente(TIEMPO_AFUERA * 2, 0));
    fprintf(stdout, "Visitantes %d dentro del jardin\n",
            obtener_miembros_jardin_general(jardin));
  }

  fprintf(stdout, "Administrador termino su jornada\n");

  return NULL;
}

int
random_siguiente(int max, int min) {
  double r = (double) rand();
  double u = (double) RAND_MAX;
  double d = r / u;

  return (int)(d * (max - min)) + min;
}
