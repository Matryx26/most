#ifndef MOST_B_H
#define MOST_B_H

#include <pthread.h>

typedef struct {
    int w_miescie_A;
    int czeka_A;
    int na_moscie;
    char kierunek;
    int czeka_B;
    int w_miescie_B;

    int most_zajety;
    pthread_mutex_t mutex_stan;
    pthread_cond_t cond_most;
} StanMostu;

extern StanMostu stan;
extern int N;

void wypisz_stan(void);
void* samochod_watek(void* arg);

#endif
