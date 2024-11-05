#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "aq.h"
#include "aux.h"

// Global queue that will be used by both threads to send and receive messages
static AlarmQueue q;

// Producer function: sends an alarm message first, then a normal message
void *producer(void *arg) {
    // Add an alarm message with the value 42 to the queue
    // This alarm message should take priority when being received
    put_alarm(q, 42);

    // Introduce a small delay (100 ms) to simulate the natural flow of events
    // This delay helps ensure the consumer has time to receive the alarm message first
    msleep(100);

    // Now send a normal message with the value 7 to the queue
    // This message will be received after the alarm, assuming the alarm was picked up first
    put_normal(q, 7);

    return NULL;  // Thread function returns NULL
}

// Consumer function: receives messages from the queue
void *consumer(void *arg) {
    int received;

    // Receive the first message from the queue
    // Since alarm messages have priority, this should be the alarm message (value 42)
    received = get(q);

    // Receive the next message from the queue
    // This should be the normal message (value 7) if the queue's priority works as expected
    received = get(q);

    return NULL;  // Thread function returns NULL
}

int main() {
    // Initialize the alarm queue before starting any threads
    q = aq_create();

    // Create two threads: one for the producer and one for the consumer
    pthread_t t1, t2;
    pthread_create(&t1, NULL, producer, NULL);   // Start the producer thread
    pthread_create(&t2, NULL, consumer, NULL);   // Start the consumer thread

    // Wait for both threads to finish their tasks
    pthread_join(t1, NULL);  // Wait for the producer to finish
    pthread_join(t2, NULL);  // Wait for the consumer to finish

    return 0;  // End of program
}
