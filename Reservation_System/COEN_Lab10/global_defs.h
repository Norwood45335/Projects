//Norwood

#ifndef global_defs_h
#define global_defs_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
//#define SIZE 5
#define MAXNAME 20
#define MAXCOMMANDNAME 20

void schedule1(void);
void cancel(void);//new
void Remove(void);//remove oldest and shift up
void list(void);
void letters1(void);
void groupsize(void);
void priority(void);
void duplicates(void);//check duplicates
void counter(int count);
void searchletter(void);
void print_statistics(void);
void delete_any_reservation(void);
int has_duplicates(const char* name);

struct reservation {
    char name[MAXNAME+1];
    int gSize;
    int order;
    int max_age;
    union {
        char guardian[MAXNAME+1];
        int prior_lessons;
        float average_age;
    } extra_info;
    int phone_contact; // a flag if the contact is a phone
    union  {
        char address[128];
        int phone[10];
    } contact_info;
};
// linked list, the very last node has next==NULL
struct node {
    struct reservation data;
    struct node *next;
};

struct input_statistics {
    const char *name;
    int ranTimes;
};

/* lab8 functions */
void show_recursive(struct node* cur);
void quit_recursive(struct node* cur);
void show_backward_recursive(struct node* cur);

/* helper commands */
void initialize_schedule(const char * file_name);
void save_schedule(const char * file_name);
void print_manual(void);
int process_answer(int answer);

void record_command(const char* command);

void display_reservation(const struct reservation* entry);

void* saver_func(void* arg);

// global variables

extern struct node *head, *tail;
extern int appointments;
extern struct input_statistics inp_stats[];

/* lan 9. Multithreading support*/
extern const char* dataFileName;
extern pthread_t saver;
extern pthread_mutex_t lock;
extern int saver_done;



#endif /* global_defs_h */
