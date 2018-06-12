#include "not_preemptive.h"

void not_preemptive(task_list_t task_lists[], char * outputname) { //funzione chiamata dal main
    
    print_input(&task_lists[0], "NEW", 0);

    moveTask(&task_lists[0], &task_lists[1], task_lists[0].first->next); //spostiamo il secondo task in ready
    moveTask(&task_lists[0], &task_lists[1], task_lists[0].last); 

    print_input(&task_lists[0], "NEW", 0);
    print_input(&task_lists[1], "READY", 0);

    /*
    pthread_t core0_id, core1_id;
    
    int pthread_create(thread0_id, NULL, &run_not_preemp, <param>);
    int pthread_create(thread1_id, NULL, &run_not_preemp, NULL);



    pthread_join(thread0_id, NULL);
    pthread_join(thread1_id, NULL);*/
}