//
//  main.c
//  COEN_Lab10
//
//  Created by OG Bobby  on 6/4/22.
//

#include "global_defs.h"

// global variables
struct node *head=NULL, *tail=NULL;
int appointments=0;
struct input_statistics inp_stats[]={
    {"add", 0},
    {"pop", 0},
    {"show", 0},
    {"show_backward", 0},
    {"free", 0},
    {"delete_any", 0},
    {"quit", 0},
    {"search_by_size", 0},
    {"search_by_text", 0},
    {"check_duplicate", 0},
    {NULL, 0}};
/* lan 9. Multithreading support*/
const char* dataFileName=NULL;
pthread_t saver;
pthread_mutex_t lock;
int saver_done=0;

int main(int argc, char* argv[]) {
    // make sure there is one arg (two total, the program and the file)
    if (argc != 2) {
        printf("Please provide a file name as the argument\n");
        return -1;
    }
    dataFileName=argv[1];
    initialize_schedule(dataFileName);
    // create a mutex, start a thread
    pthread_mutex_init(&lock, NULL);
    pthread_create(&saver, NULL, saver_func, NULL);
    
    while (1) {
        int answer;
        print_manual();
        if (scanf("%d",&answer)==1) {
            if (process_answer(answer)) {
                /* tess us to quit */
                break;
            }
        } else {
            /* clean the bad input */
            char temp[MAXCOMMANDNAME+1];
            fgets(temp,MAXCOMMANDNAME, stdin);
        }
    }
    // tell the saver to stop
    saver_done=1;
    // join the saver
    pthread_join(saver, NULL);
    pthread_mutex_destroy(&lock);
    save_schedule(dataFileName);
    // empty the list
    quit_recursive(head);
    head=NULL;
    tail=NULL;
    return 0;
}


// general support functions
// User interface for example
void print_manual(void) {
    printf("manual\n");
    //these options are for the user
    printf("1 add\n");
    printf("2 pop\n");
    //free function, function name will be declared as "remove"

    printf("3 show\n");
    printf("4 list by letter\n");
    printf("5 search by group size\n");
    printf("6 delete any reservation\n");
    printf("7 show backward\n");
 //   printf("7 count letters\n");
    printf("8 free\n");
    printf("9 duplicates\n");
    printf("10 search by name\n");
    printf("11 print statistics\n");
    printf("0 No change\n");
    printf("answer:");
}

int process_answer(int answer) {
    //int count=0;
    switch(answer)
    {
        case 1:
        schedule1();
        break;

        case 2:
        cancel();
        break;

        case 3:
        show_recursive(head);
        break;

        case 4:
        letters1();
        break;

        case 5:
        groupsize();
        break;

        case 6:
        delete_any_reservation();
        break;

        case 7:
        show_backward_recursive(head);
        break;

        case 8:
        Remove();
        break;
        
        case 9:
        duplicates();
        break;

        case 10:
        searchletter();
        break;
            
        case 11:
        print_statistics();
        break;

        case 0:
            // quit
            record_command("quit");
        return 1;
        break;

        default:
        printf("The time %d is not available\n",answer);
        break;
    }
    
    return 0;

}

void display_reservation(const struct reservation* current) {
    printf("%dpm is the time for %1s and %2d is the group size. Max age is %d.\n", current->order, current->name,current->gSize, current->max_age);
    if (current->max_age < 18) {
        printf("    Guardian: %s\n", current->extra_info.guardian);
    } else if (current->max_age > 65) {
        printf("    Prior lessons: %d\n", current->extra_info.prior_lessons);
    } else {
        printf("    Average age: %g\n", current->extra_info.average_age);
    }
    if (current->phone_contact==1) {
        printf("    Address: %s\n", current->contact_info.address);
    } else {
        printf("    Phone: ");
        int i;
        for (i=0; i<10; i++) {
            printf("%d", current->contact_info.phone[i]);
        }
        printf("\n");
    }
}

void print_statistics(void) {
    struct input_statistics *current = inp_stats;
    while (current->name != NULL) {
        printf("%s: %d\n", current->name, current->ranTimes);
        current++;
    }
}

void record_command(const char* command) {
    struct input_statistics *current = inp_stats;
    while (current->name != NULL) {
        if (strcmp(current->name, command) == 0) {
            current->ranTimes++;
            break;
        }
        current++;
    }
}
