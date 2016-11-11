/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/

#include <dlfcn.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Jit.hpp"
#include "Pow2.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/TypeDictionary.hpp"

Pow2Method::Pow2Method(TR::TypeDictionary *types) : MethodBuilder(types) {
  DefineLine(LINETOSTR(__LINE__));
  DefineFile(__FILE__);

  DefineName("pow2");
  DefineParameter("n", Int64);
  DefineReturnType(Int64);
}

bool Pow2Method::buildIL() {
  Store("a", ConstInt64(1));

  Store("b", ConstInt64(1));

  Store("i", Load("n"));

  Store("keepIterating", GreaterThan(Load("i"), ConstInt64(-1)));

  TR::IlBuilder *loopBody = NULL;
  WhileDoLoop("keepIterating", &loopBody);

  loopBody->Store("a", loopBody->Load("b"));

  loopBody->Store("b", loopBody->Add(loopBody->Load("a"), loopBody->Load("b")));

  loopBody->Store("i",
                  loopBody->Sub(loopBody->Load("i"), loopBody->ConstInt64(1)));

  loopBody->Store(
      "keepIterating",
      loopBody->GreaterThan(loopBody->Load("i"), loopBody->ConstInt64(-1)));

  Return(Load("a"));

  return true;
}

int main(int argc, char *argv[]) {
  printf("Step 1: initialize JIT\n");
  bool initialized = initializeJit();
  if (!initialized) {
    fprintf(stderr, "FAIL: could not initialize JIT\n");
    exit(-1);
  }

  printf("Step 2: define relevant types\n");
  TR::TypeDictionary types;

  printf("Step 3: compile method builder\n");
  Pow2Method Pow2Method(&types);
  uint8_t *entry = 0;
  int32_t rc = compileMethodBuilder(&Pow2Method, &entry);
  if (rc != 0) {
    fprintf(stderr, "FAIL: compilation error %d\n", rc);
    exit(-2);
  }

  printf("Step 4: invoke compiled code\n");
  Pow2FunctionType *pow2 = (Pow2FunctionType *)entry;
  int32_t n = (argc > 1) ? atoi(argv[1]) : 6000000;
  int64_t r;
  for (int32_t i = 0; i < n; i++)
    r = pow2((int64_t)45);

  printf("pow2(45) is %ld\n", r);

  printf("Step 5: shutdown JIT\n");
  shutdownJit();

  printf("PASS\n");
}
