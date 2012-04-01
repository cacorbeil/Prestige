#include "Scheduler.h"

#include "GameEngine/Modules/IEngineModule.h"

namespace GameEngine
{

   void Scheduler::Execute(ModuleDictionary& aModuleList, float aDeltaTime) const
   {
      for(ModuleDictionary::iterator it = aModuleList.Begin(); it != aModuleList.End(); ++it)
      {
         TaskTree tasks;
         it->GetTasks(tasks);
         while(tasks.GetCount() > 0)
         {
            if(tasks.GetAvailableCount() > 0)
            {
               TaskFunctionNode* task = &tasks.GetOne();

               (*task->GetTask())(aDeltaTime);
               tasks.Remove(*task);
            }
         }
   }
}