#ifndef QPROFILER_H
#define QPROFILER_H

#include <stdbool.h>
#include <stddef.h>

#define VERSION "0.0.1"

typedef struct {
  char *output;
  char *append;
  char *format;
  char *command;
  bool *minimal;
  bool *verbose;
} Options;

typedef struct {
  int _;
} RawResults;

typedef struct {
  int _;
} Results;

Options parse_args(int argc, char *argv[]);
void print_explanation(void);
void print_version(void);
void print_help(void);

RawResults monitor(Options options);
Results aggregate_data(RawResults results);
void show_results(Results results);

#endif
