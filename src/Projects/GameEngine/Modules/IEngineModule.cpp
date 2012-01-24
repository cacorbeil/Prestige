#include "IEngineModule.h"

IEngineModule::IEngineModule()
   : mId(Incrementator<INCREMENTATOR_ENUM::MODULE_ID>::GetIncrement())
   , mEnabled(true)
{
}

bool IEngineModule::Activate() { return true; } // Implementation is optional
void IEngineModule::Deactivate() { } // Implementation is optional

void IEngineModule::Enable()
{
   if(!mEnabled)
   {
      mEnabled = true;
      Activate();
   }
}

void IEngineModule::Disable()
{
   if(mEnabled)
   {
      mEnabled = false;
      Deactivate();
   }
}