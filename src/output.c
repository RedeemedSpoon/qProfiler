#include "qprofiler.h"
#include <stdlib.h>
#include <stdio.h>

void output_results(Results *results, Options *options) {
  char *output = format_output(results, options->format_style, options->mode);

  if (options->output_file != NULL) {
    char *file_options = options->append_flag ? "a" : "w";
    char *output_file = options->output_file;

    FILE *fptr = fopen(output_file, file_options);
    if (fptr == NULL) {
      print_error("failed to open output file");
      exit(EXIT_FAILURE);
    }

    fprintf(fptr, "%s\n", output);
    fclose(fptr);
    free(output);
    exit(0);
  }

  fprintf(stderr, "\n%s\n", output);
  free(output);
  exit(0);
}
