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


#ifndef LINKEDLIST_INCL
#define LINKEDLIST_INCL

#include "ilgen/MethodBuilder.hpp"

namespace TR { class TypeDictionary; }

typedef struct Element
   {
   struct Element *next;
   int key;
   int val;
   } Element;

typedef int32_t (LinkedListFunctionType)(Element *, int32_t);

class LinkedListMethod : public TR::MethodBuilder
   {
   private:
   TR::IlReference *_keyRef;
   TR::IlReference *_valRef;
   TR::IlReference *_nextRef;

   public:
   LinkedListMethod(TR::TypeDictionary *);
   virtual bool buildIL();
   };

#endif // !defined(LINKEDLIST_INCL)
