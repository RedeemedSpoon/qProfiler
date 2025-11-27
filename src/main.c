#include "qprofiler.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    print_error("!Usage: qprofiler [options] <shell_command>");
    exit(1);
  }

  Options options = parse_args(argc, argv);
  fprintf(stderr, "%s%s[qProfiler]%s Profiling command: ", COLOR_RED, STYLE_BOLD, COLOR_RESET);
  fprintf(stderr, "%s%s%s\n\n", COLOR_YELLOW, options.shell_command, COLOR_RESET);

  Results results = execute_and_monitor(options.shell_command);
  output_results(&results, &options);

  fflush(stderr);
  return 0;
}
