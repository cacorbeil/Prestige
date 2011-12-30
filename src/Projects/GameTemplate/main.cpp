#define DLL_EXPORT

#include "GameEngine/Core/GameEngineCore.h"
#include "GameEngine/Util/Incrementator.h"

#include <time.h>
#include <iostream>

#include "ParticleManager.h"

namespace IncrementatorTypeEnum
{
   enum ENUM
   {
      GAME_LOOP = GameEngine::IncrementatorTypeEnum::INCREMENTOR_GAME_FOLLOW_UP,
      SPECIFIC, 
   };
}

static bool MainLoop()
{
   static const IncrementatorType Tbb = STATIC_INCREMENTATOR(Testb,0); 

   clock_t before = clock();
   //ParticleManager::GetInstance()->Update(0.033f);
   std::cout << "Time elapsed: " << (clock() - before) << std::endl;

   return true;
}


int main( )
{
   GameEngine::GameEngineCore gameEngine = GameEngine::GameEngineCore(MainLoop);

   gameEngine.StartEngine(); 
}
