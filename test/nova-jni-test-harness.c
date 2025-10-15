#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Nova.h"


/**
 * gcc test.c -L. -lNovaJni -o test
 *
 * must have SO in local directory
 */

static bool check_args(const int argc, const char*argv[], const char flag[], const char lflag[]) {

  for (int i = 0; i < argc; i++) {
    if (strcmp(flag, argv[i]) == 0 || strcmp(lflag, argv[i]) == 0) {
      return true;
    }
  }
  return false;
}

int main(const int argc, const char*argv[]) {
  const bool execute = check_args(argc, argv, "-x", "--execute");
  const bool verbose = check_args(argc, argv, "-V", "--verbose");
  const bool help = check_args(argc, argv, "-h", "--help");

	system("clear");

	if (help || verbose || !execute || check_args(argc, argv, "-v", "--version")) {
	  printf("+-------------------------+\n");
	  printf("| NovaJni.so test harness |\n");
	  printf("| %-23s |\n", __DATE__);
	  printf("| %-23s |\n", __TIME__);
	  printf("+-------------------------+\n");
	  printf("\n");
	}

  if (verbose || check_args(argc, argv, "-l", "--library-path")) {
    printf("library path = [%s]\n", getenv("LD_LIBRARY_PATH"));
    printf("\n");
  }

	if (help || argc == 1) {
    printf("-h --help           display help\n");
	  printf("-x --execute        execute\n");
    printf("-V --verbose        verbose output\n");
    printf("-v --version        display version information\n");
    printf("-l --library-path   display library paths\n");
    printf("\n");
    printf("This application requres both NovaJni.so and libFlxCore64.so.2025.09 to be in the library path\n");
	}
	else {
    if (execute) {
      schneider_nova_jni_test();
    }
    else {
      printf("execution not requested\n");
    }
  }

  printf("\n");

	return 0;
}
