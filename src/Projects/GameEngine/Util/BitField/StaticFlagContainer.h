#ifndef STATIC_BIT_FIELD_H
#define STATIC_BIT_FIELD_H

#include "GameEngine/Util/Constants.h"

/// Collection of boolean value compacted into a single variable.
/// A bit field contains a number of boolean packed within the lowest number of variable possible.
/// A static bit field cannot be resize
/// \tparam N Number of flag
template<int N>
class StaticFlagContainer
{
public:
   typedef unsigned int FlagId;      ///< Index of a flag within the bit field

   StaticFlagContainer();

   /// Set the value of a specific flag
   void Set(FlagId aFlagIndex, bool aValue);
   bool Get(FlagId aFlagIndex) const;
protected:
   /// Size of a sector of flags
   /// \note Sector is the POD we use to store the different boolean value
   typedef long long FieldSector; 
   typedef unsigned int SectorId;   ///< Index of a sector within the bit field

   static const SectorId BIT_PER_SECTOR = (sizeof(FlagSector) * BIT_PER_BYTE); ///< Number of bit a sector supports
   /// Number of sector needed to support the number of flag of the class
   static const SectorId NUMBER_OF_SECTORS = ((N - 1) / BIT_PER_SECTOR) + 1;  
   FlagSector mFieldSectors[NUMBER_OF_SECTORS]; ///< Array of sectors. This is where the flags are stored
};

#include "StaticBitField.inl"

#endif STATIC_BIT_FIELD_H