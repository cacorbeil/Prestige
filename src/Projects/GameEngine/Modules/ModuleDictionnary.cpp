#include "ModuleDictionnary.h"
#include "IEngineModule.h"

void ModuleDictionnary::Add(IncrementatorType aId, IEngineModule* module)
{
   Modules[aId] = module;
}

IEngineModule* ModuleDictionnary::GetModule(IncrementatorType aId)
{
   return Modules[aId];
}

void ModuleDictionnary::RemoveModule(IncrementatorType aId)
{
   Modules.erase(aId);
}