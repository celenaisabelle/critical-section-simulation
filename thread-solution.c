/*
 * Assignment 3: Critical Section Problem
 * Name: Celena Valenzuela
 * Compile: gcc -o fname thread-solution.c -lpthread
 * Run: ./fname
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DEPOSIT_LIMIT 2000000  // total deposit operations (bonus-inclusive)
#define WITHDRAW_LIMIT 2000000 // total withdraw operations

static int balance = 0; // shared
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int total_deposits;
    int bonus_count;
} depositor_stats_t;

void* depositor(void* arg) {
    depositor_stats_t *stats = (depositor_stats_t*)arg;
    stats->total_deposits = 0;
    stats->bonus_count = 0;

    while (1) {
        int add_amount = 1;

        /* Entry */
        pthread_mutex_lock(&mtx);

        /* Critical (one update only) */
        if (stats->total_deposits < DEPOSIT_LIMIT) {
            if ((balance % 200 == 0) && (stats->total_deposits + 50 <= DEPOSIT_LIMIT)) {
                add_amount = 50;
                stats->bonus_count++;
            } else {
                add_amount = 1;
            }

            if (stats->total_deposits + add_amount > DEPOSIT_LIMIT) {
                add_amount = DEPOSIT_LIMIT - stats->total_deposits;
            }

            if (add_amount > 0) {
                balance += add_amount;
                stats->total_deposits += add_amount;
            }
        }

        /* Exit */
        pthread_mutex_unlock(&mtx);

        /* Remainder */
        if (stats->total_deposits >= DEPOSIT_LIMIT) break;
    }

    printf("I'm Thread A, I did %d deposit operations and I got the bonus %d times. balance = %d\n",
        stats->total_deposits, stats->bonus_count, balance);
    return NULL;
}

void* withdrawer(void* arg) {
    (void)arg;
    int withdraw_count = 0;

    while (withdraw_count < WITHDRAW_LIMIT) {
        /* Entry */
        pthread_mutex_lock(&mtx);

        /* Critical (one update only) */
        balance -= 1;
        withdraw_count += 1;

        /* Exit */
        pthread_mutex_unlock(&mtx);

        /* Remainder */
        ;
    }

    printf("I'm Thread B, I did %d withdraw operations. balance = %d\n",
        withdraw_count, balance);
    return NULL;
}

int main(void) {
    pthread_t ta, tb;
    depositor_stats_t stats;

    setvbuf(stdout, NULL, _IOLBF, 0);

    if (pthread_create(&ta, NULL, depositor, &stats) != 0) {
        perror("pthread_create depositor");
        return 1;
    }
    if (pthread_create(&tb, NULL, withdrawer, NULL) != 0) {
        perror("pthread_create withdrawer");
        return 1;
    }

    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    printf("From parent: Final balance = %d\n", balance);
    return 0;
}
