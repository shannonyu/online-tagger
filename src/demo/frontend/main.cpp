#include <iostream>
#include "ltp-argtable.h"
using namespace std;


void
init_argtable(ltp_argtable *argtable) {
    argtable->add_option("t", "train",  "str", "train",  "identify the train file.");
    argtable->add_option("d", "dev",    "str", "dev",    "identify the devolop file.");
    argtable->add_option("e", "eval",   "str", "eval",   "identify the eval file.");
    argtable->add_option("i", "iter",   "int", "iter",   "identify the iteration time.");
    argtable->add_option("a", "agenda", "int", "agenda", "identify the agenda number.");
    argtable->add_option("l", "dict",   "str", "dict",   "identify the dict file.");
    argtable->add_option("m", "model",  "str", "model",  "identify the model file.");
}

int main_learn(int argc, char **argv, ltp_argtable *argtable);
int main_tag(int argc, char **argv, ltp_argtable *argtable);

int
main(int argc, char **argv) {
    const char *usage   = NULL;
    const char *command = NULL;

    usage = "./onlinetagger [command] [option]";
    ltp_argtable *argtable = new ltp_argtable(usage);
    init_argtable(argtable);

    if (argc < 2) {
        fprintf(stderr, "ERROR: No command is specified.\n");
        argtable->print_glossary();
        return -1;
    }

    command = argv[1];
    if (0 == strcmp(command, "learn")) {
        main_learn(argc - 2, argv + 2, argtable);
    } else if (0 == strcmp(command, "tag")) {
        main_tag(argc - 2, argv + 2, argtable);
    } else {
        fprintf(stderr, "ERROR: Unrecognized command (%s) specified.\n", command);
    }

    delete argtable;
    return 0;
}
