#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>

class IEngineModule;

namespace GameEngine
{
   class Scheduler
   {
   public:
      void Execute(std::vector<IEngineModule*> aModuleList, float aDeltaTime) const;
      void Dispose();
   };
}

#endif SCHEDULER_H