#ifndef I_MODULE_H
#define I_MODULE_H

#include "IEngineModule.h"

#include "GameEngine/Macros/DLL.h"

template<class ChildClass>
class DLL_DECL IModule : public IEngineModule
{
protected:
   typedef TASK_TYPE::MemberCallback<ChildClass> Task;

   Task* CreateTask(void (ChildClass::*aFunction)(float))
   {
      return new Task(static_cast<ChildClass* const>(this), aFunction);
   }
};

#endif I_MODULE_H