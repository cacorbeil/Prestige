#ifdef BIT_FIELD_H

template<int N>
StaticFlagContainer<N>::StaticFlagContainer()
{
   // Clear all bits
   for(int i = 0; i != NUMBER_OF_SECTORS; ++i)
   {
      mFieldSectors[i] = 0;
   }
}

template <int N>
void StaticFlagContainer<N>::Set(unsigned int aFlagIndex, bool aValue)
{
   //ASSERT_RETURN(aFlagIndex < BIT_PER_SECTOR * NUMBER_OF_SECTORS, "Bit index too high", false);
   if(!(aFlagIndex < BIT_PER_SECTOR * NUMBER_OF_SECTORS))
      return;

   SectorId sectorId = aFlagIndex / BIT_PER_SECTOR;

   // Make field position relative to it's sector
   aFlagIndex -= sectorId * BIT_PER_SECTOR;

   FlagSector mask = 1 << aFlagIndex;

   // Activate flag
   if(aValue)
   {
      mFieldSectors[sectorId] |= mask;
   }
   else // Deactivate flag
   {
      mFieldSectors[sectorId] &= ~mask;
   }
}

template <int N>
bool StaticFlagContainer<N>::Get(FlagId aFlagIndex) const
{
   Assert(aFlagIndex < BIT_PER_SECTOR * NUMBER_OF_SECTORS, "Bit index too high");
   if(!(aFlagIndex < BIT_PER_SECTOR * NUMBER_OF_SECTORS))
      return false;

   SectorId sectorId = aFlagIndex / BIT_PER_SECTOR;

   // Make field position relative to it's sector
   aFlagIndex -= sectorId * BIT_PER_SECTOR;

   FlagSector mask = 1 << aFlagIndex;

  return !!(mFieldSectors[sectorId] & mask);
}

#endif // BIT_FIELD_H