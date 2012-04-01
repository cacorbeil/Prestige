#ifndef BASE_MEMORY_POOL_H
#define BASE_MEMORY_POOL_H

#include "GameEngine/Util/BitField/SlotManager.h"
#include "GameEngine/Math/Bounds.h"

#include <stdlib.h>

namespace Util
{
   /// Reserve contiguous memory and manage its reservation.
   /// The reserved memory is separated in a specified number of sector of specified size.
   /// Allocation and release of sectors are fast and keeps memory fragmentation to a minimum
   class BaseMemoryPool
   {
   public:
      typedef unsigned long SectorIndex; ///< The index of a memory sector

      /// \param aSectorSize Size of a single sector
      /// \param aSectorCount Number of sector in the pool
      BaseMemoryPool(SectorIndex aSectorSize, SectorIndex aSectorCount);
      ~BaseMemoryPool();

      /// \return A pointer to the allocated memory
      void* Alloc();
      /// \param p The pointer of the memory to free
      void Free(void* p);
      /// \param aSectorCount The new number of sector
      /// \note A resize keeps the memory like it was, but if the pool is shrinking, it must not drop allocated memory
      void Resize(SectorIndex aSectorCount);

      /// \returns True if the is unallocated memory sector
      inline bool HasRoom() const;
      /// \returns The total number of sector available or not
      inline SectorIndex GetSize() const;
   protected:
      /// \param aIndex The index of the memory sector
      /// \returns The pointer to the associated memory sector
      inline void* GetMemoryAddress(SectorIndex aIndex) const;

      void*             mMemoryBlock;     ///< The reserved contiguous memory, where the sectors will be allocated
      Util::SlotManager mFreeSector;      ///< Manager that keeps which sector is allocated or available
      SectorIndex       mSectorSize;      ///< The total number of sector available or not
      size_t            mMemoryBlockSize; ///< The size of the reserved contiguous memory
   private:
   #ifdef DEBUG
      /// Writes debug value to the memory when reserved or freed to keeps track of bad memory management
      /// \param aIndex The index of the memory sector
      /// \note DEBUG only
      void InvalidateMemoryAddress(SectorIndex aIndex);
   #endif
   };
}
#include "BaseMemoryPool.inl"

#endif BASE_MEMORY_POOL_H