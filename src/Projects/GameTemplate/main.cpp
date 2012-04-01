#include "GameEngine/Core/Engine.h"

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
   //gameEngine.RegisterModule(*ParticleManager::GetInstance());
   gameEngine.StartEngine();
}


