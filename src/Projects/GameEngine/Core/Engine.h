#ifndef ENGINE_H
#define ENGINE_H

#include <time.h>
#include <vector>

#include "Scheduler.h"

#include "GameEngine/Modules/IEngineModule.h"
#include "GameEngine/Modules/ModuleDictionary.h"
#include "GameEngine/Util/Identifier/Incrementator.h"

#include "GameEngine/Macros/DLL.h"


EXPORT_STL_VECTOR(GameEngine::IEngineModule*);

DISTRIBUTED_ENUM(INCREMENTATOR_ENUM, FRAME_ID);

/// Types of event that can be sent to the engine
namespace ENGINE_EVENT
{
   enum Enum
   {
      NONE = 0,   ///< No event, this doesn't have any effect.
      QUIT,       ///< This tells the game engine to exit the game loop
   };
}

/// Bridge classes between the game and the engine
namespace GameEngine
{
   /// Central point of the engine.
   /// Class that acts as the bridge between the engine and the game.
   /// Manages game loop, modules registring, tasks scheduling.
   class DLL_DECL Engine
   {
   public:
      typedef Util::IncrementatorType FrameId;
      typedef bool(*GAME_LOOP_FUNCTION)(float); ///< Game loop function pointer

      /// \param aGameLoopFunctionPointer The game loop function pointer.
      Engine(GAME_LOOP_FUNCTION aGameLoopFunctionPointer);

      /// Initiator function.
      /// Starts the gameloop.
      void StartEngine();

      /// \param aEvent The event that has to be flagged
      void FlagEvent(ENGINE_EVENT::Enum aEvent);

      /// \param arModule The module to register
      void RegisterModule(IEngineModule& arModule);
      /// \param arModule The module to unregister
      void UnregisterModule(IEngineModule& arModule);

      /// Get the current frame id, which is a sequential number since the engine has been running
      inline FrameId GetCurrentFrameId() const;

   protected:
      /// Repeatively calls the game loop function with the delta time
      void InternalGameLoop();

      ENGINE_EVENT::Enum mCurrentEventsFlags; ///< Current active events

   private:
      GAME_LOOP_FUNCTION mGameLoopFunction;     ///< Given game loop function
      FrameId mFrameId;               ///< Current frame id
      clock_t mPreviousClockTime;               ///< Previous frame clock time
      ModuleDictionary mModuleList;  ///< List of registered modules
      Scheduler mScheduler;                     ///< Engine tasks scheduler
   };
}

#endif ENGINE_H