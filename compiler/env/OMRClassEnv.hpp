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

#ifndef OMR_CLASSENV_INCL
#define OMR_CLASSENV_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_CLASSENV_CONNECTOR
#define OMR_CLASSENV_CONNECTOR
namespace OMR { class ClassEnv; }
namespace OMR { typedef OMR::ClassEnv ClassEnvConnector; }
#endif

#include <stdint.h>        // for int32_t, int64_t, uint32_t
#include "infra/Annotations.hpp"
#include "env/jittypes.h"

struct OMR_VMThread;
namespace TR { class Compilation; }
namespace TR { class SymbolReference; }
namespace OMR { class ResolvedMethod; }
class TR_Memory;

namespace OMR
{

class OMR_EXTENSIBLE ClassEnv
   {
public:

   // Are classes allocated on the object heap?
   //
   bool classesOnHeap() { return false; }

   // Can class objects be collected by a garbage collector?
   //
   bool classObjectsMayBeCollected() { return true; }

   // Depth of a class from the base class in a hierarchy.
   //
   uintptrj_t classDepthOf(TR_OpaqueClassBlock *clazzPointer) { return 0; }

   // Is specified class a string class?
   //
   bool isStringClass(TR_OpaqueClassBlock *clazz) { return false; }

   // Is specified object a string class?
   //
   bool isStringClass(uintptrj_t objectPointer) { return false; }

   // Class of specified object address
   //
   TR_OpaqueClassBlock *classOfObject(OMR_VMThread *vmThread, uintptrj_t objectPointer) { return NULL; }
   TR_OpaqueClassBlock *classOfObject(TR::Compilation *comp, uintptrj_t objectPointer) { return NULL; }

   bool isAbstractClass(TR::Compilation *comp, TR_OpaqueClassBlock *clazzPointer) { return false; }
   bool isInterfaceClass(TR::Compilation *comp, TR_OpaqueClassBlock *clazzPointer) { return false; }
   bool isPrimitiveClass(TR::Compilation *comp, TR_OpaqueClassBlock *clazz) { return false; }
   bool isAnonymousClass(TR::Compilation *comp, TR_OpaqueClassBlock *clazz) { return false; }
   bool isPrimitiveArray(TR::Compilation *comp, TR_OpaqueClassBlock *) { return false; }
   bool isReferenceArray(TR::Compilation *comp, TR_OpaqueClassBlock *) { return false; }
   bool isClassArray(TR::Compilation *comp, TR_OpaqueClassBlock *) { return false; }
   bool isClassFinal(TR::Compilation *comp, TR_OpaqueClassBlock *) { return false; }
   bool hasFinalizer(TR::Compilation *comp, TR_OpaqueClassBlock *classPointer) { return false; }
   bool isClassInitialized(TR::Compilation *comp, TR_OpaqueClassBlock *) { return false; }
   bool hasFinalFieldsInClass(TR::Compilation *comp, TR_OpaqueClassBlock *classPointer) { return false; }
   bool sameClassLoaders(TR::Compilation *comp, TR_OpaqueClassBlock *, TR_OpaqueClassBlock *) { return false; }
   bool isString(TR::Compilation *comp, TR_OpaqueClassBlock *clazz) { return false; }
   bool isString(TR::Compilation *comp, uintptrj_t objectPointer) { return false; }
   bool jitStaticsAreSame(TR::Compilation *comp, OMR::ResolvedMethod * method1, int32_t cpIndex1, OMR::ResolvedMethod * method2, int32_t cpIndex2) { return false; }
   bool jitFieldsAreSame(TR::Compilation *comp, OMR::ResolvedMethod * method1, int32_t cpIndex1, OMR::ResolvedMethod * method2, int32_t cpIndex2, int32_t isStatic) { return false; }

   uintptrj_t persistentClassPointerFromClassPointer(TR::Compilation *comp, TR_OpaqueClassBlock *clazz) { return 0; }
   TR_OpaqueClassBlock *objectClass(TR::Compilation *comp, uintptrj_t objectPointer) { return NULL; }
   TR_OpaqueClassBlock *classFromJavaLangClass(TR::Compilation *comp, uintptrj_t objectPointer) { return NULL; }

   // 0 <= index < getStringLength
   uint16_t getStringCharacter(TR::Compilation *comp, uintptrj_t objectPointer, int32_t index) { return 0; }
   bool getStringFieldByName(TR::Compilation *, TR::SymbolReference *stringRef, TR::SymbolReference *fieldRef, void* &pResult) { return false; }

   int32_t vTableSlot(TR::Compilation *comp, TR_OpaqueMethodBlock *, TR_OpaqueClassBlock *) { return 0; }
   int32_t flagValueForPrimitiveTypeCheck(TR::Compilation *comp) { return 0; }
   int32_t flagValueForArrayCheck(TR::Compilation *comp) { return 0; }
   int32_t flagValueForFinalizerCheck(TR::Compilation *comp) { return 0; }

   char *classNameChars(TR::Compilation *, TR::SymbolReference *symRef, int32_t & length);
   char *classNameChars(TR::Compilation *, TR_OpaqueClassBlock * clazz, int32_t & length) { return NULL; }

   char *classSignature_DEPRECATED(TR::Compilation *comp, TR_OpaqueClassBlock * clazz, int32_t & length, TR_Memory *) { return NULL; }
   char *classSignature(TR::Compilation *comp, TR_OpaqueClassBlock * clazz, TR_Memory *) { return NULL; }

   };

}

#endif
