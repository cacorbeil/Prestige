#ifndef SLOT_MANAGER_H
#define SLOT_MANAGER_H

#include "FlagContainer.h"

namespace Util
{
   /// Class that manage slot availability.
   /// It has a number of slot that start available that you can lock.
   /// The class can be queried to know if there is still some free slots or can be queried for a free slot directly
   /// Available bit are considered 0's
   /// Locked bit are considered 1's
   class SlotManager : public FlagContainer
   {
   public:
      SlotManager(FlagContainer::FlagId aSize);

      /// Returns true if 1 slot or more are available
      bool HasFree() const;

      /// Returns an available slot
      /// \note HasFree() must be called before to make sure there is an available slot
      /// \see HasFree()
      FlagId GetFree() const;

      inline bool IsFull() const;
   };

   #include "SlotManager.inl"
}

#endif SLOT_MANAGER_H