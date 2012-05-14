#include "ltp-argtable.h"
#include "ltp-configure.h"
#include "ltp-log.h"

int main_learn(ltp_configure *cfg);
int main_tag(ltp_configure *cfg);

void
init_config(ltp_configure *cfg) {
    cfg->set_cfg("target",    "NONE");
    cfg->set_cfg("algorithm", "MIRA");
    cfg->set_cfg("train",     "./train");
    cfg->set_cfg("test",      "./test");
    cfg->set_cfg("devolop",   "./dev");
    cfg->set_cfg("dict",      "./dict");
    cfg->set_cfg("model",     "./model");
    cfg->set_cfg("agenda",    "5");
    cfg->set_cfg("itertime",  "10");
    cfg->set_cfg("dataset",   "NONE");
}

int
main(int argc, const char **argv) {
    ltp_argtable* argtable = new ltp_argtable();

    // ==========================================
    // SET COMMAND LINE OPTION
    // ==========================================
    argtable->add_option("c", "config", "str", "config", "identify the config file.");
    if (argtable->parse_args(-- argc, ++ argv) == -1) {
        argtable->print_glossary();
        return -1;
    }

    // ==========================================
    // SET CONFIG PARSER AND LOAD CONFIG
    // ==========================================
    ltp_configure *cfg = new ltp_configure();
    init_config(cfg);

    if (cfg->load_cfg(*(const char **)argtable->option("config")) == -1) {
        write_log( LTP_LOG_WARNING, 
                "load config file [%s] failed.",
                *(const char **)argtable->option("config") );
        return -1;
    }

    cfg->display();

    // ==========================================
    // MAIN PROCESS
    // ==========================================
    if (cfg->config("target") == "train") {
        main_learn(cfg);
    }
    else if (cfg->config("target") == "test") {
        main_tag(cfg);
    }

    delete argtable;
    delete cfg;
    return 0;
}

