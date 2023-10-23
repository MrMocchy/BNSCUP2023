# include <Siv3D.hpp> // Siv3D v0.6.12

# include "AppData.h"
# include "GameScene.h"

void Main()
{

	App app;
	app.add<GameScene>(U"Game");

	while (System::Update())
	{
		if (not app.update())
		{
			break;
		}
	}
}
