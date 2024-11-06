#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "aq.h"
#include "aux.h"

// Global queue to be shared between threads
static AlarmQueue q;

// Producer function to send normal and alarm messages
void *producer(void *arg) {
    for (int i = 1; i <= 5; i++) {
        put_normal(q, i);
        printf("Producer sent normal message: %d\n", i);
    }
    put_alarm(q, 99);
    printf("Producer sent alarm message: 99\n");
    return NULL;
}

// Consumer function to receive messages from the queue
void *consumer(void *arg) {
    for (int i = 0; i < 6; i++) {
        int received = get(q);  // Adjusted to only pass the queue argument
        printf("Consumer received message: %d\n", received);
    }
    return NULL;
}

int main() {
    // Initialize the alarm queue
    q = aq_create();

    // Create producer and consumer threads
    pthread_t prod, cons;
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for both threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
