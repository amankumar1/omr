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

#ifndef STORAGEINFO_INCL
#define STORAGEINFO_INCL

#include <stddef.h>         // for size_t
#include <stdint.h>         // for intptr_t
#include "env/TRMemory.hpp" // for TR_Memory, etc
#include "il/DataTypes.hpp" // for DataTypes
#include "infra/Flags.hpp"  // for flags8_t

namespace TR {
class SymbolReference;
}
namespace TR {
class Compilation;
}
namespace TR {
class Node;
}

enum TR_StorageDestructiveOverlapInfo {
  TR_UnknownOverlap = 0, // May or may not be destructively overlapping.
  TR_DefinitelyDestructiveOverlap,  // Definitely destructive Overlapping
  TR_DefinitelyNoDestructiveOverlap // Definitely no destructive Overlapping
  // Note: In general, neither (    TR_DefinitelyDestructiveOverlap == not
  // TR_DefinitelyNoDestructiveOverlap)
  //                  nor     (not TR_DefinitelyDestructiveOverlap ==
  //                  TR_DefinitelyNoDestructiveOverlap holds) !!!
};

enum TR_StorageOverlapKind {
  TR_NoOverlap = 0, // s: the source (address) of data move; t: the destination
                    // (address) of the move. With regard to their lengths, they
                    // don't overlap
  TR_MayOverlap, // Not sure if s and t are overlap. Consider them overlapping
                 // in general
  TR_PostPosOverlap,     // Overlap and s's address is post to t's
  TR_SamePosOverlap,     // Overlap and s's address is same as t's
  TR_DestructiveOverlap, // Overlap and s's s < t and s + min (s_l, t_l) > t
  // where s_l is the source length while t_l is the destination length.
  TR_PriorPosOverlap, // Overlap and s's address is prior to t's but they do not
                      // destructively overlap
  TR_NumOverlapTypes
};

// Note: update TR_StorageClassNames (below) when adding new TR_StorageClass
// types
enum TR_StorageClass {
  TR_UnknownClass = 0,
  TR_DirectMappedAuto,   // direct store to an auto or loadaddr of an auto
  TR_DirectMappedStatic, // direct store to a static or loadaddr of a static
  TR_StaticBaseAddress,  // from cached static base
  TR_PrivateStaticBaseAddress, // from cached private static base
  TR_NumStorageClassTypes,
};

class TR_StorageInfo {

  TR_ALLOC(TR_Memory::CodeGenerator)

public:
  TR_StorageInfo(TR::Node *node, size_t length, TR::Compilation *c);

  void populateLoadOrStore(TR::Node *loadOrStore);
  void populateAddress(TR::Node *address);

  // conservative overlap test -- returns true for all overlap cases and some
  // non-overlapping cases
  TR_StorageOverlapKind mayOverlapWith(TR_StorageInfo *info);

  void print();

  char *getName() { return getName(_class); }

  static char *getName(TR_StorageClass klass) {
    return ((klass < TR_NumStorageClassTypes) ? TR_StorageClassNames[klass]
                                              : (char *)"invalid_class");
  }

  static char *getName(TR_StorageOverlapKind overlapType) {
    return ((overlapType < TR_NumOverlapTypes)
                ? TR_StorageOverlapKindNames[overlapType]
                : (char *)"invalid_overlap_type");
  }

  TR::Compilation *comp() { return _comp; }

private:
  TR::Node *_node;    // the input load or store or address
  TR::Node *_address; // either the first child of the input load or store or
                      // the input address (i.e. == _node)
  TR::SymbolReference *_symRef;
  intptr_t _offset;
  size_t _length;
  TR_StorageClass _class;
  TR::Compilation *_comp;
  static char *TR_StorageClassNames[TR_NumStorageClassTypes];
  static char *TR_StorageOverlapKindNames[TR_NumOverlapTypes];
};

#endif
