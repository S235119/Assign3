#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "aq.h"
#include "aux.h"

// Global queue to be shared between threads
static AlarmQueue q;

// Producer function that sends alarm messages to the queue
void *producer_alarm(void *arg) {
    // Send the first alarm message. This should succeed right away since the queue is empty.
    put_alarm(q, 1);

    // Wait 200 milliseconds to give the consumer time to pick up the first alarm.
    // This sleep helps us create the situation where the next alarm will be blocked if the first one isn’t removed.
    msleep(200);

    // Try to send a second alarm message. Since the queue only allows one alarm at a time,
    // this should block until the first alarm is received by the consumer.
    put_alarm(q, 2);
    printf("Second alarm sent after unblock\n");  // Print after the second alarm is successfully sent.
    return NULL;
}

// Consumer function that receives messages from the queue
void *consumer(void *arg) {
    // Wait 100 milliseconds to allow the producer to send the first alarm message.
    // This slight delay ensures the first alarm is already in the queue when we try to get it.
    msleep(100);

    // Get and remove the first alarm message from the queue.
    // This should unblock the producer, allowing it to send a second alarm.
    int received = get(q);
    printf("Received alarm with value: %d\n", received);
    return NULL;
}

// Normal message sender to check ordering
void *producer_normal(void *arg) {
    // Wait for the other threads to handle alarms, then send a normal message
    msleep(300);

    // Send a normal message to the queue
    put_normal(q, 3);
    printf("Normal message sent after alarms\n");
    return NULL;
}

// Main function to create threads and manage the queue
int main() {
    // Create the alarm queue before starting the threads
    q = aq_create();

    // Create three threads: two for sending alarm messages, one for sending a normal message
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, producer_alarm, NULL);
    pthread_create(&t2, NULL, consumer, NULL);
    pthread_create(&t3, NULL, producer_normal, NULL);

    // Wait for all threads to complete
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}

