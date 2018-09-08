#ifndef LISTMAKER
#define LISTMAKER

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>

//TO-DO: Find line where error occurred while reading input
//TO-DO: nel caso di memory leak sostiutiamo ad exit una funzione che termina liberando tutta la memoria (free)
extern int errno;

typedef enum {NEW = 0, 
              READY = 1, 
              RUNNING = 2, 
              BLOCKED = 3, 
              EXIT = 4, 
              N_STATES = 5 //Not an actual state
            } state_t;

typedef enum {CORE0 = 0,
              CORE1 = 1,
            } core_t;

typedef struct instruction {
    unsigned int type_flag; //1 = bloccante, 0 = non bloccante
    unsigned int length;

    struct instruction * next;
    struct instruction * prev;
} instruction_t;

typedef struct tcb {
    unsigned int id;
    unsigned int arrival_time;
    unsigned int service_time;
    state_t state;
    core_t core; //core che l'ha bloccato
    unsigned int wait_time; //ck attuale + tempo d'attesa randomico
    instruction_t * pc;
    instruction_t * instr_list;
    instruction_t * last;
    
    struct tcb * next;
    struct tcb * prev;
} task_t;

typedef struct task_list {
    task_t * first;
    task_t * last;
} task_list_t;

typedef struct thread_args {
    task_list_t * task_lists;
    FILE * fw_np;
    core_t core;
} thread_args_t;

task_t* createTask(unsigned int id, unsigned int arrival_time);

void addTask_bottom(task_list_t * tasks, task_t * new_task);

void addTask_sortedList(task_list_t * tasks, task_t * new_task);

void addBlockedTask(task_list_t * tasks, task_t * new_task);

//lista da cui lo togliamo (first e last), puntatore all'elemento da togliere
task_t * removeTask(task_list_t * tasks, task_t * del);

void moveTask(task_list_t task_lists[], state_t state_source, state_t state_dest, task_t * t);

instruction_t * createIstruction(unsigned int type_flag, unsigned int length);

void addInstruction(task_t * tasks, instruction_t * new_instr);

void print_input(task_list_t * tasks, char *c, int print_instr);

bool read_input(task_list_t * tasks, char * filename);

void freexit(task_list_t task_lists[], int EXIT_CODE);

void freeOK(task_list_t task_lists[]);

#endif