/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include "aq.h"
#include <stdlib.h>

typedef struct AlarmQueue1 {
    char MsgKind;
    void *meseg;
    struct AlarmQueue* next;
} AlarmQueue1;

AlarmQueue aq_create( ) {
    AlarmQueue1 *aq = (AlarmQueue1*)malloc(sizeof (AlarmQueue1));
    return (AlarmQueue) aq ;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
    if(k == AQ_ALARM && aq_alarms(aq)!= 0){
        return AQ_NO_ROOM;
    } else{
        AlarmQueue1 *newNode = (AlarmQueue1*)malloc(sizeof (AlarmQueue1));
        AlarmQueue1 *head = aq;
        newNode -> meseg = msg;
        newNode -> MsgKind = k;
        newNode -> next = NULL;

        while (head != NULL) {
            if(head -> next == NULL){
                head -> next = newNode;
                break;
            }
            head = head -> next;
        }
        return 0;
    }

}

int aq_recv( AlarmQueue aq, void * * msg) {
    if (aq_size(aq) != 0) {
        return AQ_NO_MSG;
    } else{
        AlarmQueue1 *head = aq;
        AlarmQueue1 *curent = NULL;

        char type;
        while (head != NULL) {
            if (head->meseg == msg) {
                type = head -> MsgKind;
            }
            head = head->next;
        }

        head = aq;
        curent = head -> next;
        free(head);
        head = curent;
        return type;
    }
}

int aq_size( AlarmQueue aq) {
    AlarmQueue1 *head = aq;
    int i = 0;
    while (head != NULL) {
        i++;
        head = head -> next;
    }
    return i;
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