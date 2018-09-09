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

void log_output(FILE * fw_np, core_t core, long long unsigned int ck, unsigned int task_id, char *c) {
    //prints the parameters on the output file

    if (core == CORE0)
        fprintf(fw_np, "core0, %lld, %u, %s\n", ck, task_id, c);
    else if (core == CORE1)
        fprintf(fw_np, "core1, %lld, %u, %s\n", ck, task_id, c);
    else {
        fprintf(fw_np, "errors unexpected on core number");
        exit(EX_OSERR);
    }
    return;
}

int main(int argc, char* argv[]) {

    task_list_t task_lists[N_STATES];   //Array di liste di task
    for (int i = 0; i < N_STATES; i++)  
        task_lists[i].first = task_lists[i].last = NULL;
    
    //variabili per la lettura da linea di comando
    int next_option;
    char *output_preemp, *output_not_preemp; //nomi dei file di output
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

    if (argc != 1) //controllo che main sia stato invocato con parametri
        while ((next_option = getopt_long_only(argc, argv, short_options, long_options, NULL)) != -1) { //verifica condizione con assegnamento
            switch (next_option) {
                case 'p':
                    printf("Output file for preemptive processor correctly set to: %s\n", optarg);
                    output_preemp = optarg;
                    break;
                case 'n':
                    printf("Output file for not preemptive processor correctly set to: %s\n", optarg);
                    output_not_preemp = optarg; 
                    break;
                case 'i':
                    read_input(&task_lists[NEW], optarg); //lettura dell'input e creazione lista NEW
                    break;
                case 'h':
                    printf("%d\n%s\n", optopt, optarg);
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

    if(output_preemp != NULL && output_not_preemp != NULL) { //ho ricevuto correttamente entrambi i nomi dei file

        pid_t preemp_pid = fork(); //primo figlio

        if (preemp_pid < 0) { //controllo su fork
            perror("Error on fork preemp");
            freexit(task_lists, EX_OSERR);
        }

        if(preemp_pid == 0) {//Sono il figlio preemp
            preemptive(task_lists, output_preemp); //AVVIO dello scheduler preemp
            freeOK(task_lists); //libero memoria PREEMP
        }
        else {//Sono il genitore

            pid_t not_preemp_pid = fork();//secondo figlio
            
            if (not_preemp_pid < 0) { //controllo su fork
                perror("Error on fork not_preemp");
                freexit(task_lists, EX_OSERR);
            }

            if (not_preemp_pid == 0) {//Sono il figlio not_preemp
                not_preemptive(task_lists, output_not_preemp); //AVVIO dello scheduler not preemp
                freeOK(task_lists); //libero memoria NOTPREEMP
            }
            else {

                freeOK(task_lists); //libero memoria MAIN

                //attendo la terminazione dei processi figli           
                if (waitpid(preemp_pid, &preemp_status, 0) == -1) { //attesa per preemp
                    perror("Error on preemptive process");
                    freexit(task_lists, EX_OSERR);
                }
                else {
                    if(WIFEXITED(preemp_status)) { //according to http://man7.org/linux/man-pages/man2/waitpid.2.html
                        printf("The preemptive processor exited normally with exit code %d.\n", WEXITSTATUS(preemp_status));
                    }
                    else if (WIFSIGNALED(preemp_status)) {
                        printf("The preemptive processor exited abnormally with signal %d.\n", WTERMSIG(preemp_status));
                    }
                }
                if(waitpid(not_preemp_pid, &not_preemp_status, 0) == -1) { //attesa per non preemp
                    perror("Error on non preemptive process");
                    freexit(task_lists, EX_OSERR);
                }
                else {
                    if(WIFEXITED(not_preemp_status)) {
                        printf("The not preemptive processor exited normally with exit code %d.\n", WEXITSTATUS(not_preemp_status));
                    }
                    else if (WIFSIGNALED(not_preemp_status)) {
                        printf("The not preemptive processor exited abnormally with signal %d.\n", WTERMSIG(not_preemp_status));
                    }
                }
            }
        }
    }

    return 0;
}
