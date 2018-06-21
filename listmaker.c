#include "listmaker.h"

//TO-DO: Find line where error occurred while reading input
//TO-DO: nel caso di memory leak sostiutiamo ad exit una funzione che termina liberando tutta la memoria (free)

task_t* createTask(unsigned int id, unsigned int arrival_time) {
    task_t* new_task = (task_t*) malloc(sizeof(task_t));
    if (new_task == NULL) {
        perror("Error while creating a new task");
        exit(EX_OSERR);
    }

    new_task->id = id;
    new_task->arrival_time = arrival_time;
    new_task->pc = NULL;
    new_task->instr_list = NULL;
    new_task->last = NULL;
    new_task->state = NEW;
    new_task->next = new_task->prev = NULL;

    return new_task;
}

void addTask_bottom(task_list_t * tasks, task_t * new_task) {
    if(tasks->first == NULL) {
        tasks->first = new_task;
        new_task->prev = NULL;
    }
    else {
        tasks->last->next = new_task;
        new_task->prev = tasks->last;
    }
    new_task->next = NULL;
    tasks->last = new_task;
    new_task->service_time = 0;
    new_task->wait_time = 0;
    
    return;
}

//lista da cui lo togliamo (first e last), puntatore all'elemento da togliere
task_t * removeTask(task_list_t * tasks, task_t * del) {
    if (NULL == del) return NULL;
    if (del == tasks->first)
        tasks->first = del->next;
    if (del == tasks->last)
        tasks->last = del->prev;
    if (NULL != del->next)
        del->next->prev = del->prev;
    if (NULL != del-> prev)
        del->prev->next = del->next;
    return del;
}

void moveTask(task_list_t * source, state_t state_source, task_list_t * dest, state_t state_dest, task_t * t, core_t core) {
    
    //TO-DO correzione per diversificare casi
    addTask_bottom(dest, removeTask(source, t));
    //log(fw_np, core, ck, run_task->id, "ready");
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

void addInstruction(task_t * task, instruction_t * new_instr) {
    if(task->instr_list == NULL)
        task->instr_list = task->pc = new_instr;
    else {
        task->last->next = new_instr;
        new_instr->prev = task->last;
    }
    task->last = new_instr;
    
    //incremento service_time
    if (new_instr->type_flag == 0)
        task->service_time += new_instr->length;
    return;
}

void print_input(task_list_t * tasks, char *c, int print_instr) {
    printf("stampo lista %s\n", c);
 
    printf("v~~top\n");
    task_t * t_tmp = tasks->first;
    instruction_t * i_tmp;
    while (NULL != t_tmp) { //ciclo che scorre i task dalla testa
        printf("task id: %u  pc: %p arrival: %u state: %u \n", t_tmp->id, t_tmp->pc, t_tmp->arrival_time, t_tmp->state);
        
        i_tmp = t_tmp->instr_list;

        if (print_instr) {
            printf("instruction top\n");
            while(NULL != i_tmp) { //ciclo che scorre instr dalla testa
                printf("\ttype: %u length: %u\n", i_tmp->type_flag, i_tmp->length);
                i_tmp = i_tmp->next;
            }

            i_tmp=t_tmp->last;
            printf("\n\ninstruction bottom\n");
            while(NULL != i_tmp) { //ciclo che scorre instr dalla coda
                printf("\ttype: %u length: %u\n", i_tmp->type_flag, i_tmp->length);
                i_tmp = i_tmp->prev;
            }
        }

        t_tmp = t_tmp->next;
    }
    
    //nel percorrerla all'indietro non testiamo nuovamente le liste delle istruzioni
    printf("\n\n\n---bottom\n");
    t_tmp = tasks->last;

    while (NULL != t_tmp) { //ciclo che scorre i task dalla coda
        printf("task id: %u  pc: %p arrival: %u state: %u \n", t_tmp->id, t_tmp->pc, t_tmp->arrival_time, t_tmp->state);
        t_tmp = t_tmp->prev;
    }
    printf("\n\n");
}

bool read_input(task_list_t * tasks, char * filename) {

    if (freopen(filename, "r", stdin) == NULL) { //it associates the file input with the stdin
        perror("Looks like there's a problem with your input file");
        exit(EX_OSFILE);
    }
    
    printf("The input file exists! That's a good starting point, I guess.\n");

    char c_read;
    int n1, n2;
    
    while (scanf("%c,%d,%d\n", &c_read, &n1, &n2) != EOF) {

        if (n1 < 0 || n2 < 0) {
            fprintf(stderr, "Error: input file %s is incorrectly formatted (unexpected negative value).\n", filename);
            exit(EX_DATAERR);
        }        
        if (c_read == 't') 
            addTask_bottom(tasks, createTask(n1,n2));
        else if (c_read == 'i') {
            addInstruction(tasks->last, createIstruction(n1,n2));
        }
        else {
            fprintf(stderr, "Error: input file %s is incorrectly formatted (unexpected character %c found).\n", filename, c_read);
            exit(EX_DATAERR);
        }
            
    }

    if (!feof(stdin)) { //errore nel raggiungere la fine del file
      fprintf(stderr, "Error: end of file not reached");
      exit(EX_DATAERR);
    }
        
    fclose (stdin);

    return true;
}