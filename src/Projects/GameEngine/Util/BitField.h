#ifndef BIT_FIELD_H
#define BIT_FIELD_H

#include "Constants.h"

template<int N>
class BitField
{
public:
   typedef long long FlagSector; // Size of a sector of flags
   typedef unsigned int FlagId; // Index of a flag
   typedef unsigned char SectorId; // Index of a sector

   BitField();

   void Flag(FlagId aFieldPosition, bool aFlagValue = true);
   bool GetValue(FlagId aFieldPosition) const;
protected:
   static const SectorId BIT_PER_SECTOR = (sizeof(FlagSector) * BIT_IN_BYTES);
   static const SectorId NUMBER_OF_SECTORS = ((N - 1) / BIT_PER_SECTOR) + 1;
   FlagSector mBitFieldsSectors[NUMBER_OF_SECTORS]; // As long as we need it to fit all these flags
private:
};

#include "BitField.inl"

#endif BIT_FIELD_H