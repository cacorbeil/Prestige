#include "GameEngine/Core/Engine.h"
#include "GameEngine/Assert/Assert.h"

#include <iostream>
#include "ParticleManager.h"
#include <functional>
#include <algorithm>

static bool MainLoop(float aDeltaTime);

GameEngine::Engine gameEngine(MainLoop);

typedef void (ParticleManager::*TASK_FUNCTION2)(float);

static bool MainLoop(float aDeltaTime)
{
   std::cout << "Frame delta time: " << aDeltaTime << std::endl;

   //ParticleManager::GetInstance();

   if(gameEngine.GetCurrentFrameId() == 120)
   {
      gameEngine.FlagEvent(ENGINE_EVENT::QUIT);
   }

   return true;
}

int main() 
{
   Assert(1 == 0, "before");
   for (int i = 0; i < 10; ++i)
   {
      Assert(1 == 0, "for");
   }
   Assert(1 == 0, "after");
   gameEngine.RegisterModule(*ParticleManager::GetInstance());
   gameEngine.StartEngine();
}


