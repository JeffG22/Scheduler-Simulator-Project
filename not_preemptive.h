#ifndef NOT_PREEMPTIVE
#define NOT_PREEMPTIVE

#include <pthread.h>
#include "listmaker.h"

void not_preemptive(task_list_t task_lists[], char * outputname);
void * run_not_preemp(void * args);

#endif