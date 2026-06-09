#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "most_b.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Użycie: %s <liczba_samochodow_N>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "Błąd: N musi być > 0.\n");
        return 1;
    }

    srand(time(NULL));

    stan.w_miescie_A = 0;
    stan.czeka_A = 0;
    stan.na_moscie = 0;
    stan.czeka_B = 0;
    stan.w_miescie_B = 0;
    stan.most_zajety = 0;

    pthread_mutex_init(&stan.mutex_stan, NULL);
    pthread_cond_init(&stan.cond_most, NULL);

    pthread_t* watki = malloc(N * sizeof(pthread_t));

    for (int i = 0; i < N; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&watki[i], NULL, samochod_watek, id) != 0) {
            perror("Błąd tworzenia wątku");
            return 1;
        }
    }

    pthread_join(watki[0], NULL);

    free(watki);
    pthread_mutex_destroy(&stan.mutex_stan);
    pthread_cond_destroy(&stan.cond_most);

    return 0;
}
