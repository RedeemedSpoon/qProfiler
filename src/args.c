#include "qprofiler.h"
#include <stdlib.h>
#include <string.h>

static bool arg_matches(const char *arg, char *short_form, char *long_form) {
  return strcmp(arg, short_form) == 0 || strcmp(arg, long_form) == 0;
}

Options parse_args(int argc, char *argv[]) {
  Options options;

  options.shell_command = NULL;
  options.format_style = "txt";
  options.minimal_mode = false;
  options.verbose_mode = false;
  options.append_flag = false;
  options.output_file = NULL;

  for (int i = 1; i < argc; i++) {
    const char *current_arg = argv[i];

    if (current_arg[0] != '-') {
      static char command_buffer[MAX_COMMAND_LENGTH];
      command_buffer[0] = '\0';

      for (int j = i; j < argc; j++) {
        if (strlen(command_buffer) + strlen(argv[j]) + 2 > sizeof(command_buffer)) {
          print_error("command is too long (max %zu characters)", sizeof(command_buffer) - 1);
          exit(1);
        }

        strcat(command_buffer, argv[j]);
        if (j < argc - 1) {
          strcat(command_buffer, " ");
        }
      }

      options.shell_command = command_buffer;
      break;
    }

    else if (arg_matches(current_arg, "-h", "--help")) {
      print_help();
      exit(0);
    }

    else if (arg_matches(current_arg, "-V", "--version")) {
      print_version();
      exit(0);
    }

    else if (arg_matches(current_arg, "-x", "--explain")) {
      print_explanation();
      exit(0);
    }

    else if (arg_matches(current_arg, "-m", "--minimal")) {
      options.minimal_mode = true;
      options.verbose_mode = false;
    }

    else if (arg_matches(current_arg, "-v", "--verbose")) {
      options.verbose_mode = true;
      options.minimal_mode = false;
    }

    else if (arg_matches(current_arg, "-a", "--append")) {
      options.append_flag = true;
    }

    else if (arg_matches(current_arg, "-o", "--output")) {
      if (i + 1 < argc) {
        options.output_file = argv[i + 1];
        i++;
      } else {
        print_error("option '%s' requires an argument.\n", current_arg);
        exit(1);
      }
    }

    else if (arg_matches(current_arg, "-f", "--format")) {
      if (i + 1 < argc) {
        char *format = argv[i + 1];
        if (!(!strcmp(format, "txt") || !strcmp(format, "json") || !strcmp(format, "csv"))) {
          print_error("invalid format specified: '%s'", format);
          exit(1);
        }

        options.format_style = format;
        i++;
      } else {
        print_error("option '%s' requires an argument.\n", current_arg);
        exit(1);
      }
    }

    else {
      print_error("unrecognized command-line option '%s'\n", current_arg);
      exit(1);
    }
  }

  if (options.shell_command == NULL) {
    print_error("missing command to profile");
    exit(1);
  }

  return options;
}
