#include "qprofiler.h"

void output_results(Results *results, Options *options) {
  char *output = format_output(results, options->format_style);
  (void)output;
  (void)results;
  (void)options;
}

Results aggregate_data(Results *results, Mode mode) {
  (void)results;
  (void)mode;
  return *results;
}
