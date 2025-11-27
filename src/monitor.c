#include "qprofiler.h"
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

static double get_time_diff(struct timespec start, struct timespec end) {
  struct timespec temp;

  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }

  return (double)temp.tv_sec + (double)temp.tv_nsec / 1000000000.0;
}

static double timeval_to_sec(struct timeval tv) { return (double)tv.tv_sec + (double)tv.tv_usec / 1e6; }

Results execute_and_monitor(char *shell_command) {
  Results results = {0};
  struct rusage usage = {0};
  struct timespec start_time, end_time;

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  signal(SIGINT, SIG_IGN);
  pid_t pid = fork();
  int status;

  if (pid == 0) {
    signal(SIGINT, SIG_DFL);
    execl("/bin/sh", "sh", "-c", shell_command, NULL);

    print_error("failed to execute command '%s': %s", shell_command, strerror(errno));
    exit(127);
  } else {
    if (wait4(pid, &status, 0, &usage) == -1) {
      print_error("failed to monitor child process statistics (wait4): %s", strerror(errno));
      exit(EXIT_FAILURE);
    }

    results.user_cpu_time = timeval_to_sec(usage.ru_utime);
    results.system_cpu_time = timeval_to_sec(usage.ru_stime);
    results.total_cpu_time = results.user_cpu_time + results.system_cpu_time;

    results.peak_rss = usage.ru_maxrss;
    results.major_page_faults = usage.ru_majflt;
    results.minor_page_faults = usage.ru_minflt;
    results.swaps = usage.ru_nswap;

    results.block_inputs = usage.ru_inblock;
    results.block_outputs = usage.ru_oublock;

    results.vol_context_switches = usage.ru_nvcsw;
    results.invol_context_switches = usage.ru_nivcsw;

    if (WIFEXITED(status)) {
      results.exit_status = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      results.exit_status = 128 + WTERMSIG(status);
    } else {
      results.exit_status = -1;
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end_time);
  results.elapsed_time = get_time_diff(start_time, end_time);
  if (results.elapsed_time > 0.000001) {
    results.cpu_utilization = (float)((results.total_cpu_time / results.elapsed_time) * 100.0);
  }

  return results;
}
