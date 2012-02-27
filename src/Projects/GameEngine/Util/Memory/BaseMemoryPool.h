#ifndef BASE_MEMORY_POOL_H
#define BASE_MEMORY_POOL_H

#include "GameEngine/Util/BitField/DynamicBitField.h"

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

protected:
   void*             mMemoryBlock;
   DynamicBitField   mFreeSector;
   SectorIndex       mSectorSize;
   size_t         mMemoryBlockSize;
private:

};

#endif BASE_MEMORY_POOL_H