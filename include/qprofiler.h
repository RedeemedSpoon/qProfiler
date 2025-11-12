#ifndef QPROFILER_H
#define QPROFILER_H

#include <stdbool.h>
#include <stddef.h>

#define VERSION "0.0.1"

typedef struct {
  int _;
} Options;

typedef struct {
  int _;
} RawResults;

typedef struct {
  int _;
} Results;

Options parse_args(int argc, char *argv[]);
RawResults monitor(Options options);
Results aggregate_data(RawResults results);
void show_results(Results results);

#endif
