#ifndef QPROFILER_H
#define QPROFILER_H

#include <stdbool.h>
#include <stddef.h>

#define VERSION "0.0.1"
#define MAX_COMMAND_LENGTH 2048

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"
#define STYLE_BOLD "\x1b[1m"

typedef enum { DEFAULT = 1, MINIMAL = 2, VERBOSE = 3 } Mode;

typedef struct {
  char *shell_command;
  char *format_style;
  char *output_file;
  bool append_flag;
  Mode mode;
} Options;

typedef struct {
    double elapsed_time;
    double user_cpu_time;
    double system_cpu_time;
    double total_cpu_time; 

    unsigned long long bytes_read;
    unsigned long long bytes_written;

    unsigned long peak_rss;
    unsigned long major_page_faults;
    unsigned long minor_page_faults;
    unsigned long swaps;
    unsigned long block_inputs;
    unsigned long block_outputs;
    unsigned long vol_context_switches;
    unsigned long invol_context_switches;
    unsigned long read_syscalls;
    unsigned long write_syscalls;
    unsigned long signals_received;

    float cpu_utilization;
    int exit_status;
} Results;

Options parse_args(int argc, char *argv[]);
void print_error(char message[], ...);
void print_explanation(void);
void print_version(void);
void print_help(void);

Results monitor(char *shell_command);
Results aggregate_data(Results *results, Mode mode);
void output_results(Results *results, Options *options);
char *format_output(Results *results, char *format);

#endif
