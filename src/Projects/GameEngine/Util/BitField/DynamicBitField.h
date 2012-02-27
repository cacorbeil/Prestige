#ifndef DYNAMIC_BIT_FIELD_H
#define DYNAMIC_BIT_FIELD_H

#include "GameEngine/Util/Constants.h"

class DynamicBitField
{
public:
   typedef long long FieldSector; // Size of a sector of flags
   typedef unsigned int BitId; // Index of a flag
   typedef unsigned int SectorId; // Index of a sector

   DynamicBitField(BitId aSize);
   ~DynamicBitField();

   void SetValue(BitId aFieldPosition, bool aBitValue);
   bool GetValue(BitId aFieldPosition) const;

   bool HasFree() const;
   BitId GetFree() const;
   
protected:
   static const SectorId BIT_PER_SECTOR = (sizeof(FieldSector) * BIT_IN_BYTES);
   FieldSector*   mBitFieldsSectors; // As long as we need it to fit all these flags
   BitId          mBitFieldCount;
private:
   SectorId mNumberOfSector;
};

#endif DYNAMIC_BIT_FIELD_H