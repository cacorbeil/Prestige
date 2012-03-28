#ifndef MODULE_DICTIONNARY_H
#define MODULE_DICTIONNARY_H

#include <map>
#include "GameEngine/Util/Incrementator.h"

#include "GameEngine/Macros/DLL.h"

class IEngineModule;

#pragma warning(push)
#pragma warning( disable : 4251 ) // std::map export

class DLL_DECL ModuleDictionnary
{
public:
   void Add(IncrementatorType aId, IEngineModule* module);
   IEngineModule* GetModule(IncrementatorType aId);
   void RemoveModule(IncrementatorType aId);

private:
   std::map<IncrementatorType, IEngineModule*> Modules;
};

#pragma warning(pop)

#endif MODULE_DICTIONNARY_H