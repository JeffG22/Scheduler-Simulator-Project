//TO-DO: Find line where error occurred while reading input
//TO-DO: nel caso di memory leak sostiutiamo ad exit una funzione che termina liberando tutta la memoria (free)
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
    unsigned int arrival_time;
    state_t state;
    instruction_t * pc;
    instruction_t * instr_list;
    instruction_t * last;
    

    struct tcb * next;
    struct tcb * prev;
} task_t;

typedef struct task_list {
    task_t * first;
    task_t * last;
} task_list_t;

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

void addTask(task_list_t * tasks, task_t * new_task) {
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
void addInstruction(task_list_t * tasks, instruction_t * new_instr) {
     if(tasks->last->instr_list == NULL)
        tasks->last->instr_list = tasks->last->pc = new_instr;
    else {
        tasks->last->last->next = new_instr;
        new_instr->prev = tasks->last->last;
    }
    tasks->last->last = new_instr;
    return;
}

void print_input(task_list_t * tasks) {
    
    printf("v~~top\n");
    
    /*instruction_t * instr_list;
    instruction_t * last;

    
    unsigned int type_flag;
    unsigned int length;

    struct instruction * next;
    struct instruction * prev;*/
    /*
    task_t * t_tmp = tasks->first;
    instruction_t * i_tmp = tasks->first->instr_list;
    while (NULL != t_tmp) {
        printf("task id: %u  pc: %u arrival: %u state: %u \n", t_tmp.id, t_tmp.pc, t_tmp.arrival_time, t_tmp.state);
        printf("instruction\n");

        while(NULL != i_tmp) {
            printf("\ttype: %u length: %u", i_tmp.type_flag, i_tmp.length);
        
        }
        t_tmp = t_tmp->next;
    }
    
    pNode tmp = p->first;
    while (NULL != tmp) {
        printf("%d: %s\n", ++i, tmp->desc);
        tmp = tmp->next;
    }
    printf("---bottom\n");
    i = 0;
    tmp = p->last;
    while (NULL != tmp) {
        printf("%d: %s\n", ++i, tmp->desc);
        tmp = tmp->prev;
    }
    printf("^~~top\n\n");*/
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
            addTask(tasks, createTask(n1,n2));
        else if (c_read == 'i') {
            addInstruction(tasks, createIstruction(n1,n2));
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