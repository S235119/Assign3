/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include "aq.h"
typedef struct AlarmQueue1 {
    char MsgKind;
    void *msg;
    struct AlarmQueue* next;
} AlarmQueue1;

AlarmQueue aq_create( ) {
    AlarmQueue1 *aq = NULL;
    return (AlarmQueue) aq ;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
    return AQ_NOT_IMPL;
}

int aq_recv( AlarmQueue aq, void * * msg) {
    if (aq_size(aq) != 0) {
        if (aq_alarms(aq) != 0) {
            return AQ_NO_MSG;
        }
        return AQ_NO_ROOM;
    }
    return 1;
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