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
    memset(opts, 0, sizeof(struct options));            // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    opts->port_in     = DEFAULT_PORT;
    opts->port_out    = DEFAULT_PORT;
    opts->fd_in       = STDIN_FILENO;
    opts->fd_out      = STDOUT_FILENO;
}

int parse_arguments(int argc, char *argv[], struct options *opts)
{
    if (argc > MAX_ARG_COUNT) {
        return FAIL;
    }

    int c;
    int is_error = 0;

    while ((c = getopt(argc, argv, "i:p:")) != -1)   // NOLINT(concurrency-mt-unsafe)
    {
        switch (c)
        {
            case 'i':
            {
                opts->ip_in = optarg;
                break;
            }
            case 'p':
            {
                // parse_port could exit the program.
                opts->port_in = parse_port(optarg, 10, &is_error);        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                if (is_error == FAIL)
                {
                    return FAIL_WITH_MSG;
                }
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
