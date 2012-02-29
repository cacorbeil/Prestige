#ifndef SLOT_MANAGER_H
#define SLOT_MANAGER_H

#include "DynamicBitField.h"

class SlotManager : public DynamicBitField
{
public:
   SlotManager(DynamicBitField::BitId aSize);

   bool HasFree() const;
   BitId GetFree() const;
protected:
private:
};

#endif SLOT_MANAGER_H