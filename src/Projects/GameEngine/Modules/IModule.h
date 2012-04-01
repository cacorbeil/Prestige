#ifndef I_MODULE_H
#define I_MODULE_H

#include "IEngineModule.h"

#include "GameEngine/Macros/DLL.h"

template class DLL_DECL Util::Callback<void, float>;
template class DLL_DECL Util::Callback<void, float>::BaseCallback;
template class DLL_DECL Util::Callback<void, float>::GlobalCallback;

namespace GameEngine
{
   /// Specialised module that can easily create member function task
   template<class ChildClass>
   class IModule : public GameEngine::IEngineModule
   {
    public:
      typedef TASK_TYPE::MemberCallback<ChildClass> Task; ///< Definition of a member function task

      /// Create a member function task
      /// \param aFunction The member function to create a task from
      Task* CreateTask(void (ChildClass::*aFunction)(float))
      {
         return new Task(static_cast<ChildClass* const>(this), aFunction);
      }
   };
}

#endif I_MODULE_H