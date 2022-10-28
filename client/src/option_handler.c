//
// Created by Shik Hur on 2022-10-28.
//


#include "conversion.h"
#include "option_handler.h"
#include <assert.h>
#include <string.h>
#include <unistd.h>


void options_init(struct options *opts)
{
    memset(opts, 0, sizeof(struct options));     // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    opts->port_out    = DEFAULT_PORT;
    opts->fd_out      = STDOUT_FILENO;
    opts->fd_in       = STDIN_FILENO;
}

int parse_arguments(int argc, char *argv[], struct options *opts)
{
    const int FAIL = 1;
    if (argc < MIN_ARG_COUNT) {
        return FAIL;
    }

    int c;

    while ((c = getopt(argc, argv, "o:p:")) != -1)   // NOLINT(concurrency-mt-unsafe)
    {
        switch (c)
        {
            case 'o':
            {
                opts->ip_out = optarg;
                break;
            }
            case 'p':
            {
                // parse_port could exit the program.
                opts->port_out = parse_port(optarg, 10);        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                break;
            }
            case ':':
            case '?':
            {
                return FAIL;
            }
            default:
            {
                assert("should not get here");
            }
        }
    }

    if (optind < argc)
    {
        return FAIL;
    }
    return 0;
}
