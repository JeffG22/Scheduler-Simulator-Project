#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

typedef enum {NEW, READY, RUNNING, BLOCKED, EXIT} state_t;

typedef struct instruction {
    int type_flag;
    int length;

    struct instruction * next;
    struct instruction * previous;
} instruction_t;

typedef struct tcb {
    int id;
    int pc;
    int arrival_time;
    instruction_t * instr_list;
    state_t state;

    struct tcb * next;
    struct tcb * previous;
} task_t;

// TO-DO Move to main function
task_t * task_list;

void addInstruction () {

}

int read_input(char * filename) {

    if (freopen(filename, "r", stdin) == NULL) { //it associates the file input with the stdin
        perror("It looks like there's a problem with your input file");
        exit(EX_OSFILE);
    }
    
    printf("The input file exists! That's a good starting point, I guess.\n");

    /*
    *   idee per leggere il file
    *   ciclo fino all'eof
    *   interpreto t oppure i
    *   chiamo aggiungi t oppure i
    */
    return 0;
}