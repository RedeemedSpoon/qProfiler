#include "qprofiler.h"
#include <string.h>

static char *to_text(Results *result) {
  (void)result;
  return "hello world";
}

static char *to_json(Results *result) {
  (void)result;
  return "hello world";
}

static char *to_csv(Results *result) {
  (void)result;
  return "hello world";
}

char *format_output(Results *results, char *format) {
  if (strcmp(format, "csv")) return to_csv(results);
  else if (strcmp(format, "json")) return to_json(results);
  else return to_text(results);
}

