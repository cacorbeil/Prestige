#ifdef SLOT_MANAGER_H

inline bool SlotManager::IsFull() const
{
   return !HasFree();
}

#endif SLOT_MANAGER_H