# include <Siv3D.hpp> // Siv3D v0.6.12

# include "AppData.h"
# include "App.h"

void Main()
{

	ScreenCapture::SetShortcutKeys({ KeyP });

	FontAsset::Register(U"Large",100,Typeface::Bold);
	FontAsset::Register(U"Medium", 50);

	App app;
	app.data.Init();

	while (System::Update())
	{
		app.commonUpdate();
		if (app.data.isInGame) app.gameUpdate();

		app.draw();

		if (app.data.isTitle) app.drawTitle();
		if (app.data.isGameClear) app.drawGameClear();
		if (app.data.isGameOver) app.drawGameOver();

	}
}
