#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
static struct option long_options[] = {
    {"concurrency",     required_argument, 0, 'C'},
    {"duration",        required_argument, 0, 'D'},
    {"timeout",         required_argument, 0, 'T'},
    {"warmup-time",     required_argument, 0, 'W'},
    {"output-format",   required_argument, 0, 'O'},
    {"pghost",          required_argument, 0, 'H'},
    {"pgport",          required_argument, 0, 'P'},
    {"pguser",          required_argument, 0, 'U'},
    {"driver",          required_argument, 0, 'd'},
    {"queryfile",       required_argument, 0, 'Q'},
    {0, 0, 0, 0}};

/**
 * @brief Output format enumeration
 * Contains two options: TEXT and JSON
 */
typedef enum {
    TEXT = 1,
    JSON = 2,
} OutputFormat;

typedef struct {
    uint16_t pgport;
    uint16_t concurrency;
    uint16_t duration;
    uint16_t timeout;
    uint16_t warmup_time;
    OutputFormat output_format;
    char *pghost;
    char *pguser;
    char *queryfile;
} Inputs;
// clang-format on

/**
 * @brief Kill the process when arguments are not found
 *
 * @param msg Message to print out before killing the process
 */
void die(char *msg)
{
    perror(msg);
    exit(1);
}

/**
 * @brief Parse integer arguments from the argument parser
 *
 * @param input Pointer to the input value (where the value will be stored)
 * @param optarg Option argument value from the argument parser (getopt)
 * @param name Name of the argument. Used for printing the error message only.
 * @param def Default value of the argument. Used for printing the error and
 * overwriting input.
 */
void parse_int_args(uint16_t *input, char *optarg, const char *name,
                    const int def)
{
    char *endptr;
    *input = strtol(optarg, &endptr, 10);
    if (errno == EINVAL || errno == ERANGE) {
        printf(
            "Could not convert %s argument. Defaulting to %d. Error: "
            "%d\n",
            name, def, errno);
        *input = def;
    };
}

/**
 * @brief Parse input arguments and store them in the Inputs structure
 * 
 * @param inputs Inputs structure where the values will be stored
 * @param argc Number of input arguments to the main function
 * @param argv Array of input arguments to the main function
 */
void parse_args(Inputs *inputs, int argc, char **argv)
{
    int c;
    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, "C:D:T:W:O:H:P:U:d:Q:", long_options,
                        &option_index);

        if (c == -1) {
            break;
        }
        char *endptr;

        switch (c) {
            case 'C':
                parse_int_args(&inputs->concurrency, optarg, "concurrency", 10);
                break;
            case 'D':
                parse_int_args(&inputs->duration, optarg, "duration", 30);
                break;
            case 'T':
                parse_int_args(&inputs->timeout, optarg, "timeout", 2);
                break;
            case 'W':
                parse_int_args(&inputs->warmup_time, optarg, "warmup time", 5);
                break;
            case 'O':
                if (strcasecmp(optarg, "text") == 0) {
                    inputs->output_format = TEXT;
                } else if (strcasecmp(optarg, "json") == 0) {
                    inputs->output_format = JSON;
                } else {
                    printf("Invalid output format %s. Defaulting to TEXT\n",
                           optarg);
                }
                break;
            case 'H':
                inputs->pghost = optarg;
                break;
            case 'P':
                parse_int_args(&inputs->pgport, optarg, "pgport", 5432);
                break;
            case 'U':
                inputs->pguser = optarg;
                break;
            case 'd':
                break;
            case 'Q':
                inputs->queryfile = optarg;
                break;
            default:
                break;
        }
    }
}

int main(int argc, char **argv)
{
    int c;
    Inputs inputs = {.concurrency = 10,
                     .duration = 30,
                     .timeout = 2,
                     .warmup_time = 5,
                     .output_format = TEXT,
                     .pghost = "127.0.0.1",
                     .pguser = "postgres",
                     .pgport = 5432,
                     .queryfile = ""};
    parse_args(&inputs, argc, argv);
    
    printf("Hello. concurrency: %d\n", inputs.concurrency);
    return 0;
}
