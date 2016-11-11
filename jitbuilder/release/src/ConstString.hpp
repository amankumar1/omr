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

#ifndef CONSTSTRING_INCL
#define CONSTSTRING_INCL

#include "ilgen/MethodBuilder.hpp"

namespace TR {
class TypeDictionary;
}

typedef void(ConstStringFunctionType)();

class ConstStringMethod : public TR::MethodBuilder {
public:
  ConstStringMethod(TR::TypeDictionary *);
  virtual bool buildIL();
};

#endif // !defined(CONSTSTRING_INCL)
