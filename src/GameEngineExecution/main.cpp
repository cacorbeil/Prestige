#include <iostream>
#include <algorithm>

#include "../GameEngine/Core/GameEngineCore.h"

class Test
{
public:
	bool operator()()
	{
		std::cout << ":";

		return true;
	}
};

int main( int argc, const char* argv[] )
{
	GameEngineCore<Test> gameEngine = GameEngineCore<Test>(Test());

	gameEngine.StartEngine();
}