#include "DynamicBitField.h"

DynamicBitField::DynamicBitField(BitId aSize)
   : mSectorCount(0)
   , mBitCount(0)
   , mFieldSectors(NULL)
{
   Resize(aSize);
}

DynamicBitField::~DynamicBitField()
{
   delete [] mFieldSectors;
}

void DynamicBitField::Resize(BitId aSize)
{
   FieldSector*   originalFieldSectors = mFieldSectors;
   BitId          originalBitCount = mBitCount;
   SectorId       originalSectorCount = mSectorCount;

   mSectorCount = ((aSize - 1) / BIT_PER_SECTOR) + 1;
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

void DynamicBitField::SetValue(BitId aFieldPosition, bool aBitValue)
{
   //ASSERT_EARLY_OUT(aFieldPosition < mBitCount, "Bit index too high", false);
   if(!(aFieldPosition < mBitCount))
      return;

   SectorId sectorId = aFieldPosition / BIT_PER_SECTOR;
   BitId relativeFieldPosition = aFieldPosition % BIT_PER_SECTOR;

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

bool DynamicBitField::GetValue(BitId aFieldPosition) const
{
   //ASSERT(aFieldPosition < mBitCount, "Bit index too high");
   if(!(aFieldPosition < mBitCount))
      return false;

   SectorId sectorId = aFieldPosition / BIT_PER_SECTOR;
   BitId relativeFieldPosition = aFieldPosition % BIT_PER_SECTOR;
   FieldSector mask = static_cast<FieldSector>(1) << static_cast<FieldSector>(relativeFieldPosition);

  return !!(mFieldSectors[sectorId] & mask);
}