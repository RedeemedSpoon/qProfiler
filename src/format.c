#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "qprofiler.h"

static char *to_csv(Results *r) {
  char *buf;
  int ret = asprintf(&buf,
    "elapsed_time,user_cpu,sys_cpu,total_cpu,cpu_util,peak_rss,block_in,block_out,exit_status\n"
    "%.6f,%.6f,%.6f,%.6f,%.1f,%lu,%lu,%lu,%d",
    r->elapsed_time, r->user_cpu_time, r->system_cpu_time, r->total_cpu_time,
    r->cpu_utilization, r->peak_rss, r->block_inputs, r->block_outputs, r->exit_status
  );
  return (ret == -1) ? NULL : buf;
}

static char *to_json(Results *r) {
  char *buf;
  int ret = asprintf(&buf,
    "{\n"
    "  \"elapsed_time_s\": %.6f,\n"
    "  \"user_cpu_time_s\": %.6f,\n"
    "  \"system_cpu_time_s\": %.6f,\n"
    "  \"total_cpu_time_s\": %.6f,\n"
    "  \"cpu_utilization_pct\": %.1f,\n"
    "  \"peak_rss_kb\": %lu,\n"
    "  \"block_inputs\": %lu,\n"
    "  \"block_outputs\": %lu,\n"
    "  \"exit_status\": %d\n"
    "}",
    r->elapsed_time, r->user_cpu_time, r->system_cpu_time, r->total_cpu_time,
    r->cpu_utilization, r->peak_rss, r->block_inputs, r->block_outputs, r->exit_status
  );
  return (ret == -1) ? NULL : buf;
}

static char *to_text(Results *r) {
  char *buf;
  const char *status_color = (r->exit_status == 0) ? COLOR_GREEN : COLOR_RED;

  int ret = asprintf(&buf,
    STYLE_BOLD COLOR_BLUE "--- qProfiler Report ---" COLOR_RESET "\n"
    "Elapsed Time:             %.3f s\n"
    "User CPU Time:            %.3f s\n"
    "System CPU Time:          %.3f s\n"
    "Total CPU Time:           %.3f s\n"
    "CPU Utilization:          %.1f %%\n"
    "Peak Memory:              %lu KB\n"
    "Block Inputs:             %lu\n"
    "Block Outputs:            %lu\n"
    "Exit Status:              %s%d" COLOR_RESET,
    
    r->elapsed_time, r->user_cpu_time, r->system_cpu_time, r->total_cpu_time,
    r->cpu_utilization, r->peak_rss, r->block_inputs, r->block_outputs, 
    status_color, r->exit_status
  );
  return (ret == -1) ? NULL : buf;
}

static char *to_csv_verbose(Results *r) {
  char *buf;
  int ret = asprintf(&buf,
    "elapsed_time,cpu_util,user_cpu,sys_cpu,total_cpu,peak_rss,maj_flt,min_flt,swaps,block_in,block_out,vol_ctx,invol_ctx,exit_status\n"
    "%.6f,%.1f,%.6f,%.6f,%.6f,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%d",
    r->elapsed_time, r->cpu_utilization, r->user_cpu_time, r->system_cpu_time, r->total_cpu_time,
    r->peak_rss, r->major_page_faults, r->minor_page_faults, r->swaps,
    r->block_inputs, r->block_outputs, r->vol_context_switches, r->invol_context_switches, r->exit_status
  );
  return (ret == -1) ? NULL : buf;
}

static char *to_json_verbose(Results *r) {
  char *buf;
  int ret = asprintf(&buf,
    "{\n"
    "  \"elapsed_time_s\": %.6f,\n"
    "  \"cpu_utilization_pct\": %.1f,\n"
    "  \"user_cpu_time_s\": %.6f,\n"
    "  \"system_cpu_time_s\": %.6f,\n"
    "  \"total_cpu_time_s\": %.6f,\n"
    "  \"peak_rss_kb\": %lu,\n"
    "  \"major_page_faults\": %lu,\n"
    "  \"minor_page_faults\": %lu,\n"
    "  \"swaps\": %lu,\n"
    "  \"block_inputs\": %lu,\n"
    "  \"block_outputs\": %lu,\n"
    "  \"vol_context_switches\": %lu,\n"
    "  \"invol_context_switches\": %lu,\n"
    "  \"exit_status\": %d\n"
    "}",
    r->elapsed_time, r->cpu_utilization, r->user_cpu_time, r->system_cpu_time, r->total_cpu_time,
    r->peak_rss, r->major_page_faults, r->minor_page_faults, r->swaps,
    r->block_inputs, r->block_outputs, r->vol_context_switches, r->invol_context_switches, r->exit_status
  );
  return (ret == -1) ? NULL : buf;
}

static char *to_text_verbose(Results *r) {
  char *buf;
  const char *status_color = (r->exit_status == 0) ? COLOR_GREEN : COLOR_RED;

  int ret = asprintf(&buf,
    STYLE_BOLD COLOR_BLUE "--- qProfiler Report (Verbose) ---" COLOR_RESET "\n"
    "Elapsed Time:             %.3f s\n"
    "CPU Utilization:          %.1f %%\n"
    "User CPU Time:            %.3f s\n"
    "System CPU Time:          %.3f s\n"
    "Total CPU Time:           %.3f s\n\n"
    "Peak Memory:              %lu KB\n"
    "Major Page Faults:        %lu\n"
    "Minor Page Faults:        %lu\n"
    "Swaps:                    %lu\n\n"
    "Block Inputs:             %lu\n"
    "Block Outputs:            %lu\n"
    "Voluntary Ctx Switches:   %lu\n"
    "Involuntary Ctx Switches: %lu\n"
    "Exit Status:              %s%d" COLOR_RESET,

    r->elapsed_time, r->cpu_utilization, r->user_cpu_time, r->system_cpu_time, r->total_cpu_time,
    r->peak_rss, r->major_page_faults, r->minor_page_faults, r->swaps,
    r->block_inputs, r->block_outputs, r->vol_context_switches, r->invol_context_switches, 
    status_color, r->exit_status
  );
  return (ret == -1) ? NULL : buf;
}

char *format_output(Results *results, char *format, Mode mode) {
  if (mode == VERBOSE) {
    if (strcmp(format, "csv") == 0)
      return to_csv_verbose(results);
    else if (strcmp(format, "json") == 0)
      return to_json_verbose(results);
    else
      return to_text_verbose(results);
  }
  else {
    if (strcmp(format, "csv") == 0)
      return to_csv(results);
    else if (strcmp(format, "json") == 0)
      return to_json(results);
    else
      return to_text(results);
  }
}
