/******************************************************
 * Toka
 *
 *|F|
 *|F| FILE: ffi.c
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

#include "toka.h"

#ifndef NOFFI
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef UNIX
#include <dlfcn.h>
#endif

#ifdef WIN32
#include <Windows.h>
#endif

extern Inst *heap;
extern long stack[], sp;
extern ENTRY dictionary[];
extern long last;
extern long parser;

typedef long (*Func)();


/******************************************************
 *|F| Variables:
 *|F|   void *library
 *|F|   Pointer to the most recently opened library
 *|F|
 ******************************************************/
void *library;


/******************************************************
 *|F| ffi_invoke()
 *|F| Call a foreign function. This translates between
 *|F| Toka and CDECL calling conventions.
 *|F|
 ******************************************************/
void ffi_invoke()
{
  long a, args, arg[32];
  Func xt;
  xt = (Func)TOS; DROP;
  args = TOS; DROP;

  if (args != 0)
  {
    for (a = args; a > 0; a--)
    {
      arg[a-1] = TOS; DROP;
    }
  }

  a = 0;
  switch(args)
  {
    case  0: a = ((xt)()); break;
    case  1: a = ((xt)(arg[0])); break;
    case  2: a = ((xt)(arg[0], arg[1])); break;
    case  3: a = ((xt)(arg[0], arg[1], arg[2])); break;
    case  4: a = ((xt)(arg[0], arg[1], arg[2], arg[3])); break;
    case  5: a = ((xt)(arg[0], arg[1], arg[2], arg[3], arg[4])); break;
    case  6: a = ((xt)(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5])); break;
    case  7: a = ((xt)(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6])); break;
    case  8: a = ((xt)(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7])); break;
    case  9: a = ((xt)(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8])); break;
    case 10: a = ((xt)(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9])); break;
    case 11: a = ((xt)(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10])); break;
    case 12: a = ((xt)(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10], arg[11])); break;
    default: error(ERROR_FFI_ARGUMENTS); break;
  }
  push(a);
}


/******************************************************
 *|G| from     ( "- )      Set the library to import from
 *|G|          ( $- )      Non-parsing form
 *
 *|F| ffi_from()
 *|F| Select a library to load from.
 *|F|
 ******************************************************/
void ffi_from()
{
  char *scratch;

  if (PARSING)
  {
    scratch = gc_alloc(256, sizeof(char), GC_TEMP);
    get_token(scratch, 32); DROP;
  }
  else
  {
    scratch = (char *)TOS; DROP;
  }

#ifdef UNIX
  library = dlopen(scratch, RTLD_LAZY);
#endif

#ifdef WIN32
  library = LoadLibrary(scratch);
#endif

  if (library == NULL)
  {
    push((long)scratch);
    error(ERROR_FFI_LIBRARY_NOT_FOUND);
  }
}


/******************************************************
 *|G| import   ( n"- )     Import a function taking 'n'
 *|G|                      arguments.
 *|G|          ( n$- )     Non-parsing form
 *
 *|F| ffi_import()
 *|F| Import and name an external function. This wraps
 *|F| the imported function in a quote.
 *|F|
 ******************************************************/
void ffi_import()
{
  long args, xt;
  char *scratch;

  if (PARSING)
  {
    scratch = gc_alloc(256, sizeof(char), GC_TEMP);
    get_token(scratch, 32); DROP;
  }
  else
  {
    scratch = (char *)TOS; DROP;
  }

  args = TOS; DROP;

#ifdef UNIX
  xt = (long)dlsym(library, scratch);
#endif

#ifdef WIN32
  xt = (long)GetProcAddress(library, scratch);
#endif

  if (xt != (long)NULL)
  {
    begin_quote();
      if (args != -1)
      {
        push(args);
        make_literal();
      }
      push(xt);
      make_literal();
      *heap++ = &ffi_invoke;
    end_quote();
    gc_keep();
    add_entry(scratch, (void *)TOS, &quote_word_class);
    DROP;
  }
  else
  {
    push((long)scratch);
    error(ERROR_FFI_FUNCTION_NOT_FOUND);
  }
}


/******************************************************
 *|G| as       ( "- )      Rename the last defined word
 *|G|          ( $-  )     Non-parsing form
 *
 *|F| ffi_rename()
 *|F| Rename the most recently defined word in the
 *|F| dictionary.
 *|F|
 ******************************************************/
void ffi_rename()
{
  char *name;
  if (PARSING)
  {
    name = gc_alloc(256, sizeof(char), GC_TEMP);
    get_token(name, 32); DROP;
  }
  else
  {
    name = (char *)TOS; DROP;
  }
  strcpy(dictionary[last].name, name);
}
#endif
