#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

typedef enum {NEW, READY, RUNNING, BLOCKED, EXIT} state_t;

typedef struct instruction {
    unsigned int type_flag;
    unsigned int length;

    struct instruction * next;
    struct instruction * prev;
} instruction_t;

typedef struct tcb {
    unsigned int id;
    unsigned int pc;
    unsigned int arrival_time;
    instruction_t * instr_list;
    instruction_t * last;
    state_t state;

    struct tcb * next;
    struct tcb * prev;
} task_t;

typedef struct task_list {
    task_t * first;
    task_t * last;
} task_list_t;

// TO-DO Move to main function


task_t* createTask(unsigned int id, unsigned int arrival_time) {
    task_t* new_task = (task_t*) malloc(sizeof(task_t));
    if (new_task == NULL) {
        perror("Error while creating a new task");
        exit(EX_OSERR);
    }

    new_task->id = id;
    new_task->arrival_time = arrival_time;
    new_task->pc = 0;
    new_task->instr_list = NULL;
    new_task->last = NULL;
    new_task->state = NEW;
    new_task->next = new_task->prev = NULL;

    return new_task;
}

void addTask(task_t * new_task) {
    if(tasks->first == NULL)
        tasks->first = new_task;
    else {
        tasks->last->next = new_task;
        new_task->prev = tasks->last;
    }
    tasks->last = new_task;
    return;
}

instruction_t * createIstruction(unsigned int type_flag, unsigned int length) {
    instruction_t* new_instr = (instruction_t*) malloc(sizeof(instruction_t));
    if (new_instr == NULL) {
        perror("Error while creating a new instruction");
        exit(EX_OSERR);
    }

    new_instr->type_flag = type_flag;
    new_instr->length = length;
    new_instr->next = new_instr->prev = NULL;

    return new_instr;
}
void addInstruction(instruction_t * new_instr) {
     if(tasks->last->instr_list == NULL)
        tasks->last->instr_list = new_instr;
    else {
        tasks->last->last->next = new_instr;
        new_instr->prev = tasks->last->last;
    }
    tasks->last->last = new_instr;
    return;
}

int read_input(char * filename) {

    if (freopen(filename, "r", stdin) == NULL) { //it associates the file input with the stdin
        perror("Looks like there's a problem with your input file");
        exit(EX_OSFILE);
    }
    
    printf("The input file exists! That's a good starting point, I guess.\n");

    char c_read;
    unsigned int n1, n2;
    
    while (scanf("%c,%u,%u", &c_read, &n1, &n2) != EOF) {
        if(c_read == 't') 
            addTask(createTask(n1,n2));
        else if(c_read == 'i') {
            addInstruction(createIstruction(n1,n2));
            printf("");
        }
        else
        {
            fprintf(stderr, "Error: input file %s is incorrectly formatted.\n", filename);
            exit(EX_DATAERR);
        }
            
    }

    if (!feof(stdin)) { //errore nel raggiungere la fine del file
      fprintf(stderr, "Error: end of file not reached");
      exit(EX_DATAERR);
    }
        
    fclose (stdin);
    return 0;
}