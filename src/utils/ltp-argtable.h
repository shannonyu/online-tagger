/*
 *
 *
 */
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

class ltp_argtable {
public:
    enum { ARG_LIT = 0,
        ARG_INT,
        ARG_DBL,
        ARG_STR};

    /*
     * The constructor
     */
    ltp_argtable();
    ltp_argtable(const char *usage);

    /*
     * The destructor
     */
    ~ltp_argtable();

    /*
     * Add an option.
     *
     *  @param[in]  short_opt   short option name for the option.
     *  @param[in]  long_opt    long option name for the option.
     *  @param[in]  data_type   data type for the option.
     *  @param[in]  dest_name   destination name for the option.
     *  @param[in]  glossary    the glossary for the option.
     */
    void add_option(const char *short_opt,
            const char *long_opt,
            const char *data_type,
            const char *dest_name,
            const char *glossary);

    /*
     * Parse the option from command line argument.
     *
     *  @param[in]  argc    number of arguments.
     *  @param[in]  argv    string for arguments.
     */
    int parse_args(int argc, const char **argv);

    /*
     * Print the glossary for the option.
     */
    void print_glossary();

    /*
     * Return the pointer to the value
     *
     *  @param[in]  dest_name   destination name for the option.
     *  @return pointer to the value,
     *          NULL on fail.
     */
    void *option(const char *dest_name);

private:
    class arg_entry {
    public:
        arg_entry() : value(NULL) {}
        ~arg_entry() {
            switch (data_type) {
                case ARG_LIT:
                    delete (char *)value;
                    break;
                case ARG_INT:
                    delete (int *)value;
                    break;
                case ARG_DBL:
                    delete (double *)value;
                    break;
                case ARG_STR:
                    delete (const char **)value;
                    break;
                default:
                    break;
            }
        }

        const char *short_opt;
        const char *long_opt;
        const char *dest_name;
        const char *glossary;
        int data_type;
        void *value;
    };

private:
    const char *usage;
    vector<arg_entry *> entries;

private:
    void arg_set_entry(arg_entry *entry, const char *value);
    arg_entry *arg_find_entry(const char *dest_name);
};
