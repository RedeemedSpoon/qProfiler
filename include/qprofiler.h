#ifndef QPROFILER_H
#define QPROFILER_H

#include <stdbool.h>
#include <stddef.h>

#define VERSION "0.0.1"
#define SEPARATOR "--"
#define MAX_COMMAND_LENGTH 2048

typedef struct {
  char *shell_command;
  char *output_file;
  char *format_style;
  bool minimal_mode;
  bool verbose_mode;
  bool append_flag;
} Options;

typedef struct {
  int _;
} RawResults;

typedef struct {
  int _;
} Results;

Options parse_args(int argc, char *argv[]);
void print_error(char message[], ...);
void print_explanation(void);
void print_version(void);
void print_help(void);

RawResults monitor(Options options);
Results aggregate_data(RawResults results);
void show_results(Results results);

#endif
