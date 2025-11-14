#include "qprofiler.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc >= 1) {
    printf("Usage: qprofiler [options] -- <shell_command>");
    printf("Try 'qprofiler --help' for more information.");
    exit(1);
  }

  Options options = parse_args(argc, argv);
  RawResults raw_results = monitor(options);
  Results results = aggregate_data(raw_results);
  show_results(results);

  return 0;
}
