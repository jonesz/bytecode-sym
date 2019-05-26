/* main.c
 * 2019, Ethan Jones <etn.jones@gmail.com>
 * See LICENSE for licensing information.
 */

#include "vm.h"
#include <stdio.h>
#include <string.h>
#include "bytecode.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: ./main bytecode\n");
    return 0;
  }

  size_t len = strlen(argv[1]);
  if (len < 1) {
    printf("Bytecode of invalid length\n");
    return -1;
  }

  stack_ctx ctx;
  if (!stack_init(&ctx)) {
    printf("Unable to initialize stack\n");
    return -1;
  }

  bytecode bt;
  if(!parse_bytecode(&bt, argv[1], len)) {
    printf("Unbale to parse bytecode\n");
    return -1;
  }

  if (!stack_run(&ctx, &bt)) {
    printf("Unable to run bytecode\n");
    return -1;
  }

  return 0;
}
