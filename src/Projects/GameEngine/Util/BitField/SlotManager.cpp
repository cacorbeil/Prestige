#include "SlotManager.h"

SlotManager::SlotManager(DynamicBitField::BitId aSize)
   : DynamicBitField(aSize)
{
}

bool SlotManager::HasFree() const
{
   for(int i = 0; i != mSectorCount; ++i)
   {
      if((~mFieldSectors[i]) != 0)
      {
         return true;
      }
   }

   return false;
}

DynamicBitField::BitId SlotManager::GetFree() const
{
   //ASSERT(HasFree(), "No field is free use HasFree() to verify if any unflagged field are left");
   for(SectorId i = 0; i != mSectorCount; ++i)
   {
      // Sector with a free field
      if(~mFieldSectors[i] != 0)
      {
         // Find the free field
         const int NEXT_SECTOR_FIRST_FLAG_ID((i + 1) * BIT_PER_SECTOR);
         for(BitId j = i * BIT_PER_SECTOR; j != NEXT_SECTOR_FIRST_FLAG_ID && j != mBitCount; ++j)
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