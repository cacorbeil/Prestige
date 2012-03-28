#ifndef I_ENGINE_MODULE_H
#define I_ENGINE_MODULE_H

#include "TaskGraph.h"
#include "GameEngine/Util/DistributedEnum.h"

#include "GameEngine/Macros/DLL.h"

DISTRIBUTED_ENUM(INCREMENTATOR_ENUM, MODULE_ID);

class DLL_DECL IEngineModule
{
public:
   IEngineModule();
   virtual void GetTasks(TaskGraph& arTaskGraphContainer) = 0; // Reference for optimization

   // inlines
   inline IncrementatorType GetId() { return mId; }
protected:
   virtual bool Activate();
   virtual void Deactivate();
private:
   IncrementatorType mId;
   bool mEnabled;

   void Enable();
   void Disable();
};

#endif I_ENGINE_MODULE_H