#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdbool.h>

#include "listmaker.h"

void print_help(FILE *stream, int EXIT_CODE) {
    fprintf(stream, "Simulator. Options:\n");
    fprintf(stream,
            "  -h   --help                           Display this help message.\n"
            "  -op  --output-preemption filename     Set output file for preemptive scheduler.\n"
            "  -on  --output-no-preemption filename  Set output file for non-preemptive scheduler.\n"
            "  -i   --input                          Set file containing tasks.\n");
    exit(EXIT_CODE);
}


int main(int argc, char* argv[]) {
    
    task_list_t tasks;
    tasks.first = tasks.last = NULL;
    int next_option;
    bool preemp, not_preemp, input;
    preemp = not_preemp = input = false;

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
                //per ogni file dovremo controllare che esista con access()
                case 'p':
                    printf("OP. One day I'll now what this means: %s\n", optarg);
                    preemp = true;
                    break;
                case 'n':
                    printf("ON. One day I'll now what this means: %s\n", optarg);
                    not_preemp = true;
                    break;
                case 'i':
                    input = read_input(&tasks, optarg);
                    break;
                case 'h':
                    printf("caso h"); //TODO diversificare i casi in cui riceviamo --h e -help perchè non sono corretti se abbiamo tempo
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
        print_help(stderr, EX_USAGE); //il programma è stato avviato senza parametri.

    // FORK HERE
    
    return 0;
}
