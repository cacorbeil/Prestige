#include "PrecompiledHeader.h"

#include "ModuleDictionary.h"

namespace GameEngine
{
   void ModuleDictionary::Add(IEngineModule* aModule)
   {
      mModules[aModule->GetId()] = aModule;
      mModulesId.push_back(aModule->GetId());
   }

   IEngineModule* ModuleDictionary::Get(ModuleId aId)
   {
      return mModules[aId];
   }

   void ModuleDictionary::Remove(IEngineModule* aModule)
   {
      mModules.erase(aModule->GetId());
      for(std::vector<ModuleId>::iterator it = mModulesId.begin(); it != mModulesId.end(); ++it)
      {
         if((*it) == aModule->GetId())
         {
            mModulesId.erase(it);
            break;
         }
      }
   }
}