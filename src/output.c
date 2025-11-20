#include "qprofiler.h"

void output_results(Results *results, Options *options) {
  char *output = format_output(results, options->format_style);
  (void)output;
  (void)results;
  (void)options;
}
