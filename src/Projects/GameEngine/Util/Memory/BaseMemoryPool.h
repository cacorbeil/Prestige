#ifndef BASE_MEMORY_POOL_H
#define BASE_MEMORY_POOL_H

#include "GameEngine/Util/BitField/SlotManager.h"
#include "GameEngine/Math/Bounds.h"

#include <stdlib.h>

class BaseMemoryPool
{
public:
   typedef unsigned long SectorIndex;

   BaseMemoryPool(SectorIndex aSectorSize, SectorIndex aSectorCount);
   ~BaseMemoryPool();

   bool HasRoom() const; //inline this
   void* Alloc();
   void Free(void* p);

   inline void* Get(SectorIndex index);

   inline SectorIndex GetSize() const;
   void Resize(SectorIndex aSectorCount);

protected:
   void*             mMemoryBlock;
   SlotManager       mFreeSector;
   SectorIndex       mSectorSize;
   size_t            mMemoryBlockSize;
private:
   inline void* GetMemoryAddress(SectorIndex index) const;

#ifdef DEBUG
   void InvalidateMemoryAddress(SectorIndex index);
#endif
};

#include "BaseMemoryPool.inl"

#endif BASE_MEMORY_POOL_H