/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
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

#include "compile/ResolvedMethod.hpp"

#include "compile/Method.hpp"

TR::RecognizedMethod TR_ResolvedMethod::getRecognizedMethod() {
  return convertToMethod()->getRecognizedMethod();
}

uintptrj_t *TR_ResolvedMethod::getMethodHandleLocation() {
  TR_ASSERT(convertToMethod()->isArchetypeSpecimen(),
            "All methods associated with a MethodHandle must be archetype "
            "specimens");
  return NULL;
}

TR_MethodParameterIterator *
TR_ResolvedMethod::getParameterIterator(TR::Compilation &comp) {
  return convertToMethod()->getParameterIterator(comp, this);
}

bool TR_ResolvedMethod::isJ9() { return convertToMethod()->isJ9(); }

bool TR_ResolvedMethod::isPython() { return convertToMethod()->isPython(); }

bool TR_ResolvedMethod::isRuby() { return convertToMethod()->isRuby(); }
