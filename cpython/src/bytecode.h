#ifndef _BYTECODE_H
#define _BYTECODE_H

#include <stddef.h>
#include <stdint.h>

typedef struct bytecode {
  // the number of positional arguments.
  uint16_t *co_argcount;
  // tuple containing the names of local variables that are referenced
  // by nested functions.
  uint16_t *co_cellvars;
  // a string representing the sequence of bytecode instructions;
  // each instruction is two 8bit values, the first being the instruction
  // and the next being the argument.
  uint8_t *co_code;
  // a tuple containing the literals used.
  // TODO: Maybe a union that holds 32bits or a pointer to a string?
  uint32_t *co_consts;
  // filename of the compiled code.
  uint8_t *co_filename;
  // first line number of the function.
  uint16_t *co_firstlineno;
  // integer encoding a number of flags for the interpreter.
  uint16_t *co_flags;
  // tuple containing the names of free variables.
  uint16_t *co_freevars;
  // TODO: couldn't find documentation for this one.
  uint16_t *co_kwonlyargcount;
  // maps line numbers to bytecode instructions.
  uint16_t *co_lnotab;
  // function name, recorded as a string.
  uint8_t  *co_name;
  // tuple containing the names of the variables.
  uint16_t *co_names;
  // the number of local variables used (including arguments).
  uint16_t *co_nlocals;
  // the required stack size.
  // TODO: Do we care about this, or do we just use a default stack size.
  uint16_t *co_stacksize;
  // a tuple containing the names of the local variables
  // TODO: type, **uint8_t ? where the final pointer is a string?
  uint8_t **co_varnames;
} bytecode;

int parse_bytecode(bytecode *bt, uint8_t *in, size_t len);

#endif // _BYTECODE_H
