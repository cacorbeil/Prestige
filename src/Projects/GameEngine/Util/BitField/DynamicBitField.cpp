#include "DynamicBitField.h"

DynamicBitField::DynamicBitField(BitId aSize)
   : mNumberOfSector(((aSize - 1) / BIT_PER_SECTOR) + 1)
   , mBitFieldCount(aSize)
{
   //Initialize bit field sectors
   mBitFieldsSectors = new FieldSector[mNumberOfSector];

   // Clear all bits
   for(int i = 0; i != mNumberOfSector; ++i)
   {
      mBitFieldsSectors[i] = 0;
      
      if(aSize < (i + 1) * BIT_PER_SECTOR)
      {
         mBitFieldsSectors[i] = 
            ~(static_cast<FieldSector>(0)) << static_cast<FieldSector>((aSize - (i * BIT_PER_SECTOR)));
      }
      else
      {
         mBitFieldsSectors[i] = 0;
      }
   }
}

DynamicBitField::~DynamicBitField()
{
   delete [] mBitFieldsSectors;
}

void DynamicBitField::SetValue(BitId aFieldPosition, bool aBitValue)
{
   //ASSERT_EARLY_OUT(aFieldPosition < mBitFieldCount, "Bit index too high", false);
   if(!(aFieldPosition < BIT_PER_SECTOR * mNumberOfSector))
      return;

   SectorId sectorId = aFieldPosition / BIT_PER_SECTOR;

   // Make field position relative to it's sector
   aFieldPosition -= sectorId * BIT_PER_SECTOR;

   FieldSector mask = static_cast<FieldSector>(1) << static_cast<FieldSector>(aFieldPosition);

   // Activate flag
   if(aBitValue)
   {
      mBitFieldsSectors[sectorId] |= mask;
   }
   else // Deactivate flag
   {
      mBitFieldsSectors[sectorId] &= ~mask;
   }
}

bool DynamicBitField::GetValue(BitId aFieldPosition) const
{
   //ASSERT(aFieldPosition < mBitFieldCount, "Bit index too high");
   if(!(aFieldPosition < BIT_PER_SECTOR * mNumberOfSector))
      return false;

   SectorId sectorId = aFieldPosition / BIT_PER_SECTOR;

   // Make field position relative to it's sector
   aFieldPosition -= sectorId * BIT_PER_SECTOR;

   FieldSector mask = static_cast<FieldSector>(1) << static_cast<FieldSector>(aFieldPosition);

  return !!(mBitFieldsSectors[sectorId] & mask);
}

bool DynamicBitField::HasFree() const
{
   for(int i = 0; i != mNumberOfSector; ++i)
   {
      if((~mBitFieldsSectors[i]) != 0)
      {
         return true;
      }
   }

   return false;
}

DynamicBitField::BitId DynamicBitField::GetFree() const
{
   //ASSERT(HasFree(), "No field is free use HasFree() to verify if any unflagged field are left");
   for(SectorId i = 0; i != mNumberOfSector; ++i)
   {
      // Sector with a free field
      if(~mBitFieldsSectors[i] != 0)
      {
         // Find the free field
         const int NEXT_SECTOR_FIRST_FLAG_ID((i + 1) * BIT_PER_SECTOR);
         for(BitId j = i * BIT_PER_SECTOR; j != NEXT_SECTOR_FIRST_FLAG_ID && j != mBitFieldCount; ++j)
         {
            if(GetValue(j) == 0)
            {
               return j;
            }
         }
      }
   }

   //ASSERT_RETURN(false, "No bit left found and we got here?", -1);
   return -1;
}
