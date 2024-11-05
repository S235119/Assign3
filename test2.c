#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "aq.h"
#include "aux.h"

// Global queue that will be used by both threads to send and receive messages
static AlarmQueue q;

// Producer function: sends an alarm message first, then a normal message
void *producer1(void *arg) {

    put_normal(q, 3);

    msleep(150);

    put_normal(q, 4);

    return NULL;  // Thread function returns NULL
}

void *producer2(void *arg) {

    put_alarm(q, 50);

    msleep(150);

    put_normal(q, 7);


    return NULL;  // Thread function returns NULL
}

// Consumer function: receives messages from the queues
void *consumer(void *arg) {

    msleep(100);
    get(q);
    get(q);
    msleep(200);
    get(q);
    get(q);

    return NULL;  // Thread function returns NULL
}

int main() {
    // Initialize the alarm queue before starting any threads
    q = aq_create();

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, producer1, NULL);
    pthread_create(&t3, NULL, producer2, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}
