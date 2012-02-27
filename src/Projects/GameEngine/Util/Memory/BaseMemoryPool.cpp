#include "BaseMemoryPool.h"

   BaseMemoryPool::BaseMemoryPool(SectorIndex aSectorSize, SectorIndex aSectorCount)
      : mFreeSector(aSectorCount)
      , mSectorSize(aSectorSize)
      , mMemoryBlockSize(aSectorSize * aSectorCount)
   {
      // Allocate memory block
      mMemoryBlock = malloc(mMemoryBlockSize);
   }

   BaseMemoryPool::~BaseMemoryPool()
   {
      free(mMemoryBlock);
   }

   //inline
   bool BaseMemoryPool::HasRoom() const
   {
      return mFreeSector.HasFree();
   }

   void* BaseMemoryPool::Alloc()
   {
      DynamicBitField::BitId freeLocationId = mFreeSector.GetFree();

      mFreeSector.SetValue(freeLocationId, true);

      return static_cast<void*>(static_cast<char*>(mMemoryBlock) + (mSectorSize * freeLocationId));
   }

   void BaseMemoryPool::Free(void* p)
   {
      DynamicBitField::BitId locationId = 
         ((static_cast<char*>(p)) - (static_cast<char*>(mMemoryBlock))) / mSectorSize;

      mFreeSector.SetValue(locationId, false);
   }