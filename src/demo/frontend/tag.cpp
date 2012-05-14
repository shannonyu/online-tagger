#include "ltp-argtable.h"

struct tag_opt_t {
    const char *eval_file;
    int agenda;
};

void init_tag_opt(tag_opt_t *opt) {
    opt->eval_file = NULL;
    opt->agenda    = 0;
}

int
main_tag(int argc, char **argv, ltp_argtable *argtable) {
    return  0;
}
