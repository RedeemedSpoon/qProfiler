#include "qprofiler.h"
#include <stdarg.h>
#include <stdio.h>

void print_error(char *format, ...) {
  if (format[0] == '!') {
    format++;
  } else {
    fprintf(stderr, "qProfiler: error: ");
  }

  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  fprintf(stderr, "\nTry 'qprofiler --help' for more information.\n");
}

void print_version(void) {
  printf("qProfiler version %s\n", VERSION);
}

void print_help(void) {
  printf("Usage: qprofiler [options] <command>\n\n");
  printf("A quick and simple profiler to measure the performance of any command.\n\n");

  printf("Options:\n");
  printf("  -o, --output <file>    Write the report to <file> instead of stdout.\n");
  printf("  -a, --append           Append to the output file if it exists (use with -o).\n");
  printf("  -f, --format <format>  Set output format. Can be 'txt', 'csv', or 'json'.\n\n");
  printf("  -v, --verbose          Display all available performance metrics for a detailed report.\n");

  printf("Information:\n");
  printf("  -x, --explain          Print a detailed explanation of each available metric and exit.\n");
  printf("  -h, --help             Display this help message and exit.\n");
  printf("  -V, --version          Display version information and exit.\n");
}

void print_explanation(void) {
  printf("qProfiler Metric Explanations:\n\n");

  printf("--- Time & CPU Metrics ---\n");
  printf("  Elapsed (Wall-Clock) Time:\n");
  printf("    The total real-world time from the command's start to its finish.\n");
  printf("    This is how long you waited for it.\n\n");
  printf("  CPU Utilization:\n");
  printf("    The percentage of the Elapsed Time that the CPU was actively working on the command.\n");
  printf("    A high percentage (~100%%) indicates a CPU-bound task, while a low percentage\n");
  printf("    suggests the task spent most of its time waiting (e.g., for disk or network I/O).\n\n");
  printf("  User CPU Time:\n");
  printf("    The total time the CPU spent executing the command's own code in \"user mode.\"\n");
  printf("    This represents the work done by the application's logic.\n\n");
  printf("  System CPU Time:\n");
  printf("    The total time the CPU spent executing kernel code on the command's behalf.\n");
  printf("    This includes tasks like opening files, writing to the network, and managing memory.\n\n");
  printf("  Total CPU Time:\n");
  printf("    The sum of User and System CPU time. It represents the total amount of\n");
  printf("    processing work the CPU performed for the command.\n\n");

  printf("--- Memory Metrics ---\n");
  printf("  Peak Memory Usage (Max RSS):\n");
  printf("    The maximum amount of physical RAM (Resident Set Size) that the command occupied\n");
  printf("    at any single moment during its execution.\n\n");
  printf("  Major Page Faults:\n");
  printf("    Occurs when the command needs to access a piece of memory that is not in RAM\n");
  printf("    and must be loaded from a storage device (like an SSD/HDD). A high number is a\n");
  printf("    strong indicator of heavy disk I/O or that the system has insufficient RAM.\n\n");
  printf("  Minor Page Faults:\n");
  printf("    Occurs when the command needs a memory page that is already in RAM but is not in\n");
  printf("    the process's direct working set. These are fast, common, and generally not a concern.\n\n");
  printf("  Swaps:\n");
  printf("    The number of times the process was entirely swapped out of RAM and onto the disk.\n");
  printf("    Any value greater than zero indicates severe memory pressure on the system.\n\n");

  printf("--- I/O Metrics ---\n");
  printf("  Block Input / Output Operations:\n");
  printf("    A count of filesystem read and write operations. This is a rough measure of\n");
  printf("    I/O activity, where an \"operation\" corresponds to a block of data.\n\n");

  printf("--- Scheduling Metrics ---\n");
  printf("  Voluntary Context Switches:\n");
  printf("    Occurs when the command gives up the CPU because it is waiting for a resource\n");
  printf("    to become available (e.g., waiting for a file to be read from the disk).\n\n");
  printf("  Involuntary Context Switches:\n");
  printf("    Occurs when the OS scheduler forces the command to give up the CPU (e.g., because\n");
  printf("    its time slice expired or a higher-priority process needed to run).\n");
}
