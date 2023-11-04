# include <Siv3D.hpp> // Siv3D v0.6.12

# include "AppData.h"
# include "App.h"

void Main()
{

	ScreenCapture::SetShortcutKeys({ KeyP });

	App app;

	while (System::Update())
	{
		app.update();

		app.draw();
	}
}
