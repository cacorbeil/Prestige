#include <iostream>
#include <algorithm>

#include "../GameEngine/Core/GameEngineCore.h"
#include "../GameEngine/Core/Incrementator.h"

namespace IncrementatorTypeEnum
{
	enum ENUM
	{
		GAME_LOOP = GameEngineCore::IncrementatorTypeEnum::INCREMENTOR_GAME_FOLLOW_UP,
		SPECIFIC,
	};
}

class Test
{
public:
	bool operator()()
	{
		
		std::cout << "=" << Incrementator<IncrementatorTypeEnum::GAME_LOOP>::GetIncrement();

		std::cout << std::endl;

		return true;
	}
};

int main( int argc, const char* argv[] )
{
	GameEngineCore::GameEngineCore<Test> gameEngine = GameEngineCore::GameEngineCore<Test>();

	gameEngine.StartEngine();
}