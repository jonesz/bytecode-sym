#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "stp/c_interface.h"

#define STACK_SIZE 256
Expr* stack[STACK_SIZE];

/* The sample C code from the STP documentation. */
int run(void) {
  VC vc = vc_createValidityChecker();
  Expr c = vc_varExpr(vc, "c", vc_bvType(vc, 32));
  Expr a = vc_bvConstExprFromInt(vc, 32, 5);
  Expr b = vc_bvConstExprFromInt(vc, 32, 6);
  Expr xp1 = vc_bvPlusExpr(vc, 32, a, b);
  Expr eq = vc_eqExpr(vc, xp1, c);
  Expr eq2 = vc_notExpr(vc, eq);
  int ret = vc_query(vc, eq2);
  assert(ret == false);
  vc_printCounterExample(vc);
  vc_Destroy(vc);
  return 0;
}

Expr *create_expr() {
  Expr *a = (Expr *)malloc(sizeof(Expr));
  return a;
}

/* A malloc version of the above that sort of looks like our vm. */
int run_malloc(void) {
  VC vc = vc_createValidityChecker();
  int stack_ptr = 0;

  int i = 0;
  while (1) {
    switch (i) {

    case 0: {
      Expr *c = create_expr();
      *c = vc_varExpr(vc, "c", vc_bvType(vc, 32));
      stack[stack_ptr++] = c;
      break;
    }

    case 1: {
      Expr *a = create_expr();
      *a = vc_bvConstExprFromInt(vc, 32, 5);
      stack[stack_ptr++] = a;
      break;
    }

    case 2: {
      Expr *b = create_expr();
      *b = vc_bvConstExprFromInt(vc, 32, 6);
      stack[stack_ptr++] = b;
      break;
    }

    case 3: {
      Expr b = *stack[--stack_ptr];
      Expr a = *stack[--stack_ptr];
      Expr c = *stack[--stack_ptr];
      Expr xp1 = vc_bvPlusExpr(vc, 32, a, b);
      Expr eq = vc_eqExpr(vc, xp1, c);
      Expr eq2 = vc_notExpr(vc, eq);
      int ret = vc_query(vc, eq2);
      assert(ret == false);
      vc_printCounterExample(vc);
      break;
    }

    case 4: {
      Expr *c = create_expr();
      c = vc_trueExpr(vc);
      stack[stack_ptr++] = c;
      break;
    }

    case 5: {
      return 0;
    }
    }
    i += 1;
  }
}

void stp_func(void) {
  VC vc = vc_createValidityChecker();
  Expr c = vc_varExpr(vc, "c", vc_bvType(vc, 32));
  Expr a = vc_bvConstExprFromInt(vc, 32, 5);
  Expr b = vc_bvXorExpr(vc, c, a);

  int res = vc_query(vc, b);
}

void stp_func_bool(void) {
  VC vc = vc_createValidityChecker();

  Expr *c = create_expr();
  *c = vc_falseExpr(vc);
  int ret = vc_query(vc, *c);
  printf("false %d\n", ret);

  Expr *d = create_expr();
  *d = vc_trueExpr(vc);
  ret = vc_query(vc, *d);
  printf("true %d\n", ret);
}

int main(void) {
  run();
  run_malloc();
  stp_func_bool();
  //stp_func();
}
