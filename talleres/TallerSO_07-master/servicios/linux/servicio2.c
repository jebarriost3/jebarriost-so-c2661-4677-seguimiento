#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

char syslog_buffer[BUFFER_SIZE];
int contador = 0;

void manejador_senal(int signo) {
  contador++;
  sprintf(syslog_buffer, "Recibida señal: %d (contador=%d)", signo, contador);
  syslog(LOG_INFO | LOG_USER, "%s", syslog_buffer);

  if (contador >= 5) {
    syslog(LOG_INFO | LOG_USER, "Se alcanzaron 5 señales, terminando servicio");
    exit(0);
  }
}

int
main(void) {
  struct sigaction sa;

  sa.sa_handler = manejador_senal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);

  openlog("servicio2", LOG_CONS | LOG_PID, LOG_USER);
  syslog(LOG_INFO | LOG_USER, "Servicio2 iniciado");

  for (;;) {
    pause();
  }

  return EXIT_SUCCESS;
}
