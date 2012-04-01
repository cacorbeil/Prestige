#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "GameEngine/Modules/ModuleDictionary.h"

namespace GameEngine
{
   class IEngineModule;

   /// Executes all the tasks given from a list of module.
   /// Execution of these tasks is left to the logic of the scheduler.
   /// This scheduler take each tasks sequentially and execute them.
   class Scheduler
   {
   public:
      /// Executes the tasks given from the given modules
      /// \param aModuleList List of modules from which to gather the tasks to execute
      /// \param aDeltaTime Time since last frame (Theorical, it may not, but should look like it)
      void Execute(ModuleDictionary& aModuleList, float aDeltaTime) const;
   };
}

#endif SCHEDULER_H
