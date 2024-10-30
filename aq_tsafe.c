/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include "aq.h"
#include <stdlib.h>
#include "stdio.h"
#include <pthread.h>

typedef struct AlarmQueue1 {
    char MsgKind;
    void *meseg;
    struct AlarmQueue1* next;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} AlarmQueue1;

AlarmQueue aq_create( ) {
    AlarmQueue1 *aq = (AlarmQueue1*)malloc(sizeof (AlarmQueue1));
    aq -> MsgKind = -1;
    pthread_mutex_init(&aq->mutex, NULL);
    pthread_cond_init(&aq->cond, NULL);
    return (AlarmQueue) aq ;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
    AlarmQueue1 *head = (AlarmQueue1 *) aq;
    pthread_mutex_lock(&head -> mutex);
    if (k == AQ_ALARM && aq_alarms(aq) > 0) {
        pthread_mutex_unlock(&head->mutex);
        return AQ_NO_ROOM;
    }
    else{
        AlarmQueue1 *newNode = (AlarmQueue1*)malloc(sizeof(AlarmQueue1));
        newNode->meseg = msg;
        newNode->MsgKind = k;
        newNode->next = NULL;
        if (k == AQ_ALARM) {
            // Insert alarm message at the head
            newNode->next = head -> next;
            head -> next = newNode;
        } else {
            // Insert normal message at the end of the queue
            while (head->next != NULL) {
                head = head->next;
            }
            head->next = newNode;
        }
    }
    pthread_mutex_unlock(&head -> mutex);
    pthread_cond_signal(&head -> cond);
    return 0;
}


int aq_recv( AlarmQueue aq, void * * msg) {
    AlarmQueue1 *head = (AlarmQueue1*)aq;
    pthread_mutex_lock(&head -> mutex);

    while (head -> next == NULL) {
        pthread_cond_wait(&head -> cond, &head -> mutex);
    }

    if (head == NULL || head -> next == NULL) {
        pthread_mutex_unlock(&head->mutex);
        return AQ_NO_MSG;
    }

    AlarmQueue1 *nodeToRemove = head->next;
    *msg = nodeToRemove->meseg;
    int msgType = nodeToRemove->MsgKind;

    // Remove the node from the queue
    head->next = nodeToRemove->next;
    free(nodeToRemove);

    pthread_mutex_unlock(&head -> mutex);
    return msgType;
}

int aq_size(AlarmQueue aq) {
    AlarmQueue1* head = aq;
    pthread_mutex_lock(&head -> mutex);
    int count = 0;

    head = head -> next;

    while (head != NULL) {
        count++; // Increment for each message
        head = head->next; // Move to the next node
    }

    pthread_mutex_unlock(&head -> mutex);
    return count; // Return total count of messages
}

int aq_alarms( AlarmQueue aq) {
    //should make a counter for the number of alarms
    AlarmQueue1 *head = aq;
    pthread_mutex_lock(&head -> mutex);

    int i = 0;
    while (head != NULL) {
        if(head -> MsgKind == AQ_ALARM){
            i++;
        }
        head = head -> next;
    }
    pthread_mutex_unlock(&head -> mutex);
    return i;
}