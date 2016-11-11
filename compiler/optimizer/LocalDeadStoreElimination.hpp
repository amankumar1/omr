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

#ifndef TR_LOCALDEADSTOREELIMINATION_INCL
#define TR_LOCALDEADSTOREELIMINATION_INCL

#include <stdint.h>         // for int32_t
//#include "cs2/allocator.h"  // for Allocator
#include "env/TRMemory.hpp" // for Allocator, etc
#include "cs2/tableof.h"    // for TableOf
#include "il/Node.hpp"      // for vcount_t, rcount_t
#include "infra/deque.hpp"
#include "optimizer/Optimization.hpp"        // for Optimization
#include "optimizer/OptimizationManager.hpp" // for OptimizationManager

namespace TR {
class Block;
}
namespace TR {
class NodeChecklist;
}
namespace TR {
class TreeTop;
}
template <class T> class List;

typedef TR::SparseBitVector SharedSparseBitVector;
typedef TR::SparseBitVector SharedBitVector;

namespace TR {

/**
 * Class LocalDeadStoreElimination
 * ===============================
 *
 * LocalDeadStoreElimination implements an algorithm that proceeds as follows:
 *
 * Scan through an extended basic block starting from the last treetop
 * and moving upwards through the block. As a store is seen, mark the symbol
 * being stored as unused and if another store is encountered above to
 * the same symbol, then it can be removed if the symbol being stored into
 * is still marked as unused. Symbols are marked as used when a load is
 * seen or they are aliased to a symbol (a call for example) that might
 * use it implicitly (through aliases).
 */

class LocalDeadStoreElimination : public TR::Optimization {
public:
  // Performs local dead store elimination within
  // a basic block.
  //
  LocalDeadStoreElimination(TR::OptimizationManager *manager);
  static TR::Optimization *create(TR::OptimizationManager *manager);

  virtual int32_t perform();
  virtual int32_t performOnBlock(TR::Block *);
  virtual void prePerformOnBlocks();
  virtual void postPerformOnBlocks();

protected:
  virtual bool isNonRemovableStore(TR::Node *storeNode,
                                   bool &seenIdentityStore);

private:
  struct PendingIdentityStore {
    TR::TreeTop *treeTop;
    TR::Node *storeNode;
    TR::Node *loadNode;
  };

  typedef TR::Node *StoreNode;

  typedef CS2::TableOf<PendingIdentityStore, TR::Allocator>
      PendingIdentityStoreTable;
  typedef TR::deque<StoreNode> StoreNodeTable;
  typedef TR::BitVector LDSBitVector;

  inline TR::LocalDeadStoreElimination *self();

  bool isIdentityStore(TR::Node *);
  void examineNode(TR::Node *, int32_t, TR::Node *, SharedBitVector &);
  void transformBlock(TR::TreeTop *, TR::TreeTop *);
  bool isEntireNodeRemovable(TR::Node *);
  void setExternalReferenceCountToTree(TR::Node *node,
                                       rcount_t *externalReferenceCount);
  bool seenIdenticalStore(TR::Node *);
  bool areLhsOfStoresSyntacticallyEquivalent(TR::Node *, TR::Node *);
  void adjustStoresInfo(TR::Node *, SharedBitVector &);
  TR::Node *getAnchorNode(TR::Node *parentNode, int32_t nodeIndex, TR::Node *,
                          TR::TreeTop *, TR::NodeChecklist &visited);

  void setupReferenceCounts(TR::Node *);
  void eliminateDeadObjectInitializations();
  void findLocallyAllocatedObjectUses(LDSBitVector &, TR::Node *, int32_t,
                                      TR::Node *, vcount_t);
  bool examineNewUsesForKill(TR::Node *, TR::Node *, List<TR::Node> *,
                             List<TR::Node> *, TR::Node *, int32_t, vcount_t);
  void killStoreNodes(TR::Node *);

  TR::TreeTop *removeStoreTree(TR::TreeTop *treeTop);

  bool isFirstReferenceToNode(TR::Node *parent, int32_t index, TR::Node *node);
  void setIsFirstReferenceToNode(TR::Node *parent, int32_t index,
                                 TR::Node *node);

protected:
  TR::TreeTop *_curTree;

private:
  StoreNodeTable *_storeNodes;
  PendingIdentityStoreTable *_pendingIdentityStores;

  bool _blockContainsReturn;
  bool _treesChanged;
  bool _treesAnchored;
};
}

#endif
