#ifndef DYNAMIC_BIT_FIELD_H
#define DYNAMIC_BIT_FIELD_H

#include "GameEngine/Util/Constants.h"

/// Generic set of tools
namespace Util
{
   /// Collection of boolean value compacted into a single variable.
   /// A bit field contains a number of boolean packed within the lowest number of variable possible.
   /// A dynamic bit field can be resize and it will keep the bit that are common between the 2 sizes
   class FlagContainer
   {
   public:
      typedef unsigned int FlagId;      ///< Index of a flag within the bit field

      /// \param aSize The number of flag the class needs
      FlagContainer(FlagId aSize);
      ~FlagContainer();

      void Resize(FlagId aSize);
      void Set(FlagId aFlagIndex, bool aBitValue);
      bool Get(FlagId aFlagIndex) const;
   
      /// Get the number of flag this class supports
      inline FlagId GetCount() const;

   protected:
      /// Size of a sector of flags
      /// \note Sector is the POD we use to store the different boolean value
      typedef long long FieldSector; 
      typedef unsigned int SectorId;   ///< Index of a sector within the bit field

      static const SectorId BIT_PER_SECTOR = (sizeof(FieldSector) * BIT_PER_BYTE); ///< Number of bit a sector supports
      FieldSector*   mFieldSectors; ///< Array of sectors. This is where the flags are stored
      FlagId          mBitCount;     ///< Number of flags this class supports
      SectorId       mSectorCount;  ///< Number of sector needed to support the number of flag of the class
   private:
   };

}
#include "FlagContainer.inl"

#endif DYNAMIC_BIT_FIELD_H