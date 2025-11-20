#include "qprofiler.h"

Results monitor(char *shell_command) {
  static Results results;
  (void)shell_command;
  return results;
}
