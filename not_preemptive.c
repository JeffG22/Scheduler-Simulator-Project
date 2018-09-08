#include "not_preemptive.h"

pthread_mutex_t mutex;

void * run_not_preemp(void * args) {
    
    //cast dei parametri
    task_list_t * task_list = ((thread_args_t *)args)->task_lists;
    FILE * fw_np = ((thread_args_t *)args)->fw_np;
    core_t core = ((thread_args_t *)args)->core;
        
    //variabili del thread
    long long unsigned ck = 0; //clock
    task_t ** blocked_task = NULL; //punto al primo task blocked
    task_t * run_task = NULL; //punto al task che porterò in running
    srand(time(NULL)); //generazione seme per sequenza numeri casuali    

    while (true) { //finché c'è almeno un processo in ready oppure new oppure blocked

        pthread_mutex_lock(&mutex);

        // check on remaining tasks
        if (task_list[NEW].first == NULL && task_list[READY].first == NULL && task_list[BLOCKED].first == NULL){
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        // check on arrived tasks
        while (task_list[NEW].first != NULL && task_list[NEW].first->arrival_time <= ck) {
            unsigned int moved_id = task_list[NEW].first->id;
            moveTask(task_list, NEW, READY, task_list[NEW].first);
            log_output(fw_np, core, ck, moved_id, "ready"); //from new to ready
        }

        // check on unblocked task
        if (core == CORE0)
            blocked_task = &(task_list[BLOCKED].first);
        else
            blocked_task = &(task_list[BLOCKED].last);

        while ((*blocked_task) != NULL && (*blocked_task)->core == core && (*blocked_task)->wait_time < ck) {
            unsigned int moved_id = (*blocked_task)->id;
            moveTask(task_list, BLOCKED, READY, (*blocked_task));
            log_output(fw_np, core, ck, moved_id, "ready"); //from blocked to ready
        }

        /* cerco il primo task da eseguire
            1) ho un task che posso portare in esecuzione -> eseguo
            2) non ho un task che posso portare in esecuzione -> aspetto ed incremento ck
        */
        
        run_task = task_list[READY].first;
                
        while (run_task != NULL && run_task->arrival_time > ck) {
            run_task = run_task->next;
        }
        if (run_task == NULL) {
            ck++;
            pthread_mutex_unlock(&mutex);
            continue;
        }
        
        moveTask(task_list, READY, RUNNING, run_task);
        log_output(fw_np, core, ck, run_task->id, "running");

        pthread_mutex_unlock(&mutex);

        while(NULL != run_task->pc && run_task->pc->type_flag != 1) {
            ck += run_task->pc->length; //esecuzione atomica dell'istruzione
            run_task->pc = run_task->pc->next;
        }

        pthread_mutex_lock(&mutex);
        if (run_task->pc == NULL) { //task concluso!
            moveTask(task_list, RUNNING, EXIT, run_task);
            log_output(fw_np, core, ck, run_task->id, "exit");
        } else { //istruzione bloccante
            run_task->core = core;
            run_task->wait_time = ck + rand() % (run_task->pc->length) + 1;
            run_task->pc = run_task->pc->next;
            
            moveTask(task_list, RUNNING, BLOCKED, run_task);
            log_output(fw_np, core, ck, run_task->id, "blocked");
        }

        pthread_mutex_unlock(&mutex);
    }
    
    fprintf(stderr, "Error on thread execution");
    return NULL;
}

void not_preemptive(task_list_t task_lists[], char * outputname) { //funzione chiamata dal main

    //apetura file di output
    //non stampiamo i processi in new perchè non sono assegnati a nessun core
    FILE * fw_np = fopen(outputname, "w"); //file write not preemp
    if (fw_np == NULL) {
        perror("Looks like there's a problem with your output file for scheduler not_preemp");
        freexit(task_lists, EX_OSFILE);
    }
    
    //variabili per i due thread
    pthread_t core0_id, core1_id;
    pthread_attr_t attrdefault;

    //incapsuliamo gli argomenti da passare al thread in una struct
    thread_args_t args0, args1;
    args0.fw_np = args1.fw_np = fw_np;
    args0.task_lists = args1.task_lists = task_lists;
    args0.core = CORE0;
    args1.core = CORE1;

    if (pthread_mutex_init(&mutex, NULL) != 0) { //inizializzazione mutex da preferire all'assegnamento
        perror("error on mutex_init");
        freexit(task_lists, EX_OSERR);
    }

    if (pthread_attr_init(&attrdefault) != 0) { //inizializza a default gli attributi del thread
        perror("error on set thread attributes");
        freexit(task_lists, EX_OSERR);
    }

    if (pthread_create(&core0_id, &attrdefault, &run_not_preemp, (void*)&args0) != 0) { //CREAZIONE CORE0
        perror("error on pthread create for core0");
        freexit(task_lists, EX_OSERR);
    }

    if (pthread_create(&core1_id, &attrdefault, &run_not_preemp, (void*)&args1) != 0) { //CREAZIONE CORE1
        perror("error on pthread create for core1");
        freexit(task_lists, EX_OSERR);
    }

    if (&attrdefault != NULL) { //distruzione attributi thread utilizzati
        if (pthread_attr_destroy(&attrdefault) != 0) {
            perror("error on pthread_attr_destroy");
            freexit(task_lists, EX_OSERR);
        }
    }

    if (pthread_join (core0_id, NULL) != 0) { //attesa core0
        perror("error on pthread_join");
        freexit(task_lists, EX_OSERR);
    }

    if (pthread_join (core1_id, NULL) != 0) { //attesa core1
        perror("error on pthread_join");
        freexit(task_lists, EX_OSERR);
    }

    if (pthread_mutex_destroy (&mutex) != 0) { //per distruggerlo deve essere unlockato se no c'è errore
        perror("error on mutex_init");
        freexit(task_lists, EX_OSERR);
    }

    if (fclose(fw_np) != 0) { //chiusura file
        perror("problem to close the file for scheduler not_preemp");
        freexit(task_lists, EX_OSFILE);
    }

    return;
}