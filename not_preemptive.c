#include "not_preemptive.h"

pthread_mutex_t mutex;

void * run_not_preemp(void * args) {
    task_list_t * task_lists;
    char * outputname;

    task_lists = ((thread_args_t *)args)->task_lists;
    outputname = ((thread_args_t *)args)->outputname;

    printf("%p\n", task_lists);

    return NULL;
}

void not_preemptive(task_list_t task_lists[], char * outputname) { //funzione chiamata dal main
    
    print_input(&task_lists[0], "NEW", 0);

    moveTask(&task_lists[0], &task_lists[1], task_lists[0].first->next); //spostiamo il secondo task in ready
    moveTask(&task_lists[0], &task_lists[1], task_lists[0].last); 

    print_input(&task_lists[0], "NEW", 0);
    print_input(&task_lists[1], "READY", 0);

    printf("%p\n", task_lists);
    
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

    return;
}