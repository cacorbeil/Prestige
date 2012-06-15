#include "PrecompiledHeader.h"

#include "FlagContainer.h"

namespace Util
{

   FlagContainer::FlagContainer(FlagId aSize)
      : mSectorCount(0)
      , mBitCount(0)
      , mFieldSectors(NULL)
   {
      Resize(aSize);
   }

   FlagContainer::~FlagContainer()
   {
      delete [] mFieldSectors;
   }

   void FlagContainer::Resize(FlagId aSize)
   {
      FieldSector*   originalFieldSectors = mFieldSectors;
      FlagId          originalBitCount = mBitCount;
      SectorId       originalSectorCount = mSectorCount;

      mSectorCount = 0;

      // Don't allocate sector if size is zero
      if(aSize != 0)
      {
         mSectorCount = ((aSize - 1) / BIT_PER_SECTOR) + 1;
      }

      mBitCount = aSize;

      //Initialize bit field sectors
      mFieldSectors = new FieldSector[mSectorCount];

      // Clear all bits
      for(SectorId i = 0; i != mSectorCount; ++i)
      {
         const FieldSector MAX_BIT_FOR_THIS_SECTOR((i + 1) * BIT_PER_SECTOR);
         const FieldSector MIN_BIT_FOR_THIS_SECTOR(i * BIT_PER_SECTOR);

         mFieldSectors[i] = 0;
      
         // If we're resizing an existing bitfield
         if(originalFieldSectors != NULL)
         {
            // If we have data from to original to copy
            if(i < originalSectorCount)
            {
               // Simple copy from original to new
               mFieldSectors[i] = originalFieldSectors[i];

               // Are we at the last sector of the original
               if(i == originalSectorCount - 1)
               {
                  // Free all the bit outside of the original's range
                  mFieldSectors[i] &= 
                     ~(~static_cast<FieldSector>(0) << static_cast<FieldSector>(originalBitCount % BIT_PER_SECTOR));
               }
            }
         }

         // Lock the bits that are outside of range
         if(mBitCount < MAX_BIT_FOR_THIS_SECTOR)
         {
            mFieldSectors[i] |= 
               ~static_cast<FieldSector>(0) << static_cast<FieldSector>(mBitCount - MIN_BIT_FOR_THIS_SECTOR);
         }
      }

      delete [] originalFieldSectors;
   }

   void FlagContainer::Set(FlagId aFlagIndex, bool aBitValue)
   {
      ASSERT_COND_EARLY_OUT(aFlagIndex < mBitCount, "Bit index too high");
      if(!(aFlagIndex < mBitCount))
         return;

      SectorId sectorId = aFlagIndex / BIT_PER_SECTOR;
      FlagId relativeFieldPosition = aFlagIndex % BIT_PER_SECTOR;

      FieldSector mask = static_cast<FieldSector>(1) << static_cast<FieldSector>(relativeFieldPosition);

      // Activate flag
      if(aBitValue)
      {
         mFieldSectors[sectorId] |= mask;
      }
      else // Deactivate flag
      {
         mFieldSectors[sectorId] &= ~mask;
      }
   }

   bool FlagContainer::Get(FlagId aFlagIndex) const
   {
      ASSERT_COND(aFlagIndex < mBitCount, "Bit index too high");
      if(!(aFlagIndex < mBitCount))
         return false;

      SectorId sectorId = aFlagIndex / BIT_PER_SECTOR;
      FlagId relativeFieldPosition = aFlagIndex % BIT_PER_SECTOR;
      FieldSector mask = static_cast<FieldSector>(1) << static_cast<FieldSector>(relativeFieldPosition);

     return !!(mFieldSectors[sectorId] & mask);
   }

}