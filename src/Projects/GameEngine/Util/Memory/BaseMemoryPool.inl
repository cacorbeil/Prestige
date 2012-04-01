#ifdef BASE_MEMORY_POOL_H

namespace Util
{
   inline bool BaseMemoryPool::HasRoom() const
   {
      return mFreeSector.HasFree();
   }

   inline BaseMemoryPool::SectorIndex BaseMemoryPool::GetSize() const
   { 
      return mFreeSector.GetCount();
   }

   inline void* BaseMemoryPool::GetMemoryAddress(SectorIndex aIndex) const
   {
      return static_cast<void*>(static_cast<char*>(mMemoryBlock) + (mSectorSize * aIndex));
   }
}

#endif BASE_MEMORY_POOL_H

