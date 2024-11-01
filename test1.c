#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "aq.h"
#include "aux.h"

static AlarmQueue q;

void *producer_alarm(void *arg) {
    put_alarm(q, 1);  // First alarm should succeed
    msleep(200);      // Ensure consumer has time to get the first alarm
    put_alarm(q, 2);  // Second alarm should block until the first alarm is received
    printf("Second alarm sent after unblock\n");
    return NULL;
}

void *consumer(void *arg) {
    msleep(100);      // Allow the producer to start
    int received = get(q);  // Get the first alarm
    printf("Received alarm with value: %d\n", received);
    return NULL;
}

int main() {
    q = aq_create();

    pthread_t t1, t2;
    pthread_create(&t1, NULL, producer_alarm, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
