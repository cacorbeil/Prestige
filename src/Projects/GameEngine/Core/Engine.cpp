#include "Engine.h"

#include <algorithm>

namespace GameEngine
{
   Engine::Engine(GAME_LOOP_FUNCTION aGameLoopFunctionPointer)
      : mFrameId(0)
      , mCurrentActiveFlags(ENGINE_EVENT::NONE)
      , mLastClockTime(0)
      , mGameLoopFunction(aGameLoopFunctionPointer)
   {
   }

   void Engine::StartEngine()
   {
      GameLoop();
   }

   void Engine::GameLoop()
   {
      // Reset flags
      mCurrentActiveFlags = ENGINE_EVENT::NONE;

      while((mCurrentActiveFlags & ENGINE_EVENT::QUIT) == 0)
      {
         clock_t currentClock = clock();
         const float deltaTime = static_cast<float>((currentClock - mLastClockTime)) / static_cast<float>(CLOCKS_PER_SEC);
         mScheduler.Execute(mModuleList, deltaTime);
         mLastClockTime = currentClock;
         mFrameId = Incrementator<INCREMENTATOR_ENUM::FRAME_ID>::GetIncrement();
      }
   }

   void Engine::FlagEvent(ENGINE_EVENT::Enum aEvent)
   {
      mCurrentActiveFlags = static_cast<ENGINE_EVENT::Enum>(mCurrentActiveFlags | aEvent);
   }

   inline IncrementatorType Engine::GetCurrentFrameId() const
   {
      return mFrameId;
   }

   void Engine::RegisterModule(IEngineModule& arModule)
   {
      mModuleList.push_back(&arModule);
   }

   void Engine::UnregisterModule(IEngineModule& arModule)
   {
      for(std::vector<IEngineModule*>::iterator it = mModuleList.begin(); it != mModuleList.end(); ++it)
      {
         if((*it)->GetId() == arModule.GetId())
         {
            // ***Needs to be tested***
            // Swap with the last (could be optimized with coding swap without assignment for last parameter
            std::swap(*it, mModuleList.back());
            // Remove the last
            mModuleList.pop_back();
            break;
         }
      }
   }
}