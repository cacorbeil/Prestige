#include "PrecompiledHeader.h"

#include "Engine.h"

#include <algorithm>

namespace GameEngine
{
   Engine::Engine(GAME_LOOP_FUNCTION aGameLoopFunctionPointer)
      : mFrameId(0)
      , mCurrentEventsFlags(ENGINE_EVENT::NONE)
      , mPreviousClockTime(0)
      , mGameLoopFunction(aGameLoopFunctionPointer)
   {
   }

   void Engine::StartEngine()
   {
      InternalGameLoop();
   }

   void Engine::InternalGameLoop()
   {
      // Reset flags
      mCurrentEventsFlags = ENGINE_EVENT::NONE;

      while((mCurrentEventsFlags & ENGINE_EVENT::QUIT) == 0)
      {
         clock_t currentClock = clock();
         const float deltaTime = static_cast<float>((currentClock - mPreviousClockTime)) / static_cast<float>(CLOCKS_PER_SEC);
         mScheduler.Execute(mModuleList, deltaTime);
         mPreviousClockTime = currentClock;
         mFrameId = Util::Incrementator<INCREMENTATOR_ENUM::FRAME_ID>::GetIncrement();
      }
   }

   void Engine::FlagEvent(ENGINE_EVENT::Enum aEvent)
   {
      mCurrentEventsFlags = static_cast<ENGINE_EVENT::Enum>(mCurrentEventsFlags | aEvent);
   }

   inline Engine::FrameId Engine::GetCurrentFrameId() const
   {
      return mFrameId;
   }

   void Engine::RegisterModule(IEngineModule& arModule)
   {
      mModuleList.Add(&arModule);
   }

   void Engine::UnregisterModule(IEngineModule& arModule)
   {
     mModuleList.Remove(&arModule);
   }
}
