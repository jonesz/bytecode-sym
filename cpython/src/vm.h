/* vm.h
 * 2019, Ethan Jones <etn.jones@gmail.com>
 * See LICENSE for licensing information.
 */

#ifndef _VM_H
#define _VM_H

#include <stddef.h>
#include <stdint.h>
#include "bytecode.h"
#include "stp/c_interface.h"

#define DATA_STACK_SIZE 1024
#define FRAME_SIZE 1024

typedef struct _data_ctx {
  Expr* stack[DATA_STACK_SIZE];
  uint16_t ptr;
} data_ctx;

typedef struct _block_ctx {
  uint16_t ptr;
} block_ctx;

typedef struct _frame_ctx {
  data_ctx  *data;
  block_ctx *block;
} frame_ctx;

typedef struct _stack_ctx {
  frame_ctx* frame[FRAME_SIZE];
  uint16_t frame_ptr;
} stack_ctx;

int stack_init(stack_ctx *ctx);
int stack_run(stack_ctx *ctx, bytecode *bytecode);

#endif // _VM_H
