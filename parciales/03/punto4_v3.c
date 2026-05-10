#include <pthread.h>
#include <semaphore.h>

struct ControlLateral {
    sem_t activacion;
    sem_t accion_disparo;
    sem_t recarga;

    struct ControlLateral *sig_etapa;
    pthread_t id_hilo_control_lateral;
};

struct ControlLateral *iniciarV3(int netapas);

void control_proyectil(struct ControlLateral estructura_control[],
                       int n_etapas)
{
    struct ControlLateral *etapa = &estructura_control[0];

    for (int i = 0; i < n_etapas && etapa != NULL; i++) {
        sem_post(&etapa->activacion);
        sem_wait(&etapa->accion_disparo);
        etapa = etapa->sig_etapa;
    }
}

void control_recarga(struct ControlLateral estructura_control[],
                     int n_etapas)
{
    for (int i = 0; i < n_etapas; i++) {
        sem_post(&estructura_control[i].recarga);
    }
}

static void *hilo_control_disparo_lateral(void *arg)
{
    struct ControlLateral *pControlLateral =
        (struct ControlLateral *) arg;

    while (1) {
        sem_wait(&pControlLateral->activacion);
        sem_post(&pControlLateral->accion_disparo);
        sem_wait(&pControlLateral->recarga);
    }

    return NULL;
}
