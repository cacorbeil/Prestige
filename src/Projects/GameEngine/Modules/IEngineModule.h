#ifndef I_ENGINE_MODULE_H
#define I_ENGINE_MODULE_H

#include "TaskTree.h"
#include "GameEngine/Util/Enum/DistributedEnum.h"

#include "GameEngine/Macros/DLL.h"

DISTRIBUTED_ENUM(INCREMENTATOR_ENUM, MODULE_ID);

namespace GameEngine
{
   typedef Util::IncrementatorType ModuleId;

   /// Interface for engine module
   /// Modules are independant part of the engine
   /// Once registered to the engine, they are polled for tasks every frame
   /// \see Engine
   class DLL_DECL IEngineModule
   {
   public:
      IEngineModule();

      /// Returns the tasks that need to be executed this frame in a task graph
      /// \param arTaskContainer The container that will contain the tasks to be executed
      virtual void GetTasks(TaskTree& arTaskContainer) = 0; // Reference for optimization

      /// Get the unique identifier of this module
      inline ModuleId GetId();
   protected:
      /// Activate the module
      virtual bool Activate();
      /// Deactivate the module
      virtual void Deactivate();
   private:
      ModuleId mId;  ///< Unique identifier of the module
   };
}

#include "IEngineModule.inl"

#endif I_ENGINE_MODULE_H
