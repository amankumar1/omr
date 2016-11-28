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

#include "compile/Method.hpp"

#include <string.h>                            // for memchr
#include "compile/Compilation.hpp"             // for Compilation
#include "compile/ResolvedMethod.hpp"          // for OMR::ResolvedMethod
#include "compile/SymbolReferenceTable.hpp"    // for SymbolReferenceTable
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "env/jittypes.h"                      // for intptrj_t, uintptrj_t
#include "il/SymbolReference.hpp"              // for classNameToSignature
#include "il/symbol/ParameterSymbol.hpp"       // for ParameterSymbol
#include "il/symbol/ResolvedMethodSymbol.hpp"  // for ResolvedMethodSymbol
#include "infra/List.hpp"                      // for ListAppender
#include "runtime/Runtime.hpp"                 // for TR_RuntimeHelper

class TR_FrontEnd;
class TR_OpaqueMethodBlock;
class TR_PrexArgInfo;
namespace TR { class IlGeneratorMethodDetails; }
namespace TR { class LabelSymbol; }

bool OMR::ResolvedMethod::isDAAMethod()
   {
   return OMR::ResolvedMethod::isDAAWrapperMethod() || OMR::ResolvedMethod::isDAAIntrinsicMethod();
   }

bool OMR::ResolvedMethod::isDAAWrapperMethod()
   {
#ifdef J9_PROJECT_SPECIFIC
   if (getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeShort        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeShortLength  ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeInt          ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeIntLength    ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeLong         ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeLongLength   ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeFloat        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeDouble       ||

       // ByteArray Unmarshalling methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readShort       ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readShortLength ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readInt         ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readIntLength   ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readLong        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readLongLength  ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readFloat       ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readDouble      ||

       // Byte array utility methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUtils_trailingZeros          ||

       // DAA Packed Decimal arithmetic methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_addPackedDecimal        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_subtractPackedDecimal   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_multiplyPackedDecimal   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_dividePackedDecimal     ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_remainderPackedDecimal  ||

       // DAA Packed Decimal comparison methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_lessThanPackedDecimal            ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_lessThanOrEqualsPackedDecimal    ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_greaterThanPackedDecimal         ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_greaterThanOrEqualsPackedDecimal ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_equalsPackedDecimal     ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_notEqualsPackedDecimal  ||

       // DAA Packed Decimal shift methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_shiftLeftPackedDecimal  ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_shiftRightPackedDecimal ||

       // DAA Packed Decimal check methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_checkPackedDecimal            ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_checkPackedDecimal_2bInlined1 ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_checkPackedDecimal_2bInlined2 ||

       // DAA Packed Decimal move method
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_movePackedDecimal             ||

       // DAA Packed Decimal <-> Integer
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToInteger   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToInteger_ByteBuffer ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertIntegerToPackedDecimal   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertIntegerToPackedDecimal_ByteBuffer ||

       // DAA Packed Decimal <-> Long
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToLong      ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToLong_ByteBuffer      ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertLongToPackedDecimal      ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertLongToPackedDecimal_ByteBuffer      ||

       // DAA External Decimal <-> Integer
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertExternalDecimalToInteger ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertIntegerToExternalDecimal ||

       // DAA External Decimal <-> Long
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertExternalDecimalToLong    ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertLongToExternalDecimal    ||

       // DAA Packed Decimal <-> External Decimal
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToExternalDecimal ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertExternalDecimalToPackedDecimal ||

       // DAA Packed Decimal <-> Unicode Decimal
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToUnicodeDecimal  ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertUnicodeDecimalToPackedDecimal  ||

       // DAA Packed Decimal <-> BigInteger
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToBigInteger   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertBigIntegerToPackedDecimal   ||

       // DAA Packed Decimal <-> BigDecimal
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToBigDecimal   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertBigDecimalToPackedDecimal   ||

       // DAA External Decimal <-> BigInteger
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertExternalDecimalToBigInteger ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertBigIntegerToExternalDecimal ||

       // DAA External Decimal <-> BigDecimal
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertExternalDecimalToBigDecimal ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertBigDecimalToExternalDecimal ||

       // DAA Unicode Decimal <-> Integer
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertUnicodeDecimalToInteger ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertIntegerToUnicodeDecimal ||

       // DAA Unicode Decimal <-> Long
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertUnicodeDecimalToLong        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertLongToUnicodeDecimal        ||

       // DAA Unicode Decimal <-> BigInteger
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertUnicodeDecimalToBigInteger  ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertBigIntegerToUnicodeDecimal  ||

       // DAA Unicode Decimal <-> BigDecimal
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertUnicodeDecimalToBigDecimal  ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertBigDecimalToUnicodeDecimal  ||


       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_slowSignedPackedToBigDecimal ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_slowBigDecimalToSignedPacked)
      {
      return true;
      }
#endif
   return false;
   }

bool OMR::ResolvedMethod::isDAAIntrinsicMethod()
   {
#ifdef J9_PROJECT_SPECIFIC
   if (getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeShort_        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeShortLength_  ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeInt_          ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeIntLength_    ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeLong_         ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeLongLength_   ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeFloat_        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayMarshaller_writeDouble_       ||

       // ByteArray Unmarshalling methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readShort_       ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readShortLength_ ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readInt_         ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readIntLength_   ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readLong_        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readLongLength_  ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readFloat_       ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUnmarshaller_readDouble_      ||

       // Byte array utility methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_ByteArrayUtils_trailingZerosQuadWordAtATime_   ||

       // DAA Packed Decimal arithmetic methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_addPackedDecimal_        ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_subtractPackedDecimal_   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_multiplyPackedDecimal_   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_dividePackedDecimal_     ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_remainderPackedDecimal_  ||

       // DAA Packed Decimal comparison methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_lessThanPackedDecimal_            ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_lessThanOrEqualsPackedDecimal_    ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_greaterThanPackedDecimal_         ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_greaterThanOrEqualsPackedDecimal_ ||

       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_equalsPackedDecimal_     ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_notEqualsPackedDecimal_  ||

       // DAA Packed Decimal shift methods
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_shiftLeftPackedDecimal_  ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_shiftRightPackedDecimal_ ||

       // DAA Packed Decimal check method
       getRecognizedMethod() == TR::com_ibm_dataaccess_PackedDecimal_checkPackedDecimal_ ||

       // DAA Packed Decimal <-> Integer
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToInteger_   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToInteger_ByteBuffer_ ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertIntegerToPackedDecimal_   ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertIntegerToPackedDecimal_ByteBuffer_ ||

       // DAA Packed Decimal <-> Long
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToLong_      ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToLong_ByteBuffer_      ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertLongToPackedDecimal_      ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertLongToPackedDecimal_ByteBuffer_      ||

       // DAA Packed Decimal <-> External Decimal
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertExternalDecimalToPackedDecimal_ ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToExternalDecimal_ ||

       // DAA Packed Decimal <-> Unicode Decimal
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToUnicodeDecimal_  ||
       getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertUnicodeDecimalToPackedDecimal_)
      {
      return true;
      }

#endif
   return false;
   }

#define notImplemented(A) TR_ASSERT(0, "OMR::Method::%s is undefined", (A) )

uint32_t              OMR::Method::numberOfExplicitParameters() { notImplemented("numberOfExplicitParameters"); return 0; }
TR::DataType        OMR::Method::parmType(uint32_t)           { notImplemented("parmType"); return TR::NoType; }
TR::ILOpCodes          OMR::Method::directCallOpCode()           { notImplemented("directCallOpCode"); return TR::BadILOp; }
TR::ILOpCodes          OMR::Method::indirectCallOpCode()         { notImplemented("indirectCallOpCode"); return TR::BadILOp; }
TR::DataType        OMR::Method::returnType()                 { notImplemented("returnType"); return TR::NoType; }
bool                  OMR::Method::returnTypeIsUnsigned()       { notImplemented("returnTypeIsUnsigned"); return TR::NoType;}
uint32_t              OMR::Method::returnTypeWidth()            { notImplemented("returnTypeWidth"); return 0; }
TR::ILOpCodes          OMR::Method::returnOpCode()               { notImplemented("returnOpCode"); return TR::BadILOp; }
uint16_t              OMR::Method::classNameLength()            { notImplemented("classNameLength"); return 0; }
uint16_t              OMR::Method::nameLength()                 { notImplemented("nameLength"); return 0; }
uint16_t              OMR::Method::signatureLength()            { notImplemented("signatureLength"); return 0; }
char *                OMR::Method::classNameChars()             { notImplemented("classNameChars"); return 0; }
char *                OMR::Method::nameChars()                  { notImplemented("nameChars"); return 0; }
char *                OMR::Method::signatureChars()             { notImplemented("signatureChars"); return 0; }
bool                  OMR::Method::isConstructor()              { notImplemented("isConstructor"); return false; }
bool                  OMR::Method::isFinalInObject()            { notImplemented("isFinalInObject"); return false; }
const char *          OMR::Method::signature(TR_Memory *, TR_AllocationKind) { notImplemented("signature"); return 0; }
void                  OMR::Method::setArchetypeSpecimen(bool b)            { notImplemented("setArchetypeSpecimen"); }

TR_MethodParameterIterator *
OMR::Method::getParameterIterator(TR::Compilation&, OMR::ResolvedMethod *)
   {
   notImplemented("getParameterIterator");
   return 0;
   }

bool
OMR::Method::isBigDecimalMethod(TR::Compilation * comp)
   {
   notImplemented("isBigDecimalMethod");
   return false;
   }

bool
OMR::Method::isUnsafeCAS(TR::Compilation * comp)
   {
   notImplemented("isUnsafeCAS");
   return false;
   }

bool
OMR::Method::isUnsafeWithObjectArg(TR::Compilation * comp)
   {
   notImplemented("isUnsafeWithObjectArg");
   return false;
   }

bool
OMR::Method::isBigDecimalConvertersMethod(TR::Compilation * comp)
   {
   notImplemented("isBigDecimalConvertersMethod");
   return false;
   }

#undef notImplemented
#define notImplemented(A) TR_ASSERT(0, "OMR::ResolvedMethod::%s is undefined for %p", (A), this )

OMR::Method *  OMR::ResolvedMethod::convertToMethod()                          { notImplemented("convertToMethod"); return 0; }
uint32_t     OMR::ResolvedMethod::numberOfParameters()                       { notImplemented("numberOfParameters"); return 0; }
uint32_t     OMR::ResolvedMethod::numberOfExplicitParameters()               { notImplemented("numberOfExplicitParameters"); return 0; }
TR::DataType OMR::ResolvedMethod::parmType(uint32_t)                         { notImplemented("parmType"); return TR::NoType; }
TR::ILOpCodes OMR::ResolvedMethod::directCallOpCode()                         { notImplemented("directCallOpCode"); return TR::BadILOp; }
TR::ILOpCodes OMR::ResolvedMethod::indirectCallOpCode()                       { notImplemented("indirectCallOpCode"); return TR::BadILOp; }
TR::DataType OMR::ResolvedMethod::returnType()                               { notImplemented("returnType"); return TR::NoType; }
uint32_t     OMR::ResolvedMethod::returnTypeWidth()                          { notImplemented("returnTypeWidth"); return 0; }
bool         OMR::ResolvedMethod::returnTypeIsUnsigned()                     { notImplemented("returnTypeIsUnsigned"); return 0; }
TR::ILOpCodes OMR::ResolvedMethod::returnOpCode()                             { notImplemented("returnOpCode"); return TR::BadILOp; }
uint16_t     OMR::ResolvedMethod::classNameLength()                          { notImplemented("classNameLength"); return 0; }
uint16_t     OMR::ResolvedMethod::nameLength()                               { notImplemented("nameLength"); return 0; }
uint16_t     OMR::ResolvedMethod::signatureLength()                          { notImplemented("signatureLength"); return 0; }
char *       OMR::ResolvedMethod::classNameChars()                           { notImplemented("classNameChars"); return 0; }
char *       OMR::ResolvedMethod::nameChars()                                { notImplemented("nameChars"); return 0; }
char *       OMR::ResolvedMethod::signatureChars()                           { notImplemented("signatureChars"); return 0; }
bool         OMR::ResolvedMethod::isConstructor()                            { notImplemented("isConstructor"); return false; }
bool         OMR::ResolvedMethod::isStatic()                                 { notImplemented("isStatic"); return false; }
bool         OMR::ResolvedMethod::isAbstract()                               { notImplemented("isAbstract"); return false; }
bool         OMR::ResolvedMethod::isCompilable(TR_Memory *)                  { notImplemented("isCompilable"); return false; }
bool         OMR::ResolvedMethod::isInlineable(TR::Compilation *)             { notImplemented("isInlineable"); return false; }
bool         OMR::ResolvedMethod::isNative()                                 { notImplemented("isNative"); return false; }
bool         OMR::ResolvedMethod::isSynchronized()                           { notImplemented("isSynchronized"); return false; }
bool         OMR::ResolvedMethod::isPrivate()                                { notImplemented("isPrivate"); return false; }
bool         OMR::ResolvedMethod::isProtected()                              { notImplemented("isProtected"); return false; }
bool         OMR::ResolvedMethod::isPublic()                                 { notImplemented("isPublic"); return false; }
bool         OMR::ResolvedMethod::isFinal()                                  { notImplemented("isFinal"); return false; }
bool         OMR::ResolvedMethod::isDebugable()                              { notImplemented("isFinal"); return false; }
bool         OMR::ResolvedMethod::isStrictFP()                               { notImplemented("isStrictFP"); return false; }
bool         OMR::ResolvedMethod::isInterpreted()                            { notImplemented("isInterpreted"); return false; }
bool         OMR::ResolvedMethod::hasBackwardBranches()                      { notImplemented("hasBackwardBranches"); return false; }
bool         OMR::ResolvedMethod::isObjectConstructor()                      { notImplemented("isObjectConstructor"); return false; }
bool         OMR::ResolvedMethod::isNonEmptyObjectConstructor()              { notImplemented("isNonEmptyObjectConstructor"); return false; }
bool         OMR::ResolvedMethod::isCold(TR::Compilation *, bool, TR::ResolvedMethodSymbol * /* = NULL */)             { notImplemented("isCold"); return false; }
bool         OMR::ResolvedMethod::isSubjectToPhaseChange(TR::Compilation *) { notImplemented("isSubjectToPhaseChange"); return false; }
bool         OMR::ResolvedMethod::isSameMethod(OMR::ResolvedMethod *)          { notImplemented("isSameMethod"); return false; }
bool         OMR::ResolvedMethod::isNewInstanceImplThunk()                   { notImplemented("isNewInstanceImplThunk"); return false; }
bool         OMR::ResolvedMethod::isJNINative()                              { notImplemented("isJNINative"); return false; }
bool         OMR::ResolvedMethod::isJITInternalNative()                      { notImplemented("isJITInternalNative"); return false; }
//bool         OMR::ResolvedMethod::isUnsafeWithObjectArg(TR::Compilation *)    { notImplemented("isUnsafeWithObjectArg"); return false; }
void *       OMR::ResolvedMethod::resolvedMethodAddress()                    { notImplemented("resolvedMethodAddress"); return 0; }
void *       OMR::ResolvedMethod::startAddressForJittedMethod()              { notImplemented("startAddressForJittedMethod"); return 0; }
void *       OMR::ResolvedMethod::startAddressForJNIMethod(TR::Compilation *) { notImplemented("startAddressForJNIMethod"); return 0; }
void *       OMR::ResolvedMethod::startAddressForJITInternalNativeMethod()   { notImplemented("startAddressForJITInternalNativeMethod"); return 0; }
void *       OMR::ResolvedMethod::startAddressForInterpreterOfJittedMethod() { notImplemented("startAddressForInterpreterOfJittedMethod"); return 0; }
bool         OMR::ResolvedMethod::isWarmCallGraphTooBig(uint32_t bcIndex, TR::Compilation *) { notImplemented("isWarmCallGraphTooBig"); return 0; }
void         OMR::ResolvedMethod::setWarmCallGraphTooBig(uint32_t bcIndex, TR::Compilation *){ notImplemented("setWarmCallGraphTooBig"); return; }

TR_FrontEnd *OMR::ResolvedMethod::fe()                                       { notImplemented("fe"); return 0; }
intptrj_t    OMR::ResolvedMethod::getInvocationCount()                       { notImplemented("getInvocationCount"); return 0; }
bool         OMR::ResolvedMethod::setInvocationCount(intptrj_t, intptrj_t)   { notImplemented("setInvocationCount"); return false; }
uint16_t     OMR::ResolvedMethod::numberOfParameterSlots()                   { notImplemented("numberOfParameterSlots"); return 0; }
uint16_t     OMR::ResolvedMethod::archetypeArgPlaceholderSlot(TR_Memory *)   { notImplemented("numberOfParameterSlots"); return 0; }
uint16_t     OMR::ResolvedMethod::numberOfTemps()                            { notImplemented("numberOfTemps"); return 0; }
uint16_t     OMR::ResolvedMethod::numberOfPendingPushes()                    { notImplemented("numberOfPendingPushes"); return 0; }
uint8_t *    OMR::ResolvedMethod::bytecodeStart()                            { notImplemented("bytecodeStart"); return 0; }
uint32_t     OMR::ResolvedMethod::maxBytecodeIndex()                         { notImplemented("maxBytecodeIndex"); return 0; }
void *       OMR::ResolvedMethod::ramConstantPool()                          { notImplemented("ramConstantPool"); return 0; }
void *       OMR::ResolvedMethod::constantPool()                             { notImplemented("constantPool"); return 0; }

TR::DataType OMR::ResolvedMethod::getLDCType(int32_t)                        { notImplemented("getLDCType"); return TR::NoType; }
bool         OMR::ResolvedMethod::isClassConstant(int32_t cpIndex)           { notImplemented("isClassConstant"); return false; }
bool         OMR::ResolvedMethod::isStringConstant(int32_t cpIndex)          { notImplemented("isStringConstant"); return false; }
bool         OMR::ResolvedMethod::isMethodTypeConstant(int32_t cpIndex)      { notImplemented("isMethodTypeConstant"); return false; }
bool         OMR::ResolvedMethod::isMethodHandleConstant(int32_t cpIndex)    { notImplemented("isMethodHandleConstant"); return false; }
uint32_t     OMR::ResolvedMethod::intConstant(int32_t)                       { notImplemented("intConstant"); return 0; }
uint64_t     OMR::ResolvedMethod::longConstant(int32_t)                      { notImplemented("longConstant"); return 0; }
float *      OMR::ResolvedMethod::floatConstant(int32_t)                     { notImplemented("floatConstant"); return 0; }
double *     OMR::ResolvedMethod::doubleConstant(int32_t, TR_Memory *)       { notImplemented("doubleConstant"); return 0; }
void *       OMR::ResolvedMethod::stringConstant(int32_t)                    { notImplemented("stringConstant"); return 0; }
bool         OMR::ResolvedMethod::isUnresolvedString(int32_t, bool optimizeForAOT)                { notImplemented("isUnresolvedString"); return false; }
void *       OMR::ResolvedMethod::methodTypeConstant(int32_t)                { notImplemented("methodTypeConstant"); return 0; }
bool         OMR::ResolvedMethod::isUnresolvedMethodType(int32_t)            { notImplemented("isUnresolvedMethodType"); return false; }
void *       OMR::ResolvedMethod::methodHandleConstant(int32_t)              { notImplemented("methodHandleConstant"); return 0; }
bool         OMR::ResolvedMethod::isUnresolvedMethodHandle(int32_t)          { notImplemented("isUnresolvedMethodHandle"); return false; }
bool         OMR::ResolvedMethod::isUnresolvedCallSiteTableEntry(int32_t callSiteIndex) { notImplemented("isUnresolvedCallSiteTableEntry"); return false; }
void *       OMR::ResolvedMethod::callSiteTableEntryAddress(int32_t callSiteIndex)      { notImplemented("callSiteTableEntryAddress"); return 0; }
bool         OMR::ResolvedMethod::isUnresolvedMethodTypeTableEntry(int32_t cpIndex) { notImplemented("isUnresolvedMethodTypeTableEntry"); return false; }
void *       OMR::ResolvedMethod::methodTypeTableEntryAddress(int32_t cpIndex)      { notImplemented("methodTypeTableEntryAddress"); return 0; }

TR_OpaqueClassBlock *OMR::ResolvedMethod::getDeclaringClassFromFieldOrStatic(TR::Compilation *comp, int32_t cpIndex)  { notImplemented("getDeclaringClassFromFieldOrStatic"); return 0; }
int32_t      OMR::ResolvedMethod::classCPIndexOfFieldOrStatic(int32_t)       { notImplemented("classCPIndexOfFieldOrStatic"); return 0; }
int32_t      OMR::ResolvedMethod::packedArrayFieldLength(int32_t, TR::Compilation* comp) { notImplemented("packedArrayFieldLength"); return 0; }
const char * OMR::ResolvedMethod::signature(TR_Memory *, TR_AllocationKind)  { notImplemented("signature"); return 0; }
char *       OMR::ResolvedMethod::fieldName (int32_t, TR_Memory *, TR_AllocationKind kind)           { notImplemented("fieldName"); return 0; }
char *       OMR::ResolvedMethod::staticName(int32_t, TR_Memory *, TR_AllocationKind kind)           { notImplemented("staticName"); return 0; }
char *       OMR::ResolvedMethod::localName (uint32_t, uint32_t, TR_Memory *){ /*notImplemented("localName");*/ return 0; }
char *       OMR::ResolvedMethod::fieldName (int32_t, int32_t &, TR_Memory *, TR_AllocationKind kind){ notImplemented("fieldName"); return 0; }
char *       OMR::ResolvedMethod::staticName(int32_t, int32_t &, TR_Memory *, TR_AllocationKind kind){ notImplemented("staticName"); return 0; }
char *       OMR::ResolvedMethod::localName (uint32_t, uint32_t, int32_t&, TR_Memory *){ /*notImplemented("localName");*/ return 0; }
char *       OMR::ResolvedMethod::fieldNameChars(int32_t, int32_t &)         { notImplemented("fieldNameChars"); return 0; }
char *       OMR::ResolvedMethod::fieldSignatureChars(int32_t, int32_t &)    { notImplemented("fieldSignatureChars"); return 0; }
char *       OMR::ResolvedMethod::staticSignatureChars(int32_t, int32_t &)   { notImplemented("staticSignatureChars"); return 0; }
void * &     OMR::ResolvedMethod::addressOfClassOfMethod()                   { notImplemented("addressOfClassOfMethod"); return *(void **)0; }
uint32_t     OMR::ResolvedMethod::vTableSlot(uint32_t)                       { notImplemented("vTableSlot"); return 0; }
bool         OMR::ResolvedMethod::virtualMethodIsOverridden()                { notImplemented("virtualMethodIsOverridden"); return false; }
void         OMR::ResolvedMethod::setVirtualMethodIsOverridden()             { notImplemented("setVirtualMethodIsOverridden"); }
void *       OMR::ResolvedMethod::addressContainingIsOverriddenBit()         { notImplemented("addressContainingIsOverriddenBit"); return 0; }
int32_t     OMR::ResolvedMethod::virtualCallSelector(uint32_t)               { notImplemented("virtualCallSelector"); return 0; }
uint32_t     OMR::ResolvedMethod::numberOfExceptionHandlers()                { notImplemented("numberOfExceptionHandlers"); return 0; }
uint8_t *    OMR::ResolvedMethod::allocateException(uint32_t,TR::Compilation*){ notImplemented("allocateException"); return 0; }

int32_t      OMR::ResolvedMethod::exceptionData(int32_t, int32_t *, int32_t *, int32_t *) { notImplemented("exceptionData"); return 0; }
char *       OMR::ResolvedMethod::getClassNameFromConstantPool(uint32_t, uint32_t &)      { notImplemented("getClassNameFromConstantPool"); return 0; }
bool         OMR::ResolvedMethod::fieldsAreSame(int32_t, OMR::ResolvedMethod *, int32_t, bool &sigSame)    { notImplemented("fieldsAreSame"); return false; }
bool         OMR::ResolvedMethod::staticsAreSame(int32_t, OMR::ResolvedMethod *, int32_t, bool &sigSame)   { notImplemented("staticsAreSame"); return false; }
char *       OMR::ResolvedMethod::classNameOfFieldOrStatic(int32_t, int32_t &)            { notImplemented("classNameOfFieldOrStatic"); return 0; }
char *       OMR::ResolvedMethod::classSignatureOfFieldOrStatic(int32_t, int32_t &)       { notImplemented("classSignatureOfFieldOrStatic"); return 0; }
char *       OMR::ResolvedMethod::staticNameChars(int32_t, int32_t &)                     { notImplemented("staticNameChars"); return 0; }
const char * OMR::ResolvedMethod::newInstancePrototypeSignature(TR_Memory *, TR_AllocationKind)        { notImplemented("newInstancePrototypeSignature"); return 0; }

bool
OMR::ResolvedMethod::getUnresolvedFieldInCP(int32_t)
   {
   notImplemented("getUnresolvedFieldInCP");
   return false;
   }

bool
OMR::ResolvedMethod::getUnresolvedStaticMethodInCP(int32_t)
   {
   notImplemented("getUnresolvedStaticMethodInCP");
   return false;
   }

bool
OMR::ResolvedMethod::getUnresolvedSpecialMethodInCP(int32_t)
   {
   notImplemented("getUnresolvedSpecialMethodInCP");
   return false;
   }

bool
OMR::ResolvedMethod::getUnresolvedVirtualMethodInCP(int32_t)
   {
   notImplemented("getUnresolvedVirtualMethod");
   return false;
   }

bool
OMR::ResolvedMethod::fieldAttributes(TR::Compilation *, int32_t, uint32_t *, TR::DataType *, bool *, bool *, bool *, bool, bool *, bool)
   {
   notImplemented("fieldAttributes");
   return false;
   }

bool
OMR::ResolvedMethod::staticAttributes(TR::Compilation *, int32_t, void * *, TR::DataType *, bool *, bool *, bool *, bool, bool *, bool)
   {
   notImplemented("staticAttributes");
   return false;
   }

TR_OpaqueClassBlock * OMR::ResolvedMethod::containingClass()                                 { notImplemented("containingClass"); return 0; }
TR_OpaqueClassBlock * OMR::ResolvedMethod::getClassFromConstantPool(TR::Compilation *, uint32_t, bool) { notImplemented("getClassFromConstantPool"); return 0; }
TR_OpaqueClassBlock * OMR::ResolvedMethod::classOfStatic(int32_t, bool)                            { notImplemented("classOfStatic"); return 0; }
TR_OpaqueClassBlock * OMR::ResolvedMethod::classOfMethod()                                   { notImplemented("classOfMethod"); return 0; }
uint32_t              OMR::ResolvedMethod::classCPIndexOfMethod(uint32_t)                    { notImplemented("classCpIndexOfMethod"); return 0; }

TR_OpaqueMethodBlock *OMR::ResolvedMethod::getNonPersistentIdentifier()                      { notImplemented("getNonPersistentIdentifier"); return 0; }
TR_OpaqueMethodBlock *OMR::ResolvedMethod::getPersistentIdentifier()                         { notImplemented("getPersistentIdentifier"); return 0; }
TR_OpaqueClassBlock * OMR::ResolvedMethod::getResolvedInterfaceMethod(int32_t, uintptrj_t *) { notImplemented("getResolvedInterfaceMethod"); return 0; }

OMR::ResolvedMethod * OMR::ResolvedMethod::owningMethod()                                      { notImplemented("owningMethod"); return 0; }
void OMR::ResolvedMethod::setOwningMethod(OMR::ResolvedMethod*)                                { notImplemented("setOwningMethod");  }

OMR::ResolvedMethod * OMR::ResolvedMethod::getResolvedStaticMethod (TR::Compilation *, int32_t, bool *)           { notImplemented("getResolvedStaticMethod"); return 0; }
OMR::ResolvedMethod * OMR::ResolvedMethod::getResolvedSpecialMethod(TR::Compilation *, int32_t, bool *)           { notImplemented("getResolvedSpecialMethod"); return 0; }
OMR::ResolvedMethod * OMR::ResolvedMethod::getResolvedVirtualMethod(TR::Compilation *, int32_t, bool, bool *)     { notImplemented("getResolvedVirtualMethod"); return 0; }
OMR::ResolvedMethod * OMR::ResolvedMethod::getResolvedDynamicMethod(TR::Compilation *, int32_t, bool *)           { notImplemented("getResolvedDynamicMethod"); return 0; }
OMR::ResolvedMethod * OMR::ResolvedMethod::getResolvedHandleMethod (TR::Compilation *, int32_t, bool *)           { notImplemented("getResolvedHandleMethod"); return 0; }
OMR::ResolvedMethod * OMR::ResolvedMethod::getResolvedHandleMethodWithSignature(TR::Compilation *, int32_t, char *){notImplemented("getResolvedHandleMethodWithSignature"); return 0; }

uint32_t
OMR::ResolvedMethod::getResolvedInterfaceMethodOffset(TR_OpaqueClassBlock *, int32_t)
   {
   notImplemented("getResolvedInterfaceMethodOffset");
   return 0;
   }

OMR::ResolvedMethod *
OMR::ResolvedMethod::getResolvedInterfaceMethod(TR::Compilation *, TR_OpaqueClassBlock *, int32_t)
   {
   notImplemented("getResolvedInterfaceMethod");
   return 0;
   }

OMR::ResolvedMethod *
OMR::ResolvedMethod::getResolvedVirtualMethod(TR::Compilation *, TR_OpaqueClassBlock *, int32_t, bool)
   {
   notImplemented("getResolvedVirtualMethod");
   return 0;
   }

void
OMR::ResolvedMethod::setMethodHandleLocation(uintptrj_t *location)
   {
   notImplemented("setMethodHandleLocation");
   }

TR::IlGeneratorMethodDetails *
OMR::ResolvedMethod::getIlGeneratorMethodDetails()
   {
   notImplemented("getIlGeneratorMethodDetails");
   return 0;
   }

TR::SymbolReferenceTable *
OMR::ResolvedMethod::_genMethodILForPeeking(TR::ResolvedMethodSymbol *, TR::Compilation *, bool resetVisitCount, TR_PrexArgInfo  *argInfo)
   {
   notImplemented("genMethodILForPeeking");
   return 0;
   }


TR::ResolvedMethodSymbol* OMR::ResolvedMethod::findOrCreateJittedMethodSymbol(TR::Compilation *comp)
   {
   return comp->createJittedMethodSymbol(this);
   }

// This version is suitable for Java
void OMR::ResolvedMethod::makeParameterList(TR::ResolvedMethodSymbol *methodSym)
   {
   if (methodSym->getTempIndex() != -1)
      return;

   const char *className    = classNameChars();
   const int   classNameLen = classNameLength();
   const char *sig          = signatureChars();
   const int   sigLen       = signatureLength();
   const char *sigEnd       = sig + sigLen;

   ListAppender<TR::ParameterSymbol> la(&methodSym->getParameterList());
   TR::ParameterSymbol *parmSymbol;
   int32_t slot;
   int32_t ordinal = 0;
   if (methodSym->isStatic())
      {
      slot = 0;
      }
   else
      {
      parmSymbol = methodSym->comp()->getSymRefTab()->createParameterSymbol(methodSym, 0, TR::Address, false);
      parmSymbol->setOrdinal(ordinal++);

      int32_t len = classNameLen; // len is passed by reference and changes during the call
      char * s = classNameToSignature(className, len, methodSym->comp(), heapAlloc);

      la.add(parmSymbol);
      parmSymbol->setTypeSignature(s, len);

      slot = 1;
      }

   const char *s = sig;
   TR_ASSERT(*s == '(', "Bad signature for method: <%s>", s);
   ++s;

   uint32_t parmSlots = numberOfParameterSlots();
   for (int32_t parmIndex = 0; slot < parmSlots; ++parmIndex)
      {
      TR::DataType type = parmType(parmIndex);
      int32_t size = methodSym->convertTypeToSize(type);
      if (size < 4) type = TR::Int32;

      const char *end = s;

      // Walk past array dims, if any
      while (*end == '[')
         {
         ++end;
         }

      // Walk to the end of the class name, if this is a class name
      if (*end == 'L')
         {
         // Assume the form is L<classname>; where <classname> is
         // at least 1 char and therefore skip the first 2 chars
         end += 2;
         end = (char *)memchr(end, ';', sigEnd - end);
         TR_ASSERT(end != NULL, "Unexpected NULL, expecting to find a parm of the form L<classname>;");
         }

      // The static_cast<int>(...) is added as a work around for an XLC bug that results in the
      // pointer subtraction below getting converted into a 32-bit signed integer subtraction
      int len = static_cast<int>(end - s) + 1;

      bool isUnsigned = (*s == 'C' || *s == 'Z'); // char or bool

      parmSymbol = methodSym->comp()->getSymRefTab()->createParameterSymbol(methodSym, slot, type, isUnsigned);
      parmSymbol->setOrdinal(ordinal++);
      parmSymbol->setTypeSignature(s, len);

      s += len;

      la.add(parmSymbol);
      if (type == TR::Int64 || type == TR::Double)
         {
         slot += 2;
         }
      else
         {
         ++slot;
         }
      }

   int32_t lastInterpreterSlot = parmSlots + numberOfTemps();

   if ((methodSym->isSynchronised() || methodSym->getResolvedMethod()->isNonEmptyObjectConstructor()) &&
       methodSym->comp()->getOption(TR_MimicInterpreterFrameShape))
      {
      ++lastInterpreterSlot;
      }

   methodSym->setTempIndex(lastInterpreterSlot, methodSym->comp()->fe());

   methodSym->setFirstJitTempIndex(methodSym->getTempIndex());
   }

