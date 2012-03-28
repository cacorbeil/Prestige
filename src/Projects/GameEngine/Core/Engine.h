#ifndef ENGINE_H
#define ENGINE_H

#include <time.h>
#include <vector>

#include "Scheduler.h"

#include "GameEngine/Modules/IEngineModule.h"
#include "GameEngine/Modules/ModuleDictionnary.h"
#include "GameEngine/Util/Incrementator.h"

#include "GameEngine/Macros/DLL.h"


EXPORT_STL_VECTOR(IEngineModule*);

DISTRIBUTED_ENUM(INCREMENTATOR_ENUM, FRAME_ID);

namespace ENGINE_EVENT
{
   enum Enum
   {
      NONE = 0,
      QUIT,
   };
}

namespace GameEngine
{
   class DLL_DECL Engine
   {
   public:
      typedef bool(*GAME_LOOP_FUNCTION)(float);

      Engine(GAME_LOOP_FUNCTION aGameLoopFunctionPointer);

      void StartEngine();

      void FlagEvent(ENGINE_EVENT::Enum aEvent);

      void RegisterModule(IEngineModule& arModule);
      void UnregisterModule(IEngineModule& arModule);

      inline IncrementatorType GetCurrentFrameId() const;

   protected:
      void GameLoop();
      ENGINE_EVENT::Enum mCurrentActiveFlags;

   private:
      GAME_LOOP_FUNCTION mGameLoopFunction;
      IncrementatorType mFrameId;
      clock_t mLastClockTime;
      std::vector<IEngineModule*> mModuleList;
      Scheduler mScheduler;
      
   };
}

#endif ENGINE_H