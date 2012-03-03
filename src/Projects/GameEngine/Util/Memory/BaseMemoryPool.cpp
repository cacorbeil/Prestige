#include "BaseMemoryPool.h"

#include "GameEngine/Util/Constants.h"
#include <cstring>

BaseMemoryPool::BaseMemoryPool(SectorIndex aSectorSize, SectorIndex aSectorCount)
   : mFreeSector(0)
   , mSectorSize(aSectorSize)
   , mMemoryBlockSize(NULL)
   , mMemoryBlock(NULL)
{
   Resize(aSectorCount);

#ifdef DEBUG
   for(SectorIndex i = 0; i != aSectorCount; ++i)
   {
      InvalidateMemoryAddress(i);
   }
#endif
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

   return GetMemoryAddress(freeLocationId);
}

void BaseMemoryPool::Free(void* p)
{
   DynamicBitField::BitId locationId = 
      ((static_cast<char*>(p)) - (static_cast<char*>(mMemoryBlock))) / mSectorSize;

   mFreeSector.SetValue(locationId, false);

#ifdef DEBUG
   InvalidateMemoryAddress(locationId);
#endif DEBUG
}

void BaseMemoryPool::Resize(SectorIndex aSectorCount)
{
#ifdef DEBUG
   for(DynamicBitField::BitId i = aSectorCount; i < mFreeSector.GetCount(); ++i)
   {
      // ASSERT(!mFreeSector.GetValue(i), "Memory Pool have sectors that are currently use that will be lost in the resizing");
      if(!(!mFreeSector.GetValue(i)))
      {
         return;
      }
   }
#endif DEBUG

   size_t originalMemoryBlockSize = mMemoryBlockSize;
   void* originalMemoryBlock = mMemoryBlock;

   mMemoryBlockSize = mSectorSize * aSectorCount;
   // Allocate memory block
   mMemoryBlock = malloc(mMemoryBlockSize);

   memcpy(mMemoryBlock, originalMemoryBlock, Math::Min(originalMemoryBlockSize, mMemoryBlockSize));
   mFreeSector.Resize(aSectorCount);

   free(originalMemoryBlock);
}

#ifdef DEBUG
   void BaseMemoryPool::InvalidateMemoryAddress(SectorIndex index)
   {
      void* address = GetMemoryAddress(index);
      for(SectorIndex i = 0; i != mSectorSize; ++i)
      {
         *(reinterpret_cast<unsigned char*>(address) + i) = static_cast<unsigned char>(INVALIDATION_BYTE);
      }
   }
#endif