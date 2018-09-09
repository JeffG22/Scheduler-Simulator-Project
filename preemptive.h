#ifndef PREEMPTIVE
#define PREEMPTIVE

#include <pthread.h>
#include "listmaker.h"

void log_output(FILE *fw, core_t core, long long unsigned int clock, unsigned int, char *c);
void preemptive(task_list_t task_lists[], char * outputname);
void * run_preemp(void * args);

#endif