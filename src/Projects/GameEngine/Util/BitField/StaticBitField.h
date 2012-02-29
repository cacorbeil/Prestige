#ifndef STATIC_BIT_FIELD_H
#define STATIC_BIT_FIELD_H

#include "GameEngine/Util/Constants.h"

template<int N>
class StaticBitField
{
public:
   typedef long long FlagSector; // Size of a sector of flags
   typedef unsigned int FlagId; // Index of a flag
   typedef unsigned int SectorId; // Index of a sector

   StaticBitField();

   void Flag(FlagId aFieldPosition, bool aFlagValue = true);
   bool GetValue(FlagId aFieldPosition) const;
protected:
   static const SectorId BIT_PER_SECTOR = (sizeof(FlagSector) * BIT_PER_BYTE);
   static const SectorId NUMBER_OF_SECTORS = ((N - 1) / BIT_PER_SECTOR) + 1;
   FlagSector mFieldSectors[NUMBER_OF_SECTORS]; // As long as we need it to fit all these flags
};

#include "StaticBitField.inl"

#endif STATIC_BIT_FIELD_H