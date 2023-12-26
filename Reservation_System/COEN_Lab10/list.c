//
//  list.c
//  COEN_Lab10
//
//  Created by OG Bobby  on 6/4/22.
//

#include "global_defs.h"


void Remove(void) { //remove oldest and shift up
    record_command("free");
    
    pthread_mutex_lock(&lock);
    if(appointments==0)
        printf("There are no reserved appointments\n");
    else if (appointments == 1) {
        display_reservation(&head->data);
        free(head);
        head = NULL;
        tail = NULL;
        appointments--;
    } else{
        // shift the pointer to the first appt, and send next to the next appt
        struct node *newHead = head->next;
        display_reservation(&head->data);
        free(head);
        head = newHead;
        appointments--;
    }
    pthread_mutex_unlock(&lock);
}
void list(void) {
    record_command("show");
    if(appointments==0)
        printf("There are no reserved appointments\n");

    else{
        struct node *cur_node = head;
        while (cur_node != NULL) {
            display_reservation(&cur_node->data);
            cur_node = cur_node->next;
        }
    }
}

void delete_any_reservation(void) {
    record_command("delete_any");
    // get the index to remove
    int index;
    printf("What is the index (1 is first) to remove: ");
    scanf("%d", &index);
    if (index > 0 && index <= appointments) {
        pthread_mutex_lock(&lock);
        if (appointments==1) {
            display_reservation(&head->data);
            free(head);
            head = NULL;
            tail = NULL;
        } else {
            struct node* prev = NULL;
            struct node* curr = head;
            while (index > 1) {
                prev = curr;
                curr = curr->next;
                index--;
            }
            if (curr == head) {
                struct node* newHead=head->next;
                display_reservation(&head->data);
                free(head);
                head = newHead;
            } else if (curr == tail) {
                prev->next=NULL;
                display_reservation(&tail->data);
                free(tail);
                tail = prev;
            } else {
                prev->next = curr->next;
                display_reservation(&curr->data);
                free(curr);
            }
        }
        appointments--;
        pthread_mutex_unlock(&lock);
    } else {
        printf("Invalid index\n");
    }
    
}


void searchletter(void) {
    record_command("search_by_text");
    char name[MAXNAME+1];
    printf("What is the name to search?\n");
    scanf("%s", name);
    int name_len = (int)strlen(name);
    struct node *cur_node = head;
    while (cur_node != NULL) {
        if ((int)strlen(cur_node->data.name) >= name_len && strncmp(name, cur_node->data.name, name_len) == 0) {
            display_reservation(&cur_node->data);
        }
        cur_node = cur_node->next;
    }
    
}

void duplicates(void) {
    record_command("check_duplicate");
    
    // run the appointments and check every pair
    int found=0;
    struct node * current_node = head;
    while (current_node != NULL) {
        struct node* next_node = current_node->next;
        while (next_node != NULL) {
            if (strcmp(current_node->data.name, next_node->data.name) == 0) {
                printf("There is a duplicate entry for %s\n", current_node->data.name);
                found=1;
                break;
            }
            next_node=next_node->next;
        }
        if (found) {
            break;
        }
        current_node=current_node->next;
    }
    if (!found) {
        printf("There are no duplicates\n");
    }
    
}//check duplicates


void groupsize(void) {
    record_command("search_by_size");
    int gSize;
    printf("what is the group size?\n");
    scanf("%d",&gSize);
    struct node *cur_node = head;
    while (cur_node != NULL) {
        if (cur_node->data.gSize<=gSize) {
            display_reservation(&cur_node->data);
        }
        cur_node = cur_node->next;
    }
    
}


void cancel(void) { //new
    record_command("pop");
    pthread_mutex_lock(&lock);
    if(appointments==0)
        printf("There are no reserved appointments\n");
    else if (appointments == 1) {
        display_reservation(&head->data);
        free(head);
        head = NULL;
        tail = NULL;
        appointments--;
    } else{
        struct node* prev = head;
        struct node* cur = head->next;
        // move to the tail
        while (cur != tail) {
            prev = cur;
            cur = cur->next;
        }
        // cur is pointing to the tail
        // prev is the new tail
        prev->next = NULL;
        tail = prev;
        display_reservation(&cur->data);
        free(cur);
        appointments--;
    }
    pthread_mutex_unlock(&lock);
    
}


void schedule1(void) {
    record_command("add");


    char name[MAXNAME+1];
    int gSize, max_age;
    printf("What is your name?\n");
    scanf("%s", name);
    if (has_duplicates(name)) {
        // do not add the duplicate
        printf("There is a duplicate entry for %s\n", name);
        return;
    }
    /// create a node f or the appointment
    struct node *newNode = malloc(sizeof(struct node));
    newNode->next=NULL; // make sure it is null
    struct reservation* current = &newNode->data;
    printf("what is the group size?\n");
    scanf("%d",&gSize);
    if(gSize>5){
        printf("There are too many in that group\n");
    }
    printf("what is the maximum age?\n");
    scanf("%d",&max_age);
    strcpy(current->name, name);
    current->gSize = gSize;
    current->order=appointments+1;
    
    current->max_age=max_age;
    // decide on exntra info
    if (max_age < 18) {
        // read the guardian name
        printf("Guardian name?\n");
        scanf("%s", name);
        strcpy(current->extra_info.guardian, name);
    } else if (max_age > 65) {
        // read the prior lesson count
        int n;
        printf("Number of prior lessons?\n");
        scanf("%d", &n);
        current->extra_info.prior_lessons=n;
    } else {
        // the average age
        float a;
        printf("Average age of the group?\n");
        scanf("%g", &a);
        current->extra_info.average_age=a;
    }
    // read the contact info
    printf("1 - address contact; 2 - phone contact");
    scanf("%d", &current->phone_contact);
    if (current->phone_contact==2) {
        // read 10 digit phone
        long phone;
        int i;
        printf("Phone?\n");
        scanf("%ld", &phone);
        // parse the phone
        for (i=9; i>=0; i--) {
            // extract the last digit
            current->contact_info.phone[i] = phone % 10;
            // discard the last digit
            phone /= 10;
        }
    } else if (current->phone_contact==1) {
        // read the address string
        // read the string to the line end
        printf("Address?\n");
        fgets(current->contact_info.address, 128, stdin);
        fgets(current->contact_info.address, 128, stdin);
        int last_char = (int)strlen(current->contact_info.address) - 1;
        // discard \n and \r and the end
        while (last_char >=0 && (current->contact_info.address[last_char] == '\n' || current->contact_info.address[last_char] == '\r')) {
            current->contact_info.address[last_char] = 0;
            last_char--;
        }
    } else {
        printf("Wrong entry\n");
        free(newNode);
        return;
    }
    
    
    printf("Your time is %d\n", appointments+1);
    
    // duplicates();
    pthread_mutex_lock(&lock);
    // very firt appointment
    if (appointments==0) {
        head = newNode;
        tail = newNode;
    } else {
        // not the first appointment, add to the tail
        tail->next = newNode;
        tail = newNode;
    }
    appointments++;
    pthread_mutex_unlock(&lock);

}
// lab 8 implementations
void quit_recursive(struct node* cur) {
    if (cur != NULL) {
        // clean recursively
        quit_recursive(cur->next);
        free(cur);
    }
}
void show_recursive(struct node* cur) {
    if (cur == head) {
       // record only once
        record_command("show");
    }
    if(appointments==0)
        printf("There are no reserved appointments\n");

    else if (cur != NULL) {
        display_reservation(&cur->data);
        show_recursive(cur->next);
    }
}

void show_backward_recursive(struct node* cur) {
    if (cur == head) {
       // record only once
        record_command("show_backward");
    }
    if(appointments==0)
        printf("There are no reserved appointments\n");

    else if (cur != NULL) {
        show_backward_recursive(cur->next);
        display_reservation(&cur->data);
        
    }
}

void letters1(void) {
    
}

int has_duplicates(const char* name) {
    record_command("check_duplicate");
    struct node *cur_node = head;
    while (cur_node != NULL) {
        if (strcmp(name, cur_node->data.name)==0) {
            return 1;
        }
        cur_node = cur_node->next;
    }

    return 0;
}
