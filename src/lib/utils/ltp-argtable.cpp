#include "ltp-argtable.h"

ltp_argtable :: ltp_argtable() : usage(0) {
    entries.clear();
}

ltp_argtable :: ltp_argtable(const char *_usage) : usage(_usage) {
    entries.clear();
}

ltp_argtable :: ~ltp_argtable() {
    for (int i = 0; i < entries.size(); ++ i) {
        delete entries[i];
    }
}

void ltp_argtable :: add_option(const char *short_opt,
        const char *long_opt,
        const char *data_type,
        const char *dest_name,
        const char *glossary) {
    arg_entry * entry = new arg_entry();

    entry->short_opt = short_opt;
    entry->long_opt  = long_opt;
    entry->dest_name = dest_name;
    entry->glossary  = glossary;

    if (strcmp(data_type, "lit") == 0 || strcmp(data_type, "literal") == 0) {
        entry->data_type = ARG_LIT;
        entry->value = (void *)(new char);
    } else if (strcmp(data_type, "int") == 0 || strcmp(data_type, "integer") == 0) {
        entry->data_type = ARG_INT;
        entry->value = (void *)(new int);
    } else if (strcmp(data_type, "dbl") == 0 || strcmp(data_type, "double") == 0) {
        entry->data_type = ARG_DBL;
        entry->value = (void *)(new double);
    } else if (strcmp(data_type, "str") == 0 || strcmp(data_type, "string") == 0) {
        entry->data_type = ARG_STR;
        entry->value = (void *)(new const char *);
    }

    entries.push_back( entry );
}

int ltp_argtable :: parse_args(int argc, const char **argv) {
    for (int i = 0; i < argc; ++ i) {
        int len = strlen(argv[i]);
        int entry_idx = -1;

        if (argv[i][0] == '-') {
            if (len > 1 && argv[i][1] == '-') { // long option identification
                for (entry_idx = 0;
                        entry_idx < entries.size() && strcmp(entries[entry_idx]->long_opt, argv[i] + 2);
                        ++ entry_idx);
            } else { // short option identification
                for (entry_idx = 0;
                        entry_idx < entries.size() && strcmp(entries[entry_idx]->short_opt, argv[i] + 1);
                        ++ entry_idx);
            }

            // not found.
            if (entry_idx == entries.size()) {
                return -1;
            } else {
                switch(entries[entry_idx]->data_type) {
                    case ARG_LIT:
                        arg_set_entry( entries[entry_idx], "" );
                        break;
                    case ARG_INT:
                        arg_set_entry( entries[entry_idx], argv[++ i]);
                        break;
                    case ARG_DBL:
                        arg_set_entry( entries[entry_idx], argv[++ i]);
                        break;
                    case ARG_STR:
                        arg_set_entry( entries[entry_idx], argv[++ i]);
                        break;
                    default:
                        return -1;
                }
            }
        } else {
            return -1;
        }
    }
    return 0;

}

void *ltp_argtable :: option(const char *dest_name) {
    arg_entry *entry = arg_find_entry(dest_name);
    if (entry == NULL) return NULL;
    else return entry->value;
}

void ltp_argtable :: print_glossary() {
    if (usage != NULL) {
        fprintf(stderr, "usage: %s\n\n", usage);
    } else {
        fprintf(stderr, "usage: ./excuatable [option]\n\n");
    }

    for (int i = 0; i < entries.size(); ++ i) {
        fprintf( stderr, "\t-%s --%-19s %s\n",
                entries[i]->short_opt,
                entries[i]->long_opt,
                entries[i]->glossary);
    }

    fprintf(stderr, "\n");
}

void ltp_argtable :: arg_set_entry(arg_entry *entry, const char *value) {
    if (entry->data_type == ARG_LIT) {
        *((bool *)entry->value) = true;
    } else if (entry->data_type == ARG_INT) {
        *((int *)entry->value) = atoi(value);
    } else if (entry->data_type == ARG_DBL) {
        *((double *)entry->value) = atof(value);
    } else if (entry->data_type == ARG_STR) {
        *((const char **)entry->value) = value;
    }
}

ltp_argtable :: arg_entry *ltp_argtable :: arg_find_entry(const char *dest_name) {
    for (int i = 0; i < entries.size(); ++ i) {
        if (strcmp(entries[i]->dest_name, dest_name) == 0) {
            return entries[i];
        }
    }
    return NULL;
}

