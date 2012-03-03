#ifdef BASE_MEMORY_POOL_H

inline void* BaseMemoryPool::Get(SectorIndex index)
{
   //ASSERT(mFreeSector.GetValue(index));

   return GetMemoryAddress(index);
}

inline BaseMemoryPool::SectorIndex BaseMemoryPool::GetSize() const
{ 
   return mFreeSector.GetCount();
}

inline void* BaseMemoryPool::GetMemoryAddress(SectorIndex index) const
{
   return static_cast<void*>(static_cast<char*>(mMemoryBlock) + (mSectorSize * index));
}

#endif BASE_MEMORY_POOL_H

