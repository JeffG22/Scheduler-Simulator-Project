#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "listmaker.h"
#include "not_preemptive.h"
#include "preemptive.h"

void print_help(FILE *stream, int EXIT_CODE) {
    fprintf(stream, "Scheduler simulator. Options:\n");
    fprintf(stream,
            "  -h   --help                           Display this help message.\n"
            "  -op  --output-preemption filename     Set output file for preemptive scheduler.\n"
            "  -on  --output-no-preemption filename  Set output file for non-preemptive scheduler.\n"
            "  -i   --input                          Set file containing tasks.\n");
    exit(EXIT_CODE);
}

void freexit(task_list_t task_lists[], int EXIT_CODE) {
    task_t * t, * t_tmp;
    instruction_t * instr, * instr_tmp;
        
    for (int i = 0; i < N_STATES; i++) {
        t = task_lists[i].first;
        while (t != NULL) {
            instr = t->instr_list;
            while ( instr != NULL) {
                if (instr->next != NULL) {
                    instr_tmp = instr;
                    instr = instr->next;
                    free(instr_tmp);
                }
                else {
                    free(instr);
                    break;
                }
            }
            if (t->next != NULL) {
                t_tmp = t;
                t = t->next;
                free(t_tmp);
            }
            else {
                free(t);
                break;
            }
        }
    }
    printf("Exit with code: %d", EXIT_CODE);
    exit(EXIT_CODE);
}

void freeOK(task_list_t task_lists[], char *c) {
    task_t * t, * t_tmp;
    instruction_t * instr, * instr_tmp;
        
    for (int i = 0; i < N_STATES; i++) {
        t = task_lists[i].first;
        while (t != NULL) {
            instr = t->instr_list;
            while ( instr != NULL) {
                if (instr->next != NULL) {
                    instr_tmp = instr;
                    instr = instr->next;
                    free(instr_tmp);
                }
                else {
                    free(instr);
                    break; //necessario! Perchè la free non mette a zero il contenuto
                }
            }
            if (t->next != NULL) {
                t_tmp = t;
                t = t->next;
                free(t_tmp);
            }
            else {
                free(t);
                break; //necessario! Perchè la free non mette a zero il contenuto
            }
        }
    }
}

void log_output(FILE * fw_np, core_t core, long long unsigned int ck, unsigned int task_id, char *c) {
    
    if (core == CORE0)
        fprintf(fw_np, "core0, %lld, %u, %s\n", ck, task_id, c);
    else if (core == CORE1)
        fprintf(fw_np, "core1, %lld, %u, %s\n", ck, task_id, c);
    else {
        fprintf(fw_np, "errors unexpected on core number");
        //freexit(task_lists, EX_OSERR);
    }
    return;
}

int main(int argc, char* argv[]) {
    
    task_list_t task_lists[N_STATES];   //Array di liste di task
    for (int i = 0; i < N_STATES; i++)  
        task_lists[i].first = task_lists[i].last = NULL;
    
    //variabili per la lettura da linea di comando
    int next_option;
    char *output_preemp, *output_not_preemp;
    output_preemp = output_not_preemp = NULL;
    int preemp_status, not_preemp_status;   //Contengono informazioni di stato per le waitpid()

    const char* const short_options = "hi:";

    const struct option long_options[] = {
            {"output-preemption",    1, NULL, 'p'},
            {"output-no-preemption", 1, NULL, 'n'},
            {"op",                   1, NULL, 'p'},
            {"on",                   1, NULL, 'n'},
            {"help",                 0, NULL, 'h'},
            {"input",                1, NULL, 'i'},
            {NULL,                   0, NULL, 0}
    };    

    if (argc != 1)
        while ((next_option = getopt_long_only(argc, argv, short_options, long_options, NULL)) != -1) { //verifica condizione con assegnamento
            switch (next_option) {
                case 'p':
                    printf("OP. One day I'll now what this means: %s\n", optarg);
                    output_preemp = optarg;
                    break;
                case 'n':
                    printf("ON. One day I'll now what this means: %s\n", optarg);
                    output_not_preemp = optarg; 
                    break;
                case 'i':
                    read_input(&task_lists[NEW], optarg);
                    //not_preemptive(task_lists, "output.txt");
                    break;
                case 'h':
                    printf("%d\n%s\n", optopt, optarg); //TODO diversificare i casi in cui riceviamo --h e -help perchè non sono corretti se abbiamo tempo
                    print_help(stdout, EX_OK); //EX_OK equivale a 0
                    break;
                case '?': /* getopt_long printed an error message. */
                    print_help(stderr, EX_USAGE); //EX_USAGE indica errore nell'invocazione da linea di comando
                    break;
                default:
                    abort();
            }
        }
    else
        print_help(stderr, EX_USAGE); //Il programma è stato avviato senza parametri.

    if(output_preemp != NULL && output_not_preemp != NULL) {

        pid_t preemp_pid = fork();

        if (preemp_pid < 0) { //controllo su fork
            perror("Error on fork preemp");
            exit(EX_OSERR);
            //freexit(task_lists, EX_OSERR);
        }

        if(preemp_pid == 0) {//Sono il figlio preemp
            //preemptive(task_lists, output_preemp);
            freeOK(task_lists, "PREEMP");
        }
        else { //Sono il genitore

            pid_t not_preemp_pid = fork(); //qui forka anche il primo figlio
            
            if (not_preemp_pid < 0) { //controllo su fork
                perror("Error on fork not_preemp");
                exit(EX_OSERR);
                //freexit(task_lists, EX_OSERR);
            }

            if (not_preemp_pid == 0) { //Sono il figlio not_preemp
                not_preemptive(task_lists, output_not_preemp);
                freeOK(task_lists, "NOT_PREEMP");
            }
            else {
                               
                if (waitpid(preemp_pid, &preemp_status, 0) == -1) {
                    perror("Error on preemptive process");
                    exit(EX_OSERR);
                    //freexit(task_lists, EX_OSERR);
                }
                else {
                    if(WIFEXITED(preemp_status)) { //according to http://man7.org/linux/man-pages/man2/waitpid.2.html
                        printf("The preemptive processor exited normally with exit code %d.\n", WEXITSTATUS(preemp_status));
                    }
                    else if (WIFSIGNALED(preemp_status)) {
                        printf("The preemptive processor exited abnormally with signal %d.\n", WTERMSIG(preemp_status));
                    }
                }

                if(waitpid(not_preemp_pid, &not_preemp_status, 0) == -1) {
                    perror("Error on non preemptive process");
                    exit(EX_OSERR);
                    //freexit(task_lists, EX_OSERR);
                }
                else {
                    if(WIFEXITED(not_preemp_status)) {
                        printf("The not preemptive processor exited normally with exit code %d.\n", WEXITSTATUS(not_preemp_status));
                    }
                    else if (WIFSIGNALED(not_preemp_status)) {
                        printf("The not preemptive processor exited abnormally with signal %d.\n", WTERMSIG(not_preemp_status));
                    }
                    freeOK(task_lists, "MAIN");
                }
            }
        }
    }

    return 0; //non raggiungibile
}
