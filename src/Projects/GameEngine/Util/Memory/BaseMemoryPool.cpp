#include "PrecompiledHeader.h"

#include "BaseMemoryPool.h"

#include "GameEngine/Util/Constants.h"
#include <cstring>

namespace Util
{
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

   void* BaseMemoryPool::Alloc()
   {
      FlagContainer::FlagId freeLocationId = mFreeSector.GetFree();

      mFreeSector.Set(freeLocationId, true);

      return GetMemoryAddress(freeLocationId);
   }

   void BaseMemoryPool::Free(void* p)
   {
      const char* slotPointer = static_cast<char*>(p);
      const char* blockPointer = static_cast<char*>(mMemoryBlock);
      ASSERT_COND_EARLY_OUT(
         slotPointer >= blockPointer && 
         slotPointer < blockPointer + mFreeSector.GetCount() * mSectorSize, "Given pointer is not from the memory pool");

      FlagContainer::FlagId locationId = 
         (slotPointer - blockPointer) / mSectorSize;

      mFreeSector.Set(locationId, false);

   #ifdef DEBUG
      InvalidateMemoryAddress(locationId);
   #endif DEBUG
   }

   void BaseMemoryPool::Resize(SectorIndex aSectorCount)
   {
   #ifdef DEBUG
      for(FlagContainer::FlagId i = aSectorCount; i < mFreeSector.GetCount(); ++i)
      {
         ASSERT_COND(!mFreeSector.IsFull(), "Memory Pool have sectors that are currently use that will be lost in the resizing");
         if(!(!mFreeSector.Get(i)))
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
      void BaseMemoryPool::InvalidateMemoryAddress(SectorIndex aIndex)
      {
         void* address = GetMemoryAddress(aIndex);
         for(SectorIndex i = 0; i != mSectorSize; ++i)
         {
            *(reinterpret_cast<unsigned char*>(address) + i) = static_cast<unsigned char>(INVALIDATION_BYTE);
         }
      }
   #endif

}