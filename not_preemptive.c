#include "not_preemptive.h"

pthread_mutex_t mutex;
/*
void * run_not_preemp(void * args) {
    task_list_t * task_lists;
    char * outputname;

    task_lists = ((thread_args_t *)args)->task_lists;
    outputname = ((thread_args_t *)args)->outputname;

    pthread_mutex_lock(&mutex);

    printf("%p\n", task_lists);

    printf("%p, %p\n", blocked_task, *blocked_task);
    printf("Should be: %p\n", task_lists[NEW].first);
    task_lists[NEW].first = task_lists[NEW].last;
    printf("%p, %p\n", blocked_task, *blocked_task);
    printf("Should be: %p\n\n", task_lists[NEW].first);

    pthread_mutex_unlock(&mutex);

    return NULL;
}*/

void not_preemptive(task_list_t task_lists[], char * outputname) { //funzione chiamata dal main
    //void moveTask(task_list_t * source, state_t state_source, task_list_t * dest, state_t state_dest, task_t * t, core_t core);
    state_t s;
    
    print_input(&task_lists[NEW], "NEW" , 0);
    //print_input(&task_lists[READY], "READY" , 0);
    //print_input(&task_lists[BLOCKED], "BLOCKED" , 0);
    //print_input(&task_lists[RUNNING], "RUNNING" , 0);
    //print_input(&task_lists[EXIT], "EXIT" , 0);

    moveTask(task_lists, NEW, READY, task_lists[NEW].first);
    moveTask(task_lists, NEW, READY, task_lists[NEW].last);
    moveTask(task_lists, NEW, READY, task_lists[NEW].first);
    moveTask(task_lists, NEW, READY, task_lists[NEW].first);
    
    print_input(&task_lists[NEW], "NEW" , 0);
    print_input(&task_lists[READY], "READY" , 0);

    moveTask(task_lists, READY, RUNNING, task_lists[READY].first);

    task_lists[RUNNING].first->core = 0;
    moveTask(task_lists, RUNNING, BLOCKED, task_lists[RUNNING].first);
    task_lists[READY].first->core = 1;
    moveTask(task_lists, READY, BLOCKED, task_lists[READY].first);
    print_input(&task_lists[BLOCKED], "BLOCKED" , 0);
    moveTask(task_lists, BLOCKED, READY, task_lists[BLOCKED].first);
    
    moveTask(task_lists, READY, EXIT, task_lists[READY].first);
    moveTask(task_lists, READY, BLOCKED, task_lists[READY].first);
    
    print_input(&task_lists[NEW], "NEW" , 0);
    print_input(&task_lists[READY], "READY" , 0);
    print_input(&task_lists[BLOCKED], "BLOCKED" , 0);
    print_input(&task_lists[RUNNING], "RUNNING" , 0);
    print_input(&task_lists[EXIT], "EXIT" , 0);
/*    
    pthread_t core0_id, core1_id;
    pthread_attr_t attrdefault;
    thread_args_t args;
    args.outputname = outputname;
    args.task_lists = task_lists;

    if (pthread_mutex_init(&mutex, NULL) != 0) { //inizializzazione mutex da preferire all'assegnamento
        perror("error on mutex_init");
        exit(EX_OSERR);
    }

    if (pthread_attr_init(&attrdefault) != 0) { //inizializza a default
        perror("error on set thread attributes");
        exit(EX_OSERR);
    }

    if (pthread_create(&core0_id, &attrdefault, &run_not_preemp, (void*)&args) != 0) {
        perror("error on pthread create for core0");
        exit(EX_OSERR);
    }

    if (pthread_create(&core1_id, &attrdefault, &run_not_preemp, (void*)&args) != 0) {
        perror("error on pthread create for core1");
        exit(EX_OSERR);
    }

    if (&attrdefault != NULL) {
        if (pthread_attr_destroy(&attrdefault) != 0) {
            perror("error on pthread_attr_destroy");
        exit(EX_OSERR);
        }
    }

    if (pthread_join (core0_id, NULL) != 0) {
        perror("error on pthread_join");
        exit(EX_OSERR);
    }

    if (pthread_join (core1_id, NULL) != 0) {
        perror("error on pthread_join");
        exit(EX_OSERR);
    }

    if (pthread_mutex_destroy (&mutex) != 0) { //per distruggerlo deve essere unlockato se no c'è errore
        perror("error on mutex_init");
        exit(EX_OSERR);
    }
*/
    return;
}