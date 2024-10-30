/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include "aq.h"
#include <stdlib.h>
#include "stdio.h"

typedef struct AlarmQueue1 {
    char MsgKind;
    void *meseg;
    struct AlarmQueue* next;
} AlarmQueue1;

AlarmQueue aq_create( ) {
    AlarmQueue1 *aq = (AlarmQueue1*)malloc(sizeof (AlarmQueue1));
    aq -> MsgKind = -1;
    return (AlarmQueue) aq ;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
    AlarmQueue1 *head = (AlarmQueue1 *) aq;
    if (k == AQ_ALARM && aq_alarms(aq) > 0) {
        return AQ_NO_ROOM;
    } else if(head -> meseg == NULL){
        head -> MsgKind = k;
        head -> meseg = msg;
        head -> next = NULL;
    }
    else{
        AlarmQueue1 *newNode = (AlarmQueue1*)malloc(sizeof(AlarmQueue1));
        newNode->meseg = msg;
        newNode->MsgKind = k;
        newNode->next = NULL;
        AlarmQueue1 *head = (AlarmQueue1 *) aq;
        /*if (k == AQ_ALARM) {
            // Insert alarm message at the head
            newNode->next = head -> next;
            head -> next = newNode;
        } else {*/
            // Insert normal message at the end of the queue
            while (head->next != NULL) {
                head = head->next;
            }
            head->next = newNode;
        //}
    }
    return 0;
}


int aq_recv( AlarmQueue aq, void * * msg) {
    AlarmQueue1 *head = (AlarmQueue1*)aq;
    if (head == NULL || head -> next == NULL) {
        return AQ_NO_MSG;
    }
    AlarmQueue1 *trueHead = head->next;


    *msg = trueHead->meseg;
    int msgType = trueHead->MsgKind;

    head->next = trueHead->next;
    free(trueHead);

    return msgType;
}

int aq_size(AlarmQueue aq) {
    AlarmQueue1* head = aq;
    int count = 0;

    printf("Counting messages in the queue...\n"); // Debug print

    while (head != NULL) {
        count++; // Increment for each message
        printf("Message %d: Type = %d\n", count, head->MsgKind); // Debug print to show type of message
        head = head->next; // Move to the next node
    }

    printf("Total messages in the queue: %d\n", count); // Final count
    return count; // Return total count of messages
}

int aq_alarms( AlarmQueue aq) {
    //should make a counter for the number of alarms
    AlarmQueue1 *head = aq;

    int i = 0;
    while (head != NULL) {
        if(head -> MsgKind == AQ_ALARM){
            i++;
        }
        head = head -> next;
    }
    return i;
}