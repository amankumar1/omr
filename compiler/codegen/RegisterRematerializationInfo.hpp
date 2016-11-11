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
 *******************************************************************************/

#ifndef REMATERIALIZATIONINFO_INCL
#define REMATERIALIZATIONINFO_INCL

#include <stddef.h>                      // for NULL
#include "codegen/RegisterConstants.hpp" // for TR_RematerializableTypes
#include "env/TRMemory.hpp"              // for TR_Memory, etc
#include "env/jittypes.h"                // for intptrj_t
#include "infra/Flags.hpp"               // for flags16_t

namespace TR {
class Instruction;
}
namespace TR {
class Node;
}
namespace TR {
class Register;
}
namespace TR {
class SymbolReference;
}

class TR_RematerializationInfo {
public:
  struct MemoryReference {
    TR::SymbolReference *_symbolReference;
    TR::Register *_baseRegister;
  };

private:
  enum // _flags
  { RematerializableFromAddress =
        0x0001, // Register contents can be rematerialized from an address
    RematerializableFromConstant =
        0x0002, // Register contents can be rematerialized from a constant
    RematerializableFromMemory =
        0x0004, // Register contents can be rematerialized from memory
    RematerializableFromSpillReg =
        0x0008,          // Register contents are in a global spill register
    IsIndirect = 0x0010, // Rematerializable memory access is indirect
    IsStore = 0x0020,    // Rematerializable memory access is a store
    IsActive = 0x0040,   // Register is currently active
    IsRematerialized = 0x0080 // Register is currently spilled/rematerialized
  };

  union {
    intptrj_t _constant;
    MemoryReference _mr;
  };

  TR::Node *_NodeForwardDeclinition;
  TR::Instruction *_definition;
  TR_RematerializableTypes _type;
  flags16_t _flags;

public:
  TR_ALLOC(TR_Memory::RematerializationInfo)

  TR_RematerializationInfo(TR::Instruction *instr,
                           TR_RematerializableTypes type,
                           TR::SymbolReference *symRef)
      : _definition(instr), _type(type), _NodeForwardDeclinition(NULL) {
    _mr._symbolReference = symRef;
    setRematerializableFromAddress();
  }

  TR_RematerializationInfo(TR::Instruction *instr,
                           TR_RematerializableTypes type, intptrj_t constant)
      : _definition(instr), _type(type), _constant(constant),
        _NodeForwardDeclinition(NULL) {
    setRematerializableFromConstant();
  }

  TR_RematerializationInfo(TR::Instruction *instr,
                           TR_RematerializableTypes type,
                           TR::SymbolReference *symRef, TR::Register *baseReg)
      : _definition(instr), _type(type), _NodeForwardDeclinition(NULL) {
    _mr._symbolReference = symRef;
    _mr._baseRegister = baseReg;
    setRematerializableFromMemory();
    if (baseReg)
      setIndirect();
  }

  TR_RematerializationInfo(TR::Instruction *instr,
                           TR_RematerializableTypes type,
                           TR::Register *spillReg, TR::Node *defNode)
      : _definition(instr), _type(type), _NodeForwardDeclinition(defNode) {
    _mr._baseRegister = spillReg;
    setRematerializableFromSpillReg();
  }

  intptrj_t getConstant() { return _constant; }
  intptrj_t setConstant(intptrj_t c) { return (_constant = c); }

  TR::Register *getBaseRegister() { return _mr._baseRegister; }
  TR::Register *setBaseRegister(TR::Register *r) {
    return (_mr._baseRegister = r);
  }

  TR::Register *getSpillRegister() { return _mr._baseRegister; }
  TR::Register *setSpillRegister(TR::Register *r) {
    return (_mr._baseRegister = r);
  }

  TR::SymbolReference *getSymbolReference() { return _mr._symbolReference; }
  TR::SymbolReference *setSymbolReference(TR::SymbolReference *sr) {
    return (_mr._symbolReference = sr);
  }

  TR::Node *getNodeForwardDeclinition() { return _NodeForwardDeclinition; }
  TR::Node *setNodeForwardDeclinition(TR::Node *defNode) {
    return (_NodeForwardDeclinition = defNode);
  }

  TR::Instruction *getDefinition() { return _definition; }
  TR::Instruction *setDefinition(TR::Instruction *definition) {
    return (_definition = definition);
  }

  TR_RematerializableTypes getDataType() { return _type; }
  TR_RematerializableTypes setDataType(TR_RematerializableTypes t) {
    return (_type = t);
  }

  bool isRematerializableFromAddress() {
    return _flags.testAny(RematerializableFromAddress);
  }
  void setRematerializableFromAddress() {
    _flags.set(RematerializableFromAddress);
  }

  bool isRematerializableFromConstant() {
    return _flags.testAny(RematerializableFromConstant);
  }
  void setRematerializableFromConstant() {
    _flags.set(RematerializableFromConstant);
  }

  bool isRematerializableFromMemory() {
    return _flags.testAny(RematerializableFromMemory);
  }
  void setRematerializableFromMemory() {
    _flags.set(RematerializableFromMemory);
  }

  bool isRematerializableFromSpillReg() {
    return _flags.testAny(RematerializableFromSpillReg);
  }
  void setRematerializableFromSpillReg() {
    _flags.set(RematerializableFromSpillReg);
  }

  bool isIndirect() { return _flags.testAny(IsIndirect); }
  void setIndirect() { _flags.set(IsIndirect); }

  bool isStore() { return _flags.testAny(IsStore); }
  void setStore() { _flags.set(IsStore); }

  bool isActive() { return _flags.testAny(IsActive); }
  void setActive() { _flags.set(IsActive); }
  void resetActive() { _flags.reset(IsActive); }

  bool isRematerialized() { return _flags.testAny(IsRematerialized); }
  void setRematerialized() { _flags.set(IsRematerialized); }
  void resetRematerialized() { _flags.reset(IsRematerialized); }
#if defined(DEBUG)
  const char *toString(TR::Compilation *);
#endif
};

#endif
