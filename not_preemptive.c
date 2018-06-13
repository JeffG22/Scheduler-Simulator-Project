/*
    log delle cose fatte
    1) apertura del file in processo e passaggio di puntatore a file
    2) variabili utili del thread per scheduling
    2.1) definito l'enum core_t
    3) aggiunto in task_t il campo core e wait_time per indicare da quale processo è stato bloccato e per quanto tempo
    4) modificata la funzione moveTask aggiungendo da quale coda a quale altra viene spostato
    5) nello scheduling not_preemptive ho fatto la scelta della coda da cui prendere il task nel mutex
    6) fatto avanzamento di task, manca la funzione random
    6.1)fatto caso in cui manchino dei colpi di ck per prossimo task con waitck
    6.2) dopo esecuzione il task è mandato in blocked o exit
    7) funzione log per stampare transizioni
    
    rimane da fare:
    - inserimento ordinato in blocked dalla testa per core0 -> movetask riceve anche il core
    - stessa cosa per core1 dalla coda
    - trovare heap min in c, adattarlo al programma facendo test fuori e inserirlo
    - controllare i punti dal 1 al 7 del log ahahahahha :'-)

    //nb queste scelte sono quelle che mi sono venute in mente, ovviamente arbitrarie, quindi parliamone :)

*/

#include "not_preemptive.h"

pthread_mutex_t mutex;

void log(FILE *fw_np, core_t core, long long unsigned int ck, unsigned int task_id, char *c) {
    if (core == CORE0)
        fprintf(fw_np, "core0, %lld, %u, %s", ck, task_id, c);
    else
        fprintf(fw_np, "core1, %lld, %u, %s", ck, task_id, c);
    return;
}

void * run_not_preemp(void * args) {
    
    //cast dei parametri
    task_list_t * task_lists = ((thread_args_t *)args)->task_lists;
    FILE * fw_np = ((thread_args_t *)args)->fw_np;
    core_t core = ((thread_args_t *)args)->core;


    //variabili del thread
    bool scheduler = true;
    long long unsigned ck = 0; //clock
    unsigned int waitck = 0; //tempo di attesa del clock per schedulare prossimo task
    task_t * run_task = NULL; //punto al task che porterò in running
    task_t * blocked_task = NULL; //punto al primo task blocked
    task_t * new_task = NULL; //punto al primo task new

    while (scheduler) { //finchè c'è almeno un processo in ready oppure new oppure blocked

        pthread_mutex_lock(&mutex);

        if (task_lists[NEW].first == NULL && task_lists[READY].first == NULL && task_lists[BLOCKED].first == NULL){
            printf("ho finito i task %llu\n", ck);
            scheduler = false;
            return NULL;
        }

        run_task = task_lists[READY].first;

        if (NULL == run_task) { //se in ready non c'è nessuno

            if (core == CORE0 && task_lists[BLOCKED].first != NULL && task_lists[BLOCKED].first->core == CORE0)
                blocked_task = task_lists[BLOCKED].first;
            else if (core == CORE1 && task_lists[BLOCKED].first != NULL && task_lists[BLOCKED].first->core == CORE1)
                blocked_task = task_lists[BLOCKED].last;
            else
                blocked_task = NULL;

            new_task = task_lists[NEW].first;

            if ( new_task == NULL || (blocked_task != NULL && 
                    (blocked_task->wait_time < ck) ||
                    (new_task->arrival_time > ck && blocked_task->wait_time <= new_task->arrival_time))) { 
                //schedulo questo da blocked
                if (blocked_task->wait_time > ck)
                        waitck = blocked_task->wait_time - ck;
                else {
                    run_task = blocked_task;
                    log(fw_np, core, ck, run_task->id, "ready"); //from blocked to ready
                    moveTask(&task_lists[BLOCKED], BLOCKED, &task_lists[READY], READY, run_task, core);
                }
            } else {
                //schedulo questo da reay
                if (new_task->arrival_time > ck)
                    waitck = new_task->arrival_time - ck;
                else {
                    run_task = new_task;
                    log(fw_np, core, ck, run_task->id, "ready"); //from new to ready
                    moveTask(&task_lists[NEW], NEW, &task_lists[READY], READY, run_task, core);
                }
            }
        }

        if (waitck == 0) {
            log(fw_np, core, ck, run_task->id, "running");
            moveTask(&task_lists[READY], READY, &task_lists[RUNNING], RUNNING, run_task, core);
        }

        pthread_mutex_unlock(&mutex);

        if (waitck > 0) {
            ck += waitck;
            waitck = 0;
        } else {
            while(NULL != run_task->pc || run_task->pc->type_flag != 1) {
                ck += run_task->pc->length;
                run_task->pc = run_task->pc->next;
            }

            pthread_mutex_lock(&mutex);
            if (run_task->pc == NULL) { //task concluso!
                log(fw_np, core, ck, run_task->id, "exit");
                moveTask(&task_lists[RUNNING], RUNNING, &task_lists[EXIT], EXIT, run_task, core);
            } else { //istruzione bloccante
                run_task->core = core;
                //run_task->wait_time = ck + numerorandom(run_task->pc->length); //TO-DO
                run_task->pc = run_task->pc->next;
                log(fw_np, core, ck, run_task->id, "blocked");
                moveTask(&task_lists[RUNNING], RUNNING, &task_lists[BLOCKED], BLOCKED, run_task, core);
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    
    printf("%p\n", task_lists);
    return NULL;
}

void not_preemptive(task_list_t task_lists[], char * outputname) { //funzione chiamata dal main

    //apetura file di output
    //non stampiamo i processi in new perchè non sono assegnati a nessun core
    FILE *fw_np; //file write not preemp
    if (fopen(outputname, "w") == NULL) {
        perror("Looks like there's a problem with your output file for scheduler not_preemp");
        exit(EX_OSFILE);
    }
    printf("The output file exists! That's a good starting point, I guess.\n");
  
    /*print_input(&task_lists[0], "NEW", 0);
    moveTask(&task_lists[0], &task_lists[1], task_lists[0].first->next); //spostiamo il secondo task in ready
    moveTask(&task_lists[0], &task_lists[1], task_lists[0].last); 
    print_input(&task_lists[0], "NEW", 0);
    print_input(&task_lists[1], "READY", 0);
    printf("%p\n", task_lists);*/
    
    pthread_t core0_id, core1_id;
    pthread_attr_t attrdefault;

    thread_args_t args;
    args.fw_np = fw_np;
    args.task_lists = task_lists;
    args.core = CORE0;

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

    args.core = CORE1;
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