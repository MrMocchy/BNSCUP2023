#pragma once

struct GameData {

	Rect sky{ Arg::center = Point(600,50), 1200, 100};
	Rect sea{ Arg::center = Point(600, 250), 1200, 300 };
	Rect beach{ Arg::center = Point(600,500), 1200, 200};

	double viewX = 0.0;

	InputGroup rightKey = InputGroup{ KeyRight, KeyD };
	InputGroup leftKey = InputGroup{ KeyLeft, KeyA };
	double viewScrollSpeed = 10.0;
	
};

using App = SceneManager<String, GameData>;
