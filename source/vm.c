/******************************************************
 * Toka
 *
 *|F|
 *|F| FILE: vm.c
 *|F| 
 *
 * Copyright (c) 2006, 2007  Charles R. Childers
 *
 * Permission to use, copy, modify, and distribute this 
 * software for any purpose with or without fee is hereby 
 * granted, provided that the above copyright notice and 
 * this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR 
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE 
 * FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL 
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF 
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING 
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 ******************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "toka.h"

/******************************************************
 *|F| Variables:
 *|F|   Inst *heap
 *|F|   Pointer into the current heap
 *|F|
 *|F|   Inst *ip
 *|F|   The instruction pointer
 *|F|
 *|F|   long stack[MAX_DATA_STACK], rstack[MAX_RETURN_STACK]
 *|F|   The data and return stacks
 *|F|
 *|F|   long sp, rsp
 *|F|   The stack pointers
 *|F|
 ******************************************************/
Inst *heap, *ip;
long stack[MAX_DATA_STACK], rstack[MAX_RETURN_STACK];
long sp = 0, rsp = 0;


/******************************************************
 *|F| vm_run(Inst)
 *|F| Run through a list of instructions
 *|F| Side effects:
 *|F|   modifes *ip
 *|F|
 ******************************************************/
void vm_run(Inst prog[])
{
  ip = prog;
  while (*ip != 0)
  {
    vm_stack_check();
    ((*ip++)());
  }
}


/******************************************************
 *|F| vm_stack_check()
 *|F| Check for over/underflow and reset if detected
 *|F| If the return stack over/underflows, exit Toka
 *|F| 
 ******************************************************/
void vm_stack_check()
{
  if ((sp < 0 || sp > MAX_DATA_STACK) || (rsp < 0 || rsp > MAX_RETURN_STACK))
    error(ERROR_STACK);
}


/******************************************************
 *|F| push(long a)
 *|F| Push a number to the stack.
 *|F|
 ******************************************************/
void push(long a)
{
  sp++; TOS = a;
}


/******************************************************
 *|F| lit()
 *|F| Push the value in the following memory location
 *|F| to the stack
 *|F|
 ******************************************************/
void lit()
{
  sp++;
  TOS = (long)*ip;
  ip++;
}




/******************************************************
 *|F| string_lit()
 *|F| Push the pointer in the following memory location
 *|F| to the stack. This is a helper function for 
 *|F| strings.
 *|F|
 ******************************************************/
void string_lit()
{
  sp++;
  TOS = (long)*ip;
  ip++;
}
