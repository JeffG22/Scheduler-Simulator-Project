//
// Created by Fede on 16/05/2018.
//
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

const char *program_name;

void print_help(FILE *stream) {
    fprintf(stream, "Simulator. Options:\n");
    fprintf(stream,
            "  -h   --help                           Display this help message.\n"
            "  -op  --output-preemption filename     Set output file for preemptive scheduler.\n"
            "  -on  --output-no-preemption filename  Set output file for non-preemptive scheduler.\n"
            "  -i   --input                          Set file containing tasks.\n");
    exit(0);
}


int main(int argc, char* argv[]) {
    int next_option;
    extern char *optarg;

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
        do {
            next_option = getopt_long_only(argc, argv, short_options, long_options, NULL);

            switch (next_option) {
                case 'p':
                    printf("OP. One day I'll now what this means: %s\n", optarg);
                    break;
                case 'n':
                    printf("ON. One day I'll now what this means: %s\n", optarg);
                    break;
                case 'i':
                    printf("ON. One day I'll now what this means: %s\n", optarg);
                    break;
                case 'h':
                case '?':
                    print_help(stdout);
                    break;
                case -1:
                    break;
                default:
                    abort();
            }
        } while (next_option != -1);
    return 0;
}
