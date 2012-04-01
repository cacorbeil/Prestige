#include "IEngineModule.h"

namespace GameEngine
{

   IEngineModule::IEngineModule()
      : mId(Util::Incrementator<INCREMENTATOR_ENUM::MODULE_ID>::GetIncrement())
   {
   }

   bool IEngineModule::Activate() { return true; } // Implementation is optional
   void IEngineModule::Deactivate() { } // Implementation is optional
}