/* vm.c
 * 2019, Ethan Jones <etn.jones@gmail.com>
 * See LICENSE for licensing information.
 */

// TODO: Go through calls to malloc, fail out if they're not allocating.
// TODO: Free pop'd off the stack expressions.

#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bytecode.h"
#include "opcode.h"

static frame_ctx *init_frame();
static int free_frame(frame_ctx *frame);

// TODO: cur_frame, push, pop, should be preprocessor macros.
static frame_ctx *cur_frame(stack_ctx *stack);
static int push(frame_ctx *frame, Expr *a);
static Expr* pop(frame_ctx *frame);

static Expr *create_expr();
static int free_expr(Expr *a);

static frame_ctx *init_frame() {
  frame_ctx *frame = (frame_ctx *)malloc(sizeof(frame_ctx));
  data_ctx  *data  = (data_ctx *)malloc(sizeof(data_ctx));
  block_ctx *block = (block_ctx *)malloc(sizeof(block_ctx));

  data->ptr  = 0;
  block->ptr = 0;

  frame->data  = data;
  frame->block = block;
  return frame;
}

// TODO: This should attempt to free objects on the data block, i.e. Expr.
static int free_frame(frame_ctx *frame) {
  free(frame->data);
  free(frame->block);
  free(frame);
  return 0;
}

static frame_ctx *cur_frame(stack_ctx *stack) {
  return stack->frame[stack->frame_ptr];
}

// TODO: Overflow check?
static int push(frame_ctx *frame, Expr *a) {
  data_ctx *data = frame->data;
  data->stack[data->ptr++] = a;
  return 1;
}

// TODO: Underflow issues.
static Expr* pop(frame_ctx *frame) {
  data_ctx *data = frame->data;
  return data->stack[--data->ptr];
}

static Expr* create_expr() {
  Expr *a = (Expr *)malloc(sizeof(Expr));
  return a;
}

// TODO: There's a vc_Destroy(?) Expr that more than likely needs to be
// called here.
static int free_expr(Expr* a) {
  free(a);
  return 0;
}

int stack_init(stack_ctx *ctx) {
  frame_ctx *cur = init_frame();
  if (cur == NULL)
    return -1;
  ctx->frame_ptr = 0;
  ctx->frame[ctx->frame_ptr] = cur;
  return 1;
}

int stack_run(stack_ctx *ctx, bytecode *bt) {
  VC vc = vc_createValidityChecker();
  uint8_t *instr_ptr = bt->co_code;

  // main instruction loop.
  while(1) {

    uint8_t instr = *instr_ptr++;
    uint8_t arg   = *instr_ptr;
    int r = 0;

    switch (instr) {

    //
    // GENERAL INSTRUCTIONS
    //
    case NOP: {
      break;
    }

    case POP_TOP: {
      Expr *a = pop(cur_frame(ctx));
      //free_expr(a);
      break;
    }

    case ROT_TWO: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      push(cur_frame(ctx), a);
      push(cur_frame(ctx), b);
      break;
    }

    case ROT_THREE: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr* c = pop(cur_frame(ctx));
      push(cur_frame(ctx), a);
      push(cur_frame(ctx), c);
      push(cur_frame(ctx), b);
      break;
    }

    case DUP_TOP: {
      return -1;
    }

    case DUP_TOP_TWO: {
      return -1;
    }

    //
    // UNARY OPERATIONS
    //
    case UNARY_POSITIVE: {
      return -1;
    }

    case UNARY_NEGATIVE: {
      return -1;
    }

    case UNARY_NOT: {
      return -1;
    }

    case UNARY_INVERT: {
      return -1;
    }

    case GET_ITER: {
      return -1;
    }

    case GET_YIELD_FROM_ITER: {
      return -1;
    }

    //
    // BINARY OPERATIONS
    //
    case BINARY_POWER: {
      return -1;
    }

    case BINARY_MULTIPLY: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bv32MultExpr(vc, *a, *b);
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      break;
    }

    case BINARY_MATRIX_MULTIPLY: {
      return -1;
    }

    case BINARY_FLOOR_DIVIDE: {
      return -1;
    }

    case BINARY_TRUE_DIVIDE: {
      return -1;
    }

    // Unsigned modulo.
    case BINARY_MODULO: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bvModExpr(vc, 32, *a, *b);
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      break;
    }

    case BINARY_ADD: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bv32PlusExpr(vc, *a, *b);
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      break;
    }

    case BINARY_SUBTRACT: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bv32MinusExpr(vc, *a, *b);
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      break;
    }

    case BINARY_SUBSCR: {
      return -1;
    }

    // Unsigned shifts.
    case BINARY_LSHIFT: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bvLeftShiftExprExpr(vc, 32, *b, *a); 
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      break;
    }

    case BINARY_RSHIFT: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bvRightShiftExprExpr(vc, 32, *b, *a);
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      break;
    }

    case BINARY_AND: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bvAndExpr(vc, *b, *a);
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      break;
    }

    case BINARY_XOR: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bvXorExpr(vc, *b, *a);
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      break;
    }
    case BINARY_OR: {
      Expr* a = pop(cur_frame(ctx));
      Expr* b = pop(cur_frame(ctx));
      Expr *c = create_expr();
      *c = vc_bvOrExpr(vc, *b, *a);
      push(cur_frame(ctx), c);
      //free_expr(a);
      //free_expr(b);
      return -1;
    }

    case INPLACE_POWER: {
      return -1;
    }

    case INPLACE_MULTIPLY: {
      return -1;
    }

    case INPLACE_MATRIX_MULTIPLY: {
      return -1;
    }

    case INPLACE_FLOOR_DIVIDE: {
      return -1;
    }

    case INPLACE_TRUE_DIVIDE: {
      return -1;
    }

    case INPLACE_MODULO: {
      return -1;
    }

    case INPLACE_ADD: {
      return -1;
    }

    case INPLACE_SUBTRACT: {
      return -1;
    }

    case INPLACE_LSHIFT: {
      return -1;
    }

    case INPLACE_RSHIFT: {
      return -1;
    }

    case INPLACE_AND: {
      return -1;
    }

    case INPLACE_XOR: {
      return -1;
    }

    case INPLACE_OR: {
      return -1;
    }

    case RETURN_VALUE: {
      /* If we're in the first frame; return the value on the stack
       * to the outside world. Otherwise, push onto the previous frame
       * and attempt to free it. */
      if (ctx->frame_ptr == 0) {
        Expr *a = pop(cur_frame(ctx));
        // TODO: Attempt to concretize the output of the var?
        return 0;
      } else {
        Expr *a = pop(cur_frame(ctx));
        free_frame(cur_frame(ctx));
        ctx->frame_ptr--;
        push(cur_frame(ctx), a);
        break;
      }
    }

    case STORE_NAME: {
      return -1;
    }

    case DELETE_NAME: {
      return -1;
    }

    case LOAD_CONST: {
      Expr *a = create_expr();
      *a = vc_bvConstExprFromInt(vc, 32, bt->co_consts[arg]);
      push(cur_frame(ctx), a);
      break;
    }

    /* Forks are done on comparison operations; we result in two processes
     * that have a succesful and failure compare operation. */
    case COMPARE_OP: {
      Expr *a = pop(cur_frame(ctx));
      Expr *b = pop(cur_frame(ctx));
      // This will allow us to print two values, one where the value
      // results in true, one where it results in false.
      if (fork() == 0) {
        Expr expr;
        // TODO: We attempted to set these expressions to True or False
        // and then solve, so we'd get a set of values that sovle for
        // True or False.. but that didn't work, so we've flipped the
        // LT/GT, EQ/NE, etc.
        switch (arg) {
        case LT:
          expr = vc_bvLtExpr(vc, *a, *b);
          break;
        case LE:
          expr = vc_bvLeExpr(vc, *a, *b);
          break;
        case EQ:
          expr = vc_eqExpr(vc, *a, *b);
          break;
        case NE:
          expr = vc_notExpr(vc, vc_eqExpr(vc, *a, *b));
          break;
        case GT:
          expr = vc_bvGtExpr(vc, *a, *b);
          break;
        case GE:
          expr = vc_bvGeExpr(vc, *a, *b);
          break;
        }

        // Print a counterexample.
        int r = vc_query(vc, expr);
        vc_printCounterExample(vc);

        Expr *c = create_expr();
        *c = vc_falseExpr(vc);
        push(cur_frame(ctx), c);

      } else {
        Expr expr;
        // TODO: See above with thoughts on solving these expressions.
        switch (arg) {
        case LT:
          expr = vc_bvGtExpr(vc, *a, *b);
          break;
        case LE:
          expr = vc_bvGeExpr(vc, *a, *b);
          break;
        case EQ:
          expr = vc_notExpr(vc, vc_eqExpr(vc, *a, *b));
          break;
        case NE:
          expr = vc_eqExpr(vc, *a, *b);
          break;
        case GT:
          expr = vc_bvLtExpr(vc, *a, *b);
          break;
        case GE:
          expr = vc_bvLeExpr(vc, *a, *b);
          break;
        }
        
        // Print a counterexample.
        int r = vc_query(vc, expr);
        vc_printCounterExample(vc);

        Expr *c = create_expr();
        *c = vc_trueExpr(vc);
        push(cur_frame(ctx), c);
      }
      //free_expr(a);
      //free_expr(b);
      break;
    }

    // BRANCH
    case POP_JUMP_IF_TRUE: {
      Expr *a = pop(cur_frame(ctx));
      // TODO: vc_isBool throws segfaults (apparently isn't supposed to
      // sed for things like this? Use vc_query, which I bet does retarded
      // stuff if we pass it bad on values off the stack.
      if (vc_query(vc, *a)) {
        // We increment the value of the instr_ptr at the end of
        // this switch; decrement the argument here.
        instr_ptr = &bt->co_code[arg-1];
      }
      break;
    }

    case POP_JUMP_IF_FALSE: {
      Expr *a = pop(cur_frame(ctx));
      // TODO: see above.
      if (!vc_query(vc, *a)) {
        // We increment the value of the instr_ptr at the end of
        // this switch; decrement the argument here.
        instr_ptr = &bt->co_code[arg-1];
      }
      //free_expr(a);
      break;
    }

    case JUMP_IF_TRUE_OR_POP: {
      Expr *a = pop(cur_frame(ctx));
      // TODO: see above
      if (vc_query(vc, *a)) {
        // We increment the value of the instr_ptr at the end of
        // this switch; decrement the argument here.
        instr_ptr = &bt->co_code[arg-1];
        push(cur_frame(ctx), a);
      } else {
        //free_expr(a);
      }
      break;
    }

    case JUMP_IF_FALSE_OR_POP: {
      Expr *a = pop(cur_frame(ctx));
      // TODO: see above
      if (!vc_query(vc, *a)) {
        // We increment the value of the instr_ptr at the end of
        // this switch; decrement the argument here.
        instr_ptr = &bt->co_code[arg-1];
        push(cur_frame(ctx), a);
      } else {
        //free_expr(a);
      }
      break;
    }

    case LOAD_FAST: {
      Expr *a = create_expr();
      *a = vc_varExpr(vc, bt->co_varnames[arg], vc_bvType(vc, 32));
      push(cur_frame(ctx), a);
      break;
    }

    case STORE_FAST: {
      return -1;
    }

    case DELETE_FAST: {
      return -1;
    }
    default:
      break;
    }
    instr_ptr++;
  }

  return 0;
}

