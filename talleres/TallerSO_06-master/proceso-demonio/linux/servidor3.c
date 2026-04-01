#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

#include "../../tuberias-nombradas/linux/tuberias_nombradas.h"
#include "../../tuberias-nombradas/linux/servidor.h"

static int consecutivos[MAXIMO + 1] = { 0, 0, 0 };

static const char *tuberia_peticion = TUBERIA_PETICION;
static const char *tuberia_solicitud = TUBERIA_SOLICITUD;
static int crear_tuberias = 0;
static int modo_demonio = 0;

void uso(const char *programa);
void manejar_sigquit(int sig);
int iniciar_demonio(void);

int
main(int argc, char *argv[]) {

  int opcion;

  while ((opcion = getopt(argc, argv, "dcp:s:")) != -1) {
    switch (opcion) {
      case 'd':
        modo_demonio = 1;
        break;
      case 'c':
        crear_tuberias = 1;
        break;
      case 'p':
        tuberia_peticion = optarg;
        break;
      case 's':
        tuberia_solicitud = optarg;
        break;
      default:
        uso(argv[0]);
    }
  }

  signal(SIGQUIT, manejar_sigquit);

  if (crear_tuberias) {
    crear_tuberia(tuberia_peticion);
    crear_tuberia(tuberia_solicitud);
  }

  if (modo_demonio) {
    if (iniciar_demonio() == -1) {
      fprintf(stderr, "Error convirtiendo el proceso en demonio\n");
      _exit(EXIT_FAILURE);
    }
  }

  int df_lectura = -2;

  for (;;) {

    if (df_lectura == -2 &&
        (df_lectura = open(tuberia_peticion, O_RDONLY)) == -1) {
      _exit(EXIT_FAILURE);
    }

    char amortiguador_entrada[LONGITUD_MENSAJE_PETICION];

    ssize_t caracteres_leidos =
      read(df_lectura, amortiguador_entrada, LONGITUD_MENSAJE_PETICION);

    if (caracteres_leidos < 0) {
      close(df_lectura);
      break;
    }

    if (caracteres_leidos > 0) {

      amortiguador_entrada[FIN_PID] = '\0';
      amortiguador_entrada[LONGITUD_MENSAJE_PETICION - 1] = '\0';

      pid_t proceso_cliente = atoi(amortiguador_entrada);
      Consecutivo_t consecutivo = atoi(&amortiguador_entrada[FIN_PID + 1]);

      int df_escritura;

      if ((df_escritura = open(tuberia_solicitud, O_WRONLY)) == -1) {
        _exit(EXIT_FAILURE);
      }

      char amortiguador_salida[LONGITUD_MENSAJE_SOLICITUD];

      bzero(amortiguador_salida, LONGITUD_MENSAJE_SOLICITUD);
      sprintf(amortiguador_salida, "%06d %06d\n",
              proceso_cliente, consecutivos[consecutivo]++);
      write(df_escritura, amortiguador_salida, LONGITUD_MENSAJE_SOLICITUD);

      close(df_escritura);
    }
    else {
      close(df_lectura);
      df_lectura = -2;
    }
  }

  return EXIT_SUCCESS;
}

void
uso(const char *programa) {
  fprintf(stderr,
          "Uso: %s [-d] [-c] [-p <peticion-nombre>] [-s <solicitud-nombre>]\n",
          programa);
  _exit(EXIT_FAILURE);
}

void
manejar_sigquit(int sig) {
  (void)sig;
  borrar_tuberia(tuberia_peticion);
  borrar_tuberia(tuberia_solicitud);
  _exit(EXIT_SUCCESS);
}

int
iniciar_demonio(void) {
  pid_t pid;

  pid = fork();

  if (pid < 0) {
    return -1;
  }

  if (pid > 0) {
    _exit(EXIT_SUCCESS);
  }

  if (setsid() < 0) {
    return -1;
  }

  if (chdir("/") < 0) {
    return -1;
  }

  umask(0);

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  return 0;
}
