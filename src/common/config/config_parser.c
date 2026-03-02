#include "config/config.h"
#include <bits/getopt_core.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline char *get_usage_string() {
  // TODO: improve usage string
  return "Usage: rvsim [-m ooo|in_order] [-i imem_size] [-d dmem_size] "
         "<elf_file>\n";
}
void parse_config(int argc, char *argv[], sim_config *cfg) {
  int opt = 1;
  static struct option long_options[] = {
      {"imem-size", required_argument, 0, 'i'},
      {"dmem-size", required_argument, 0, 'd'},
      {"mode", required_argument, 0, 'm'},
      {0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "i:d:m:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'm':
      if (!strcmp(optarg, "ooo")) {
        cfg->mode = MODE_OOO;
      } else if (!strcmp(optarg, "in_order")) {
        cfg->mode = MODE_IN_ORDER;
      } else {
        fprintf(stderr, get_usage_string(), argv[0]);
        exit(EXIT_FAILURE);
      }
      break;
    case 'i':
      cfg->imem_size = atoi(optarg);
      break;
    case 'd':
      cfg->dmem_size = atoi(optarg);
      break;
    default: /* '?' */
      fprintf(stderr, get_usage_string(), argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  if (optind < argc) {
    cfg->file_path = argv[optind];
  } else {
    fprintf(stderr, "Error: Missing file path\n");
    exit(1);
  }
}
