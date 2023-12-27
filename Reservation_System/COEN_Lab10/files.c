//Norwood
//

#include "global_defs.h"

void initialize_schedule(const char * file_name) {
  /*  int i;
    struct reservation *current=schedule;
    for(i=0; i<SIZE; i++){
        current->name[0]='\0';
        current->order=0;
        current->gSize=0;
        current++;
    }*/
    appointments=0;
    head=NULL;
    tail=NULL;
    // try to load the schedule from the file
    FILE* inf = fopen(file_name, "r");
    if (inf != NULL) {
        // read the content, all fields possible
        // skip the headers to the new line
        while (1) {
            int c = getc(inf);
            if (c == EOF || c == '\n') {
                break;
            }
        }

        
        
        char name[MAXNAME+1];
        int gSize;
        int order;
        int max_age;
        char guardian[MAXNAME+1];
        int prior_lessons;
        float average_age;
        int phone_contact; // a flag if the contact is a phone
        char address[128];
        long phone;

        while (fscanf(inf, " %[^\t]%d %d %d %[^\t]%d %g %d %[^\t]%ld ", name, &gSize, &order, &max_age, guardian, &prior_lessons, &average_age, &phone_contact, address, &phone)==10) {
            // read ok, add to the schedule
            
            
            
            
            /// create a node f or the appointment
            struct node *newNode = malloc(sizeof(struct node));
            newNode->next=NULL; // make sure it is null
            struct reservation* current = &newNode->data;
            strncpy(current->name, name, MAXNAME);
            current->gSize = gSize;
            current->order=appointments+1;
            current->max_age=max_age;
            // decide on exntra info
            if (max_age < 18) {
                strncpy(current->extra_info.guardian, guardian, MAXNAME);
            } else if (max_age > 65) {
                current->extra_info.prior_lessons=prior_lessons;
            } else {
                // the average age
                current->extra_info.average_age=average_age;
            }
            // read the contact info
            current->phone_contact=phone_contact;
            if (current->phone_contact==2) {
                // read 10 digit phone
                int i;
                for (i=9; i>=0; i--) {
                    // extract the last digit
                    current->contact_info.phone[i] = phone % 10;
                    // discard the last digit
                    phone /= 10;
                }
            } else if (current->phone_contact==1) {
                strncpy(current->contact_info.address, address, 128);
            } else {
                printf("Wrong entry\n");
                free(newNode);
                return;
            }
            
            // duplicates();
            
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

            
        }
        
        fclose(inf);
    }
}

void save_schedule(const char * file_name) {
    FILE* outf = fopen(file_name, "w");
    if (outf != NULL) {
        fprintf(outf, "Name\tGroup Size\tOrder\tMax Age\tGuardian\tPrior Lessons\tAverage Age\tContact Type\tAddress\tPhone\n");
        // walk the list and save
        struct node* cur=head;
        while (cur != NULL) {
            char name[MAXNAME+1]="NA";
            int gSize;
            int order;
            int max_age;
            char guardian[MAXNAME+1]="NA";
            int prior_lessons=0;
            float average_age=0;
            int phone_contact; // a flag if the contact is a phone
            char address[128]="NA";
            long phone=0;
            
            struct reservation* current = &cur->data;
            
            // fill out the fields
            strncpy(name, current->name, MAXNAME);
            gSize= current->gSize;
            order = current->order;
            max_age = current->max_age;
        
            if (current->max_age < 18) {
                strncpy(guardian, current->extra_info.guardian, MAXNAME);
            } else if (current->max_age > 65) {
                prior_lessons=current->extra_info.prior_lessons;
            } else {
                average_age=current->extra_info.average_age;
            }
            phone_contact=current->phone_contact;
            if (current->phone_contact==1) {
                strncpy(address, current->contact_info.address, 128);
            } else {
                int i;
                for (i=0; i<10; i++) {
                    phone = 10 * phone + current->contact_info.phone[i];
                }
            }
            
            fprintf(outf, "%s\t%d\t%d\t%d\t%s\t%d\t%g\t%d\t%s\t%ld\n", name, gSize, order, max_age, guardian, prior_lessons, average_age, phone_contact, address, phone);
            cur = cur->next;
        }
        fclose(outf);
    } else {
        printf("Unable to open file %s for writing!\n", file_name);
    }
}

void* saver_func(void* arg) {
    while (saver_done==0) {
        // run until signaled to exit
        sleep(10);
        // critical section
        // obtain the lock
        pthread_mutex_lock(&lock);
        // open the file as binary and save
        
        FILE* binFile = fopen(dataFileName, "wb");
        if (binFile != NULL) {
            // walk the list and save data
            struct node *cur_node = head;
            while (cur_node != NULL) {
                fwrite(&cur_node->data, sizeof(cur_node->data), 1, binFile);
                cur_node = cur_node->next;
            }
            fclose(binFile);
        }
        
        pthread_mutex_unlock(&lock);
    }
    return  NULL;
}

