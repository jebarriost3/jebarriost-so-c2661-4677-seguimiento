#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "jardin_ornamental_general.h"

void usage(char const *progname, DWORD codigo_salida);
int random_siguiente(int max, int min);

DWORD WINAPI visitante(PVOID pv_arg);
DWORD WINAPI admin(PVOID pv_arg);

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
    fprintf(stderr, "Error: Estructura del jardin ornamental no pudo ser creada\r\n");
    ExitProcess(EXIT_FAILURE);
  }

  fprintf(stdout,
          "Numero visitanes antes de iniciar: %d\r\n",
          obtener_miembros_jardin_general(jardin));

  HANDLE h_hilos[maximo + 1];
  DWORD h_hilos_id[maximo + 1];

  srand(time(NULL));

  for (int i = 0; i < maximo; i++) {
    h_hilos[i] = CreateThread(NULL,
                              0,
                              visitante,
                              (PVOID) jardin,
                              0,
                              &h_hilos_id[i]);

    if (h_hilos[i] == NULL) {
      fprintf(stderr,
              "Error: No se pudo crear %d-esimo hilo debido a %lu\r\n",
              i,
              (unsigned long) GetLastError());
      ExitProcess(EXIT_FAILURE);
    }
  }

  h_hilos[maximo] = CreateThread(NULL,
                                 0,
                                 admin,
                                 (PVOID) jardin,
                                 0,
                                 &h_hilos_id[maximo]);

  if (h_hilos[maximo] == NULL) {
    fprintf(stderr,
            "Error: No se pudo crear hilo administrador debido a %lu\r\n",
            (unsigned long) GetLastError());
    ExitProcess(EXIT_FAILURE);
  }

  WaitForMultipleObjects(maximo + 1, h_hilos, TRUE, INFINITE);

  for (int i = 0; i < maximo + 1; i++) {
    CloseHandle(h_hilos[i]);
  }

  fprintf(stdout,
          "Numero visitanes antes de cerrar jardin: %d\r\n",
          obtener_miembros_jardin_general(jardin));

  cerrar_jardin_general(jardin);

  ExitProcess(EXIT_SUCCESS);
}

void
usage(char const* progname, DWORD codigo_salida) {
  fprintf(stderr,
          "Uso: %s [-l <limite>] [-m <maximo>]\r\n"
          "     %s [-m <maximo>]\r\n"
          "     %s -h\r\n",
          progname, progname, progname);

  ExitProcess(codigo_salida);
}

DWORD WINAPI
visitante(PVOID pv_arg) {
  PJardinGeneral jardin = (PJardinGeneral) pv_arg;
  DWORD hilo_id = GetCurrentThreadId();

  for (int veces = 0; veces < MAX_VISITAS; veces++) {
    fprintf(stdout, "Visitante %lu afuera\r\n", (unsigned long) hilo_id);
    Sleep(random_siguiente(TIEMPO_AFUERA, 0));

    entrar_jardin_general(jardin);
    fprintf(stdout, "Visitante %lu dentro del jardin\r\n", (unsigned long) hilo_id);

    Sleep(random_siguiente(TIEMPO_VISITA, 0));
    salir_jardin_general(jardin);
  }

  fprintf(stdout, "Visitante %lu termino su jornada\r\n", (unsigned long) hilo_id);

  return 0;
}

DWORD WINAPI
admin(PVOID pv_arg) {
  PJardinGeneral jardin = (PJardinGeneral) pv_arg;

  for (int veces = 0; veces < MAX_VISITAS; veces++) {
    Sleep(random_siguiente(TIEMPO_AFUERA * 2, 0));
    fprintf(stdout,
            "Visitantes %d dentro del jardin\r\n",
            obtener_miembros_jardin_general(jardin));
  }

  fprintf(stdout, "Administrador termino su jornada\r\n");

  return 0;
}

int
random_siguiente(int max, int min) {
  double r = (double) rand();
  double u = (double) RAND_MAX;
  double d = r / u;

  return (int)(d * (max - min)) + min;
}
