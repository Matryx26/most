#ifndef MOST_H
#define MOST_H

#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int w_miescie_A;
    int czeka_A;
    int na_moscie;
    char kierunek;
    int czeka_B;
    int w_miescie_B;
    
    pthread_mutex_t mutex_stan;
    sem_t sem_most;
} StanMostu;

extern StanMostu stan;
extern int N;

void wypisz_stan(void);
void* samochod_watek(void* arg);

#endif