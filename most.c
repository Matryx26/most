#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "most.h"

StanMostu stan;
int N;

void wypisz_stan(void) {
    pthread_mutex_lock(&stan.mutex_stan);
    
    int miasto_A_wolne = stan.w_miescie_A - stan.czeka_A;
    int miasto_B_wolne = stan.w_miescie_B - stan.czeka_B;
    
    if (stan.na_moscie > 0) {
        if (stan.kierunek == '>') {
            printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n", 
                   miasto_A_wolne, stan.czeka_A, stan.na_moscie, stan.czeka_B, miasto_B_wolne);
        } else {
            printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n", 
                   miasto_A_wolne, stan.czeka_A, stan.na_moscie, stan.czeka_B, miasto_B_wolne);
        }
    } else {
        printf("A-%d %d>>> [   Pusty   ] <<<%d %d-B\n", 
               miasto_A_wolne, stan.czeka_A, stan.czeka_B, miasto_B_wolne);
    }
    fflush(stdout);
    
    pthread_mutex_unlock(&stan.mutex_stan);
}

void* samochod_watek(void* arg) {
    int id = *(int*)arg;
    free(arg);
    
    int lokalizacja = rand() % 2; 
    
    pthread_mutex_lock(&stan.mutex_stan);
    if (lokalizacja == 0) stan.w_miescie_A++;
    else stan.w_miescie_B++;
    pthread_mutex_unlock(&stan.mutex_stan);
    
    wypisz_stan();

    while (1) {
        sleep(rand() % 3 + 1    );

        if (lokalizacja == 0) {
            pthread_mutex_lock(&stan.mutex_stan);
            stan.czeka_A++;
            pthread_mutex_unlock(&stan.mutex_stan);
            wypisz_stan();

            sem_wait(&stan.sem_most);

            pthread_mutex_lock(&stan.mutex_stan);
            stan.czeka_A--;
            stan.w_miescie_A--;
            stan.na_moscie = id;
            stan.kierunek = '>';
            pthread_mutex_unlock(&stan.mutex_stan);
            wypisz_stan();

            sleep(2);

            pthread_mutex_lock(&stan.mutex_stan);
            stan.na_moscie = 0;
            stan.w_miescie_B++;
            lokalizacja = 1;
            pthread_mutex_unlock(&stan.mutex_stan);
            wypisz_stan();

            sem_post(&stan.sem_most);

        } else {
            pthread_mutex_lock(&stan.mutex_stan);
            stan.czeka_B++;
            pthread_mutex_unlock(&stan.mutex_stan);
            wypisz_stan();

            sem_wait(&stan.sem_most);

            pthread_mutex_lock(&stan.mutex_stan);
            stan.czeka_B--;
            stan.w_miescie_B--;
            stan.na_moscie = id;
            stan.kierunek = '<';
            pthread_mutex_unlock(&stan.mutex_stan);
            wypisz_stan();

            sleep(2);

            pthread_mutex_lock(&stan.mutex_stan);
            stan.na_moscie = 0;
            stan.w_miescie_A++;
            lokalizacja = 0;
            pthread_mutex_unlock(&stan.mutex_stan);
            wypisz_stan();

            sem_post(&stan.sem_most);
        }
    }
    return NULL;
}