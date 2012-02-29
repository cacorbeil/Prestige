#ifndef DYNAMIC_BIT_FIELD_H
#define DYNAMIC_BIT_FIELD_H

#include "GameEngine/Util/Constants.h"

// Free bit are 0's
// Locked bit are 1's
class DynamicBitField
{
public:
   typedef long long FieldSector; // Size of a sector of flags
   typedef unsigned int BitId; // Index of a flag
   typedef unsigned int SectorId; // Index of a sector

   DynamicBitField(BitId aSize);
   ~DynamicBitField();

   void Resize(BitId aSize);
   void SetValue(BitId aFieldPosition, bool aBitValue);
   bool GetValue(BitId aFieldPosition) const;
   
   inline BitId GetCount() const;

protected:
   static const SectorId BIT_PER_SECTOR = (sizeof(FieldSector) * BIT_PER_BYTE);
   FieldSector*   mFieldSectors; // As long as we need it to fit all these flags
   BitId          mBitCount;
   SectorId       mSectorCount;
private:
};

#include "DynamicBitField.inl"

#endif DYNAMIC_BIT_FIELD_H