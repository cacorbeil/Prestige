#define DLL_EXPORT

#include "../GameEngine/Core/GameEngineCore.h"
#include "../GameEngine/Core/Incrementator.h"

#include <iostream>

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
	std::cout << "=" << Incrementator<IncrementatorTypeEnum::GAME_LOOP>::GetIncrement();

	std::cout << std::endl;
	static const IncrementatorType Tbb = STATIC_INCREMENTATOR(Testb,0); 

	return true;
}


int main( int argc, const char* argv[] )
{
	GameEngine::GameEngineCore gameEngine = GameEngine::GameEngineCore(MainLoop);

	gameEngine.StartEngine();
}
