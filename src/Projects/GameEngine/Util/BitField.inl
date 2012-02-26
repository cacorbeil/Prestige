#ifdef BIT_FIELD_H

template<int N>
BitField<N>::BitField()
{
   // Clear all bits
   for(int i = 0; i != NUMBER_OF_SECTORS; ++i)
   {
      mBitFieldsSectors[i] = 0;
   }
}

template <int N>
void BitField<N>::Flag(unsigned int aFieldPosition, bool aFlagValue /* = true*/)
{
   //ASSERT_RETURN(aFieldPosition < BIT_PER_SECTOR * NUMBER_OF_SECTORS, "Bit index too high", false);
   if(!(aFieldPosition < BIT_PER_SECTOR * NUMBER_OF_SECTORS))
      return;

   SectorId sectorId = aFieldPosition / BIT_PER_SECTOR;

   // Make field position relative to it's sector
   aFieldPosition -= sectorId * BIT_PER_SECTOR;

   FlagSector mask = 1 << aFieldPosition;

   // Activate flag
   if(aFlagValue)
   {
      mBitFieldsSectors[sectorId] |= mask;
   }
   else // Deactivate flag
   {
      mBitFieldsSectors[sectorId] &= ~mask;
   }
}

template <int N>
bool BitField<N>::GetValue(unsigned int aFieldPosition) const
{
   //ASSERT(aFieldPosition < BIT_PER_SECTOR * NUMBER_OF_SECTORS, "Bit index too high");
   if(!(aFieldPosition < BIT_PER_SECTOR * NUMBER_OF_SECTORS))
      return false;

   SectorId sectorId = aFieldPosition / BIT_PER_SECTOR;

   // Make field position relative to it's sector
   aFieldPosition -= sectorId * BIT_PER_SECTOR;

   FlagSector mask = 1 << aFieldPosition;

  return !!(mBitFieldsSectors[sectorId] & mask);
}

#endif // BIT_FIELD_H