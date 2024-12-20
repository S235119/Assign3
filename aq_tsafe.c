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

int aq_send(AlarmQueue aq, void *msg, MsgKind k) {
    AlarmQueue1 *head = (AlarmQueue1 *)aq;
    pthread_mutex_lock(&head->mutex);

    while (k == AQ_ALARM && aq_alarms(aq) > 0) {
        pthread_cond_wait(&head->cond, &head->mutex);
    }

    if (k == AQ_ALARM && aq_alarms(aq) > 0) {
        pthread_mutex_unlock(&head->mutex);
        return AQ_NO_ROOM;
    }

    AlarmQueue1 *newNode = (AlarmQueue1 *)malloc(sizeof(AlarmQueue1));
    newNode->meseg = msg;
    newNode->MsgKind = k;
    newNode->next = NULL;

    if (k == AQ_ALARM) {
        newNode->next = head->next;
        head->next = newNode;
    } else {
        AlarmQueue1 *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    pthread_mutex_unlock(&head->mutex);
    pthread_cond_signal(&head->cond);


    return 0;

}



int aq_recv( AlarmQueue aq, void * * msg) {
    AlarmQueue1 *head = (AlarmQueue1*)aq;
    pthread_mutex_lock(&head -> mutex);

    while (aq_size(aq) == 0) {
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
    pthread_cond_signal(&head -> cond);

    pthread_mutex_unlock(&head -> mutex);
    return msgType;
}

int aq_size(AlarmQueue aq) {
    AlarmQueue1* head = aq;
    int count = 0;

    head = head -> next;

    while (head != NULL) {
        count++;
        head = head->next;
    }

    return count;
}

int aq_alarms( AlarmQueue aq) {
    AlarmQueue1 *head = aq;

    head = head -> next;
    //Since there can only be one alarm in the queue, we just check if the header is an alarm
    //In case head -> next is NULL we return 0, so that the program will run
    if (head  == NULL) {
        return 0;
    }
    if(head -> MsgKind == AQ_ALARM){
        return 1;
    } else return 0;
}