#include "Scheduler.h"

#include "GameEngine/Modules/IEngineModule.h"

namespace GameEngine
{

   void Scheduler::Execute(std::vector<IEngineModule*> aModuleList, float aDeltaTime) const
   {
      for(std::vector<IEngineModule*>::iterator it = aModuleList.begin(); it != aModuleList.end(); ++it)
      {
         TaskGraph tasks;
         (*it)->GetTasks(tasks);
         while(tasks.GetCount() > 0)
         {
            if(tasks.GetAvailableCount() > 0)
            {
               TaskFunctionNode* task = &tasks.GetOne();
               // Execute task
               (*task->GetValue())(aDeltaTime);

               tasks.Remove(*task);
            }
         }
      }
   }

   void Scheduler::Dispose()
   {
      // Destroy threadpool
   }

}