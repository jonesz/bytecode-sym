/* test_vm.c
 * 2019, Ethan Jones <etn.jones@gmail.com>
 * See LICENSE for licensing information.
 */

#include <assert.h>
#include <stdio.h>
#include "../bytecode.h"
#include "../vm.h"

static uint8_t  bad_co_code[]   = {1, 1, 1, 1, 1, 1, 83, 1};
static uint32_t bad_co_consts[] = {1, 1, 1, 1, 1, 1, 1, 1};

// 4 NOPs and a return
static uint8_t  do_nothing_co_code[]    = {9, 0, 9, 0, 9, 0, 9, 0, 83, 0};
static uint32_t do_nothing_co_consts[]  = {0};

// A branching program
// def k(x):
//    if x > 5:
//        return x
//    else:
//        return 0
//
//   dis.dis(k) ==>
//   2           0 LOAD_FAST                0 (x)
//               2 LOAD_CONST               1 (5)
//               4 COMPARE_OP               4 (>)
//               6 POP_JUMP_IF_FALSE       12
//
//   3           8 LOAD_FAST                0 (x)
//               10 RETURN_VALUE
//
//   5     >>    12 LOAD_CONST              2 (0)
//               14 RETURN_VALUE
//               16 LOAD_CONST              0 (None)
//               18 RETURN_VALUE
//
// TODO: This is hand written; we don't parse the bytecode into a "usable"
// (whatever dumpster fire we've implemented) format.
static uint8_t  branching_co_code[]   = {124, 0, 100, 1, 107, 4, 114, 12,
124, 0, 83, 0, 100, 2, 83, 0, 100, 0, 83, 0};

// 88 = x in ascii;
// TODO: We should be loading out of const? not varnames?
static uint32_t branching_co_consts[] = {88, 5, 0};
static uint8_t* branching_co_varnames[] = {"x"};

int main(void) {

  stack_ctx ctx;
  assert(stack_init(&ctx) == 1);

  // bunk garbage.
  bytecode bc;
  bc.co_code = (uint8_t *)bad_co_code;
  bc.co_consts = (uint32_t *)bad_co_consts;
  // TODO: What should this return?
  //assert(stack_run(&ctx, &bc) == 0);

  bc.co_code = (uint8_t *)do_nothing_co_code;
  bc.co_consts = (uint32_t *)do_nothing_co_consts;
  //assert(stack_run(&ctx, &bc) == 0);

  bc.co_code = (uint8_t *)branching_co_code;
  bc.co_consts = (uint32_t *)branching_co_consts;
  bc.co_varnames = (uint8_t **)branching_co_varnames;
  assert(stack_run(&ctx, &bc) == 0);
  printf("Tests passed!\n");

  return 0;

}
