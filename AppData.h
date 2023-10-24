#pragma once

struct GameData {
	

	Rect sky{ Arg::center = Point(600,50), 1200, 100};
	Rect sea{ Arg::center = Point(600, 250), 1200, 300 };
	Rect beach{ Arg::center = Point(600,500), 1200, 200};

	double viewX = 0.0;

	InputGroup rightKey = InputGroup{ KeyRight, KeyD };
	InputGroup leftKey = InputGroup{ KeyLeft, KeyA };
	double viewScrollSpeed = 300.0;

	struct Human {
		Vec2 pos;
		size_t faceIndex;
		double waveOffset;
	};
	Array<Human> people;
	const size_t peopleNum = 100;
	size_t faceNum = 0;
	double faceSizeS = 30.0;
	double faceSizeL = 40.0;

	GameData() {
		// 顔文字の登録
		Array<String> faces{ U"👶",U"🧒",U"👦",U"👧",U"🧑",U"👨",U"🧔",U"👩",U"🧓",U"👴",U"👵",U"👨‍🦱",U"👱",U"👨‍🦰",U"👨‍🦳",U"👨‍🦲",U"👩‍🦰",U"🧑‍🦰",U"👩‍🦱",U"🧑‍🦱",U"👩‍🦳",U"🧑‍🦳",U"👩‍🦲",U"🧑‍🦲" };
		faceNum = faces.size();
		for (auto i : step(faceNum))
		{
			TextureAsset::Register(U"face{}"_fmt(i), Emoji{ faces[i] });
		}

		// 人の作成
		for (auto i : step(faceNum))
		{
			people.push_back({ RandomVec2(sea), Random(faceNum-1), Random(0.0, 360.0_deg) });
			//people.push_back({ Vec2(50*i,300), i, Random(0.0, 360.0_deg)});
		}
	}
};

using App = SceneManager<String, GameData>;
